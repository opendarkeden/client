//-----------------------------------------------------------------------------
// CD3DObject.h
//-----------------------------------------------------------------------------
// 3D 공간에서 존재하는 하나의 사물을 표현한다.
//-----------------------------------------------------------------------------

#ifndef	__CD3DOBJECT_H__
#define	__CD3DOBJECT_H__

#pragma warning(disable:4786)

#include "CDirect3D.h"
#include "CD3DMath.h"
//#include "d3dfile.h"
#include <list>


class CD3DObject {
	public :
		CD3DObject();
		~CD3DObject();

		//----------------------------------------------------
		// Init/Release
		//----------------------------------------------------
		void				Init(DWORD nVertices, DWORD nIndices);
		void				Release();
		void				SetOrigin();

		//----------------------------------------------------
		// Draw
		//----------------------------------------------------
		void				Draw(bool bRestoreWorld=true);
		void				Draw(D3DMATRIX& matParentWorld, bool bRestoreWorld=true);

		//----------------------------------------------------
		// Create Functions
		//----------------------------------------------------		
		void				CreateCube(float sizeX=1.0f, float sizeY=1.0f, float sizeZ=1.0f);
		void				CreatePlane(float size = 1.0f);
		void				CreateCylinder(float r=1.0f, float l=1.0f, int n=3);
		void				CreateTilePlane(int width, int height, float firstX, float firstY, float size);

		//----------------------------------------------------
		// Position & Direction
		//----------------------------------------------------
		void				SetOrigin(D3DVECTOR& origin);
		void				SetOrigin(float x, float y, float z);
		void				SetPosition(D3DVECTOR& position);
		void				SetPosition(float x, float y, float z);
		void				SetDirection(D3DVECTOR& direction);
		void				SetDirection(float x, float y, float z);
		void				SetScale(D3DVECTOR& Scale);
		void				SetScale(float x, float y, float z);

		D3DVECTOR			GetPosition() const		{ return m_Position; }

		//----------------------------------------------------
		// Translate, Scale, Rotate
		//----------------------------------------------------
		void				Move(float x, float y, float z);
		void				Scale(float x, float y, float z);
		void				Rotate(float x, float y, float z);

		//----------------------------------------------------
		// Get Functions
		//----------------------------------------------------		
		const D3DVERTEX*	GetVertices() const		{ return m_pVertices; }
		const WORD*			GetIndices() const		{ return m_pIndices; }
		DWORD				GetVerticesSize() const	{ return m_nVertices; }
		DWORD				GetIndicesSize() const	{ return m_nIndices; }

		//----------------------------------------------------		
		// Material
		//----------------------------------------------------		
		bool				IsMaterialNULL() const	{ return m_pMaterial==NULL; }
		bool				IsMaterialNotNULL() const	{ return m_pMaterial!=NULL; }
		D3DMATERIAL7*		GetMaterial() const		{ return m_pMaterial; }	
		void				SetMaterial(D3DMATERIAL7& mtrl);


		//----------------------------------------------------
		// Object Hierarchy
		//----------------------------------------------------
		void				AddChild(CD3DObject* pObject);
		void				ReleaseChild();

		//----------------------------------------------------
		// File I/O
		//----------------------------------------------------
		//bool				LoadX(char *filename);

		void				ApplySphereMap();


	protected :
		//----------------------------------------------------
		// Position & Direction
		//----------------------------------------------------
		D3DMATRIX			m_matStatus;		
		D3DVECTOR			m_Position;
		D3DVECTOR			m_Direction;
		D3DVECTOR			m_Scale;

		//----------------------------------------------------
		// Vertices + Indices
		//----------------------------------------------------
		// 메모리가 잡혀있는 상태인가?
		bool				m_bNew;

		// Vertices
		DWORD				m_nVertices;	// Vertex개수
		D3DVERTEX*			m_pVertices;

		// Indices
		DWORD				m_nIndices;		// Index개수
		WORD*				m_pIndices;

		//----------------------------------------------------
		// 외부 File
		//----------------------------------------------------
		//CD3DFile*			m_pFile;   

		//----------------------------------------------------
		// Material
		//----------------------------------------------------
		D3DMATERIAL7*		m_pMaterial;

	public :
		typedef	std::list<CD3DObject*>		CD3DOBJECT_LIST;

	protected :
		//----------------------------------------------------
		// Object Hierarchy
		//----------------------------------------------------
		CD3DObject*			m_pParent;
		CD3DOBJECT_LIST		m_listChild;		
		 
};	

#endif
