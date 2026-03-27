#include "pch.h"
#include "tool_process.h"
#include "driver_conn.h"
#include "mcp_proto.h"

// ─────────────────────────────────────────────────────────────────────────────
// Grow-and-retry helper: sends req, retries with larger VirtualAlloc buffers
// until driver returns S_OK.  Returns the VirtualAlloc'd buffer (caller must
// VirtualFree it) or nullptr on failure.
// ─────────────────────────────────────────────────────────────────────────────
static PVOID DoEnum(PVOID req, DWORD reqSz, DWORD &outSz, DWORD &hint)
{
    DWORD cb = 0;
    for (;;) {
        if (hint && !cb) cb = hint;
        else             cb += 0x40000;

        PVOID buf = VirtualAlloc(nullptr, cb, MEM_COMMIT, PAGE_READWRITE);
        if (!buf) return nullptr;

        DWORD got = 0;
        HRESULT hr = PyArkSend(req, reqSz, buf, cb, &got);
        if (hr == S_OK) {
            if (cb > hint) hint = cb;
            outSz = got;
            return buf;
        }
        VirtualFree(buf, 0, MEM_RELEASE);
        if (hr != (HRESULT)0x80070018u) return nullptr;
    }
}

// ─────────────────────────────────────────────────────────────────────────────
std::string ToolListProcesses()
{
    UCHAR req = cls_enum_process;
    static DWORD s_hint = 0;
    DWORD outSz = 0;
    PVOID buf = DoEnum(&req, sizeof(req), outSz, s_hint);
    if (!buf)
        return R"({"error":"driver enum failed"})";

    rapidjson::StringBuffer sb;
    rapidjson::Writer<rapidjson::StringBuffer> w(sb);
    w.StartArray();

    auto p = (cls_enum_process_data *)buf;
    while (p) {
        if ((PUCHAR)p + sizeof(cls_enum_process_data) > (PUCHAR)buf + outSz) break;
        if (p->ProcessId != 0) {
            std::string name = p->ProcessNameBuffer
                ? W2U8((PWCH)p->ProcessNameBuffer, p->ProcessNameLength / 2) : "";
            std::string path = p->ProcessPathBuffer
                ? W2U8((PWCH)p->ProcessPathBuffer, p->ProcessPathLength / 2) : "";

            w.StartObject();
            w.Key("pid");      w.Uint(p->ProcessId);
            w.Key("ppid");     w.Uint(p->ParentId);
            w.Key("session");  w.Uint(p->SessionId);
            w.Key("eprocess"); w.Uint64(p->EProcess);
            w.Key("name");     w.String(name.c_str());
            w.Key("path");     w.String(path.c_str());
            w.EndObject();
        }
        if (p->NextEntryOffset == 0) break;
        p = (cls_enum_process_data *)((PUCHAR)p + p->NextEntryOffset);
        if ((PUCHAR)p >= (PUCHAR)buf + outSz) break;
    }

    w.EndArray();
    VirtualFree(buf, 0, MEM_RELEASE);
    return sb.GetString();
}

// ─────────────────────────────────────────────────────────────────────────────
std::string ToolGetProcessInfo(ULONG pid)
{
    cls_pid_request req = {};
    DWORD got = 0;

    // Base info
    req.protocol  = cls_get_process_baseinfo;
    req.ProcessId = pid;
    cls_get_process_baseinfo_data bi = {};
    PyArkSend(&req, sizeof(req), &bi, sizeof(bi), &got);

    // Image path (DOS style)
    req.protocol = cls_get_process_path;
    cls_get_process_imagepath_data pathData = {};
    PyArkSend(&req, sizeof(req), &pathData, sizeof(pathData), &got);

    // Command line
    req.protocol = cls_get_process_cmdline;
    cls_get_process_imagepath_data cmdData = {};
    PyArkSend(&req, sizeof(req), &cmdData, sizeof(cmdData), &got);

    std::string path = W2U8(pathData.ImagePath);
    std::string cmd  = W2U8(cmdData.ImagePath);

    rapidjson::StringBuffer sb;
    rapidjson::Writer<rapidjson::StringBuffer> w(sb);
    w.StartObject();
    w.Key("pid");        w.Uint(pid);
    w.Key("ppid");       w.Uint(bi.ParentProcessId);
    w.Key("session");    w.Uint(bi.SessionId);
    w.Key("is64");       w.Bool(bi.Is64Bit != 0);
    w.Key("critical");   w.Bool(bi.IsCritical != 0);
    w.Key("debug_port"); w.Uint64(bi.DebugPort);
    w.Key("create_time");w.Uint64(bi.CreateTime);
    w.Key("path");       w.String(path.c_str());
    w.Key("cmdline");    w.String(cmd.c_str());
    w.EndObject();
    return sb.GetString();
}
