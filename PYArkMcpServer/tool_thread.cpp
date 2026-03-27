#include "pch.h"
#include "tool_thread.h"
#include "driver_conn.h"
#include "mcp_proto.h"

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

std::string ToolListThreads(ULONG pid)
{
    cls_pid_request req = {};
    req.protocol  = cls_enum_process_threads;
    req.ProcessId = pid;

    static DWORD s_hint = 0;
    DWORD outSz = 0;
    PVOID buf = DoEnum(&req, sizeof(req), outSz, s_hint);
    if (!buf)
        return R"({"error":"driver enum failed"})";

    rapidjson::StringBuffer sb;
    rapidjson::Writer<rapidjson::StringBuffer> w(sb);
    w.StartArray();

    auto p = (cls_enum_process_threads_data *)buf;
    while (p) {
        if ((PUCHAR)p + sizeof(cls_enum_process_threads_data) > (PUCHAR)buf + outSz) break;

        char startBuf[32]; sprintf_s(startBuf, "0x%llX", p->StartAddress);
        char ethrBuf[32];  sprintf_s(ethrBuf,  "0x%llX", p->EThread);
        char tebBuf[32];   sprintf_s(tebBuf,   "0x%llX", p->teb);

        w.StartObject();
        w.Key("tid");             w.Uint(p->ThreadId);
        w.Key("pid");             w.Uint(p->ProcessId);
        w.Key("state");           w.Uint(p->State);
        w.Key("priority");        w.Uint(p->Priority);
        w.Key("context_switches");w.Uint(p->ContextSwitches);
        w.Key("start_address");   w.String(startBuf);
        w.Key("ethread");         w.String(ethrBuf);
        w.Key("teb");             w.String(tebBuf);
        w.Key("create_time");     w.Uint64(p->CreateTime);
        w.EndObject();

        if (p->NextEntryOffset == 0) break;
        p = (cls_enum_process_threads_data *)((PUCHAR)p + p->NextEntryOffset);
        if ((PUCHAR)p >= (PUCHAR)buf + outSz) break;
    }

    w.EndArray();
    VirtualFree(buf, 0, MEM_RELEASE);
    return sb.GetString();
}
