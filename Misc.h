#pragma once


#ifndef __MISC_H__
#define __MISC_H__
#include <Windows.h>
#include "stdafx.h"

extern const void* DetourFunc64(BYTE* const src, const BYTE* dest, const unsigned int jumplength);

#endif