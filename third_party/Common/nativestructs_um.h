#ifndef NATIVESTRUCTS_H
#define NATIVESTRUCTS_H

#pragma once

#include <Windows.h>

#define ULONG_TO_ULONG64(addr) ((ULONG64)addr & 0xFFFFFFFF)
#define PVOID_TO_ULONG64(addr) (sizeof(addr) == 4 ? ULONG_TO_ULONG64(addr) : (ULONG64)addr)

typedef struct _SYSTEM_TIME_INFORMATION {
	LARGE_INTEGER liKeBootTime;
	LARGE_INTEGER liKeSystemTime;
	LARGE_INTEGER liExpTimeZoneBias;
	ULONG uCurrentTimeZoneId;
	DWORD dwReserved;
} SYSTEM_TIME_INFORMATION;

template<class T>
struct _SYSTEM_BASIC_INFORMATION_T
{
	ULONG Reserved;
	ULONG TimerResolution;
	ULONG PageSize;
	ULONG NumberOfPhysicalPages;
	ULONG LowestPhysicalPageNumber;
	ULONG HighestPhysicalPageNumber;
	T AllocationGranularity;
	T MinimumUserModeAddress;
	T MaximumUserModeAddress;
	T ActiveProcessorsAffinityMask;
	CCHAR NumberOfProcessors;
};

template<class T>
struct _PROCESS_BASIC_INFORMATION_T {
	T ExitStatus;
	T PebBaseAddress;
	T AffinityMask;
	T BasePriority;
	T UniqueProcessId;
	T InheritedFromUniqueProcessId;
};

#pragma pack(push)
#pragma pack(1)

template <class T>
struct _LIST_ENTRY_T
{
	T Flink;
	T Blink;
};

template <class T>
struct _UNICODE_STRING_T
{
	union
	{
		struct
		{
			WORD Length;
			WORD MaximumLength;
		};
		T dummy;
	};
	T Buffer;
};

template <class T>
struct _NT_TIB_T
{
	T ExceptionList;
	T StackBase;
	T StackLimit;
	T SubSystemTib;
	T FiberData;
	T ArbitraryUserPointer;
	T Self;
};

template <class T>
struct _CLIENT_ID_T_
{
	T UniqueProcess;
	T UniqueThread;
};

template <class T>
struct _TEB_T_
{
	_NT_TIB_T<T> NtTib;
	T EnvironmentPointer;
	_CLIENT_ID_T_<T> ClientId;
	T ActiveRpcHandle;
	T ThreadLocalStoragePointer;
	T ProcessEnvironmentBlock;
	DWORD LastErrorValue;
	DWORD CountOfOwnedCriticalSections;
	T CsrClientThread;
	T Win32ThreadInfo;
	DWORD User32Reserved[26];
	//rest of the structure is not defined for now, as it is not needed
};

template <class T>
struct _LDR_DATA_TABLE_ENTRY_T
{
	_LIST_ENTRY_T<T> InLoadOrderLinks;
	_LIST_ENTRY_T<T> InMemoryOrderLinks;
	_LIST_ENTRY_T<T> InInitializationOrderLinks;
	T DllBase;
	T EntryPoint;
	union
	{
		DWORD SizeOfImage;
		T dummy01;
	};
	_UNICODE_STRING_T<T> FullDllName;
	_UNICODE_STRING_T<T> BaseDllName;
	DWORD Flags;
	WORD LoadCount;
	WORD TlsIndex;
	union
	{
		_LIST_ENTRY_T<T> HashLinks;
		struct
		{
			T SectionPointer;
			T CheckSum;
		};
	};
	union
	{
		T LoadedImports;
		DWORD TimeDateStamp;
	};
	T EntryPointActivationContext;
	T PatchInformation;
	_LIST_ENTRY_T<T> ForwarderLinks;
	_LIST_ENTRY_T<T> ServiceTagLinks;
	_LIST_ENTRY_T<T> StaticLinks;
	T ContextInformation;
	T OriginalBase;
	_LARGE_INTEGER LoadTime;
};

template <class T>
struct _PEB_LDR_DATA_T
{
	DWORD Length;
	DWORD Initialized;
	T SsHandle;
	_LIST_ENTRY_T<T> InLoadOrderModuleList;
	_LIST_ENTRY_T<T> InMemoryOrderModuleList;
	_LIST_ENTRY_T<T> InInitializationOrderModuleList;
	T EntryInProgress;
	DWORD ShutdownInProgress;
	T ShutdownThreadId;

};

