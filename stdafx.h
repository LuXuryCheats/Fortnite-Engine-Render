#pragma once
#define skCrypt(str) _xor_(str).c_str()

static const void* game_rbx_jmp;

#include <set>
#include <string>
#include <Windows.h>
#include <iostream>
#include <Psapi.h>
#include <vector>
#include <d3d11.h>


static PBYTE pGetBoneMatrix = 0;
static PBYTE fnLineOfSightTo = 0;
inline float width, height;

inline bool bAimingLock = false;

#pragma comment(lib, "d3d11.lib")



inline float GlobalFOV = 80.f;

#include "SDK.hpp"
#include "xorstr.hpp"
#include "framework.h"
#include "hkfunctions.h"
#include "hkrender.h"
#include "world.h"
#include "hkpost.h"
#include "spoof_call.h"
#include "Misc.h"

