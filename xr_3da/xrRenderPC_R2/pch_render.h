// pch_render.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently

#pragma once

#ifdef _DEBUG
#define D3D_DEBUG_INFO
#endif

#pragma warning(disable : 4995)
#include "../pch_engine.h"
#pragma warning(disable : 4995)
#include <d3d9.h>
#include <d3dx9.h>
#pragma warning(default : 4995)
#pragma warning(disable : 4714)
#pragma warning(4 : 4018)
#pragma warning(4 : 4244)
#pragma warning(disable : 4237)

#pragma comment(lib, "d3d9.lib")

#include "../xrRender/xrD3DDefs.h"

#include "../xrRender/HW.h"
#include "../xrRender/Shader.h"
#include "../xrRender/R_Backend.h"
#include "../xrRender/R_Backend_Runtime.h"

#define R_GL -1
#define R_R1 1
#define R_R2 2
#define R_R3 3
#define RENDER R_R2

#include "../xrRender/resourcemanager.h"

#include "../vis_common.h"
#include "../render.h"
#include "../_d3d_extensions.h"
#include "../igame_level.h"
#include "../xrRender/blenders\blender.h"
#include "../xrRender/blenders\blender_clsid.h"
#include "../psystem.h"
#include "../xrRender/xrRender_console.h"
#include "r2.h"

IC void jitter(CBlender_Compile& C)
{
	C.r_Sampler("jitter0", JITTER(0), true, D3DTADDRESS_WRAP, D3DTEXF_POINT, D3DTEXF_NONE, D3DTEXF_POINT);
	C.r_Sampler("jitter1", JITTER(1), true, D3DTADDRESS_WRAP, D3DTEXF_POINT, D3DTEXF_NONE, D3DTEXF_POINT);
	C.r_Sampler("jitter2", JITTER(2), true, D3DTADDRESS_WRAP, D3DTEXF_POINT, D3DTEXF_NONE, D3DTEXF_POINT);
	C.r_Sampler("jitter3", JITTER(3), true, D3DTADDRESS_WRAP, D3DTEXF_POINT, D3DTEXF_NONE, D3DTEXF_POINT);
	C.r_Sampler("jitter4", JITTER(4), true, D3DTADDRESS_WRAP, D3DTEXF_POINT, D3DTEXF_NONE, D3DTEXF_POINT);
}
