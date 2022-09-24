#include "pch_render.h"
#include "dxConsoleRender.h"

dxConsoleRender::dxConsoleRender()
{
#ifdef USE_DX10
	m_Shader.create("hud\\crosshair");
	m_Geom.create(FVF::F_TL, RCache.Vertex.Buffer(), RCache.QuadIB);
#endif
}

void dxConsoleRender::Copy(IConsoleRender& _in) { *this = *(dxConsoleRender*)&_in; }

void dxConsoleRender::OnRender(bool bGame)
{
	VERIFY(HW.pDevice);

	D3DRECT R = {0, 0, (LONG)Device.dwWidth, (LONG)Device.dwHeight};
	if (bGame) R.y2 /= 2;

#ifdef USE_DX10
	u32 vOffset = 0;
	//	TODO: DX10: Implement console background clearing for DX10
	FVF::TL* verts = (FVF::TL*)RCache.Vertex.Lock(4, m_Geom->vb_stride, vOffset);
	verts->set((float)R.x1, (float)R.y2, (u32)D3DCOLOR_XRGB(32, 32, 32), 0.0f, 0.0f);
	verts++;
	verts->set((float)R.x1, (float)R.y1, (u32)D3DCOLOR_XRGB(32, 32, 32), 0.0f, 0.0f);
	verts++;
	verts->set((float)R.x2, (float)R.y2, (u32)D3DCOLOR_XRGB(32, 32, 32), 0.0f, 0.0f);
	verts++;
	verts->set((float)R.x2, (float)R.y1, (u32)D3DCOLOR_XRGB(32, 32, 32), 0.0f, 0.0f);
	verts++;
	RCache.Vertex.Unlock(4, m_Geom->vb_stride);

	RCache.set_Element(m_Shader->E[0]);
	RCache.set_Geometry(m_Geom);

	RCache.Render(D3DPT_TRIANGLELIST, vOffset, 0, 4, 0, 2);
#else  //	USE_DX10
	CHK_DX(HW.pDevice->Clear(1, &R, D3DCLEAR_TARGET, D3DCOLOR_XRGB(32, 32, 32), 1, 0));
#endif //	USE_DX10
}
