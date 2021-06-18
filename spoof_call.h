#pragma once
#include "stdafx.h"

#include <algorithm>

inline uintptr_t Pattern_Scan(uintptr_t pModuleBaseAddress, const char* sSignature, size_t nSelectResultIndex)
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

inline uintptr_t Pattern_Scan(const char* sSignature, size_t nSelectResultIndex)
{
	static bool bIsSetted = false;

	static MODULEINFO info = { 0 };

	if (!bIsSetted)
	{
		GetModuleInformation(GetCurrentProcess(), GetModuleHandle(0), &info, sizeof(info));
		bIsSetted = true;
	}

	return Pattern_Scan((uintptr_t)info.lpBaseOfDll, sSignature, nSelectResultIndex);
}

namespace _SpoofCallInternal
{
	extern "C" PVOID RetSpoofStub();

	template <typename Ret, typename... Args>
	inline Ret Wrapper(PVOID shell, Args... args)
	{
		auto fn = (Ret(*)(Args...))(shell);
		return fn(args...);
	}

	template <std::size_t Argc, typename>
	struct Remapper
	{
		template<typename Ret, typename First, typename Second, typename Third, typename Fourth, typename... Pack>
		static Ret Call(PVOID shell, PVOID shell_param, First first, Second second, Third third, Fourth fourth, Pack... pack)
		{
			return Wrapper<Ret, First, Second, Third, Fourth, PVOID, PVOID, Pack...>(shell, first, second, third, fourth, shell_param, nullptr, pack...);
		}
	};

	template <std::size_t Argc>
	struct Remapper<Argc, std::enable_if_t<Argc <= 4>>
	{
		template<typename Ret, typename First = PVOID, typename Second = PVOID, typename Third = PVOID, typename Fourth = PVOID>
		static Ret Call(PVOID shell, PVOID shell_param, First first = First{}, Second second = Second{}, Third third = Third{}, Fourth fourth = Fourth{}) {
			return Wrapper<Ret, First, Second, Third, Fourth, PVOID, PVOID>(shell, first, second, third, fourth, shell_param, nullptr);
		}
	};
}

template <typename Ret, typename... Args>
Ret SpoofCall(Ret(*fn)(Args...), Args... args)
{
	static uintptr_t pTrampoline = 0;

	if (!pTrampoline)
	{
		pTrampoline = g_Scanner->PatternScan((uintptr_t)GetModuleHandleW(0), skCrypt("FF 27"), 0);
	}

	struct
	{
		PVOID Trampoline;
		PVOID Function;
		PVOID Reg;
	} params = {
		(PVOID)pTrampoline,
		reinterpret_cast<void*>(fn),
	};

	return _SpoofCallInternal::Remapper<sizeof...(Args), void>::template Call<Ret, Args...>(&_SpoofCallInternal::RetSpoofStub, &params, args...);
}