template <class T, class NGF, int A>
struct _PEB_T
{
	union
	{
		struct
		{
			BYTE InheritedAddressSpace;
			BYTE ReadImageFileExecOptions;
			BYTE BeingDebugged;
			BYTE BitField;
		};
		T dummy01;
	};
	T Mutant;
	T ImageBaseAddress;
	T Ldr;
	T ProcessParameters;
	T SubSystemData;
	T ProcessHeap;
	T FastPebLock;
	T AtlThunkSListPtr;
	T IFEOKey;
	T CrossProcessFlags;
	T UserSharedInfoPtr;
	DWORD SystemReserved;
	DWORD AtlThunkSListPtr32;
	T ApiSetMap;
	T TlsExpansionCounter;
	T TlsBitmap;
	DWORD TlsBitmapBits[2];
	T ReadOnlySharedMemoryBase;
	T HotpatchInformation;
	T ReadOnlyStaticServerData;
	T AnsiCodePageData;
	T OemCodePageData;
	T UnicodeCaseTableData;
	DWORD NumberOfProcessors;
	union
	{
		DWORD NtGlobalFlag;
		NGF dummy02;
	};
	LARGE_INTEGER CriticalSectionTimeout;
	T HeapSegmentReserve;
	T HeapSegmentCommit;
	T HeapDeCommitTotalFreeThreshold;
	T HeapDeCommitFreeBlockThreshold;
	DWORD NumberOfHeaps;
	DWORD MaximumNumberOfHeaps;
	T ProcessHeaps;
	T GdiSharedHandleTable;
	T ProcessStarterHelper;
	T GdiDCAttributeList;
	T LoaderLock;
	DWORD OSMajorVersion;
	DWORD OSMinorVersion;
	WORD OSBuildNumber;
	WORD OSCSDVersion;
	DWORD OSPlatformId;
	DWORD ImageSubsystem;
	DWORD ImageSubsystemMajorVersion;
	T ImageSubsystemMinorVersion;
	T ActiveProcessAffinityMask;
	T GdiHandleBuffer[A];
	T PostProcessInitRoutine;
	T TlsExpansionBitmap;
	DWORD TlsExpansionBitmapBits[32];
	T SessionId;
	ULARGE_INTEGER AppCompatFlags;
	ULARGE_INTEGER AppCompatFlagsUser;
	T pShimData;
	T AppCompatInfo;
	_UNICODE_STRING_T<T> CSDVersion;
	T ActivationContextData;
	T ProcessAssemblyStorageMap;
	T SystemDefaultActivationContextData;
	T SystemAssemblyStorageMap;
	T MinimumStackCommit;
	T FlsCallback;
	_LIST_ENTRY_T<T> FlsListHead;
	T FlsBitmap;
	DWORD FlsBitmapBits[4];
	T FlsHighIndex;
	T WerRegistrationData;
	T WerShipAssertPtr;
	T pContextData;
	T pImageHeaderHash;
	T TracingFlags;
};

template<class T>
struct _SYSTEM_MODULE_INFORMATION_ENTRY_T {
	T Section;
	T MappedBase;
	T Base;
	ULONG Size;
	ULONG Flags;
	USHORT LoadOrderIndex;
	USHORT InitOrderIndex;
	USHORT LoadCount;
	USHORT PathLength;
	CHAR ImageName[256];
};

template<class T>
struct _RTL_USER_PROCESS_PARAMETERS_T {
	ULONG						MaximumLength;//+0
	ULONG						Length;//+4
	ULONG						Flags;//+8
	ULONG						DebugFlags;//+12
	T							ConsoleHandle;//+16
	union
	{
		ULONG					ConsoleFlags;//+24
		T dummy;
	};	
	//this should be ULONG be we have qword alignment
	T							StdInputHandle;//+32
	T							StdOutputHandle;//+40
	T							StdErrorHandle;//+48
	_UNICODE_STRING_T<T>        CurrentDirectoryPath;//+56
	T							CurrentDirectoryHandle;//+72
	_UNICODE_STRING_T<T>        DllPath;//+80
	_UNICODE_STRING_T<T>		ImagePathName;//Offset=96 in x64
	_UNICODE_STRING_T<T>		CommandLine;
	//MORE since xp
	T Environment;
	ULONG StartingX;
	ULONG StartingY;
	ULONG CountX;
	ULONG CountY;
	ULONG CountCharsX;
	ULONG CountCharsY;
	ULONG FillAttribute;
	ULONG WindowFlags;
	ULONG ShowWindowFlags;
	_UNICODE_STRING_T<T> WindowTitle;
	_UNICODE_STRING_T<T> DesktopInfo;
	_UNICODE_STRING_T<T> ShellInfo;
	_UNICODE_STRING_T<T> RuntimeData;
};

