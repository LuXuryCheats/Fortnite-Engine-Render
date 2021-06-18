#include "stdafx.h"

#define RELATIVE_ADDR(addr, size)       ((uintptr_t)((uintptr_t)(addr) + *(PINT)((uintptr_t)(addr) + ((size) - sizeof(INT))) + (size)))

#define rva(addr, size) (reinterpret_cast<unsigned char*>(addr + *reinterpret_cast<int*>(addr + (size - 4)) + size))




void Main() {

	

	static DWORD_PTR funcGetObjectName = NULL;

	auto basemodule = GetModuleHandle(0);


	Basic::UWorld_Offset = g_Scanner->FindPattern(basemodule, skCrypt("\x48\x89\x05\x00\x00\x00\x00\x48\x8B\x8B\x00\x00\x00\x00"), skCrypt("xxx????xxx????"), 0);
	Basic::UWorld_Offset = reinterpret_cast<decltype(Basic::UWorld_Offset)>(rva(Basic::UWorld_Offset, 7)); // ((DWORD64)GetModuleHandleW(NULL) + 0x93538B0);
	
	
	
	
	 pGetBoneMatrix = g_Scanner->FindPattern(basemodule, skCrypt("\xE8\x00\x00\x00\x00\x48\x8B\x47\x30\xF3\x0F\x10\x45\x00"), skCrypt("x????xxxxxxxx?"), 0);
	pGetBoneMatrix = reinterpret_cast<decltype(pGetBoneMatrix)>(rva(pGetBoneMatrix, 5));
	


	PBYTE addr = (PBYTE)(DWORD64)GetModuleHandleW(NULL) + 0x5E24844; //maybe wrong
	LOS = reinterpret_cast<decltype(LOS)>(addr);


	/*
	auto CamHook = g_Scanner->FindPattern(basemodule, skCrypt("\xE9\x00\x00\x00\x00\xCC\xCC\xCC\x48\x83\xEC\x28\x4C\x8D\x0D\x00\x00\x00\x00\xBA\x18\x00\x00\x00"), skCrypt("x????xxxxxxxxxx????xx???"), 0);
	nigga_hook = reinterpret_cast<decltype(nigga_hook)>(rva(CamHook, 5));
	MH_CreateHook(CamHook, CameraHook, (PVOID*)&nigga_hook);
	MH_EnableHook(CamHook);
	*/
	
	
	
	//// E9 ? ? ? ? CC CC CC 48 83 EC 28 4C 8D 0D ? ? ? ? BA 18 ? ? ?
	auto address = g_Scanner->FindPattern(basemodule, ("\x0F\x84\x00\x00\x00\x00\x48\x8D\x05\x00\x00\x00\x00\x48\x83\xC4\x28\xC3"), ("xx????xxx????xxxxx"), 0);

	auto offset = *reinterpret_cast<uint32_t*>(address + 9);
	funcGetObjectName = reinterpret_cast<decltype(funcGetObjectName)>(address + 13 + offset);
	
	printf("\n GNames -> 0x%x", funcGetObjectName - (uintptr_t)basemodule);

	

	/*
	auto Gobjects = g_Scanner->FindPattern(basemodule, skCrypt("\x48\x89\x05\x00\x00\x00\x00\xE8\x00\x00\x00\x00\x45\x84\xF6"),
		("xxx????x????xxx"), 0);

	auto Gnames = g_Scanner->FindPattern(basemodule, skCrypt("\x48\x8B\x1D\x00\x00\x00\x00\x48\x03\xC0"),
		"xxx????xxx", 0);
		*/


	/*
	auto BoneMatrix = g_Scanner->FindPattern(basemodule, skCrypt("\x48\x8B\xC4\x48\x89\x58\x08\x48\x89\x70\x10\x48\x89\x78\x20\x44\x89\x40\x18\x55\x41\x54\x41\x55\x41\x56\x41\x57\x48\x8D\x6C\x24\x00\x48\x81\xEC\x00\x00\x00\x00\x0F\x29\x70\xC8"),
		"xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx?xxx????xxxx", 0);
		

	BoneMatrix = reinterpret_cast<decltype(BoneMatrix)>(rva(BoneMatrix, 5));
	*/

	/*
	uintptr_t printfobject = 0;
	const auto offset = *reinterpret_cast<uint32_t*>(Gobjects + 3);
	printfobject = reinterpret_cast<decltype(printfobject)>(Gobjects + 7 + offset);
	*/


//	

	/*
	PBYTE addr = (PBYTE)((DWORD64)GetModuleHandleW(NULL) + 0x5CC50D8); //dissassemble result then it's the 'call'
	LineOfSightToInternal = reinterpret_cast<decltype(LineOfSightToInternal)>(addr);
	*/


	
	hkpost::hkpostexecute();
}

BOOL WINAPI DllMain(HMODULE hModule, DWORD dwReason, LPVOID lpReserved)
{
	if (dwReason == DLL_PROCESS_ATTACH) {
		DisableThreadLibraryCalls(hModule);
	//	AllocConsole();
    //	freopen("conin$", "r", stdin);
   // 	freopen("conout$", "w", stdout);
	//    freopen("conout$", "w", stderr);
		Main();

		
	}

	return TRUE;
}
