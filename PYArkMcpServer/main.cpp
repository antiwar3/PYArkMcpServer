// PYArkMcpServer — MCP stdio bridge
// Claude Desktop launches this as a subprocess (stdin/stdout = MCP JSON-RPC).
// Tool calls are forwarded to PYArkClient.exe via named pipe \\.\pipe\PYArkMcpServer.
// initialize and tools/list are handled locally; only tools/call crosses the pipe.

#include "pch.h"
#include "mcp_proto.h"

#define PIPE_NAME  L"\\\\.\\pipe\\PYArkMcpServer"
#define PIPE_WAIT_MS  5000

static HANDLE g_hPipe = INVALID_HANDLE_VALUE;

// ── pipe connection ───────────────────────────────────────────────────────────

static bool EnsurePipe()
{
    if (g_hPipe != INVALID_HANDLE_VALUE) return true;

    fprintf(stderr, "[MCP] Connecting to pipe %ls ...\n", PIPE_NAME);
    if (!WaitNamedPipeW(PIPE_NAME, PIPE_WAIT_MS)) {
        fprintf(stderr, "[MCP] WaitNamedPipe failed, error=%lu (PYArkClient not running?)\n", GetLastError());
        return false;
    }

    g_hPipe = CreateFileW(PIPE_NAME,
        GENERIC_READ | GENERIC_WRITE,
        0, nullptr, OPEN_EXISTING, 0, nullptr);

    if (g_hPipe == INVALID_HANDLE_VALUE) {
        fprintf(stderr, "[MCP] CreateFile pipe failed, error=%lu\n", GetLastError());
        return false;
    }
    fprintf(stderr, "[MCP] Pipe connected OK\n");
    return true;
}

static void ClosePipe()
{
    if (g_hPipe != INVALID_HANDLE_VALUE) {
        CloseHandle(g_hPipe);
        g_hPipe = INVALID_HANDLE_VALUE;
    }
}

// Send {tool, args} to PYArkClient and return the JSON result.
static std::string CallPipe(const char* toolName, const rapidjson::Value& args)
{
    if (!EnsurePipe())
        return R"({"error":"PYArkClient not running \/ MCP pipe server not started"})";

    // Serialize request
    rapidjson::StringBuffer reqSb;
    rapidjson::Writer<rapidjson::StringBuffer> rw(reqSb);
    rw.StartObject();
    rw.Key("tool"); rw.String(toolName);
    rw.Key("args");
    if (args.IsObject()) {
        args.Accept(rw);
    } else {
        rw.StartObject(); rw.EndObject();
    }
    rw.EndObject();

    const std::string& req = reqSb.GetString();
    DWORD len = (DWORD)req.size(), wrote = 0, total = 0;

    // Write length + body
    if (!WriteFile(g_hPipe, &len, 4, &wrote, nullptr) || wrote != 4) {
        ClosePipe(); return R"({"error":"pipe write failed"})";
    }
    while (total < len) {
        if (!WriteFile(g_hPipe, req.c_str() + total, len - total, &wrote, nullptr) || !wrote) {
            ClosePipe(); return R"({"error":"pipe write failed"})";
        }
        total += wrote;
    }

    // Read length + body
    DWORD rlen = 0, got = 0;
    if (!ReadFile(g_hPipe, &rlen, 4, &got, nullptr) || got != 4 || !rlen || rlen > 16 * 1024 * 1024) {
        ClosePipe(); return R"({"error":"pipe read failed"})";
    }
    std::string resp(rlen, '\0');
    total = 0;
    while (total < rlen) {
        if (!ReadFile(g_hPipe, &resp[total], rlen - total, &got, nullptr) || !got) {
            ClosePipe(); return R"({"error":"pipe read failed"})";
        }
        total += got;
    }
    return resp;
}

// ── tools/list ────────────────────────────────────────────────────────────────

// Parameter descriptor
struct Param {
    const char* name;
    const char* type;   // "integer", "string", "boolean"
    const char* desc;
    bool        required;
};

