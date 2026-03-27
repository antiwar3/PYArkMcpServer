// driverprotocol.h  —  PYArk data structures (public release)
// Connection-specific constants (port name, protocol version) are not
// included in this public release.  See PYArkClient for runtime details.
#ifndef XUBEI_DRIVER_PROTOCOL_H
#define XUBEI_DRIVER_PROTOCOL_H

#pragma once


#else
#endif


//Driver ClientToServer protocol
enum drv_cls_protocol
{
	cls_nop,
	cls_set_capture_enable,//打开关闭事件捕获
	cls_get_system_time,//取内核TickCount
	cls_get_image_path,//取模块路径(dos style)
	cls_get_image_baseinfo,//取模块ImageBase ImageSize 是否是64位模块
	cls_get_process_path,//取进程路径(dos style)
	cls_get_process_cmdline,//取进程PEB->ProcessParameters->CommandLine
	cls_get_process_curdir,//取进程PEB->ProcessParameters->CurrentDirectory
	cls_get_process_baseinfo,//取进程ParentPID、SessionId、BreakOnTermination、是否是64位进程
	cls_read_virtual_memory,//跨进程读内存
	cls_query_virtual_memory,//跨进程取进程MEMORY_BASIC_INFORMATION
	cls_get_ssdt_index,//根据ssdt函数名取ssdt函数index deprecated
	cls_get_system_service_table,
	cls_get_system_service_table_shadow,
	cls_read_kernel_memory,//读内核内存
	cls_get_cioptions,//取g_CiOptions
	cls_is_file_exists,//判断文件是否存在
	cls_get_file_size,//取文件大小
	cls_read_file,//读取文件（文件对象）
cls_get_process_peb,//取进程PEB
cls_kill_process,//杀进程
cls_suspend_process,//暂停进程
cls_resume_process,//恢复进程
cls_is_process_running,//进程是否存活
cls_kill_child_processes,//杀子进程
cls_enum_process,//枚举进程
cls_set_global_inject_file,//设置全局注入dll文件
cls_add_file_redirection,//添加文件重定向(for game)
cls_add_hide_file,//隐藏文件(for game)
cls_add_protect_file,//保护文件(for game)
cls_add_inject_target_process,//添加注入进程的列表
cls_nt2dos_filename,//nt文件名转dos文件名
cls_dos2nt_filename,//dos文件名转nt文件名
cls_add_hide_registry,//隐藏注册表(for game)
cls_get_process_imagebase,//取进程image base	
cls_set_protect_process,//保护进程
cls_set_option_state,
cls_enum_system_modules,//枚举内核模块
cls_scan_driver_routines,//扫描未知驱动的函数
cls_enum_process_handle,//枚举进程的句柄表
cls_scan_memory_drivers,//扫描内存加载的驱动
cls_add_white_process,//将进程添加到信任链
cls_get_file_id,//取文件Id(for NTFS)
cls_protect_file_directory,//保护目录
cls_is_relative_process,//是否在相关进程链中
cls_is_white_process,//是否在白名单进程链中
cls_add_thread_protect,//线程保护
cls_get_function_entry,//获取函数名
cls_get_thread_info,//获取线程信息
cls_add_white_thread,//添加线程白名单
cls_enum_process_threads,//枚举进程的线程
cls_add_allow_load_file,//添加白名单文件（可以无视restrict mode直接加载）
cls_add_kernel_pattern,//添加内核内存特征
cls_get_option_state,
cls_get_thread_stackwalk,//回溯线程调用堆栈
cls_scan_kernel_hook,//扫描内核钩子
cls_add_tes_protect_process,//
cls_get_tes_protect_process,//
cls_get_image_entry,
cls_add_relative_process,//将进程添加到relative链
cls_get_kernel_memory_region,//取内核内存dump区域
cls_is_patchguard_code,
cls_protect_virtual_memory,
cls_write_virtual_memory,
cls_dump_process,//dump进程内存
cls_unload_processmodule,//卸载dll
cls_dump_process_module,//dump进程模块
cls_kill_thread,
cls_suspend_thread,
cls_resume_thread,
cls_close_handle,
cls_enum_window,
cls_enum_dpctimer,
cls_unload_driver,
cls_dump_drivermemory,
cls_enum_callback,
cls_remove_callback,
cls_enum_filter,
cls_enum_iotimer,
cls_remove_iotimer,
cls_runstop_iotimer,
cls_enum_systemthread,
cls_enum_ssdthook,
cls_recovery_ssdthook,
cls_enum_shadowssdthook,
cls_recovery_shadowssdthook,
cls_checksyshook,
cls_enumidthook,
cls_del_inject_target_process,//删除注入进程的列表
cls_enum_messagehook,
cls_close_dep,
cls_protect_process,
cls_enum_object_type,
cls_enum_object_hook,
cls_bb_inject,
cls_NtOpenKey,
cls_NtEnumerateValueKey,
cls_NtEnumerateKey,
cls_NtDeleteKey,
cls_NtCreateKey,
cls_NtSetValueKey,
cls_NtDeleteValueKey,
cls_NtRenameKey,
cls_NtReadFile,
cls_NtWriteFile,
cls_NtCreateFile,
cls_QueryDirectory,
cls_NtDeleteFile,
SetFileNormalAttributeDispatch,
cls_EnumPort,
cls_StartDebugEngine,
cls_Init_Sym,
cls_add_debugtoolname,
cls_enum_hotkey,
cls_remove_hotkey,
cls_memload_kernelmod,
cls_scan_unloaded_drivers,
cls_ForceDeleteFile,
cls_SetDumpPath,
cls_enumprivilege,
cls_change_privilege,
cls_get_pml4,
cls_get_pdpt,
cls_get_pd,
cls_get_pt,
cls_add_gamepath,
cls_sethooksetting,
cls_get_wfp_calloutinfo,
cls_remove_wfp_callout,
cls_stopdpctimer,
cls_del_filter,
cls_open_process,
cls_queryfilelockinfo,
cls_closefilehandle,
};