template<class T>
struct _MEMORY_SECTION_NAME_T
{
	_UNICODE_STRING_T<T> SectionFileName;
	WCHAR NameBuffer[1];
};

template<class T>
struct _DIRECTORY_BASIC_INFORMATION_T {
	_UNICODE_STRING_T<T> ObjectName;
	_UNICODE_STRING_T<T> ObjectTypeName;
};

template<class T>
struct _SYSTEM_THREAD_INFORMATION_T
{
	LARGE_INTEGER KernelTime;
	LARGE_INTEGER UserTime;
	LARGE_INTEGER CreateTime;
	union {
		ULONG WaitTime;
		T Dummy1;
	};
	T StartAddress;
	_CLIENT_ID_T_<T> ClientId;
	LONG Priority;
	LONG BasePriority;
	ULONG ContextSwitches;
	ULONG ThreadState;
	union {
		ULONG WaitReason;
		T Dummy2;
	};
};

template<class T>
struct _SYSTEM_PROCESS_INFORMATION_T
{
	ULONG NextEntryOffset;
	ULONG NumberOfThreads;
	LARGE_INTEGER SpareLi1;
	LARGE_INTEGER SpareLi2;
	LARGE_INTEGER SpareLi3;
	LARGE_INTEGER CreateTime;
	LARGE_INTEGER UserTime;
	LARGE_INTEGER KernelTime;
	_UNICODE_STRING_T<T> ImageName;
	union {
		LONG BasePriority;
		T Dummy1;
	};
	T UniqueProcessId;
	T InheritedFromUniqueProcessId;
	ULONG HandleCount;
	ULONG SessionId;
	T PageDirectoryBase;

	//
	// This part corresponds to VM_COUNTERS_EX.
	// NOTE: *NOT* THE SAME AS VM_COUNTERS!
	//
	T PeakVirtualSize;
	union {
		ULONG VirtualSize;
		T Dummy2;
	};
	T PageFaultCount;
	T PeakWorkingSetSize;
	T WorkingSetSize;
	T QuotaPeakPagedPoolUsage;
	T QuotaPagedPoolUsage;
	T QuotaPeakNonPagedPoolUsage;
	T QuotaNonPagedPoolUsage;
	T PagefileUsage;
	T PeakPagefileUsage;
	T PrivatePageCount;

	//
	// This part corresponds to IO_COUNTERS
	//
	LARGE_INTEGER ReadOperationCount;
	LARGE_INTEGER WriteOperationCount;
	LARGE_INTEGER OtherOperationCount;
	LARGE_INTEGER ReadTransferCount;
	LARGE_INTEGER WriteTransferCount;
	LARGE_INTEGER OtherTransferCount;

	_SYSTEM_THREAD_INFORMATION_T<T> Threads[1];
};

typedef struct _UNICODE_STRING_T<DWORD> UNICODE_STRING32, *PUNICODE_STRING32;
typedef struct _UNICODE_STRING_T<DWORD64> UNICODE_STRING64, *PUNICODE_STRING64;

typedef struct _LIST_ENTRY_T<DWORD> LIST_ENTRY_32, *PLIST_ENTRY_32;
typedef struct _LIST_ENTRY_T<DWORD64> LIST_ENTRY_64, *PLIST_ENTRY_64;

template<class T>
struct _SYSTEM_MODULE_INFORMATION_T {
	T Count;
	struct _SYSTEM_MODULE_INFORMATION_ENTRY_T<T> Module[1];
};

typedef _SYSTEM_MODULE_INFORMATION_T<DWORD> SYSTEM_MODULE_INFORMATION32, *PSYSTEM_MODULE_INFORMATION32;
typedef _SYSTEM_MODULE_INFORMATION_T<DWORD64> SYSTEM_MODULE_INFORMATION64, *PSYSTEM_MODULE_INFORMATION64;
typedef _SYSTEM_MODULE_INFORMATION_T<PVOID> SYSTEM_MODULE_INFORMATION, *PSYSTEM_MODULE_INFORMATION;

