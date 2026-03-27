#include "pch.h"
#include "tool_memory.h"
#include "driver_conn.h"
#include "mcp_proto.h"

// ─────────────────────────────────────────────────────────────────────────────
std::string ToolQueryMemory(ULONG pid, ULONG64 addr)
{
    cls_query_vm_request req = {};
    req.protocol    = cls_query_virtual_memory;
    req.ProcessId   = pid;
    req.BaseAddress = addr;

    cls_query_vm_result res = {};
    DWORD got = 0;
    HRESULT hr = PyArkSend(&req, sizeof(req), &res, sizeof(res), &got);

    rapidjson::StringBuffer sb;
    rapidjson::Writer<rapidjson::StringBuffer> w(sb);
    w.StartObject();

    if (hr != S_OK) {
        char hrBuf[32]; sprintf_s(hrBuf, "0x%08X", (ULONG)hr);
        w.Key("error");   w.String("query failed");
        w.Key("hresult"); w.String(hrBuf);
    } else {
        char baseBuf[32];  sprintf_s(baseBuf,  "0x%llX", res.BaseAddress);
        char allocBuf[32]; sprintf_s(allocBuf, "0x%llX", res.AllocationBase);
        w.Key("base");         w.String(baseBuf);
        w.Key("alloc_base");   w.String(allocBuf);
        w.Key("size");         w.Uint64(res.RegionSize);
        w.Key("state");        w.String(MemState(res.State));
        w.Key("protect");      w.String(ProtStr(res.Protect).c_str());
        w.Key("alloc_protect");w.String(ProtStr(res.AllocationProtect).c_str());
        w.Key("type");         w.String(MemType(res.Type));
    }

    w.EndObject();
    return sb.GetString();
}

// ─────────────────────────────────────────────────────────────────────────────
std::string ToolEnumMemoryRegions(ULONG pid, ULONG64 startAddr, ULONG maxRegions)
{
    if (maxRegions == 0 || maxRegions > 4096) maxRegions = 256;

    rapidjson::StringBuffer sb;
    rapidjson::Writer<rapidjson::StringBuffer> w(sb);
    w.StartArray();

    ULONG64 addr = startAddr;
    for (ULONG i = 0; i < maxRegions; ++i) {
        cls_query_vm_request req = {};
        req.protocol    = cls_query_virtual_memory;
        req.ProcessId   = pid;
        req.BaseAddress = addr;

        cls_query_vm_result res = {};
        DWORD got = 0;
        HRESULT hr = PyArkSend(&req, sizeof(req), &res, sizeof(res), &got);
        if (hr != S_OK || got < sizeof(res) || res.RegionSize == 0) break;

        char baseBuf[32];  sprintf_s(baseBuf,  "0x%llX", res.BaseAddress);
        char allocBuf[32]; sprintf_s(allocBuf, "0x%llX", res.AllocationBase);

        w.StartObject();
        w.Key("base");         w.String(baseBuf);
        w.Key("alloc_base");   w.String(allocBuf);
        w.Key("size");         w.Uint64(res.RegionSize);
        w.Key("state");        w.String(MemState(res.State));
        w.Key("protect");      w.String(ProtStr(res.Protect).c_str());
        w.Key("alloc_protect");w.String(ProtStr(res.AllocationProtect).c_str());
        w.Key("type");         w.String(MemType(res.Type));
        w.EndObject();

        addr = res.BaseAddress + res.RegionSize;
    }

    w.EndArray();
    return sb.GetString();
}

// ─────────────────────────────────────────────────────────────────────────────
std::string ToolReadMemory(ULONG pid, ULONG64 addr, ULONG size)
{
    if (size == 0)       size = 64;
    if (size > 0x10000)  size = 0x10000;   // cap at 64 KB

    cls_read_vm_request req = {};
    req.protocol    = cls_read_virtual_memory;
    req.ProcessId   = pid;
    req.BaseAddress = addr;
    req.RegionSize  = size;

    std::vector<BYTE> outBuf(size, 0);
    DWORD got = 0;
    HRESULT hr = PyArkSend(&req, sizeof(req), outBuf.data(), size, &got);

    // S_OK or ERROR_PARTIAL_COPY (0x8007012b) are both partial successes
    bool ok = (hr == S_OK) || (hr == (HRESULT)0x8007012bu);

    rapidjson::StringBuffer sb;
    rapidjson::Writer<rapidjson::StringBuffer> w(sb);
    w.StartObject();

    char addrBuf[32]; sprintf_s(addrBuf, "0x%llX", addr);
    w.Key("address"); w.String(addrBuf);
    w.Key("size");    w.Uint(got);

    if (ok && got > 0) {
        static const char *HEX = "0123456789ABCDEF";
        std::string hex;
        hex.reserve((size_t)got * 2);
        for (DWORD i = 0; i < got; ++i) {
            hex += HEX[(outBuf[i] >> 4) & 0xF];
            hex += HEX[outBuf[i] & 0xF];
        }
        w.Key("hex"); w.String(hex.c_str());
    } else {
        char hrBuf[32]; sprintf_s(hrBuf, "0x%08X", (ULONG)hr);
        w.Key("error");   w.String("read failed");
        w.Key("hresult"); w.String(hrBuf);
    }

    w.EndObject();
    return sb.GetString();
}
