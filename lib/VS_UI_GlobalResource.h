/*-----------------------------------------------------------------------------

	VS_UI_GlobalResource.h

	특정 Object의 자원이 아닌 것들.

	2000.8.9. KJTINC

-----------------------------------------------------------------------------*/

#ifndef __VS_UI_GLOBALRESOURCE_H__
#define __VS_UI_GLOBALRESOURCE_H__

#include "VS_UI_Base.h"
#include "VS_UI_Util.h"

//-----------------------------------------------------------------------------
// C_GLOBAL_RESOURCE
//
//
//-----------------------------------------------------------------------------
class C_GLOBAL_RESOURCE
{
public:
	//
	// 외부에서 직접 접근한다.
	//
	//C_ANI_OBJECT *				m_pC_ao_ani_button;
//	Tga *							m_pic_box_back; // 조립박스 배경
//	C_SPRITE_PACK *			m_pC_box_line_spk;
//	C_SPRITE_PACK *			m_pC_ok_spk;

//	enum COMMON_BUTTON_INDEX
//	{
//		SMALL_BUTTON,
//		SMALL_FOCUS_BUTTON,
//		SMALL_PUSHED_BUTTON,
//		GO_BUTTON,
//		GO_FOCUS_BUTTON,
//		GO_PUSHED_BUTTON,
//		BACK_BUTTON,
//		BACK_FOCUS_BUTTON,
//		BACK_PUSHED_BUTTON,
//		NEXT_BUTTON_OFFSET = 9, // 3x3
//	};

//	C_SPRITE_PACK *			m_pC_common_button; // toggle용 작은버튼, OK, Cancel ...

	//
	//	(1) (2) (3)
	// (4) (5) (6)
	// (7) (8) (9)
	//
	enum ASSEMBLE_BOX_INDEX
	{
//		AB_LEFTUP,		// (1)
//		AB_UP,			// (2)
//		AB_RIGHTUP,		// (3)
//		AB_LEFT,			// (4)
//		AB_CENTER,		// (5)
//		AB_RIGHT,		// (6)
//		AB_LEFTDOWN,	// (7)
//		AB_DOWN,			// (8)
//		AB_RIGHTDOWN,	// (9)
		AB_BACK,
		AB_BACK_ALPHA,
		AB_RIGHT,
		AB_DOWN,
		AB_RIGHTDOWN,
		AB_BACK2,
		AB_BACK_ALPHA2,
		AB_RIGHT2,
		AB_DOWN2,
		AB_RIGHTDOWN2,
	};

	C_SPRITE_PACK *			m_pC_assemble_box_spk;

	enum ASSEMBLE_BOX_BUTTON_INDEX
	{
		AB_BUTTON_OK,
		AB_BUTTON_CANCEL,
		AB_BUTTON_CLOSE,
		AB_BUTTON_HELP,
		AB_BUTTON_O,
		AB_BUTTON_QUESTION,
		AB_BUTTON_DESC,
		AB_BUTTON_MONEY,
		AB_BUTTON_X,
		AB_BUTTON_CHANGE_MONEY,
		AB_BUTTON_OK_HILIGHTED,
		AB_BUTTON_CANCEL_HILIGHTED,
		AB_BUTTON_CLOSE_HILIGHTED,
		AB_BUTTON_HELP_HILIGHTED,
		AB_BUTTON_O_HILIGHTED,
		AB_BUTTON_QUESTION_HILIGHTED,
		AB_BUTTON_DESC_HILIGHTED,
		AB_BUTTON_MONEY_HILIGHTED,
		AB_BUTTON_X_HILIGHTED,
		AB_BUTTON_CHANGE_MONEY_HILIGHTED,
		AB_BUTTON_OK_PUSHED,
		AB_BUTTON_CANCEL_PUSHED,
		AB_BUTTON_CLOSE_PUSHED,
		AB_BUTTON_HELP_PUSHED,
		AB_BUTTON_O_PUSHED,
		AB_BUTTON_QUESTION_PUSHED,
		AB_BUTTON_DESC_PUSHED,
		AB_BUTTON_MONEY_PUSHED,
		AB_BUTTON_X_PUSHED,
		AB_BUTTON_CHANGE_MONEY_PUSHED,

		AB_DIALOG_TAG,
		AB_DIALOG_TAG_SELECTED,

		AB_NAME_BAR,
		AB_MONEY_BAR,

		AB_BUTTON_PUSHPIN,
		AB_BUTTON_PUSHPIN_PUSHED,

		AB_BUTTON_ALPHA,
		AB_BUTTON_ALPHA_PUSHED,