typedef _RTL_USER_PROCESS_PARAMETERS_T<DWORD> RTL_USER_PROCESS_PARAMETERS32, *PRTL_USER_PROCESS_PARAMETERS32;
typedef _RTL_USER_PROCESS_PARAMETERS_T<DWORD64> RTL_USER_PROCESS_PARAMETERS64, *PRTL_USER_PROCESS_PARAMETERS64;

typedef _LDR_DATA_TABLE_ENTRY_T<DWORD> LDR_DATA_TABLE_ENTRY32;
typedef _LDR_DATA_TABLE_ENTRY_T<DWORD64> LDR_DATA_TABLE_ENTRY64;

typedef _TEB_T_<DWORD> TEB32;
typedef _TEB_T_<DWORD64> TEB64;

typedef _PEB_LDR_DATA_T<DWORD> PEB_LDR_DATA32;
typedef _PEB_LDR_DATA_T<DWORD64> PEB_LDR_DATA64;

typedef _PEB_T<DWORD, DWORD64, 34> PEB32;
typedef _PEB_T<DWORD64, DWORD, 30> PEB64;

typedef  _MEMORY_SECTION_NAME_T<DWORD> MEMORY_SECTION_NAME32, *PMEMORY_SECTION_NAME32;
typedef  _MEMORY_SECTION_NAME_T<DWORD64> MEMORY_SECTION_NAME64, *PMEMORY_SECTION_NAME64;

typedef _PROCESS_BASIC_INFORMATION_T<DWORD> PROCESS_BASIC_INFORMATION32, *PPROCESS_BASIC_INFORMATION32;
typedef _PROCESS_BASIC_INFORMATION_T<DWORD64> PROCESS_BASIC_INFORMATION64, *PPROCESS_BASIC_INFORMATION64;

typedef _SYSTEM_BASIC_INFORMATION_T<DWORD> SYSTEM_BASIC_INFORMATION32, *PSYSTEM_BASIC_INFORMATION32;
typedef _SYSTEM_BASIC_INFORMATION_T<DWORD64> SYSTEM_BASIC_INFORMATION64, *PSYSTEM_BASIC_INFORMATION64;

typedef _DIRECTORY_BASIC_INFORMATION_T<PVOID> DIRECTORY_BASIC_INFORMATION, *PDIRECTORY_BASIC_INFORMATION;
typedef _DIRECTORY_BASIC_INFORMATION_T<DWORD> DIRECTORY_BASIC_INFORMATION32, *PDIRECTORY_BASIC_INFORMATION32;
typedef _DIRECTORY_BASIC_INFORMATION_T<DWORD64> DIRECTORY_BASIC_INFORMATION64, *PDIRECTORY_BASIC_INFORMATION64;

typedef _SYSTEM_PROCESS_INFORMATION_T <DWORD> SYSTEM_PROCESS_INFORMATION32, *PSYSTEM_PROCESS_INFORMATION32;
typedef _SYSTEM_PROCESS_INFORMATION_T <DWORD64> SYSTEM_PROCESS_INFORMATION64, *PSYSTEM_PROCESS_INFORMATION64;

struct _XSAVE_FORMAT64
{
	WORD ControlWord;
	WORD StatusWord;
	BYTE TagWord;
	BYTE Reserved1;
	WORD ErrorOpcode;
	DWORD ErrorOffset;
	WORD ErrorSelector;
	WORD Reserved2;
	DWORD DataOffset;
	WORD DataSelector;
	WORD Reserved3;
	DWORD MxCsr;
	DWORD MxCsr_Mask;
	_M128A FloatRegisters[8];
	_M128A XmmRegisters[16];
	BYTE Reserved4[96];
};

