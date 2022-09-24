#include "pch_core.h"
#pragma hdrstop

#include "cpuid.h"

/***
 *
 * int _cpuid (_p_info *pinfo)
 *
 * Entry:
 *
 *   pinfo: pointer to _p_info, NULL is not allowed!
 *
 * Exit:
 *
 *   Returns int with capablity bit set.
 *
 ****************************************************/
#ifdef _EDITOR
int _cpuid(_processor_info* pinfo)
{
	ZeroMemory(pinfo, sizeof(_processor_info));

	pinfo->feature = _CPU_FEATURE_MMX | _CPU_FEATURE_SSE;
	return pinfo->feature;
}
#else

#undef _CPUID_DEBUG

// TODO: Make normal _cpuid
int _cpuid(_processor_info* pinfo)
{
	ZeroMemory(pinfo, sizeof(_processor_info));

	pinfo->feature = _CPU_FEATURE_MMX | _CPU_FEATURE_SSE;
	return pinfo->feature;
}
#endif
