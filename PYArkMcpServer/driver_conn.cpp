#include "pch.h"
#include "driver_conn.h"

// ─────────────────────────────────────────────────────────────────────────────
// NOTE: The driver connection implementation is not included in this public
// release.  The MCP bridge in main.cpp forwards all tool calls to
// PYArkClient.exe via named pipe (\\.\pipe\PYArkMcpServer) instead of
// connecting to the driver directly.
//
// If you want to use direct driver communication, you need PYArkClient.exe
// running alongside this process.
// ─────────────────────────────────────────────────────────────────────────────

bool ConnectDriver()    { return false; }
bool IsDriverConnected(){ return false; }

HRESULT PyArkSend(LPVOID /*inBuf*/,  DWORD /*inSize*/,
                  LPVOID /*outBuf*/, DWORD /*outSize*/,
                  DWORD* pRet)
{
    if (pRet) *pRet = 0;
    return E_NOTIMPL;
}