const int user_routine_function = 0;
const int driver_routine_irpcreate = 1;
const int driver_routine_irpioctl = 2;
const int driver_routine_unload = 3;
const int driver_routine_createprocess = 4;
const int driver_routine_loadimage = 5;
const int driver_routine_registry = 6;
const int driver_routine_threadstart = 7;
const int driver_routine_threadstacktrace = 8;
const int driver_routine_processobcallback = 9;
const int driver_routine_threadobcallback = 10;
const int user_routine_threadstart = 11;
const int driver_routine_foundpattern = 12;
const int user_routine_foundpattern = 13;
const int user_routine_threadstarttrace = 14;
const int user_routine_steamgameoverlay = 15;
const int user_routine_game_crc = 16;
const int driver_routine_irpread = 17;
const int driver_routine_irpwrite = 18;
const int driver_routine_fastio_ioctl = 19;
const int driver_routine_fastio_read = 20;
const int driver_routine_fastio_write = 21;
const int user_routine_virtual_alloc = 22;
const int user_routine_gamepattern = 23;
const int driver_routine_ssdt = 24;
const int driver_routine_sssdt = 25;
const int driver_routine_imageintegrity = 26;

const int routine_flags_nofile = 1;
const int routine_flags_nonimage = 2;
const int routine_flags_notmyimage = 4;
const int routine_flags_inlinehook = 8;
const int routine_flags_invaliddrvobj = 16;
const int routine_flags_fakepath = 32;
const int routine_flags_unreadable = 64;
const int routine_flags_hooktarget = 128;
const int routine_flags_invalidsigning = 256;
const int routine_flags_codemodified = 512;
const int routine_flags_iathook = 1024;

const int protect_obcallback = 1;
const int protect_eprocess_flags = 2;
const int protect_tes_vsandbox = 3;
const int protect_hide_client = 4;
const int protect_hook_openprocedure = 5;
const int protect_tes_vsandbox_ex = 6;

const int kernelpattern_ReadOnly = 1;
const int kernelpattern_NoExecute = 2;
const int kernelpattern_Image = 4;
const int kernelpattern_SysPte = 8;
const int kernelpattern_BigPool = 16;
const int kernelpattern_SessionPool = 32;

const int option_state_restrictmode = 1;
const int option_state_ddimon = 2;
const int option_state_memscan = 3;
const int option_state_memscan_fullspeed = 4;
const int option_state_dedicatemode = 5;
const int option_state_superint3 = 6;
const int option_state_antiantidebug = 7;
const int option_state_hidedrx = 8;
const int option_state_skipException = 9;
const int option_state_newdebugtype = 10;
const int option_state_enumprocesstype = 11;
enum drv_svc_protocol
{
	svc_nop,
	svc_callstack,
	svc_ps_create_process,
	svc_ps_create_thread,
	svc_ps_load_image,
	svc_nt_load_driver,
	svc_nt_open_process,
	svc_nt_open_thread,
	svc_fs_create_file,
	svc_memory_access,
	svc_scan_kernel_pattern,
	svc_memory_alloc,
	svc_pslookup,
	svc_dumpmemory,
	svc_scan_kernel_start,     // 内核扫描开始
	svc_scan_kernel_progress,  // 内核扫描进度
	svc_scan_kernel_complete,  // 内核扫描完成
	svc_maximum

};

#pragma pack(1)


typedef union
{
	ULONG All;
	struct
	{
		ULONG SystemThread : 1;
		ULONG BreakOnTermination : 1;
		ULONG HideFromDebugger : 1;
	}Fields;
}SVC_ThreadFlags;