		AB_BUTTON_HILIGHTED_OFFSET = 10,
		AB_BUTTON_PUSHED_OFFSET = 20,
	};

	C_SPRITE_PACK *			m_pC_assemble_box_button_spk;

//	enum ASSEMBLE_BOX_ETC
//	{
//		AB_SCROLL_UP,
//		AB_SCROLL_DOWN,
//		AB_SCROLL_UP_PUSHED,
//		AB_SCROLL_DOWN_PUSHED,
//		AB_SCROLL_TAG,
//		AB_LEFT_ARROW,
//		AB_RIGHT_ARROW,
//	};
//
//	C_SPRITE_PACK *			m_pC_assemble_box_etc_spk;

	enum SCROLL_BAR
	{
		SB_BAR,
		SB_BUTTON,
		SB_BUTTON_PUSHED,
		SB_BUTTON_HILIGHTED,
		SB_BUTTON_HILIGHTED_PUSHED,
		SB_TAG,
	};

	C_SPRITE_PACK *			m_pC_scroll_bar_spk;

	enum BOX
	{
		OB_MAIN,
		OB_RIGHT,
		OB_DOWN,
		OB_RIGHTDOWN,
	};

		enum INFO_SPK
	{
		//common
		TAB_CHAR,
		TAB_SKILL,
		TAB_GRADE1,
		TAB_GRADE2,
		BUTTON_CLOSE_BACK,
		BUTTON_CLOSE,
		BUTTON_CLOSE_PUSHED,
		BUTTON_CLOSE_HILIGHTED,
		BUTTON_CLOSE_HILIGHTED_PUSHED,
		BUTTON_HELP_BACK,
		BUTTON_HELP,
		BUTTON_HELP_PUSHED,
		BUTTON_HELP_HILIGHTED,
		BUTTON_HELP_HILIGHTED_PUSHED,
		CHAR_BOX,
		DESC_BOX,
		LARGE_BAR,
		LARGE_BAR_RIGHT,
		SMALL_BAR,
		SMALL_BAR2,
		EXP_BAR,
		GUILD_BACK,
		SKILL_TITLE,
		SKILL_ICON_BACK,
		SKILL_BACK,
		SKILL_BACK_HILIGHTED,
		SKILL_BACK_ALPHA,
		SKILL_BOX,
		TITLE_NAME,
		TITLE_ALIGN,
		TITLE_GUILD,
		TITLE_STR,
		TITLE_DEX,
		TITLE_INT,
		TITLE_HP,
		TITLE_TOHIT,
		TITLE_DAMAGE,
		TITLE_DEFENCE,
		TITLE_PROTECTION,
		GRADE_LEVEL,
		GRADE_LEVEL_HILIGHTED,
		GRADE_EXP,
		GRADE_EXP_HILIGHTED,		
		GRADE_RED_BUTTON,
		GRADE_RED_BUTTON_DISABLE,

		//slayer
		TITLE_FAME,
		TITLE_MP,
		TITLE_DOMAINLEVEL,
		TITLE_DOMAINEXP,
		PLUS_BUTTON,
		PLUS_BUTTON_HILIGHTED,
		PLUS_BUTTON_HILIGHTED_PUSHED,
		MINUS_BUTTON,
		MINUS_BUTTON_HILIGHTED,
		MINUS_BUTTON_HILIGHTED_PUSHED,
		SUM_BUTTON,
		SUM_BUTTON_HILIGHTED,
		SUM_BUTTON_HILIGHTED_PUSHED,
		DOMAIN_BACK,
		DOMAIN_BAR,
		DM_BLADE,
		DM_BLADE_HILIGHTED,
		DM_SWORD,
		DM_SWORD_HILIGHTED,
		DM_GUN,
		DM_GUN_HILIGHTED,
		DM_HEAL,
		DM_HEAL_HILIGHTED,
		DM_ENCHANT,
		DM_ENCHANT_HILIGHTED,
		DM_TOTAL,
		DM_TOTAL_HILIGHTED,
		TITLE_GRADE,
		TITLE_TEAM,
		GRADE_PIVATE,
		GRADE_PIVATE_HILIGHTED,
		GRADE_SERENT,
		GRADE_SERENT_HILIGHTED,
		GRADE_FEACEL,								
		GRADE_FEACEL_HILIGHTED,
		GRADE_LITENA,
		GRADE_LITENA_HILIGHTED,
		GRADE_KAINEL,
		GRADE_KAINEL_HILIGHTED,
		SLAYER_GRADE_BAR,
		SLAYER_GRADE_BAR_HILIGHTED,
		SLAYER_GRADE_BAR_PUSHED,
		SLAYER_GRADE_BAR_HILIGHTED_PUSHED,
		SLAYER_GRADE_MARK_EDGE,
		
