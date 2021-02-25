//----------------------------------------------------------------------
// CD3DTextureEffect.h
//----------------------------------------------------------------------
// CDirect3D를 이용해서..
// 2D좌표계의 사각 영역에 texture를 입히는 방법으로 effect를 표현한다.
//
// Draw..()가 실행될때는
// BeginScene()이 되어 있는 상태어야 하고..
// 임의의 Texture가 설정되어 있다고 가정한다.
//----------------------------------------------------------------------

#ifndef __CD3DTEXTUREEFFECT_H__
#define __CD3DTEXTUREEFFECT_H__

#include "CDirect3D.h"

class CD3DTextureEffect {
	public :
		CD3DTextureEffect();
		~CD3DTextureEffect();

		void		DrawEffect2D(RECT* pRect);

	protected :
		D3DTLVERTEX					m_TextureVertices[4];
};

#endif