struct ToolDef {
    const char* name;
    const char* desc;
    std::vector<Param> params;
};

static void EmitTool(rapidjson::Writer<rapidjson::StringBuffer>& w, const ToolDef& t)
{
    w.StartObject();
      w.Key("name");        w.String(t.name);
      w.Key("description"); w.String(t.desc);
      w.Key("inputSchema");
      w.StartObject();
        w.Key("type"); w.String("object");
        w.Key("properties");
        w.StartObject();
        for (auto& p : t.params) {
            w.Key(p.name);
            w.StartObject();
              w.Key("type");        w.String(p.type);
              w.Key("description"); w.String(p.desc);
            w.EndObject();
        }
        w.EndObject();
        w.Key("required");
        w.StartArray();
        for (auto& p : t.params)
            if (p.required) w.String(p.name);
        w.EndArray();
      w.EndObject();
    w.EndObject();
}

static std::string BuildToolsList(const rapidjson::Value& id)
{
    static const ToolDef kTools[] = {
        // ── process / thread / module enumeration ─────────────────────────────
        { "list_processes",
          "List all running processes (pid, ppid, session, eprocess, peb, name, path)",
          {} },
        { "list_threads",
          "List threads of a process (tid, teb, start_addr, ethread, priority)",
          { {"pid","integer","Process ID",true} } },
        { "list_modules",
          "List loaded DLL modules of a process (base, size, name, is64, unlinked)",
          { {"pid","integer","Process ID",true} } },
        { "list_system_modules",
          "List loaded kernel driver modules (base, size, name, entry)",
          {} },
        { "get_process_info",
          "Get detailed process info (ppid, session, is64, path, cmdline, debug_port, create_time)",
          { {"pid","integer","Process ID",true} } },
        { "get_process_peb",
          "Get the PEB address of a process",
          { {"pid","integer","Process ID",true},
            {"wow64","boolean","true to get WoW64 (32-bit) PEB",false} } },
        { "get_process_cur_dir",
          "Get the current working directory of a process",
          { {"pid","integer","Process ID",true} } },
        { "get_process_image_base",
          "Get the main executable image base address of a process",
          { {"pid","integer","Process ID",true} } },
        { "get_image_basic_info",
          "Get image base, size and bitness for a module loaded in a process",
          { {"pid","integer","Process ID",true},
            {"base","string","Module base address (hex)",true} } },
        { "get_image_path",
          "Get the DOS-style file path of a module loaded in a process",
          { {"pid","integer","Process ID",true},
            {"base","string","Module base address (hex)",true} } },
        { "get_image_entry",
          "Resolve a kernel/user address to its containing module (base, name)",
          { {"address","string","Virtual address (hex)",true} } },
        { "get_function_entry",
          "Resolve a kernel/user address to its function (mod_base, func_begin, func_name)",
          { {"address","string","Virtual address (hex)",true} } },
        // ── memory ─────────────────────────────────────────────────────────────
        { "query_memory",
          "Query a single memory region (MEMORY_BASIC_INFORMATION)",
          { {"pid","integer","Process ID",true},
            {"address","string","Base address to query (hex)",true} } },
        { "enum_memory_regions",
          "Walk consecutive memory regions of a process (max 5000)",
          { {"pid","integer","Process ID",true},
            {"start","string","Start address (hex, default 0x0)",false},
            {"max_regions","integer","Max regions to return (default 256)",false} } },
        { "read_memory",
          "Read process virtual memory, returns hex-encoded bytes (max 65536)",
          { {"pid","integer","Process ID",true},
            {"address","string","Start address (hex)",true},
            {"size","integer","Bytes to read",true} } },
        { "write_memory",
          "Write process virtual memory from a hex string",
          { {"pid","integer","Process ID",true},
            {"address","string","Start address (hex)",true},
            {"hex","string","Data as uppercase hex string (e.g. 4D5A90)",true} } },
        { "protect_memory",
          "Change virtual memory protection flags of a region",
          { {"pid","integer","Process ID",true},
            {"address","string","Region base address (hex)",true},
            {"size","integer","Region size in bytes",true},
            {"protect","integer","Win32 page protection constant (e.g. 0x40 = PAGE_EXECUTE_READWRITE)",true} } },
        // ── handles ────────────────────────────────────────────────────────────
        { "enum_process_handles",
          "Enumerate all handles held by a process (handle, obj_type, obj_name, access)",
          { {"pid","integer","Process ID",true} } },
        { "close_file_handle",
          "Close a handle inside a remote process",
          { {"pid","integer","Process ID",true},
            {"handle","integer","Handle value",true} } },
        { "kill_handle",
          "Force-close any handle in any process",
          { {"pid","integer","Process ID",true},
            {"handle","integer","Handle value",true},
            {"handle_object","string","Handle object pointer (hex)",true},
            {"force_kill","boolean","Use forceful termination",false} } },
        // ── process / thread control ───────────────────────────────────────────
        { "kill_process",
          "Terminate a process via driver",
          { {"pid","integer","Process ID",true} } },
        { "suspend_process",
          "Suspend all threads of a process",
          { {"pid","integer","Process ID",true} } },
        { "resume_process",
          "Resume all threads of a process",
          { {"pid","integer","Process ID",true} } },
        { "kill_thread",
          "Terminate a thread by ETHREAD address",
          { {"ethread","string","ETHREAD kernel address (hex)",true} } },
        { "suspend_thread",
          "Suspend a thread by ETHREAD address",
          { {"ethread","string","ETHREAD kernel address (hex)",true} } },
        { "resume_thread",
          "Resume a thread by ETHREAD address",
          { {"ethread","string","ETHREAD kernel address (hex)",true} } },
        // ── process protection ─────────────────────────────────────────────────
        { "protect_process",
          "Enable ObCallback protection for a process",
          { {"pid","integer","Process ID",true} } },
        { "set_protect_process",
          "Set a specific protection type for a process (1=ObCallback, 2=EPROCESS, 3=TES, 4=HideClient, 5=HookOpen)",
          { {"pid","integer","Process ID",true},
            {"type","integer","Protection type (1-6)",true},
            {"enable","boolean","Enable or disable",true} } },
        // ── privileges ────────────────────────────────────────────────────────
        { "enum_process_privileges",
          "Enumerate token privileges of a process",
          { {"pid","integer","Process ID",true} } },
        // ── windows ───────────────────────────────────────────────────────────
        { "enum_process_windows",
          "Enumerate top-level windows owned by a process (hwnd, pid, tid)",
          { {"pid","integer","Process ID",true} } },
        // ── kernel callbacks ──────────────────────────────────────────────────
        { "enum_callbacks",
          "Enumerate kernel callbacks (CreateProcess, CreateThread, LoadImage, Shutdown, CmCallback, ObCallbacks, etc.)",
          {} },
        { "remove_callback",
          "Remove a kernel callback by address and type",
          { {"callback","string","Callback address (hex)",true},
            {"type","integer","Callback type (0=CreateProcess,1=CreateThread,2=LoadImage,3=Shutdown,4=Cm,5=PsObProc,6=PsObThread,...)",true} } },
        // ── filter drivers ────────────────────────────────────────────────────
        { "enum_filters",
          "Enumerate filter drivers (File, Disk, Volume, Keyboard, Mouse, Tcpip, NDIS, etc.)",
          {} },
        { "remove_filter",
          "Remove a filter driver entry",
          { {"filter_flag","integer","Filter type flag",true},
            {"device","string","Device object address (hex)",true} } },
        // ── SSDT hooks ────────────────────────────────────────────────────────
        { "enum_ssdt_hooks",
          "Enumerate SSDT (System Service Descriptor Table) hooks",
          {} },
        { "enum_shadow_ssdt_hooks",
          "Enumerate Shadow SSDT hooks (Win32k)",
          {} },
        { "recover_ssdt",
          "Restore a hooked SSDT entry to its original value",
          { {"index","integer","SSDT function index",true},
            {"type","integer","Recovery type",true} } },
        { "recover_shadow_ssdt",
          "Restore a hooked Shadow SSDT entry to its original value",
          { {"index","integer","Shadow SSDT function index",true},
            {"type","integer","Recovery type",true} } },
        // ── IDT / inline hooks ────────────────────────────────────────────────
        { "enum_idt_hooks",
          "Enumerate IDT (Interrupt Descriptor Table) hooks per CPU core",
          {} },
        { "enum_sys_inline_hooks",
          "Detect inline patches in a kernel module by comparing against a clean copy",
          { {"path","string","Path to clean copy of the sys/dll file",true},
            {"reload_address","string","Address to load clean copy (hex, 0=auto)",false},
            {"size","integer","Size of image (0=auto from PE header)",false} } },
        // ── message hooks ─────────────────────────────────────────────────────
        { "enum_message_hooks",
          "Enumerate Windows message hooks (WH_KEYBOARD_LL, WH_MOUSE_LL, etc.)",
          { {"base","string","Module base to scan (hex, 0=all)",false} } },
        // ── system threads / timers ───────────────────────────────────────────
        { "enum_system_threads",
          "Enumerate system (PID 4) threads (thread_obj, tid, start_addr, state, ethread, priority)",
          {} },
        { "enum_io_timers",
          "Enumerate active IO timers (timer_entry, device_object, timer_routine, context, period)",
          {} },
        { "enum_dpc_timers",
          "Enumerate DPC timers (timer_object, real_dpc, cycle, time_dispatch)",
          { {"pid","integer","Process ID (0 for system-wide)",false} } },
        { "remove_io_timer",
          "Remove an IO timer entry",
          { {"timer_entry","string","Timer entry address (hex)",true} } },
        { "run_stop_io_timer",
          "Start or stop an IO timer",
          { {"device","string","Device object address (hex)",true},
            {"run","boolean","true=start, false=stop",true} } },
        { "stop_dpc_timer",
          "Stop a DPC timer associated with a device object",
          { {"device","string","Device object address (hex)",true} } },
        // ── driver scanning ───────────────────────────────────────────────────
        { "scan_driver_routines",
          "Scan driver IRP dispatch routines for anomalies (unlisted modules, inline hooks, invalid paths, etc.)",
          {} },
        { "scan_unloaded_drivers",
          "Scan the unloaded driver log (MmUnloadedDrivers) for recently unloaded modules",
          {} },
        // ── object hooks ──────────────────────────────────────────────────────
        { "enum_object_types",
          "Enumerate Windows kernel object types (type_object, type_name)",
          {} },
        { "enum_object_hooks",
          "Enumerate object type procedure hook table (8 kernel object callbacks × 3: current/original/inline per type)",
          {} },
        // ── page table enumeration ────────────────────────────────────────────
        { "enum_process_pml4",
          "Enumerate PML4 page table entries for a process",
          { {"pid","integer","Process ID",true} } },
        { "enum_process_pdpt",
          "Enumerate PDPT page table entries for a given PML4 index",
          { {"pid","integer","Process ID",true},
            {"pml4_index","integer","PML4 entry index",true} } },
        { "enum_process_pd",
          "Enumerate Page Directory entries for given PML4/PDPT indices",
          { {"pid","integer","Process ID",true},
            {"pml4_index","integer","PML4 entry index",true},
            {"pdpt_index","integer","PDPT entry index",true} } },
        { "enum_process_pt",
          "Enumerate Page Table entries (physical pages) for given PML4/PDPT/PD indices",
          { {"pid","integer","Process ID",true},
            {"pml4_index","integer","PML4 entry index",true},
            {"pdpt_index","integer","PDPT entry index",true},
            {"pd_index","integer","PD entry index",true} } },
        // ── hotkeys ───────────────────────────────────────────────────────────
        { "enum_process_hotkeys",
          "Enumerate registered hotkeys for a process (name, vk, mod1, mod2, id, tid, wnd, hkobj)",
          { {"pid","integer","Process ID",true} } },
        { "remove_process_hotkey",
          "Remove a registered hotkey",
          { {"name","string","Hotkey name (max 64 chars)",true},
            {"wnd","integer","Window handle value",true},
            {"mod1","integer","Modifier 1",true},
            {"mod2","integer","Modifier 2",true},
            {"vk","integer","Virtual key code",true},
            {"id","integer","Hotkey ID",true},
            {"pid","integer","Process ID",true},
            {"tid","integer","Thread ID",true},
            {"hkobj","string","Hotkey object address (hex)",true} } },
        // ── WFP callouts ──────────────────────────────────────────────────────
        { "get_wfp_callout_info",
          "Get WFP (Windows Filtering Platform) callout details (entry, classify, notify, flow_delete addresses)",
          { {"callout_id","integer","WFP callout ID",true} } },
        { "remove_wfp_callout",
          "Remove a WFP callout by ID",
          { {"callout_id","integer","WFP callout ID",true} } },
        // ── driver / module control ───────────────────────────────────────────
        { "unload_driver",
          "Unload a kernel driver by its DRIVER_OBJECT address",
          { {"driver_object","string","DRIVER_OBJECT address (hex)",true} } },
        { "remove_module_peb",
          "Unlink a module from the process PEB loader list (stealth unload)",
          { {"pid","integer","Process ID",true},
            {"base","string","Module base address (hex)",true} } },
        { "unload_process_module",
          "Force-unload a DLL from a process",
          { {"pid","integer","Process ID",true},
            {"base","string","Module base address (hex)",true} } },
        // ── injection ─────────────────────────────────────────────────────────
        { "bb_inject",
          "Inject a DLL into a process using BlackBone (inject_type: 0=Thread,1=APC,2=ManualMap; mmap_flags: bitmask)",
          { {"pid","integer","Target process ID",true},
            {"path","string","Full path to DLL to inject",true},
            {"inject_type","integer","0=CreateThread, 1=APC, 2=ManualMap",true},
            {"mmap_flags","integer","KMmapFlags bitmask (0=none, 1=ManualImports, 4=WipeHeader, 0x10=HideVAD, ...)",false} } },
        { "bb_map_driver",
          "Load a kernel driver using BlackBone manual mapping",
          { {"path","string","Full path to .sys driver file",true} } },
        // ── dump ──────────────────────────────────────────────────────────────
        { "set_dump_path",
          "Set the output directory for dump operations",
          { {"path","string","Dump output directory path",true} } },
        { "dump_process",
          "Dump a process to disk (output goes to dump path set by set_dump_path)",
          { {"pid","integer","Process ID",true} } },
        { "dump_process_module",
          "Dump a specific module from a process to disk",
          { {"pid","integer","Process ID",true},
            {"base","string","Module base address (hex)",true},
            {"size","integer","Module image size in bytes",true} } },
        { "dump_driver_memory",
          "Dump a kernel driver memory range to disk",
          { {"base","string","Driver base address (hex)",true},
            {"size","string","Dump size in bytes (hex or decimal)",true},
            {"dump_all","boolean","true to dump all mapped regions",false} } },
        // ── file utilities ────────────────────────────────────────────────────
        { "look_file_lock_info",
          "Find which processes have a file or directory locked (returns pid, handle, path list)",
          { {"path","string","File or directory path to query",true},
            {"is_directory","boolean","true if querying a directory",false} } },
        { "dos_to_nt_path",
          "Convert a DOS path (C:\\Windows\\...) to an NT path (\\Device\\HarddiskVolume3\\...)",
          { {"path","string","DOS-style path to convert",true} } },
        // ── driver options ────────────────────────────────────────────────────
        { "set_option_state",
          "Toggle a driver feature flag (1=RestrictMode, 2=DDIMon, 3=MemScan, 4=MemScanFullSpeed, 5=DedicateMode, 6=SuperInt3, 7=AntiAntiDebug, 8=HideDRx, 9=SkipException, 10=NewDebugType, 11=EnumProcessType)",
          { {"option","integer","Option ID (1-11)",true},
            {"state","boolean","Enable or disable",true} } },
    };

    rapidjson::StringBuffer sb;
    rapidjson::Writer<rapidjson::StringBuffer> w(sb);
    w.StartObject();
      w.Key("jsonrpc"); w.String("2.0");
      w.Key("id");      WriteId(w, id);
      w.Key("result");
      w.StartObject();
        w.Key("tools");
        w.StartArray();
        for (auto& t : kTools)
            EmitTool(w, t);
        w.EndArray();
      w.EndObject();
    w.EndObject();
    return sb.GetString();
}