		//vampire
		TITLE_LEVEL = TITLE_FAME,
		TITLE_BONUS,
		TITLE_EXP,
		BONUS_BUTTON,
		BONUS_BUTTON_HILIGHTED,
		BONUS_BUTTON_HILIGHTED_PUSHED,
		DM_POISON,
		DM_POISON_HILIGHTED,
		DM_ACID,
		DM_ACID_HILIGHTED,
		DM_CURSE,
		DM_CURSE_HILIGHTED,
		DM_SUMMON,
		DM_SUMMON_HILIGHTED,
		DM_BLOOD,
		DM_BLOOD_HILIGHTED,
		DM_INNATE,
		DM_INNATE_HILIGHTED,
		TITLE_GRADE2,
		TITLE_CLAN,
		GRADE_RITTER,
		GRADE_RITTER_HILIGHTED,
		GRADE_REICHSRITTER,
		GRADE_REICHSRITTER_HILIGHTED,
		GRADE_BARONET,	
		GRADE_BARONET_HILIGHTED,
		GRADE_PREYHER,
		GRADE_PREYHER_HILIGHTED,
		GRADE_GRAF,
		GRADE_GRAF_HILIGHTED,
		VAMPIRE_GRADE_BAR,
		VAMPIRE_GRADE_BAR_HILIGHTED,
		VAMPIRE_GRADE_BAR_PUSHED,
		VAMPIRE_GRADE_BAR_HILIGHTED_PUSHED,
		GRADE_BAR_BACK,
		GRADE_BAR,
		VAMPIRE_GRADE_MARK_EDGE,
		VAMPIRE_FAME,
	};


	C_SPRITE_PACK *			m_pC_box_spk;
	C_SPRITE_PACK	* m_pC_info_spk;

	/*
	enum COMMON_BUTTON_IN_TITLE_INDEX
	{
		
	};

	C_SPRITE_PACK *			m_pC_common_in_title; // Title에서의 common
*/
/*	enum COMMON_SPK_INDEX
	{
		HAN_MARK, // '가'
		ENG_MARK, // 'A'
		CHECK_MARK,
		CHECK_MARK_BIG,
	};
*/
	//C_SPRITE_PACK *			m_pC_common_spk;

	C_GLOBAL_RESOURCE();
	~C_GLOBAL_RESOURCE();

	void	LoadAssemble();
	void	FreeAssemble();

	// OutBox?
	void	DrawOutBox(int x, int y, int w, int h);
	void	DrawOutBox(Rect &rect);
	void	DrawOutBoxLocked(int x, int y, int w, int h);
	void	DrawOutBoxLocked(Rect &rect);

	// Dialog
	void	DrawDialog(int x, int y, int w, int h, bool alpha = false);
	void	DrawDialog(Rect &rect, bool alpha = false);
	void	DrawDialogLocked(int x, int y, int w, int h, bool alpha = false);
	void	DrawDialogLocked(Rect &rect, bool alpha = false);
	void	DrawDialog2(int x, int y, int w, int h, bool alpha = false);
	void	DrawDialog2(Rect &rect, bool alpha = false);
	void	DrawDialogLocked2(int x, int y, int w, int h, bool alpha = false);
	void	DrawDialogLocked2(Rect &rect, bool alpha = false);
	// 알파는 빼고 테두리만
	void	DrawDialogOnly(int x, int y, int w, int h);
	void	DrawDialogOnly(Rect &rect);
	void	DrawDialogOnlyLocked(int x, int y, int w, int h);
	void	DrawDialogOnlyLocked(Rect &rect);
	void	DrawDialogOnly2(int x, int y, int w, int h);
	void	DrawDialogOnly2(Rect &rect);
	void	DrawDialogOnlyLocked2(int x, int y, int w, int h);
	void	DrawDialogOnlyLocked2(Rect &rect);

	//void	LoadAniButton();
	//void	FreeAniButton();
	//void	LoadAssembleBox();
	//void	FreeAssembleBox();
	//void	LoadCommonImage();
	//void	FreeCommonImage();
//	void	LoadCommonButton();
//	void	FreeCommonButton();
//	void	LoadCommonInTitle();
//	void	FreeCommonInTitle();
};

extern C_GLOBAL_RESOURCE *	gpC_global_resource;
extern bool	gbl_wood_skin;

#endif