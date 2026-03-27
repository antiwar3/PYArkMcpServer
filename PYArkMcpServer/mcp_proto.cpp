#include "pch.h"
#include "mcp_proto.h"

// ─────────────────────────────────────────────────────────────────────────────
// String helpers
// ─────────────────────────────────────────────────────────────────────────────
std::string W2U8(const wchar_t *w, int cch)
{
    if (!w || cch == 0) return {};
    int n = WideCharToMultiByte(CP_UTF8, 0, w, cch, nullptr, 0, nullptr, nullptr);
    if (n <= 0) return {};
    std::string s(n, '\0');
    WideCharToMultiByte(CP_UTF8, 0, w, cch, &s[0], n, nullptr, nullptr);
    while (!s.empty() && s.back() == '\0') s.pop_back();
    return s;
}

std::string ProtStr(ULONG p)
{
    static const struct { ULONG v; const char *n; } T[] = {
        {0x01,"NOACCESS"},{0x02,"READONLY"},{0x04,"READWRITE"},
        {0x08,"WRITECOPY"},{0x10,"EXECUTE"},{0x20,"EXECUTE_READ"},
        {0x40,"EXECUTE_READWRITE"},{0x80,"EXECUTE_WRITECOPY"},
    };
    std::string r;
    for (auto &e : T) if ((p & 0xFF) == e.v) { r = e.n; break; }
    if (r.empty()) { char b[12]; sprintf_s(b, "0x%02X", p & 0xFF); r = b; }
    if (p & 0x100) r += "|GUARD";
    if (p & 0x200) r += "|NOCACHE";
    return r;
}

const char *MemState(ULONG s)
{
    if (s == 0x1000)  return "COMMIT";
    if (s == 0x2000)  return "RESERVE";
    if (s == 0x10000) return "FREE";
    return "UNKNOWN";
}

const char *MemType(ULONG t)
{
    if (t == 0x20000)   return "PRIVATE";
    if (t == 0x40000)   return "MAPPED";
    if (t == 0x1000000) return "IMAGE";
    return "";
}

void WriteId(rapidjson::Writer<rapidjson::StringBuffer> &w,
             const rapidjson::Value &id)
{
    if      (id.IsString())  w.String(id.GetString());
    else if (id.IsInt64())   w.Int64(id.GetInt64());
    else if (id.IsUint64())  w.Uint64(id.GetUint64());
    else                     w.Null();
}

// ─────────────────────────────────────────────────────────────────────────────
// MCP stdio framing
// ─────────────────────────────────────────────────────────────────────────────
std::string ReadMsg()
{
    // Accumulate header bytes until \r\n\r\n
    std::string hdr; char c;
    for (;;) {
        if (fread(&c, 1, 1, stdin) != 1) return {};
        hdr += c;
        size_t n = hdr.size();
        if (n >= 4 &&
            hdr[n-4]=='\r' && hdr[n-3]=='\n' &&
            hdr[n-2]=='\r' && hdr[n-1]=='\n') break;
    }

    // Parse Content-Length
    size_t p = hdr.find("Content-Length:");
    if (p == std::string::npos) p = hdr.find("content-length:");
    if (p == std::string::npos) return {};
    p += 15;
    while (p < hdr.size() && hdr[p] == ' ') ++p;
    size_t clen = (size_t)strtoull(hdr.c_str() + p, nullptr, 10);
    if (!clen) return {};

    std::string body(clen, '\0');
    if (fread(&body[0], 1, clen, stdin) != clen) return {};
    return body;
}

void WriteMsg(const std::string &body)
{
    char hdr[64];
    sprintf_s(hdr, "Content-Length: %zu\r\n\r\n", body.size());
    fwrite(hdr,        1, strlen(hdr), stdout);
    fwrite(body.c_str(),1, body.size(), stdout);
    fflush(stdout);
}

// ─────────────────────────────────────────────────────────────────────────────
// Response builders
// ─────────────────────────────────────────────────────────────────────────────
std::string ToolResp(const rapidjson::Value &id,
                     const std::string &jsonText, bool isError)
{
    rapidjson::StringBuffer sb;
    rapidjson::Writer<rapidjson::StringBuffer> w(sb);
    w.StartObject();
      w.Key("jsonrpc"); w.String("2.0");
      w.Key("id");      WriteId(w, id);
      w.Key("result");
      w.StartObject();
        w.Key("content");
        w.StartArray();
          w.StartObject();
            w.Key("type"); w.String("text");
            w.Key("text"); w.String(jsonText.c_str());
          w.EndObject();
        w.EndArray();
        w.Key("isError"); w.Bool(isError);
      w.EndObject();
    w.EndObject();
    return sb.GetString();
}

std::string ErrResp(const rapidjson::Value &id, int code, const char *message)
{
    rapidjson::StringBuffer sb;
    rapidjson::Writer<rapidjson::StringBuffer> w(sb);
    w.StartObject();
      w.Key("jsonrpc"); w.String("2.0");
      w.Key("id");      WriteId(w, id);
      w.Key("error");
      w.StartObject();
        w.Key("code");    w.Int(code);
        w.Key("message"); w.String(message);
      w.EndObject();
    w.EndObject();
    return sb.GetString();
}
