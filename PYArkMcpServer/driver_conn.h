#pragma once
#include "pch.h"

// fltLib — FilterConnectCommunicationPort / FilterSendMessage
#include <fltUser.h>
#pragma comment(lib, "fltLib.lib")

// NT internal types needed by driverprotocol.h (OBJECT_ATTRIBUTES, CLIENT_ID, etc.)
#include <winternl.h>

// CLIENT_ID / PCLIENT_ID may be missing or incomplete in user-mode winternl.h
__if_not_exists(CLIENT_ID) {
    typedef struct _CLIENT_ID {
        HANDLE UniqueProcess;
        HANDLE UniqueThread;
    } CLIENT_ID, *PCLIENT_ID;
}
// winternl.h sometimes defines CLIENT_ID but omits the PCLIENT_ID pointer typedef
__if_not_exists(PCLIENT_ID) {
    typedef CLIENT_ID *PCLIENT_ID;
}

// PYArk kernel-driver protocol structs
// Additional Include Dirs must contain:  ..\..\Common
#define UNICODE_STRING_MAX_CHARS 32767
#include "driverprotocol.h"     // sets #pragma pack(1) internally
#pragma pack(pop)               // restore default packing

// ─────────────────────────────────────────────────────────────────────────────
// Connect to the PYArk driver via FilterConnectCommunicationPort.
// Must be called before any PyArkSend.
// ─────────────────────────────────────────────────────────────────────────────
bool ConnectDriver();
bool IsDriverConnected();

// ─────────────────────────────────────────────────────────────────────────────
// Encrypted FilterSendMessage (mirrors CDriverLoader::FltSendEx).
// Applies XOR(session-key) + EncryptMunge<16> before sending.
// ─────────────────────────────────────────────────────────────────────────────
HRESULT PyArkSend(LPVOID inBuf,  DWORD inSize,
                  LPVOID outBuf, DWORD outSize,
                  DWORD *pBytesRet = nullptr);
