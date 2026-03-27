#include "pch.h"
#include "tool_module.h"
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

std::string ToolListSystemModules()
{
    UCHAR req = cls_enum_system_modules;
    static DWORD s_hint = 0;
    DWORD outSz = 0;
    PVOID buf = DoEnum(&req, sizeof(req), outSz, s_hint);
    if (!buf)
        return R"({"error":"driver enum failed"})";

    rapidjson::StringBuffer sb;
    rapidjson::Writer<rapidjson::StringBuffer> w(sb);
    w.StartArray();

    auto p = (cls_enum_sysmodule_data *)buf;
    while (p) {
        if ((PUCHAR)p + offsetof(cls_enum_sysmodule_data, ImageNameBuffer) + sizeof(ULONG64)
                > (PUCHAR)buf + outSz) break;

        char baseBuf[32];  sprintf_s(baseBuf,  "0x%llX", p->ImageBase);
        char epBuf[32];    sprintf_s(epBuf,    "0x%llX", p->EntryPoint);

        std::string name = p->ImageNameBuffer
            ? W2U8((PWCH)p->ImageNameBuffer, p->ImageNameLength / 2) : "";

        w.StartObject();
        w.Key("base");       w.String(baseBuf);
        w.Key("size");       w.Uint(p->ImageSize);
        w.Key("entry");      w.String(epBuf);
        w.Key("load_index"); w.Uint(p->LoadIndex);
        w.Key("name");       w.String(name.c_str());
        w.EndObject();

        if (p->NextEntryOffset == 0) break;
        p = (cls_enum_sysmodule_data *)((PUCHAR)p + p->NextEntryOffset);
        if ((PUCHAR)p >= (PUCHAR)buf + outSz) break;
    }

    w.EndArray();
    VirtualFree(buf, 0, MEM_RELEASE);
    return sb.GetString();
}
