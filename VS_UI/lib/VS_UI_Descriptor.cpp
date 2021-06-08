// VS_UI_Descriptor.cpp

#include "client_PCH.h"
#include "VS_UI_Descriptor.h"

DescriptorManager	g_descriptor_manager;

//----------------------------------------------------------------------------
// Operations
//----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// DescriptorManager::DescriptorManager
//
// 
//-----------------------------------------------------------------------------
DescriptorManager::DescriptorManager()
{
	m_fp_show = NULL;
	m_fp_show_param.rect.Set(0, 0, 0, 0);
	m_fp_show_param.void_ptr = NULL;
	m_fp_show_param.left = 0;
	m_fp_show_param.right = 0;
}

//-----------------------------------------------------------------------------
// DescriptorManager::~DescriptorManager
//
// 
//-----------------------------------------------------------------------------
DescriptorManager::~DescriptorManager()
{
	DESCRIBED_UNIT * data;
	for (int i=0; i < Size(); i++)
		if (Data(i, data))
			delete data;
}

//-----------------------------------------------------------------------------
// DescriptorManager::Set
//
// 
//-----------------------------------------------------------------------------
void	DescriptorManager::Set(id_t id, int x, int y, void * void_ptr, long left, long right)
{
	if (m_fp_show) // doing?
		return;

	DESCRIBED_UNIT * data;
	for (int i=0; i < Size(); i++)
		if (Data(i, data))
			if (data->id == id)
			{
				assert(!gpC_base->m_p_DDSurface_back->IsLock());
				data->fp_rect_calculator(data->fp_show, x, y, void_ptr, left, right);
				return;
			}
}

//-----------------------------------------------------------------------------
// DescriptorManager::Unset
//
// 
//-----------------------------------------------------------------------------
//void	DescriptorManager::Unset()
//{
//	m_fp_show = NULL;
//}

//-----------------------------------------------------------------------------
// DescriptorManager::Unset 
//
// by sigi
// 
// NULL인 경우는 그냥 UnSet
//-----------------------------------------------------------------------------
void	DescriptorManager::Unset(void* pPtr)
{
	if (pPtr==NULL)
	{
		m_fp_show = NULL;
		return;
	}

	if (m_fp_show)
	{		
		// 같은 pointer인 경우만 제거한다.
		if (pPtr==m_fp_show_param.void_ptr)
		{
			m_fp_show = NULL;
		}
	}
}


//-----------------------------------------------------------------------------
// DescriptorManager::Show
//
// 
//-----------------------------------------------------------------------------
void	DescriptorManager::Show()
{
	assert(!gpC_base->m_p_DDSurface_back->IsLock());
	if (m_fp_show)
		m_fp_show(m_fp_show_param.rect, m_fp_show_param.void_ptr, m_fp_show_param.left, m_fp_show_param.right);
}

//-----------------------------------------------------------------------------
// DescriptorManager::AddDescribedUnit
//
// 
//-----------------------------------------------------------------------------
void	DescriptorManager::AddDescribedUnit(id_t id, void (*fp_rect_calculator)(void (*fp_show)(Rect, void *, long, long), int, int, void *, long, long), void (*fp_show)(Rect, void *, long, long), bool bl_immediate)
{
	// same id?
	DESCRIBED_UNIT * data;
	for (int i=0; i < Size(); i++)
		if (Data(i, data))
			if (data->id == id)
				return;

	DESCRIBED_UNIT * p_unit = new DESCRIBED_UNIT;

	p_unit->bl_immediate = bl_immediate;
	p_unit->fp_rect_calculator = fp_rect_calculator;
	p_unit->fp_show = fp_show;
	p_unit->id = id;

	Add(p_unit);
}

//-----------------------------------------------------------------------------
// DescriptorManager::RectCalculationFinished
//
// 
//-----------------------------------------------------------------------------
void	DescriptorManager::RectCalculationFinished(void (*fp_show)(Rect, void *, long, long), Rect rect, void * void_ptr, long left, long right)
{
	m_fp_show = fp_show;
	m_fp_show_param.rect = rect;
	m_fp_show_param.void_ptr = void_ptr;
	m_fp_show_param.left = left;
	m_fp_show_param.right = right;
}