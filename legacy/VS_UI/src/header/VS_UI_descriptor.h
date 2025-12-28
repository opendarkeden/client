/*-----------------------------------------------------------------------------

	VS_UI_Descriptor.h

	Descriptor class.

	객체에 대한 설명 또는 정보를 출력하는 객체이다.

	2000.7.3. KJTINC
	2000.12.9.

-----------------------------------------------------------------------------*/

#ifndef __VS_UI_DESCRIPTOR_H__
#define __VS_UI_DESCRIPTOR_H__

#include "VS_UI_Base.h"

struct DESCRIBED_UNIT
{
	id_t		id;
	bool		bl_immediate; // immediate(true)/delay(false)
	void		(*fp_rect_calculator)(void (*fp_show)(Rect, void *, long, long), int, int, void *, long, long);
	void		(*fp_show)(Rect, void *, long, long);
};

//-----------------------------------------------------------------------------
// class DescriptorManager
//
// > tool-tip과 같은 하나의 작은 info-box를 description이라 정의한다.
//
// DESCRIBED_UNIT은 하나의 통일된 description을 위한 group(descriptor)을 의미한다.
// 어떤 것은 즉시 description되길 원하겠지만, 어떤 것은 delay를 원한다.
// 즉 이러한 특징이 하나의 group이 가질 수 있는 것이다. 이것은 그들만의 format으로
// 출력할 수 있고, rect를 계산할 수 있다.
//
// 사용법>
//				AddDescribeUnit으로 추가한다. 거기에 첫번째인자로 정의한 id는 group을
//				선택하는데 필요하다.
//				Set을 하면 description이 출력될 준비를 모두 수행한다. rect를 계산하고
//				그 정보를 즉시 m_fp_show_param에 설정하며 해당 show fp ptr을 m_fp_show에
//				설정하므로써 바로 보여지게 한다.
//
//				description을 해제하기 위해서는 단순히 unset을 실행하면 된다.
//-----------------------------------------------------------------------------
class DescriptorManager : public SimpleDataList<DESCRIBED_UNIT *>
{
private:
	//
	// m_fp_show
	//
	// 현재 출력할 descrition show ptr.
	// 이것이 null이 아닌 동안은 계속 출력해야 한다.
	//
	void							(*m_fp_show)(Rect, void *, long, long);

	struct FP_SHOW_PARAM
	{
		Rect						rect;
		void *					void_ptr;
		long						left;
		long						right;
	};

	FP_SHOW_PARAM				m_fp_show_param;

public:
	DescriptorManager();
	~DescriptorManager();

	void	Set(id_t id, int x, int y, void * void_ptr, long left=0, long right=0);
	//void	Unset();

	// 같은 종류의 pointer인 경우만 제거한다.
	void	Unset(void* pPtr=NULL);	// by sigi

	void	Show();

	void	AddDescribedUnit(id_t id, void (*fp_rect_calculator)(void (*fp_show)(Rect, void *, long, long), int, int, void *, long, long), void (*fp_show)(Rect, void *, long, long), bool bl_immediate=true);
	void	RectCalculationFinished(void (*fp_show)(Rect, void *, long, long), Rect rect, void * void_ptr, long left, long right);
};

extern DescriptorManager	g_descriptor_manager;

#endif