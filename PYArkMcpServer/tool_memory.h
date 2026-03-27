#pragma once
#include "pch.h"

std::string ToolQueryMemory(ULONG pid, ULONG64 addr);
std::string ToolEnumMemoryRegions(ULONG pid, ULONG64 startAddr, ULONG maxRegions);
std::string ToolReadMemory(ULONG pid, ULONG64 addr, ULONG size);
