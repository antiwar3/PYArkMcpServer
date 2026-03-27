#pragma once
#include "pch.h"

// ─────────────────────────────────────────────────────────────────────────────
// JSON / string utilities
// ─────────────────────────────────────────────────────────────────────────────
std::string  W2U8(const wchar_t *w, int cch = -1);   // UTF-16 → UTF-8
std::string  ProtStr(ULONG protect);                  // VirtualAlloc protection flags → string
const char  *MemState(ULONG state);                   // MEM_COMMIT/RESERVE/FREE
const char  *MemType(ULONG type);                     // MEM_PRIVATE/MAPPED/IMAGE

// Emit a rapidjson "id" value (string | int | null)
void WriteId(rapidjson::Writer<rapidjson::StringBuffer> &w,
             const rapidjson::Value &id);

// ─────────────────────────────────────────────────────────────────────────────
// MCP stdio framing  (Content-Length: N\r\n\r\nJSON)
// ─────────────────────────────────────────────────────────────────────────────
std::string ReadMsg();                          // blocks until one full message arrives
void        WriteMsg(const std::string &body); // writes Content-Length header + body

// ─────────────────────────────────────────────────────────────────────────────
// JSON-RPC 2.0 response builders
// ─────────────────────────────────────────────────────────────────────────────

// tools/call success/error result  (text content wrapper)
std::string ToolResp(const rapidjson::Value &id,
                     const std::string &jsonText,
                     bool isError = false);

// JSON-RPC error response
std::string ErrResp(const rapidjson::Value &id,
                    int code, const char *message);
