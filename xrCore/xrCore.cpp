// xrCore.cpp : Defines the entry point for the DLL application.
//
#include "pch_core.h"
#pragma hdrstop

#include <mmsystem.h>
#include <objbase.h>
#include "xrCore.h"

#pragma comment(lib, "winmm.lib")

#ifdef DEBUG
#include <malloc.h>
#endif // DEBUG

XRCORE_API xrCore Core;

namespace CPU
{
	extern void Detect();
};

static u32 init_counter = 0;

extern char g_application_path[256];

//. extern xr_vector<shared_str>*	LogFile;

void xrCore::_initialize(LPCSTR _ApplicationName, LogCallback cb, BOOL init_fs, LPCSTR fs_fname)
{
	strcpy_s(ApplicationName, _ApplicationName);
	if (0 == init_counter)
	{
#ifdef XRCORE_STATIC
		_clear87();
		_control87(_PC_53, MCW_PC);
		_control87(_RC_CHOP, MCW_RC);
		_control87(_RC_NEAR, MCW_RC);
		_control87(_MCW_EM, MCW_EM);
#endif
		// Init COM so we can use CoCreateInstance
		//		HRESULT co_res =
		CoInitializeEx(NULL, COINIT_MULTITHREADED);

		strcpy_s(Params, sizeof(Params), GetCommandLine());
		_strlwr_s(Params, sizeof(Params));

		string_path fn, dr, di;

		// application path
		GetModuleFileName(GetModuleHandle("xrCore"), fn, sizeof(fn));
		_splitpath(fn, dr, di, 0, 0);
		strconcat(sizeof(ApplicationPath), ApplicationPath, dr, di);

#ifdef _EDITOR
		// working path
		if (strstr(Params, "-wf"))
		{
			string_path c_name;
			sscanf(strstr(Core.Params, "-wf ") + 4, "%[^ ] ", c_name);
			SetCurrentDirectory(c_name);
		}
#endif

		GetCurrentDirectory(sizeof(WorkingPath), WorkingPath);

		// User/Comp Name
		DWORD sz_user = sizeof(UserName);
		GetUserName(UserName, &sz_user);

		DWORD sz_comp = sizeof(CompName);
		GetComputerName(CompName, &sz_comp);

		// Mathematics & PSI detection
		CPU::Detect();

		Memory._initialize(strstr(Params, "-mem_debug") ? TRUE : FALSE);

		DUMP_PHASE;

		InitLog();
		_initialize_cpu();

		R_ASSERT(CPU::ID.feature & _CPU_FEATURE_SSE);
		//		Debug._initialize	();

		rtc_initialize();

		xr_FS = xr_new<CLocatorAPI>();

		xr_EFS = xr_new<EFS_Utils>();
		//.		R_ASSERT			(co_res==S_OK);
	}
	if (init_fs)
	{
		u32 flags = 0;
		if (0 != strstr(Params, "-build")) flags |= CLocatorAPI::flBuildCopy;
		if (0 != strstr(Params, "-ebuild")) flags |= CLocatorAPI::flBuildCopy | CLocatorAPI::flEBuildCopy;
#ifdef DEBUG
		if (strstr(Params, "-cache"))
			flags |= CLocatorAPI::flCacheFiles;
		else
			flags &= ~CLocatorAPI::flCacheFiles;
#endif         // DEBUG
#ifdef _EDITOR // for EDITORS - no cache
		flags &= ~CLocatorAPI::flCacheFiles;
#endif // _EDITOR
		flags |= CLocatorAPI::flScanAppRoot;

#ifndef _EDITOR
#ifndef ELocatorAPIH
		if (0 != strstr(Params, "-file_activity")) flags |= CLocatorAPI::flDumpFileActivity;
#endif
#endif
		FS._initialize(flags, 0, fs_fname);
		CalculateBuildId();
		Msg("'%s' build %d, %s\n", "xrCore", buildId, buildDate);
		EFS._initialize();
#ifdef DEBUG
#ifndef _EDITOR
		Msg("CRT heap 0x%08x", _get_heap_handle());
		Msg("Process heap 0x%08x", GetProcessHeap());
#endif
#endif // DEBUG
	}
	SetLogCB(cb);
	init_counter++;
}

#ifndef _EDITOR
#include "compression_ppmd_stream.h"
extern compression::ppmd::stream* trained_model;
#endif
void xrCore::_destroy()
{
	--init_counter;
	if (0 == init_counter)
	{
		FS._destroy();
		EFS._destroy();
		xr_delete(xr_FS);
		xr_delete(xr_EFS);

#ifndef _EDITOR
		if (trained_model)
		{
			void* buffer = trained_model->buffer();
			xr_free(buffer);
			xr_delete(trained_model);
		}
#endif

		Memory._destroy();
	}
}

void xrCore::CalculateBuildId()
{
	const int startDay = 31;
	const int startMonth = 1;
	const int startYear = 1999;
	const char* monthId[12] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};
	const int daysInMonth[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
	buildDate = __DATE__;
	int days;
	int months = 0;
	int years;
	string16 month;
	string256 buffer;
	strcpy(buffer, buildDate);
	sscanf(buffer, "%s %d %d", month, &days, &years);
	for (int i = 0; i < 12; i++)
	{
		if (_stricmp(monthId[i], month)) continue;
		months = i;
		break;
	}
	buildId = (years - startYear) * 365 + days - startDay;
	for (int i = 0; i < months; i++)
		buildId += daysInMonth[i];
	for (int i = 0; i < startMonth - 1; i++)
		buildId -= daysInMonth[i];
}

#ifndef XRCORE_STATIC

//. why ???
#ifdef _EDITOR
BOOL WINAPI DllEntryPoint(HINSTANCE hinstDLL, DWORD ul_reason_for_call, LPVOID lpvReserved)
#else
BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD ul_reason_for_call, LPVOID lpvReserved)
#endif
{
	switch (ul_reason_for_call)
	{
		case DLL_PROCESS_ATTACH:
			{
				_clear87();
				_control87(_PC_53, MCW_PC);
				_control87(_RC_CHOP, MCW_RC);
				_control87(_RC_NEAR, MCW_RC);
				_control87(_MCW_EM, MCW_EM);
			}
			//.		LogFile.reserve		(256);
			break;
		case DLL_THREAD_ATTACH:
			CoInitializeEx(NULL, COINIT_MULTITHREADED);
			timeBeginPeriod(1);
			break;
		case DLL_THREAD_DETACH:
			break;
		case DLL_PROCESS_DETACH:
#ifdef USE_MEMORY_MONITOR
			memory_monitor::flush_each_time(true);
#endif // USE_MEMORY_MONITOR
			break;
	}
	return TRUE;
}
#endif // XRCORE_STATIC