struct _CONTEXT64
{
	DWORD64 P1Home;
	DWORD64 P2Home;
	DWORD64 P3Home;
	DWORD64 P4Home;
	DWORD64 P5Home;
	DWORD64 P6Home;
	DWORD ContextFlags;
	DWORD MxCsr;
	WORD SegCs;
	WORD SegDs;
	WORD SegEs;
	WORD SegFs;
	WORD SegGs;
	WORD SegSs;
	DWORD EFlags;
	DWORD64 Dr0;
	DWORD64 Dr1;
	DWORD64 Dr2;
	DWORD64 Dr3;
	DWORD64 Dr6;
	DWORD64 Dr7;
	DWORD64 Rax;
	DWORD64 Rcx;
	DWORD64 Rdx;
	DWORD64 Rbx;
	DWORD64 Rsp;
	DWORD64 Rbp;
	DWORD64 Rsi;
	DWORD64 Rdi;
	DWORD64 R8;
	DWORD64 R9;
	DWORD64 R10;
	DWORD64 R11;
	DWORD64 R12;
	DWORD64 R13;
	DWORD64 R14;
	DWORD64 R15;
	DWORD64 Rip;
	_XSAVE_FORMAT64 FltSave;
	_M128A Header[2];
	_M128A Legacy[8];
	_M128A Xmm0;
	_M128A Xmm1;
	_M128A Xmm2;
	_M128A Xmm3;
	_M128A Xmm4;
	_M128A Xmm5;
	_M128A Xmm6;
	_M128A Xmm7;
	_M128A Xmm8;
	_M128A Xmm9;
	_M128A Xmm10;
	_M128A Xmm11;
	_M128A Xmm12;
	_M128A Xmm13;
	_M128A Xmm14;
	_M128A Xmm15;
	_M128A VectorRegister[26];
	DWORD64 VectorControl;
	DWORD64 DebugControl;
	DWORD64 LastBranchToRip;
	DWORD64 LastBranchFromRip;
	DWORD64 LastExceptionToRip;
	DWORD64 LastExceptionFromRip;
};

typedef struct _FILE_ID_BOTH_DIR_INFORMATION {
	ULONG         NextEntryOffset;
	ULONG         FileIndex;
	LARGE_INTEGER CreationTime;
	LARGE_INTEGER LastAccessTime;
	LARGE_INTEGER LastWriteTime;
	LARGE_INTEGER ChangeTime;
	LARGE_INTEGER EndOfFile;
	LARGE_INTEGER AllocationSize;
	ULONG         FileAttributes;
	ULONG         FileNameLength;
	ULONG         EaSize;
	CCHAR         ShortNameLength;
	WCHAR         ShortName[12];
	LARGE_INTEGER FileId;
	WCHAR         FileName[1];
} FILE_ID_BOTH_DIR_INFORMATION, *PFILE_ID_BOTH_DIR_INFORMATION;

#pragma pack(pop)

typedef struct _SYSTEM_CODEINTEGRITY_INFORMATION {
	ULONG  Length;
	ULONG  CodeIntegrityOptions;
} SYSTEM_CODEINTEGRITY_INFORMATION, *PSYSTEM_CODEINTEGRITY_INFORMATION;


#define SystemCodeIntegrityInformation 0x67
#define CODEINTEGRITY_OPTION_HVCI_KMCI_ENABLED 0x400
#define CODEINTEGRITY_OPTION_HVCI_KMCI_AUDITMODE_ENABLED 0x800
#define CODEINTEGRITY_OPTION_HVCI_KMCI_STRICTMODE_ENABLED 0x1000

typedef struct _API_SET_VALUE_ENTRY_10
{
	ULONG Flags;
	ULONG NameOffset;
	ULONG NameLength;
	ULONG ValueOffset;
	ULONG ValueLength;
} API_SET_VALUE_ENTRY_10, *PAPI_SET_VALUE_ENTRY_10;

typedef struct _API_SET_VALUE_ARRAY_10
{
	ULONG Flags;
	ULONG NameOffset;
	ULONG Unk;
	ULONG NameLength;
	ULONG DataOffset;
	ULONG Count;
} API_SET_VALUE_ARRAY_10, *PAPI_SET_VALUE_ARRAY_10;

typedef struct _API_SET_NAMESPACE_ENTRY_10
{
	ULONG Limit;
	ULONG Size;
} API_SET_NAMESPACE_ENTRY_10, *PAPI_SET_NAMESPACE_ENTRY_10;

typedef struct _API_SET_NAMESPACE_ARRAY_10
{
	ULONG Version;
	ULONG Size;
	ULONG Flags;
	ULONG Count;
	ULONG Start;
	ULONG End;
	ULONG Unk[2];
} API_SET_NAMESPACE_ARRAY_10, *PAPI_SET_NAMESPACE_ARRAY_10;

#endif // NATIVESTRUCTS_H
