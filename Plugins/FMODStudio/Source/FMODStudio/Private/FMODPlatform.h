// Copyright (c), Firelight Technologies Pty, Ltd. 2012-2025.
#pragma once

#include "Containers/UnrealString.h"
#include "fmod_common.h"
#include "FMODSettings.h"

FString FMODPlatform_GetDllPath(const TCHAR *ShortName, bool bExplicitPath, bool bUseLibPrefix);

EFMODPlatforms::Type FMODPlatform_CurrentPlatform();

FString FMODPlatform_PlatformName();

#ifdef FMOD_PLATFORM_LOAD_DLL
void* FMODPlatformLoadDll(const TCHAR* LibToLoad);
#endif

FMOD_RESULT FMODPlatformSystemSetup();