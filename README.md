# PYArkMcpServer

**English** | [中文](#中文)

---

## English

**PYArkMcpServer** is the **MCP (Model Context Protocol) bridge** for the [PYArk](https://github.com/antiwar3) anti-rootkit toolkit. It lets Claude Desktop and other MCP-compatible AI tools call PYArk's kernel-level system analysis capabilities through the standard MCP protocol.

```
Claude Desktop ──MCP stdio──► PYArkMcpServer.exe ──Named Pipe──► PYArkClient.exe ──IOCTL──► PYArk Driver
```

### Features

64 tools across 15 categories:

| Category | Tools |
|---|---|
| Process / Thread / Module | `list_processes`, `list_threads`, `list_modules`, `list_system_modules`, `get_process_info`, `get_process_peb`, `get_process_cur_dir`, `get_process_image_base`, `get_image_basic_info`, `get_image_path`, `get_image_entry`, `get_function_entry` |
| Memory | `query_memory`, `enum_memory_regions`, `read_memory`, `write_memory`, `protect_memory` |
| Handles | `enum_process_handles`, `close_file_handle`, `kill_handle` |
| Process / Thread Control | `kill_process`, `suspend_process`, `resume_process`, `kill_thread`, `suspend_thread`, `resume_thread`, `protect_process`, `set_protect_process` |
| Kernel Callbacks | `enum_callbacks`, `remove_callback` |
| Filter Drivers | `enum_filters`, `remove_filter` |
| SSDT / IDT / Inline Hooks | `enum_ssdt_hooks`, `enum_shadow_ssdt_hooks`, `recover_ssdt`, `recover_shadow_ssdt`, `enum_idt_hooks`, `enum_message_hooks`, `enum_sys_inline_hooks` |
| System Threads & Timers | `enum_system_threads`, `enum_io_timers`, `enum_dpc_timers`, `remove_io_timer`, `run_stop_io_timer`, `stop_dpc_timer` |
| Driver Scanning | `scan_driver_routines`, `scan_unloaded_drivers` |
| Object Hooks | `enum_object_types`, `enum_object_hooks` |
| Page Tables (4-level) | `enum_process_pml4`, `enum_process_pdpt`, `enum_process_pd`, `enum_process_pt` |
| Privileges / Windows / Hotkeys | `enum_process_privileges`, `enum_process_windows`, `enum_process_hotkeys`, `remove_process_hotkey` |
| WFP Callouts | `get_wfp_callout_info`, `remove_wfp_callout` |
| Driver / Module Control | `unload_driver`, `remove_module_peb`, `unload_process_module`, `bb_inject`, `bb_map_driver` |
| Dump & Utilities | `set_dump_path`, `dump_process`, `dump_process_module`, `dump_driver_memory`, `look_file_lock_info`, `dos_to_nt_path`, `set_option_state` |

### Architecture

```
┌──────────────────────────────────────────────────────────┐
│  Claude Desktop / any MCP client                         │
│               │  MCP JSON-RPC (stdio)                    │
└───────────────┼──────────────────────────────────────────┘
                ▼
┌──────────────────────────────────────────────────────────┐
│  PYArkMcpServer.exe                                      │
│  ├── main.cpp        MCP protocol + 64 tool schemas      │
│  ├── mcp_proto.cpp   Content-Length framing / JSON-RPC   │
│  └── driver_conn.cpp Named-pipe relay to PYArkClient     │
│               │  Named Pipe: \\.\pipe\PYArkMcpServer     │
└───────────────┼──────────────────────────────────────────┘
                ▼
┌──────────────────────────────────────────────────────────┐
│  PYArkClient.exe  (mcp_pipe_server.cpp embedded)         │
│  Receives request → calls drvinterface() → returns JSON  │
│               │  Filter Communication Port               │
└───────────────┼──────────────────────────────────────────┘
                ▼
┌──────────────────────────────────────────────────────────┐
│  PYArk Kernel Driver  (PYArkSafe64.sys)                  │
│  Kernel-level process / thread / memory / hook analysis  │
└──────────────────────────────────────────────────────────┘
```

**Named-pipe protocol:**
```
DWORD(length) + UTF-8 JSON body
Request:  {"tool": "list_processes", "args": {}}
Response: [{"pid": 4, "name": "System", ...}, ...]
```

### Build

**Requirements**
- Visual Studio 2019+ (C++17, x64)
- Windows SDK 10.0+
- Dependencies already bundled in `third_party/`:
  - [RapidJSON](https://github.com/Tencent/rapidjson) (header-only)
  - PYArk common protocol headers (`driverprotocol.h`, `rc4.h`, …)

**Steps**
```
1. Open  PYArkMcpServer/PYArkMcpServer.vcxproj  in Visual Studio
2. Select  Release | x64
3. Build → PYArkMcpServer.exe
```

> Make sure `PYArkClient.exe` (with the PYArk driver loaded) is running before starting the MCP server.

### Configure Claude Desktop

Edit `%APPDATA%\Claude\claude_desktop_config.json`:

```json
{
  "mcpServers": {
    "PYArk": {
      "command": "C:\\path\\to\\PYArkMcpServer.exe"
    }
  }
}
```

Restart Claude Desktop. The 64 PYArk tools will appear in the tool bar.

### Usage Examples

Ask Claude in natural language:

> "List all processes and find any with a debug port set."

> "Enumerate SSDT hooks and show which syscalls have been hijacked."

> "Read 256 bytes from PID 1234 at address 0x7FF000000000."

> "Scan all driver routines and flag entries with inline-hook markers."

> "Enumerate kernel callbacks and analyze the CreateProcess callback chain."

### Notes

- Requires **administrator privileges**
- PYArk driver must be loaded (`PYArkClient.exe` running)
- Destructive operations (write memory, kill process, unload driver, etc.) are irreversible — use with care
- For legitimate use only: security research, reverse engineering, anti-cheat development

---

## 中文

**PYArkMcpServer** 是 [PYArk（盘雨ARK）](https://github.com/antiwar3) 反内核工具包的 **MCP（Model Context Protocol）桥接服务**，让 Claude Desktop 等 AI 工具能够通过标准 MCP 协议直接调用 PYArk 的驱动级系统分析能力。

```
Claude Desktop ──MCP stdio──► PYArkMcpServer.exe ──Named Pipe──► PYArkClient.exe ──IOCTL──► PYArk Driver
```

### 功能概览

共 **64 个工具**，分 15 个类别：

| 类别 | 工具 |
|---|---|
| 进程 / 线程 / 模块枚举 | `list_processes`, `list_threads`, `list_modules`, `list_system_modules`, `get_process_info`, `get_process_peb`, `get_process_cur_dir`, `get_process_image_base`, `get_image_basic_info`, `get_image_path`, `get_image_entry`, `get_function_entry` |
| 内存操作 | `query_memory`, `enum_memory_regions`, `read_memory`, `write_memory`, `protect_memory` |
| 句柄操作 | `enum_process_handles`, `close_file_handle`, `kill_handle` |
| 进程 / 线程控制 | `kill_process`, `suspend_process`, `resume_process`, `kill_thread`, `suspend_thread`, `resume_thread`, `protect_process`, `set_protect_process` |
| 内核回调 | `enum_callbacks`, `remove_callback` |
| 过滤器驱动 | `enum_filters`, `remove_filter` |
| SSDT / IDT / 内联Hook | `enum_ssdt_hooks`, `enum_shadow_ssdt_hooks`, `recover_ssdt`, `recover_shadow_ssdt`, `enum_idt_hooks`, `enum_message_hooks`, `enum_sys_inline_hooks` |
| 系统线程 & 定时器 | `enum_system_threads`, `enum_io_timers`, `enum_dpc_timers`, `remove_io_timer`, `run_stop_io_timer`, `stop_dpc_timer` |
| 驱动扫描 | `scan_driver_routines`, `scan_unloaded_drivers` |
| 对象类型Hook | `enum_object_types`, `enum_object_hooks` |
| 页表枚举（四级） | `enum_process_pml4`, `enum_process_pdpt`, `enum_process_pd`, `enum_process_pt` |
| 权限 / 窗口 / 热键 | `enum_process_privileges`, `enum_process_windows`, `enum_process_hotkeys`, `remove_process_hotkey` |
| WFP标注 | `get_wfp_callout_info`, `remove_wfp_callout` |
| 驱动 / 模块控制 | `unload_driver`, `remove_module_peb`, `unload_process_module`, `bb_inject`, `bb_map_driver` |
| Dump & 工具 | `set_dump_path`, `dump_process`, `dump_process_module`, `dump_driver_memory`, `look_file_lock_info`, `dos_to_nt_path`, `set_option_state` |

### 架构说明

```
┌──────────────────────────────────────────────────────────┐
│  Claude Desktop / 其他MCP客户端                           │
│               │  MCP JSON-RPC (stdio)                    │
└───────────────┼──────────────────────────────────────────┘
                ▼
┌──────────────────────────────────────────────────────────┐
│  PYArkMcpServer.exe                                      │
│  ├── main.cpp        MCP协议 + 64工具schema注册           │
│  ├── mcp_proto.cpp   Content-Length framing + JSON-RPC   │
│  └── driver_conn.cpp 管道转发至PYArkClient               │
│               │  Named Pipe: \\.\pipe\PYArkMcpServer     │
└───────────────┼──────────────────────────────────────────┘
                ▼
┌──────────────────────────────────────────────────────────┐
│  PYArkClient.exe（内嵌 mcp_pipe_server.cpp）              │
│  接收请求 → 调用 drvinterface() → 返回JSON               │
│               │  Filter Communication Port               │
└───────────────┼──────────────────────────────────────────┘
                ▼
┌──────────────────────────────────────────────────────────┐
│  PYArk 内核驱动（PYArkSafe64.sys）                        │
│  驱动级进程/线程/内存/Hook/回调等系统信息采集              │
└──────────────────────────────────────────────────────────┘
```

**命名管道协议：**
```
DWORD(length) + UTF-8 JSON body
Request:  {"tool": "list_processes", "args": {}}
Response: [{"pid": 4, "name": "System", ...}, ...]
```

### 编译

**依赖**
- Visual Studio 2019+（C++17，x64）
- Windows SDK 10.0+
- 已包含在 `third_party/`：
  - [RapidJSON](https://github.com/Tencent/rapidjson)（header-only）
  - PYArk Common 协议头文件（`driverprotocol.h`、`rc4.h` 等）

**步骤**
```
1. 用 Visual Studio 打开 PYArkMcpServer/PYArkMcpServer.vcxproj
2. 选择 Release | x64
3. 生成 → 输出 PYArkMcpServer.exe
```

> 运行前确保 `PYArkClient.exe`（含 PYArk 驱动）已启动。

### 配置 Claude Desktop

编辑 `%APPDATA%\Claude\claude_desktop_config.json`：

```json
{
  "mcpServers": {
    "PYArk": {
      "command": "C:\\path\\to\\PYArkMcpServer.exe"
    }
  }
}
```

重启 Claude Desktop，工具栏会出现 PYArk 的 64 个工具。

### 使用示例

在 Claude 中直接用自然语言：

> "列出所有进程，找出有调试端口的进程"

> "枚举 SSDT Hook，看看哪些系统调用被劫持了"

> "读取 pid 1234 进程在 0x7FF000000000 处的 256 字节内存"

> "扫描所有驱动例程，找出有内联Hook标记的条目"

> "枚举所有内核回调，分析 CreateProcess 回调链"

### 注意事项

- 需要**管理员权限**运行
- PYArk 驱动必须已加载（`PYArkClient.exe` 运行中）
- 写内存、杀进程、卸载驱动等操作具有破坏性，请谨慎使用
- 仅供安全研究、逆向分析、反作弊开发等合法用途

---

## License

MIT