typedef struct
{
	ULONG head;
	ULONG ver;
	CHAR key[32];

typedef struct
{
	UCHAR protocol;
	UCHAR Type;
	BOOLEAN Enable;
}cls_set_capture_enable_request;

typedef struct
{
	UCHAR protocol;
	ULONG ProcessId;
}cls_pid_request;


typedef struct
{
	UCHAR protocol;
	ULONG ProcessId;
	ULONG64 pEprocess;
	PTOKEN_PRIVILEGES pPrivilegeState;
}cls_changeprivilege_request;

typedef struct
{
	UCHAR protocol;
	ULONG ProcessId;
	ULONG64 pEprocess;
}cls_privilege_request;
typedef struct
{
	UCHAR protocol;
	ULONG ProcessId;

}cls_pml4_request;
typedef struct
{
	UCHAR protocol;
	ULONG ProcessId;
	ULONG Index;

}cls_pdpt_request;
typedef struct
{
	UCHAR protocol;
	ULONG ProcessId;
	ULONG Pml4Index;
	ULONG PtpdIndex;

}cls_pd_request;
typedef struct
{
	UCHAR protocol;
	ULONG ProcessId;
	ULONG Pml4Index;
	ULONG PtpdIndex;
	ULONG PdIndex;
}cls_pt_request;
typedef struct  
{
	UCHAR protocol;
	WCHAR strSys[255];
	ULONG64 uReloadAddress;
	ULONG uReloadSize;
}cls_check_syshook;

typedef struct
{
	UCHAR protocol;
	
}cls_start_debugengine;

typedef struct
{
	UCHAR protocol;
	CHAR bOpen;
	CHAR bHookNtAllocateVirtualMemory;
	CHAR bHookExQueueWorkItem;
	CHAR bHookPsLookupProcessByProcessId;
}cls_hooksetting;

typedef struct _KERNELFUNC_ADDRESS_INFORMATION {
	ULONG64 ulAddress;
	CHAR FuncName[260];
}KERNELFUNC_ADDRESS_INFORMATION, *PKERNELFUNC_ADDRESS_INFORMATION;



typedef struct
{
	UCHAR protocol;
	ULONG ulCount;
	KERNELFUNC_ADDRESS_INFORMATION NtosFuncInfo[1];
}cls_init_symbols;

typedef struct
{
	UCHAR protocol;
	ULONG64 base;
	ULONG64 size;
	BOOLEAN badlldump;
}cls_dumpdriver_request;

typedef struct
{
	UCHAR protocol;
	ULONG type;
	ULONG64 calladdr;
}cls_removecallback_request;

typedef struct
{
	UCHAR protocol;
	ULONG64     DeviceObject;
	BOOLEAN      bRun;
}cls_runstopiotime_request;


typedef struct
{
	UCHAR protocol;
	ULONG64     DeviceObject;
	
}cls_stopdpctime_request;

typedef struct
{
	UCHAR protocol;
	ULONG index;
	ULONG type;
}cls_recoveryssdt_request;
typedef struct
{
	UCHAR protocol;
	ULONG64 timer_entry;
}cls_removeiotime_request;


typedef struct
{
	UCHAR protocol;
	ULONG FilterFlag;
	ULONG64 DeviceObject;
}cls_del_filter_request;

typedef struct
{

	UCHAR protocol;
	PHANDLE hprocess;
	ACCESS_MASK DesiredAccess;
	POBJECT_ATTRIBUTES ObjectAttributes;
	PCLIENT_ID ClientId;

}cls_openprocess_request;



typedef struct _cls_file_request
{
	UCHAR protocol;

	union {
		struct {
			WCHAR* szPath;
		} Delete;

		struct {
			WCHAR* szPath;
			ULONG nPathLen; // 字节大小
			ACCESS_MASK DesiredAccess;
			ULONG  FileAttributes;
			ULONG  ShareAccess;
			ULONG  CreateDisposition;
			ULONG  CreateOptions;
		} Create;

		struct {
			HANDLE hDirectory;
			PVOID  FileInformation;
			ULONG  Length;
			ULONG  FileInformationClass;
			BOOLEAN  ReturnSingleEntry;
			BOOLEAN  RestartScan;
			PULONG pWrittenLen;
		} QueryDirectoryFile;

		struct {
			WCHAR* szPath;
			ULONG nPathLen; // 字节大小
			BOOLEAN bDirectory;
		} QueryFileLock;

		struct {
			ULONG nPid;
			ULONG64 pEprocess;
			ULONG nHandle;
		} CloseHandle;

		struct {
			HANDLE hFile;
			ULONG Attribute;
		}SetFileAttribute;

	} op;

}cls_file_request;






typedef struct 
{
	UCHAR protocol;
}cls_enumcallback_request;
typedef struct
{
	UCHAR protocol;
}cls_enumport_request;
typedef struct
{
	UCHAR protocol;
	ULONG64 DriverObject;
}cls_unloaddriver_request;

typedef struct 
{
	UCHAR protocol;
	ULONG ProcessId;
	ULONG handle;
	ULONG64 HandleObject;
	BOOLEAN bForceKill;
}cls_closehandle_request;

typedef struct
{
	UCHAR protocol;
	ULONG64 Thread;
}cls_tid_request;

typedef struct
{
	UCHAR protocol;
	ULONG ProcessId;
	ULONG64 ImageBase;
	ULONG ImageSize;
}cls_dump_processmodule_request;

typedef struct 
{
	UCHAR protocol;
	ULONG ProcessId;
	ULONG64 base;

}cls_unload_processmodule_request;

typedef struct
{
	UCHAR protocol;
	ULONG timeout;
}cls_threadprotect_request;

typedef struct
{
	UCHAR protocol;
	ULONG ProcessId;
	ULONG Type;
	BOOLEAN Enable;
}cls_pid_protect_request;

typedef struct
{
	UCHAR protocol;
	USHORT Length;
	WCHAR Buffer[1];
}cls_file_data;

typedef struct
{
	UCHAR protocol;
	ULONG ProcessId;
	ULONG64 BaseAddress;
}cls_get_image_request;

typedef struct
{
	ULONG64 ImageBase;
	ULONG ImageSize;
	BOOLEAN Is64Bit;
}cls_get_image_baseinfo_data;

typedef struct
{
	ULONG ParentProcessId;
	ULONGLONG CreateTime;
	ULONGLONG KernelTime;
	ULONGLONG UserTime;
	ULONG SessionId;
	BOOLEAN Is64Bit;
	BOOLEAN IsCritical;
	ULONG64 DebugPort;
}cls_get_process_baseinfo_data;

typedef struct
{
	WCHAR ImagePath[260];
}cls_get_process_imagepath_data;

typedef struct
{
	UCHAR protocol;
	ULONG ProcessId;
	ULONG64 BaseAddress;
	ULONG RegionSize;
}cls_read_vm_request;

typedef struct
{
	UCHAR protocol;
	ULONG ProcessId;
	ULONG64 BaseAddress;
	ULONG RegionSize;
	ULONG Protect;
}cls_protect_vm_request;

typedef struct
{
	UCHAR protocol;
	ULONG ProcessId;
	ULONG64 BaseAddress;
}cls_query_vm_request;

typedef struct
{
	ULONG64 BaseAddress;
	ULONG64 AllocationBase;
	ULONG AllocationProtect;
	ULONG64 RegionSize;
	ULONG State;
	ULONG Protect;
	ULONG Type;
}cls_query_vm_result;

typedef struct
{
	UCHAR protocol;
	char FuncName[64];
}cls_get_ssdt_index_request;

typedef struct
{
	UCHAR protocol;
	ULONG index;
}cls_get_ssdt_address_request;

typedef struct
{
	UCHAR protocol;
	ULONG64 BaseAddress;
	ULONG RegionSize;
}cls_read_km_request;

typedef struct
{
	ULONG crc;
	ULONG64 FileSize;
}cls_get_file_crc_data;

typedef struct
{
	UCHAR Digest[16];
	ULONG64 FileSize;
}cls_get_file_md5_data;

typedef struct
{
	UCHAR protocol;
	ULONG ProcessId;
	BOOLEAN Wow64Peb;
}cls_get_process_peb_request;
typedef struct
{
	UCHAR protocol;
	ULONG CallOutId;

}cls_get_wfp_callout_request;
typedef struct
{
	UCHAR protocol;
	BOOLEAN Is64Bit;
	USHORT Length;
	WCHAR Buffer[1];
}cls_set_globalinject_file_request;

typedef struct
{
	UCHAR protocol;
	USHORT SrcLength;
	USHORT DstLength;
	WCHAR Buffer[1];
}cls_add_redirection_file_request;

typedef struct
{
	UCHAR protocol;
	USHORT NameLength;
	USHORT ParentNameLength;
	WCHAR Buffer[1];
}cls_add_inject_process_request;

typedef struct
{
	UCHAR protocol;
	ULONG64 uSharebase;
	WCHAR Buffer[1];
}cls_enummessagehook_request;


typedef struct
{
	UCHAR protocol;
	ULONG Option;
	BOOLEAN State;
}cls_set_enable_request;

typedef struct
{
	UCHAR protocol;
	USHORT DosNameLength;
	USHORT NtNameLength;
	WCHAR Buffer[1];
}cls_file_operation_request;

typedef struct
{
	UCHAR protocol;
	ULONG64 VirtualAddress;
}cls_get_function_entry_request, cls_scan_kernel_hook_request;

#ifdef UNICODE_STRING_MAX_CHARS

typedef struct
{

	ULONG NextEntryOffset;
	ULONG64 EProcess;
	ULONG ProcessId;
	ULONG ParentId;
	ULONG SessionId;
	ULONG64 CreateTime;
	SHORT ProcessPathLength;
	SHORT ProcessPathMaximumLength;
	ULONG64 ProcessPathBuffer;
	SHORT ProcessNameLength;
	SHORT ProcessNameMaximumLength;
	ULONG64 ProcessNameBuffer;
	
}cls_enum_process_data;



typedef struct  
{
	ULONG NextEntryOffset;
	ULONG64 hWnd;
	ULONG Processid;
	ULONG Threadid;
}cls_enum_window_data;

typedef struct
{
	ULONG Status;
	ULONG RequiredLength;
}cls_common_result_data;

typedef struct
{
	ULONG NextEntryOffset;
	ULONG64 ImageBase;
	ULONG ImageSize;
	ULONG LoadIndex;
	ULONG64 EntryPoint;
	SHORT ImageNameLength;
	SHORT ImageNameMaximumLength;
	ULONG64 ImageNameBuffer;
}cls_enum_sysmodule_data;

typedef struct
{
	ULONG NextEntryOffset;
	ULONG Type;
	ULONG Flags;
	ULONG64 ImageBase;
	ULONG64 RoutineAddress;
	ULONG64 HookRoutineAddress;
	SHORT DriverNameLength;
	SHORT DriverNameMaximumLength;
	ULONG64 DriverNameBuffer;
	SHORT DeviceNameLength;
	SHORT DeviceNameMaximumLength;
	ULONG64 DeviceNameBuffer;
}cls_scan_driver_routines_data;

typedef struct
{
	ULONG NextEntryOffset;
	ULONG64 Object;
	ULONG HandleValue;
	ULONG ObjectTypeIndex;
	ULONG GrantedAccess;
	ULONG PspCid;
	ULONG ReferenceCount;
	SHORT ObjectNameLength;
	SHORT ObjectNameMaximumLength;
	ULONG64 ObjectNameBuffer;
	SHORT ObjectTypeNameLength;
	SHORT ObjectTypeNameMaximumLength;
	ULONG64 ObjectTypeNameBuffer;
}cls_enum_process_handle_data;

typedef struct
{
	ULONG NextEntryOffset;
	ULONG64 ImageBase;
	ULONG ImageSize;
	ULONG PoolSize;
}cls_scan_memory_driver_data;

typedef struct
{
	ULONG64 ImageBase;
	ULONG64 FunctionBegin;
	CHAR FunctionName[512];
}cls_get_function_entry_data;

typedef struct
{
	ULONG64 ImageBase;
	WCHAR ImageName[512];
}cls_get_image_entry_data;

typedef struct
{
	ULONG ProcessId;
	ULONG64 StartAddress;
	ULONG64 CreateTime;
	SVC_ThreadFlags Flags;
}cls_get_thread_info_data;

typedef struct
{
	UCHAR protocol;
	ULONG ProcessId;
	ULONG ThreadId;
}cls_thread_request;

typedef struct
{
	ULONG NextEntryOffset;
	ULONG ProcessId;
	ULONG ThreadId;
	ULONG64 teb;
	ULONG64 StartAddress;
	ULONG64 CreateTime;
	ULONG64 EThread;
	ULONG ContextSwitches;
	ULONG State;
	ULONG Priority;

}cls_enum_process_threads_data;

typedef struct
{
	ULONG NextEntryOffset;
	ULONG64 Function;
}cls_enum_callbacks_data;

typedef struct
{
	ULONG Flags;
	ULONG64 TargetImageBase;
	ULONG64 TargetAddress;
}cls_scan_kernel_hook_data;

typedef struct
{
	UCHAR protocol;
	ULONG PatternFlags;
	UCHAR PatternKey;
	USHORT BufferLength;
	UCHAR Buffer[1];
}cls_add_kernel_pattern_request;

typedef struct
{
	UCHAR protocol;
	ULONG64 RegionBase;
	ULONG64 RegionSize;
	ULONG RegionFlags;
}cls_get_km_region_data;


typedef struct 
{
	int CalloutId;
	DWORD64 CalloutEntry;
	DWORD64 ClassifyCalloutAddress;
	DWORD64 NotifyCalloutAddress;
	DWORD64 FlowDeleteFunctionAddress;
} cls_get_wfp_calloutinfo_data;
#endif

//svc

typedef struct
{
	UCHAR protocol;
	ULONG size;
	ULONG64 time;
	ULONG64 eventId;
	ULONG ProcessId;
	ULONG ThreadId;
}svc_nop_data;

typedef struct
{
	UCHAR protocol;
	ULONG size;
	ULONG64 time;
	ULONG64 eventId;
	//Add for xubei
	BOOLEAN Relative;
	BOOLEAN Create;
	ULONG ProcessId;
	ULONG ThreadId;
	ULONG ParentProcessId;
	ULONGLONG CreateTime;
	BOOLEAN Is64Bit;
	ULONG SessionId;
	WCHAR ImagePath[512];
	WCHAR CommandLine[512];
	ULONG64 Callers[32];
}svc_ps_create_process_data;

static_assert(sizeof(svc_ps_create_process_data) < 4096, "Size Check");

typedef struct
{
	UCHAR protocol;
	ULONG size;
	ULONG64 time;
	ULONG64 eventId;
	BOOLEAN Create;
	ULONG ProcessId;
	ULONG ThreadId;
	ULONG CurProcessId;
	ULONG CurThreadId;
	ULONG64 ThreadStartAddress;
	SVC_ThreadFlags ThreadFlags;
	ULONG64 Callers[32];
}svc_ps_create_thread_data;

static_assert(sizeof(svc_ps_create_thread_data) < 4096, "Size Check");

typedef struct
{
	UCHAR protocol;
	ULONG size;
	ULONG64 time;
	ULONG64 eventId;
	ULONG ProcessId;
	ULONG ThreadId;
	ULONG64 ImageBase;
	ULONG ImageSize;
	WCHAR ImagePath[512];
	ULONG64 Callers[32];
}svc_ps_load_image_data;

static_assert(sizeof(svc_ps_load_image_data) < 4096, "Size Check");

typedef struct
{
	UCHAR protocol;
	ULONG size;
	ULONG64 time;
	ULONG64 eventId;
	ULONG ProcessId;
	ULONG ThreadId;
	ULONG TargetProcessId;
	ULONG DesiredAccess;
	ULONG ResultStatus;
	WCHAR ProcessPath[512];
	ULONG64 Callers[32];
}svc_nt_open_process_data;

static_assert(sizeof(svc_nt_open_process_data) < 4096, "Size Check");

typedef struct
{
	UCHAR protocol;
	ULONG size;
	ULONG64 time;
	ULONG64 eventId;
	ULONG ProcessId;
	ULONG ThreadId;
	ULONG TargetProcessId;
	ULONG TargetThreadId;
	ULONG DesiredAccess;
	ULONG ResultStatus;
	ULONG64 Callers[32];
}svc_nt_open_thread_data;

static_assert(sizeof(svc_nt_open_thread_data) < 4096, "Size Check");

typedef struct
{
	UCHAR protocol;
	ULONG size;
	ULONG64 time;
	ULONG64 eventId;
	ULONG ProcessId;
	ULONG ThreadId;
	ULONG DesiredAccess;
	ULONG Disposition;
	ULONG Options;
	ULONG ShareAccess;
	ULONG Attributes;
	ULONG ResultStatus;
	WCHAR FilePath[512];
	ULONG64 Callers[32];
}svc_fs_create_file_data;

static_assert(sizeof(svc_fs_create_file_data) < 4096, "Size Check");

typedef struct
{
	UCHAR protocol;
	ULONG size;
	ULONG64 time;
	ULONG64 eventId;
	ULONG ProcessId;
	WCHAR ProcessPath[256];
	ULONG TargetProcessId;
	ULONG ThreadId;
	ULONG64 VirtualAddress;
	ULONG AccessCount;
	ULONG KernelFrame;
	ULONG UserFrame;
	ULONG64 CallFrame[32];
}svc_memory_access_data;

static_assert(sizeof(svc_memory_access_data) < 4096, "Size Check");

typedef struct
{
	UCHAR protocol;
	ULONG size;
	ULONG64 time;
	ULONG64 eventId;
	ULONG64 ImageBase;
	ULONG64 VirtualAddress;
	USHORT PatternLength;
	UCHAR PatternKey;
	UCHAR Pattern[128];
	int RegionFlags;
}svc_scan_kernel_pattern_data;

static_assert(sizeof(svc_scan_kernel_pattern_data) < 4096, "Size Check");

// 内核扫描开始消息
typedef struct
{
	UCHAR protocol;
	ULONG size;
	ULONG64 time;
	ULONG64 eventId;
	ULONG PatternCount;      // 特征码数量
	int MixedPatternFlags;   // 混合的扫描标志
	ULONG64 ScanRangeCount;  // 扫描区域数量
}svc_scan_kernel_start_data;

static_assert(sizeof(svc_scan_kernel_start_data) < 4096, "Size Check");

// 内核扫描进度消息
typedef struct
{
	UCHAR protocol;
	ULONG size;
	ULONG64 time;
	ULONG64 eventId;
	ULONG CurrentRangeIndex;     // 当前扫描的范围索引 (1-based)
	ULONG TotalRangeCount;       // 总范围数
	ULONG64 RangeBaseAddress;    // 当前范围的基地址
	ULONG64 RangeSize;           // 当前范围的大小
	int RangeFlags;              // 当前范围的标志
	ULONG64 TotalScannedBytes;   // 已扫描的总字节数
}svc_scan_kernel_progress_data;

static_assert(sizeof(svc_scan_kernel_progress_data) < 4096, "Size Check");

// 内核扫描完成消息
typedef struct
{
	UCHAR protocol;
	ULONG size;
	ULONG64 time;
	ULONG64 eventId;
	ULONG64 TotalScannedBytes;  // 总共扫描的字节数
	ULONG MatchCount;            // 匹配数量
	ULONG ElapsedTimeMs;         // 耗时（毫秒）
}svc_scan_kernel_complete_data;

static_assert(sizeof(svc_scan_kernel_complete_data) < 4096, "Size Check");

typedef struct
{
	UCHAR protocol;
	ULONG size;
	ULONG64 time;
	ULONG64 eventId;
	ULONG ProcessId;
	WCHAR ProcessPath[256];
	ULONG TargetProcessId;
	ULONG ThreadId;
	ULONG64 OldBaseAddress;
	ULONG64 OldRegionSize;
	ULONG64 NewBaseAddress;
	ULONG64 NewRegionSize;
	ULONG AllocationType;
	ULONG Protect;
	ULONG KernelFrame;
	ULONG UserFrame;
	ULONG64 CallFrame[32];
}svc_memory_alloc_data;

static_assert(sizeof(svc_memory_alloc_data) < 4096, "Size Check");

typedef struct
{
	UCHAR protocol;
	ULONG size;
	ULONG64 time;
	ULONG64 eventId;
	ULONG ProcessId;
	WCHAR ProcessPath[256];
	ULONG TargetProcessId;
	ULONG ThreadId;
	ULONG KernelFrame;
	ULONG UserFrame;
	ULONG64 CallFrame[32];
}svc_pslookup_data;

static_assert(sizeof(svc_pslookup_data) < 4096, "Size Check");

typedef struct
{
	UCHAR protocol;
	ULONG size;
	ULONG64 time;
	ULONG64 eventId;
	ULONG pid;
	PVOID shellcodebase;
	ULONG shellcodesize;
}svc_dumpshellcode_data;

static_assert(sizeof(svc_dumpshellcode_data) < 4096, "Size Check");


typedef struct _IO_TIMER_ENTRY_INFORMATION
{
	ULONG64 TimerObject;
	ULONG64 DeviceObject;
	ULONG64 TimeDispatch;
	ULONG64 TimerEntry;
	ULONG64   Status;
} IO_TIMER_ENTRY_INFORMATION, *PIO_TIMER_ENTRY_INFORMATION;

typedef struct _IO_TIMER_INFORMATION
{
	ULONG                   NumberOfIoTimers;
	IO_TIMER_ENTRY_INFORMATION IoTimer[1];
} IO_TIMER_INFORMATION, *PIO_TIMER_INFORMATION;

typedef struct _OPERATION_ON_IO_TIMER_INFORMATION
{
	UINT_PTR     DeviceObject;
	BOOLEAN      bRun;
} OPERATION_ON_IO_TIMER_INFORMATION, *POPERATION_ON_IO_TIMER_INFORMATION;


typedef struct _PROCESS_THREAD_ENTRY_INFORMATION
{
	ULONG64 EThread;
	ULONG   ThreadId;
	ULONG64 Teb;
	ULONG    Priority;
	ULONG64 Win32StartAddress;
	ULONG   ContextSwitches;
	ULONG    State;
} PROCESS_THREAD_ENTRY_INFORMATION, *PPROCESS_THREAD_ENTRY_INFORMATION;


typedef struct _PROCESS_THREAD_INFORMATION
{
	ULONG                           NumberOfThreads;
	ULONG							ncrl;
	ULONG status;
	PROCESS_THREAD_ENTRY_INFORMATION Threads[1];
} PROCESS_THREAD_INFORMATION, *PPROCESS_THREAD_INFORMATION;

////////// 以下结构用来枚举过SSDT HOOK /////////////


typedef struct _SSDT_HOOK_INFO_  // ssdt hook
{
	ULONG nIndex;
	ULONG64 pOriginAddress;
	ULONG64 pNowAddress;
	ULONG64 pInlineHookAddress;
	WCHAR szFunctionName[100];
}SSDT_HOOK_INFO, *PSSDT_HOOK_INFO;

typedef struct _ALL_SSDT_HOOK_ // ssdt hook
{
	ULONG nCnt;
	ULONG count;
	SSDT_HOOK_INFO SsdtHook[1];
}ALL_SSDT_HOOK, *PALL_SSDT_HOOK;

typedef struct _SYS_HOOK_INFO
{
	ULONG PatchLen;
	ULONG64 PatchedAddress;	// 被Patch的地址
	ULONG64 HookAddress;		// Hook后执行的地址
	UCHAR NowBytes[50];
	UCHAR OriginBytes[50];
};
typedef struct _ALL_SYS_HOOK_
{
	ULONG nCnt;
	ULONG count;
	_SYS_HOOK_INFO SysHook[1];
}ALL_SYS_HOOK,*PALL_SYS_HOOK;

typedef struct
{
	ULONG NextEntryOffset;
	ULONG LoadStatus;
	ULONG64 TimeStamp;
	ULONG Type;
	SHORT DriverNameLength;
	SHORT DriverNameMaximumLength;
	ULONG64 DriverNameBuffer;
}cls_scan_unloaded_driver_data;



typedef struct _IDT_HOOK_INFO_
{
	ULONG ncpu;
	ULONG nIndex;
	ULONG64 pNowAddress;
	ULONG64 pOriginAddress64;
	ULONG64 pInlineHookAddress;
}IDT_HOOK_INFO, *PIDT_HOOK_INFO;
typedef struct _ALL_IDT_HOOK_
{
	ULONG nCnt;
	ULONG count;
	IDT_HOOK_INFO IdtHook[1];
}ALL_IDT_HOOK, *PALL_IDT_HOOK;
typedef enum  _HOOK_TYPE {
	R_WH_MSGFILTER = -1,
	R_WH_JOURNALRECORD = 0,
	R_WH_JOURNALPLAYBACK = 1,
	R_WH_KEYBOARD = 2,
	R_WH_GETMESSAGE = 3,
	R_WH_CALLWNDPROC = 4,
	R_WH_CBT = 5,
	R_WH_SYSMSGFILTER = 6,
	R_WH_MOUSE = 7,
	R_WH_HARDWARE = 8,
	R_WH_DEBUG = 9,
	R_WH_SHELL = 10,
	R_WH_FOREGROUNDIDLE = 11,
	R_WH_CALLWNDPROCRET = 12,
	R_WH_KEYBOARD_LL = 13,
	R_WH_MOUSE_LL = 14
}HOOK_TYPE;
typedef struct _MESSAGE_HOOK_INFO
{
	HOOK_TYPE iHookType;
	HANDLE Handle;
	ULONG64 offPfn;
	LONG ihmod;
	ULONG flags;
	ULONG Pid;
	ULONG Tid;
	WCHAR ModuleName[260];
	WCHAR szPath[260];
}MESSAGE_HOOK_INFO, *PMESSAGE_HOOK_INFO;
typedef struct _MESSAGE_HOOK_
{
	ULONG nCnt;
	ULONG nRetCnt;
	MESSAGE_HOOK_INFO MessageHookInfo[1];
}MESSAGE_HOOK, *PMESSAGE_HOOK;



#define MAX_TYPE_NAME 50

typedef struct _OBJECT_TYPE_INFO
{
	ULONG64 ObjectType;
	WCHAR szTypeName[MAX_TYPE_NAME];
}OBJECT_TYPE_INFO, *POBJECT_TYPE_INFO;

typedef enum
{
	eDirectoryType,
	eMutantType,
	eThreadType,
	eFilterCommunicationPortType,
	eControllerType,
	eProfileType,
	eEventType,
	eTypeType,
	eSectionType,
	eEventPairType,
	eSymbolicLinkType,
	eDesktopType,
	eTimerType,
	eFileType,
	eWindowStationType,
	eDriverType,
	eWmiGuidType,
	eKeyedEventType,
	eDeviceType,
	eTokenType,
	eDebugObjectType,
	eIoCompletionType,
	eProcessType,
	eAdapterType,
	eKeyType,
	eJobType,
	eWaitablePortType,
	ePortType,
	eCallbackType,
	eFilterConnectionPortType,
	eSemaphoreType,
	eMaxType,
	eUnknowType,
}ENUM_OBJECT_TYPE;

typedef struct _OBJECT_HOOK_INOF_
{
	ULONG Type;			// 对象索引
	ULONG64 TypeObject;				// 类型对象

	ULONG64 DumpProcedure;			// Now Procedure
	ULONG64 OpenProcedure;
	ULONG64 CloseProcedure;
	ULONG64 DeleteProcedure;
	ULONG64 ParseProcedure;
	ULONG64 SecurityProcedure;
	ULONG64 QueryNameProcedure;
	ULONG64 OkayToCloseProcedure;

	ULONG64 OriginDumpProcedure;		// Origin Procedure
	ULONG64 OriginOpenProcedure;
	ULONG64 OriginCloseProcedure;
	ULONG64 OriginDeleteProcedure;
	ULONG64 OriginParseProcedure;
	ULONG64 OriginSecurityProcedure;
	ULONG64 OriginQueryNameProcedure;
	ULONG64 OriginOkayToCloseProcedure;

	ULONG64 InlineDumpProcedure;			// Inline Procedure
	ULONG64 InlineOpenProcedure;
	ULONG64 InlineCloseProcedure;
	ULONG64 InlineDeleteProcedure;
	ULONG64 InlineParseProcedure;
	ULONG64 InlineSecurityProcedure;
	ULONG64 InlineQueryNameProcedure;
	ULONG64 InlineOkayToCloseProcedure;
}OBJECT_HOOK_INOF, *POBJECT_HOOK_INOF;

typedef struct _COMMUNICATE_KEY_
{
	
	UCHAR protocol;
	union {
		struct {
			ACCESS_MASK DesiredAccess;
			POBJECT_ATTRIBUTES ObjectAttributes;
		} Open;

		struct {
			HANDLE hKey;
			ULONG Index;
			ULONG InformationClass;
			ULONG Length;
		} Enum;

		struct {
			HANDLE hKey;
		} Delete;

		struct {
			HANDLE hKey;
			PUNICODE_STRING ValueName;
			ULONG Type;
			PVOID Data;
			ULONG DataSize;
		} SetValue;

		struct {
			HANDLE hKey;
			PUNICODE_STRING ValueName;
		} DeleteValue;

		struct {
			HANDLE hKey;
			PUNICODE_STRING NewName;
		} RenameKey;
	} op;

}COMMUNICATE_KEY, *PCOMMUNICATE_KEY;

typedef struct _ENUM_VALUE_
{
	PULONG RetLength;
	PVOID pValueInfo;
}ENUM_VALUE, *PENUM_VALUE;
typedef struct _CREATE_KEY_RETURN_
{
	PHANDLE pKeyHandle;
	PULONG  Disposition;
}CREATE_KEY_RETURN, *PCREATE_KEY_RETURN;



typedef struct _COMMUNICATE_FILE_
{
	UCHAR protocol;

	union {
		struct {
			WCHAR *szPath;
		} Delete;

		struct {
			WCHAR *szPath;
			ULONG nPathLen; // 字节大小
			ACCESS_MASK DesiredAccess;
			ULONG  FileAttributes;
			ULONG  ShareAccess;
			ULONG  CreateDisposition;
			ULONG  CreateOptions;
		} Create;

		struct {
			HANDLE hDirectory;
			PVOID  FileInformation;
			ULONG  Length;
			ULONG  FileInformationClass;
			BOOLEAN  ReturnSingleEntry;
			BOOLEAN  RestartScan;
			PULONG pWrittenLen;
		} QueryDirectoryFile;

		struct {
			WCHAR *szPath;
			ULONG nPathLen; // 字节大小
			BOOLEAN bDirectory;
		} QueryFileLock;

		struct {
			ULONG nPid;
			ULONG64 pEprocess;
			ULONG nHandle;
		} CloseHandle;

		struct {
			HANDLE hFile;
			ULONG Attribute;
		}SetFileAttribute;

	} op;

}COMMUNICATE_FILE, *PCOMMUNICATE_FILE;



typedef struct _COMMUNICATE_FILE_OPERATION_
{
	UCHAR protocol;

	union {

		struct {
			WCHAR *szFilePath;
			ULONG nPathLen;  // in bytes
			ACCESS_MASK  DesiredAccess;
			ULONG  FileAttributes;
			ULONG  ShareAccess;
			ULONG  CreateDisposition;
			ULONG  CreateOptions;
		} Create;

		struct {
			HANDLE hFile;
			PVOID pBuffer;
			ULONG nReadBytes;
		} Read;

		struct {
			HANDLE hFile;
			PVOID pBuffer;
			ULONG nWriteBytes;
		} Write;

	} op;

}COMMUNICATE_FILE_OPERATION, *PCOMMUNICATE_FILE_OPERATION;

typedef struct _LOCKED_FILES
{
	ULONG nPid;
	ULONG nHandle;
	WCHAR szFilePath[260 * 2];
}LOCKED_FILES, *PLOCKED_FILES;

typedef struct _LOCKED_FILE_INFO
{
	ULONG nRetCount;
	LOCKED_FILES files[1];
}LOCKED_FILE_INFO, *PLOCKED_FILE_INFO;

typedef enum _PORT_TYPE
{
	enumTcp,
	enumUdp,
}PORT_TYPE;

typedef struct _PORT_INFO_
{
	PORT_TYPE nPortType;
	ULONG nConnectState;
	ULONG nLocalAddress;
	ULONG nLocalPort;
	ULONG nRemoteAddress;
	ULONG nRemotePort;
	ULONG nPid;
}PORT_INFO, *PPORT_INFO;

typedef struct _COMMUNICATE_PORT_
{
	ULONG nCnt;					// 总共有几项
	PORT_INFO Ports[1];
}COMMUNICATE_PORT, *PCOMMUNICATE_PORT;

enum HOTKEY_OPS {
	HOTKEY_ENUM,
	HOTKEY_REMOVE,
};

typedef struct _HOTKEY_ITEM {
	UCHAR protocol;
	UCHAR name[64];
	UINT32 wnd;
	UINT16 mod1;
	UINT16 mod2;
	UINT32 vk;
	UINT32 id;
	UINT32 pid;
	UINT32 tid;
	ULONG64 hkobj;
} HOTKEY_ITEM, *PHOTKEY_ITEM;
typedef struct _HOTKEY_INFO {

	ULONG count;
	HOTKEY_ITEM items[1];
} HOTKEY_INFO, *PHOTKEY_INFO;




#pragma pack()

#endif//XUBEI_DRIVER_PROTOCOL_H