// ── main ─────────────────────────────────────────────────────────────────────

int wmain()
{
    _setmode(_fileno(stdin),  _O_BINARY);
    _setmode(_fileno(stdout), _O_BINARY);

    fprintf(stderr, "[MCP] PYArkMcpServer started, waiting for messages...\n");
    fflush(stderr);

    rapidjson::Document doc;
    rapidjson::Value    nullId;
    nullId.SetNull();

    for (;;) {
        std::string msg = ReadMsg();
        if (msg.empty()) { fprintf(stderr, "[MCP] stdin closed, exiting\n"); break; }

        fprintf(stderr, "[MCP] recv: %.200s\n", msg.c_str());
        fflush(stderr);

        doc.Parse(msg.c_str(), msg.size());
        if (doc.HasParseError()) { fprintf(stderr, "[MCP] JSON parse error\n"); continue; }

        const rapidjson::Value& id =
            doc.HasMember("id") ? doc["id"]
                                : static_cast<const rapidjson::Value&>(nullId);

        if (!doc.HasMember("method")) continue;
        const char* method = doc["method"].GetString();
        fprintf(stderr, "[MCP] method=%s\n", method);
        fflush(stderr);

        // ── initialize ───────────────────────────────────────────────────────
        if (strcmp(method, "initialize") == 0) {
            rapidjson::StringBuffer sb;
            rapidjson::Writer<rapidjson::StringBuffer> w(sb);
            w.StartObject();
              w.Key("jsonrpc"); w.String("2.0");
              w.Key("id");      WriteId(w, id);
              w.Key("result");
              w.StartObject();
                w.Key("protocolVersion"); w.String("2024-11-05");
                w.Key("capabilities");
                w.StartObject();
                  w.Key("tools"); w.StartObject(); w.EndObject();
                w.EndObject();
                w.Key("serverInfo");
                w.StartObject();
                  w.Key("name");    w.String("PYArkMcp");
                  w.Key("version"); w.String("2.0");
                w.EndObject();
              w.EndObject();
            w.EndObject();
            WriteMsg(sb.GetString());
            continue;
        }

        // ── notifications — no response ──────────────────────────────────────
        if (strncmp(method, "notifications/", 14) == 0) continue;

        // ── tools/list ───────────────────────────────────────────────────────
        if (strcmp(method, "tools/list") == 0) {
            WriteMsg(BuildToolsList(id));
            continue;
        }

        // ── tools/call — forward to PYArkClient via named pipe ───────────────
        if (strcmp(method, "tools/call") == 0) {
            if (!doc.HasMember("params") || !doc["params"].HasMember("name")) {
                WriteMsg(ErrResp(id, -32602, "Invalid params"));
                continue;
            }
            const char* toolName = doc["params"]["name"].GetString();

            static const rapidjson::Value kEmpty(rapidjson::kObjectType);
            const rapidjson::Value* pArgs = &kEmpty;
            if (doc["params"].HasMember("arguments") &&
                doc["params"]["arguments"].IsObject())
                pArgs = &doc["params"]["arguments"];

            std::string result = CallPipe(toolName, *pArgs);
            WriteMsg(ToolResp(id, result));
            continue;
        }

        WriteMsg(ErrResp(id, -32601, "Method not found"));
    }

    ClosePipe();
    return 0;
}
