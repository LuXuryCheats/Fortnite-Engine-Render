#include "stdafx.h"

PIMAGE_NT_HEADERS Scanner::getHeader(HMODULE mxdule) {
	return (PIMAGE_NT_HEADERS)((PBYTE)mxdule + PIMAGE_DOS_HEADER(mxdule)->e_lfanew);
}


PBYTE Scanner::FindPattern(PBYTE start, DWORD size, LPCSTR pattern, LPCSTR mask) {

    auto checkMask = [](PBYTE buffer, LPCSTR pattern, LPCSTR mask) -> BOOL
    {
        for (auto x = buffer; *mask; pattern++, mask++, x++) {
            auto addr = *(BYTE*)(pattern);
            if (addr != *x && *mask != '?')
                return FALSE;
        }

        return TRUE;
    };

    for (auto x = 0; x < size - strlen(mask); x++) {

        auto addr = (PBYTE)start + x;
        if (checkMask(addr, pattern, mask)) {
            return addr;
        }
    }
}

PBYTE Scanner::FindPattern(HMODULE mxdule, LPCSTR pattern, LPCSTR mask, DWORD occurence) {

    size_t size = getHeader(mxdule)->OptionalHeader.SizeOfImage;

    auto checkMask = [](PBYTE buffer, LPCSTR pattern, LPCSTR mask) -> BOOL
    {
        for (auto x = buffer; *mask; pattern++, mask++, x++) {
            auto addr = *(BYTE*)(pattern);
            if (addr != *x && *mask != '?')
                return FALSE;
        }

        return TRUE;
    };

    if (occurence)
        occurence -= 1;

    for (auto x = 0; x < size - strlen(mask); x++) {

        auto addr = (PBYTE)mxdule + x;

        if (checkMask(addr, pattern, mask)) {

            if (!occurence || loopCount == occurence && addr) {
                loopCount = NULL; // resets the count. (Incase we need to use it again)
                return addr;
            }

            if (loopCount < occurence) {
                addr = FindPattern((PBYTE)mxdule + x, (PBYTE)size - addr, pattern, mask);
                ++loopCount;
                continue;
            }
        }
    }

    return NULL;
}

uintptr_t Scanner::PatternScan(uintptr_t pModuleBaseAddress, const char* sSignature, size_t nSelectResultIndex)
{
    static auto patternToByte = [](const char* pattern)
    {
        auto       bytes = std::vector<int>{};
        const auto start = const_cast<char*>(pattern);
        const auto end = const_cast<char*>(pattern) + strlen(pattern);

        for (auto current = start; current < end; ++current)
        {
            if (*current == '?')
            {
                ++current;
                if (*current == '?')
                    ++current;
                bytes.push_back(-1);
            }
            else {
                bytes.push_back(strtoul(current, &current, 16));
            }
        }
        return bytes;
    };

    const auto dosHeader = (PIMAGE_DOS_HEADER)pModuleBaseAddress;
    const auto ntHeaders = (PIMAGE_NT_HEADERS)((std::uint8_t*)pModuleBaseAddress + dosHeader->e_lfanew);

    const auto sizeOfImage = ntHeaders->OptionalHeader.SizeOfImage;
    auto       patternBytes = patternToByte(sSignature);
    const auto scanBytes = reinterpret_cast<std::uint8_t*>(pModuleBaseAddress);

    const auto s = patternBytes.size();
    const auto d = patternBytes.data();

    size_t nFoundResults = 0;

    for (auto i = 0ul; i < sizeOfImage - s; ++i)
    {
        bool found = true;

        for (auto j = 0ul; j < s; ++j)
        {
            if (scanBytes[i + j] != d[j] && d[j] != -1)
            {
                found = false;
                break;
            }
        }

        if (found)
        {
            if (nSelectResultIndex != 0)
            {
                if (nFoundResults < nSelectResultIndex)
                {
                    nFoundResults++;                                   // Skip Result To Get nSelectResultIndex.
                    found = false;                                     // Make sure we can loop again.
                }
                else
                {
                    return reinterpret_cast<uintptr_t>(&scanBytes[i]);  // Result By Index.
                }
            }
            else
            {
                return reinterpret_cast<uintptr_t>(&scanBytes[i]);      // Default/First Result.
            }
        }
    }

    return NULL;
}

uintptr_t Scanner::PatternScan(const char* sSignature, size_t nSelectResultIndex)
{
    static bool bIsSetted = false;

    static MODULEINFO info = { 0 };

    if (!bIsSetted)
    {
        GetModuleInformation(GetCurrentProcess(), GetModuleHandle(0), &info, sizeof(info));
        bIsSetted = true;
    }

    return PatternScan((uintptr_t)info.lpBaseOfDll, sSignature, nSelectResultIndex);
}