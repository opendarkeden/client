/*-----------------------------------------------------------------------------

	VS_UI_Title.h

	VS User Interface Title.

	Title Object Components:
										Login
										New User
										Character Manager:
																	New character creation:
																							Appearance
																	Character info:
																							Character delete

	2000.8.9. KJTINC

-----------------------------------------------------------------------------*/

#ifndef __VS_UI_TITLE_H__
#define __VS_UI_TITLE_H__

#include "VS_UI_Base.h"
#include "VS_UI_Util.h"
#include "VS_UI_Widget.h"
#include "VS_UI_Dialog.h"
#include "VS_UI_DESC.h"

#include <vector>

enum MAIN_SPK_INDEX // C_VS_UI_NEWCHAR 에서도 사용하므로...
{
	MAIN,
	HEART_LEFT,
	HEART_CENTER,
	HEART_RIGHT,
	HEART_LEFT_HIGHLIGHT,
	HEART_CENTER_HIGHLIGHT,
	HEART_RIGHT_HIGHLIGHT,
	NAME_BOARD,
};

enum
{
	TOTAL_ENERGY_LINE = 109,
};

// character ani
enum
{
	ANI_MILLISEC = 100, 
};

//
// character 생성시 character part...
// CFPK 순서
//
enum CHAR_MAN
{
	M_OLD_HELMET,
	M_OLD_COAT,
	M_OLD_TROUSER,
	M_SMG,
	M_BLADE,
	M_SHIELD,
	M_DRAGON_SHIELD,
	M_BRONZE_SHIELD,
	M_SG,
	M_SWORD,
	M_AR,
	M_FACE1,
	M_FACE2,
	M_FACE3,
	M_NEW_HELMET,
	M_NEW_COAT,
	M_NEW_TROUSER,
	M_CROSS,
	M_SR,
	M_MOTOR,
	M_UP_BODY,
	M_DOWN_BODY,
	M_NEW2_COAT,
	M_NEW2_TROUSER,
	M_MACE,
	M_HAIR1,
	M_HAIR2,
	M_HAIR3,

	M_NO_WEAR
};

enum CHAR_WOMAN
{
	W_OLD_HELMET,
	W_OLD_COAT,
	W_OLD_TROUSER,
	W_SMG,
	W_BLADE,
	W_SHIELD,
	W_DRAGON_SHIELD,
	W_BRONZE_SHIELD,
	W_SG,
	W_SWORD,
	W_AR,
	W_FACE1,
	W_FACE2,
	W_FACE3,
	W_NEW_HELMET,
	W_NEW_COAT,
	W_NEW_TROUSER,
	W_CROSS,
	W_SR,
	W_MOTOR,
	W_UP_BODY,
	W_DOWN_BODY,
	W_NEW2_COAT,
	W_NEW2_TROUSER,
	W_MACE,
	W_HAIR1,
	W_HAIR2,
	W_HAIR3,

	W_NO_WEAR
};

struct MAN_INFO
{
	CHAR_MAN		face;
	CHAR_MAN		hair;
	CHAR_MAN		helmet;
	CHAR_MAN		coat;
	CHAR_MAN		trouser;
	CHAR_MAN		right; // 무기
	CHAR_MAN		left;  // 방어구
};

struct WOMAN_INFO
{
	CHAR_WOMAN		face;
	CHAR_WOMAN		hair;
	CHAR_WOMAN		helmet;
	CHAR_WOMAN		coat;
	CHAR_WOMAN		trouser;
	CHAR_WOMAN		right; // 무기
	CHAR_WOMAN		left;  // 방어구
};

enum ALIGNMENT
{
	UI_EVIL_PLUS,
	UI_EVIL,
	UI_NEUTRAL,
	UI_GOOD,
	UI_GOOD_PLUS,
};

// character creation slot
struct S_SLOT
{
	S_SLOT() { m_AdvancementLevel = 0; }
	bool								bl_set; // slot이 설정되었는가?
//	bool								bl_vampire;
	bool								bl_female;
	bool								bl_drained; //흡혈된 상태인가?
	MAN_INFO							man_info;
	WOMAN_INFO							woman_info;
	
	int									helmet_color;
	int									trouser_color;
	int									coat_color;
	int									skin_color;
	int									hair_color;
	int									left_color;
	int									right_color;

	std::string							sz_name;
	std::string							sz_guild_name;
	ALIGNMENT							alignment;
	int									alignment_num;
	int									level;		// vampire only
	int									STR_PURE;	// by larosel
	int									STR_CUR;	// by larosel
	int									STR_MAX;	// by larosel
	int									DEX_PURE;	// by larosel
	int									DEX_CUR;	// by larosel
	int									DEX_MAX;	// by larosel
	int									INT_PURE;	// by larosel
	int									INT_CUR;	// by larosel
	int									INT_MAXX;	// by larosel	//INT_MAX 는 못씀
	int									STR_EXP_REMAIN;
	int									DEX_EXP_REMAIN;
	int									INT_EXP_REMAIN;
	int									EXP_REMAIN;		// vampire only
	int									DAM;			// max
	int									DAM2;			// min
	int									SILVER_DAM;		//max
	int									SILVER_DAM2;	//min
	int									CHANGE_VAMPIRE;	// 뱀파이어로 변하는 남은 시간 분
//	int									AC;
	int									DEFENSE;	// by larosel
	int									PROTECTION;	// by larosel
	int									TOHIT;
	int									HP;
	int									MP;
	int									HP_MAX;
	int									MP_MAX;
	int									SILVER_HP;

	int									DOMAIN_SWORD;
	int									DOMAIN_BLADE;
	int									DOMAIN_GUN;
	int									DOMAIN_HEAL;
	int									DOMAIN_ENCHANT;

//	int									CC;
	int									FAME;		// slayer only
//	int									NOTERITY;
	int									bonus_point; // vampire only
	int									skill_point; // ousters only

	struct UI_EFFECTSTATUS_STRUCT
	{
		TYPE_ACTIONINFO actionInfo;
		DWORD			delayFrame;
	};

	typedef std::vector<UI_EFFECTSTATUS_STRUCT>			UI_EFFECTSTATUS_TYPE;
	UI_EFFECTSTATUS_TYPE				STATUS;	
	
	// Vampire일 경우 mp는 무시한다.
//	WORD									hp_cur_line;
//	WORD									mp_cur_line;	// slayer only
//	WORD									hp_percent;
//	WORD									mp_percent;		// slayer only

	WORD								GUILD_ID;
	BYTE								GUILD_GRADE;

	BYTE								WS;
	BYTE								WeaponSpeed;
	int									GRADE;
	int									GRADE_EXP_REMAIN;

	Race								Race;
	
	int									ElementalFire;
	int									ElementalWater;
	int									ElementalEarth;
	int									ElementalWind;
	
	int									m_SMS_Charge;
	int									m_Powerjjang_Point;
	int									m_AdvancementLevel;	// 전직 레벨 
//	BYTE								m_NickNameType;
//	std::string							m_NickName;
};	

//-----------------------------------------------------------------------------
// C_VS_UI_LOGIN
//
// 
//-----------------------------------------------------------------------------
class C_VS_UI_LOGIN : public Window, public Exec, public ButtonVisual
{
private:
	enum 
	{
		NEW_ID,
		OK,
		CANCEL,
		PUSHED_NEW_ID,
		PUSHED_OK,
		PUSHED_CANCEL,
	};

	//enum INPUT_POSITION // 입력 위치
	//{
	//	ID,
	//	PASSWORD,
	//};

	enum { SHADOW = 1 };

	C_SPRITE_PACK *				m_pC_login_spk;
//	C_SPRITE_PACK				m_pC_login_menu_default;
	C_SPRITE_PACK				m_pC_login_menu;
	C_SPRITE_PACK				m_title1_spk;

	ButtonGroup *				m_pC_button_group;

	//
	// string line buffer (ID / PASSWORD)
	//
	//STRING						m_string_line_ID;
	//STRING						m_string_line_PASSWORD;

	LineEditorVisual			m_lev_id;
	LineEditorVisual			m_lev_password;
	std::string					m_lev_id_backup;

	//INPUT_POSITION	m_e_ip; // input position

	void	SendLoginToClient();
	//bool	NextFocus();
	bool	ReadySend();
	void	ChangeFocus();

public:
	C_VS_UI_LOGIN();
	~C_VS_UI_LOGIN();

	void	AcquireDisappear() {}
	void	CancelPushState() { m_pC_button_group->CancelPushState(); }
	void	AcquireFirstSequence();
	void	UnacquireMouseFocus();
	void	ShowButtonWidget(C_VS_UI_EVENT_BUTTON * p_button);
	void	ShowButtonDescription(C_VS_UI_EVENT_BUTTON * p_button);
	void	WindowEventReceiver(id_t event);
	bool	IsPixel(int _x, int _y);

	void	Run(id_t id);
	bool	MouseControl(UINT message, int _x, int _y);
	void	KeyboardControl(UINT message, UINT key, long extra);
	void	Show();
	void	Process();
	
	void	ExecuteObject();
	void	Start();
	void	Finish();
};
/*
//-----------------------------------------------------------------------------
// C_VS_UI_NEWUSER
//
// 
//-----------------------------------------------------------------------------
class C_VS_UI_NEWUSER : public Window, public Exec, public ButtonVisual
{
private:
	ButtonGroup *				m_pC_button_group;
	C_SPRITE_PACK *			m_pC_newuser_spk;

	enum EXEC_ID
	{
		ID_REPEAT_CHECK,
		FINISH,
		CANCEL,
		GENDER_MALE,
		GENDER_FEMALE,
		ANNOUNCE_MY_INFO,
	};

	enum ITEM_SEARCH_SEQUENCE
	{
		ISS_ID,
		ISS_PASSWORD,
		ISS_REPASSWORD,
		ISS_NAME,
		ISS_STATE,
		ISS_SSN,
		ISS_ADDRESS,
		ISS_WOO, // 우편번호
		ISS_PHONE,
		ISS_HOMEPAGE,
		ISS_EMAIL,

		ISS_COUNT,
	};

	static S_RECT						m_item_rect[ISS_COUNT];
	static ITEM_SEARCH_SEQUENCE	m_item_search_sequence[ISS_COUNT];

	enum NEWUSER_SPK_INDEX
	{
		NEWUSER_WINDOW,
		ID_REPEAT_CHECK_IMAGE,
		FINISH_IMAGE,
		CANCEL_IMAGE,
		CHECK_IMAGE,
	};

	// input item
	LineEditorVisual			m_lev_id;
	LineEditorVisual			m_lev_password;
	LineEditorVisual			m_lev_repassword;
	LineEditorVisual			m_lev_name;
	LineEditorVisual			m_lev_ssn_part1;
	LineEditorVisual			m_lev_ssn_part2;
	LineEditorVisual			m_lev_email;
	LineEditorVisual			m_lev_state;
	LineEditorVisual			m_lev_phone;
	LineEditorVisual			m_lev_woo;
	LineEditorVisual			m_lev_homepage;
	LineEditorVisual			m_lev_address;
	bool							m_bl_female;
	bool							m_bl_announce_my_info;

	int							m_current_focus;
	bool							m_bl_ssn_ip_part1; // input position

	void	SendNewUserInfoToClient();
	void	ChangeFocus(ITEM_SEARCH_SEQUENCE n);

public:
	C_VS_UI_NEWUSER();
	~C_VS_UI_NEWUSER();

	void	AcquireDisappear() {}
	void	CancelPushState() { m_pC_button_group->CancelPushState(); }
	void	WindowEventReceiver(id_t event);
	bool	IsPixel(int _x, int _y);
	void	UnacquireMouseFocus();

	void	ShowButtonWidget(C_VS_UI_EVENT_BUTTON * p_button);
	void	Run(id_t id);
	void	Show();
	bool	MouseControl(UINT message, int _x, int _y);
	void	KeyboardControl(UINT message, UINT key, long extra);
	void	Start();
	void	Finish();
	void	Process();
};
*/
//-----------------------------------------------------------------------------
// C_VS_UI_CHAR_DELETE
//
// 
//-----------------------------------------------------------------------------
class C_VS_UI_CHAR_DELETE : public Window, public Exec, public ButtonVisual
{
private:
	enum CHAR_DELETE_MENU
	{
		DELETE_OK,
		DELETE_CANCEL,
	};

//	enum IMAGE_SPK_INDEX
//	{
//		DELETE_WINDOW,
//	};

	int							m_selected_slot;

//	C_SPRITE_PACK *			m_pC_image_spk;
	ButtonGroup *				m_pC_button_group;

	// 주민등록번호(SSN) string buffer.
	LineEditorVisual			m_lev_ssn_part1;
	LineEditorVisual			m_lev_ssn_part2;
	bool							m_bl_ssn_ip_part1; // input position

	void	SendCharacterDeleteToClient();

public:
	C_VS_UI_CHAR_DELETE();
	~C_VS_UI_CHAR_DELETE();

	void	ShowButtonWidget(C_VS_UI_EVENT_BUTTON * p_button);
	void	ShowButtonDescription(C_VS_UI_EVENT_BUTTON * p_button);
	void	UnacquireMouseFocus() { m_pC_button_group->UnacquireMouseFocus(); }
	void	CancelPushState() { m_pC_button_group->CancelPushState(); }
	void	AcquireDisappear() {}
	void	WindowEventReceiver(id_t event);
	bool	IsPixel(int _x, int _y);

	void	Start();
	void	Finish();
	void	Process();
	void	Show();
	void	Run(id_t id);
	bool	MouseControl(UINT message, int _x, int _y);
	void	KeyboardControl(UINT message, UINT key, long extra);
	void	Slot(int slot) { m_selected_slot = slot; }
};

//-----------------------------------------------------------------------------
// C_VS_UI_CHAR_APPEARANCE
//
// Chacter 외모 변경 class.
//-----------------------------------------------------------------------------
/*
class C_VS_UI_CHAR_APPEARANCE : public Window, public Exec
{
private:
	enum FACE_SPK_INDEX
	{
		M_GUNNER,
		M_FIGHTER,
		M_PRIEST,
		W_GUNNER,
		W_FIGHTER,
		W_PRIEST,
	};

	enum APPEARANCE_MENU
	{
		APPERANCE_OK,
		APPERANCE_CANCEL,
		FACE1,
		FACE2,
		FACE3,
		COLORSET1,
		COLORSET2,
		MENU_COUNT,
	};

	enum 
	{ 
		// 0 ~ 35 (color set range)
		COLORSET_X = 9, 
		COLORSET_Y = 4,

		// appearance Window position
		APPEARANCE_X = 342,
		APPEARANCE_Y = 182,

		// color set offset
		COLORSET_OFFSET_X = 45,
		COLORSET_OFFSET_Y = 170,

		// color unit rect width, height
		COLOR_UNIT_W = 20,
		COLOR_UNIT_H = 20,
	};

	//
	// Color Window 관련
	//
	BYTE							m_b_hair_colorset; // 선택된 color set
	BYTE							m_b_skin_colorset; // 선택된 color set
	int							m_focused_x; // color 선택 focus
	int							m_focused_y;
	BYTE							m_b_colorset_array[COLORSET_Y][COLORSET_X];
	bool							m_bl_colorset1; // Appearance Window에서 colorset1이 선택되었는가?
	bool							m_bl_push_colorset;

	C_VS_UI_BUTTON *			m_pC_button[MENU_COUNT];
	C_SPRITE_PACK *			m_pC_appearance_spk;
	C_SPRITE_PACK *			m_pC_face_spk;

	S_SLOT *						m_p_slot;

public:
	C_VS_UI_CHAR_APPEARANCE();
	~C_VS_UI_CHAR_APPEARANCE();

	void	WindowEventReceiver(id_t event);
	bool	IsPixel(int _x, int _y);

	void	Show();
	void	Run(id_t id);
	bool	MouseControl(UINT message, int _x, int _y);
	void	KeyboardControl(UINT message, UINT key, long extra);
	void	Start();
	void	Finish();
	void	Process();
	void	SetSlotCharacter(S_SLOT * p_slot) { m_p_slot = p_slot; }
};*/

//-----------------------------------------------------------------------------
// C_VS_UI_NEWCHAR
//
// 
//-----------------------------------------------------------------------------
class C_VS_UI_NEWCHAR : public Window, public Exec, public ButtonVisual, public C_VS_UI_DESC
{
private:
//	C_ANI_OBJECT *				m_pC_ao_fin;
//	C_ANIMATION *				m_pC_ani_fin;

	enum COMMON_SPK_ID
	{
		BACK,
		BACK_BUTTON,
		BACK_BUTTON_PUSHED,
		BACK_BUTTON_HILIGHTED,
		NEXT_BUTTON,
		NEXT_BUTTON_PUSHED,
		NEXT_BUTTON_HILIGHTED,
	};

	enum CREATE_SPK_ID
	{
		WINDOW,
		CHARACTER_GUARD,
		TITLE,
		MALE_CHECK,
		MALE_CHECK_HILIGHTED,
		SLAYER_BUTTON,
		VAMPIRE_BUTTON,
		OUSTERS_BUTTON,
		MALE_BUTTON,
		FEMALE_BUTTON,
		FACE_BACK_BUTTON,
		FACE_BACK_BUTTON_PUSHED,
		FACE_BACK_BUTTON_HILIGHTED,
		FACE_NEXT_BUTTON,
		FACE_NEXT_BUTTON_PUSHED,
		FACE_NEXT_BUTTON_HILIGHTED,
		CHECK_BUTTON,
		CHECK_BUTTON_PUSHED,
		CHECK_BUTTON_HILIGHTED,
		SAVE_BUTTON,
		SAVE_BUTTON_PUSHED,
		SAVE_BUTTON_HILIGHTED,
		LOAD_BUTTON,
		LOAD_BUTTON_PUSHED,
		LOAD_BUTTON_HILIGHTED,
		REROLL_BUTTON,
		REROLL_BUTTON_PUSHED,
		REROLL_BUTTON_HILIGHTED,
		BONUS_LINE,
		PLUS_BUTTON,
		PLUS_BUTTON_PUSHED,
		PLUS_BUTTON_HILIGHTED,
		MINUS_BUTTON,
		MINUS_BUTTON_PUSHED,
		MINUS_BUTTON_HILIGHTED,
	};

	enum EXEC_ID
	{
		BACK_ID,
		NEXT_ID,
		SLAYER_ID,
		VAMPIRE_ID,
		OUSTERS_ID,
		MALE_ID,
		FEMALE_ID,
		FACE_BACK_ID,
		FACE_NEXT_ID,
		CHECK_ID,
		SAVE_ID,
		LOAD_ID,
		REROLL_ID,
		STR_PLUS_ID,
		DEX_PLUS_ID,
		INT_PLUS_ID,
		STR_MINUS_ID,
		DEX_MINUS_ID,
		INT_MINUS_ID,
	};

	enum 
	{ 
		NAME_BOARD_X = 421,
		NAME_BOARD_Y = 183,

		// hair/skin 24 color
		COLOR_LIST_X = 8,
		COLOR_LIST_Y = 3,

		COLOR_UNIT_X = 15,
		COLOR_UNIT_Y = 9,

		TABLE_X = 289, // color table
		TABLE_Y = 371,
		TABLE_Y2 = 431,
	}; 

//	enum 
//	{ 
//		ENERGY_X = 622, 
//		ENERGY_HP_Y = 338, 
//		ENERGY_MP_Y = 374,
//	};

	enum FACE_SPK_INDEX
	{
		M_GUNNER,
		M_FIGHTER,
		M_PRIEST,
		W_GUNNER,
		W_FIGHTER,
		W_PRIEST,

		BLIGHT_FACE_OFFSET=0,
		GENDER_OFFSET=3,
	};

//	bool							m_bl_change_face;

//	class ifstream				m_slayer_ispk_file;
//	class ifstream				m_vampire_ispk_file;
	CCreatureFramePack 		m_slayer_cfpk;
	CCreatureFramePack 		m_vampire_cfpk;
	CCreatureFramePack 		m_ousters_cfpk;
	
	CCreatureFramePack		m_AdvancementSlayerManCfpk;
	CCreatureFramePack		m_AdvancementSlayerWomanCfpk;
	CCreatureFramePack		m_AdvancementVampireManCfpk;
	CCreatureFramePack		m_AdvancementVampireWomanCfpk;
	CCreatureFramePack		m_AdvancementOustersCfpk;
	
//	CCreatureFramePack 		m_pC_slayer_woman_cfpk;
	CIndexSpritePack 			m_slayer_ispk;
//	CIndexSpritePack 			m_pC_slayer_woman_ispk;
	CIndexSpritePack 			m_vampire_ispk;
	CIndexSpritePack 			m_ousters_ispk;
	
	CIndexSpritePack			m_AdvancementSlayerManIspk;
	CIndexSpritePack			m_AdvancementSlayerWomanIspk;
	CIndexSpritePack			m_AdvancementVampireManIspk;
	CIndexSpritePack			m_AdvancementVampireWomanIspk;
	CIndexSpritePack			m_AdvancementOustersIspk;

	C_SPRITE_PACK				m_common_spk;
	C_SPRITE_PACK				m_image_spk;
//	C_SPRITE_PACK				m_pC_etc;
	C_SPRITE_PACK				m_face_spk;
//	CFileIndexTable			m_SlayerPackIndex;
//	CFileIndexTable			m_VampirePackIndex;

//	void	AssertSlayerSprite(int m_sprite_id);
//	void	AssertVampireSprite(int m_sprite_id);

	enum HELP
	{
		HELP_DEFAULT,
		HELP_BACK,
		HELP_NEXT,
		HELP_NAME,
		HELP_CHECK,
		HELP_TRIBE,
		HELP_GENDER,
		HELP_HAIRCOLOR,
		HELP_SKINCOLOR,
		HELP_FACETYPE,
		HELP_STR,
		HELP_DEX,
		HELP_INT,
		HELP_HP,
		HELP_MP,
		HELP_DEFENCE,
		HELP_PROTECTION,
		HELP_TOHIT,
		HELP_DAMAGE,
		HELP_SAVE,
		HELP_LOAD,
		HELP_REROLL,
		HELP_BONUS,
		HELP_MAX,
	};
	int							m_focused_help;

//	bool							m_bl_skin_color_control;
	S_SLOT *						m_p_slot; // character manager의 slot중 하나를 가리킨다.
	int							m_selected_slot; // 선택된 slot: char manager에 있지만, 필요함.
//	WORD *						m_p_hp_buf;
//	WORD *						m_p_mp_buf;
//	int							m_hp_width;
//	int							m_hp_height;
//	int							m_mp_width;
//	int							m_mp_height;

//	C_SPRITE_PACK *			m_pC_board_spk;
//	C_SPRITE_PACK *			m_pC_new_char_spk;
//	C_VS_UI_CHAR_APPEARANCE *	m_pC_char_appearance;

	ButtonGroup *				m_pC_button_group;
//	ButtonGroup *				m_pC_button_group_face;

	LineEditorVisual			m_lev_name;

	// for color change
	static int					m_hair_color_array[COLOR_LIST_X][COLOR_LIST_Y];
	static int					m_skin_color_array[COLOR_LIST_X][COLOR_LIST_Y];
	Point							m_hair_point;
	Point							m_skin_point;
	bool							m_bl_pushed_table;

	int							m_failed_code;

	int							m_iSave[3];

	//
	// face 선택 list.
	// 현재 선택된 face가 맨 먼저 나오게 된다. 다른 것을 선택하면 다른 것이 먼저 나오고...
	//
//	int							m_man_face_list[3]; // 3 = face count
//	int							m_woman_face_list[3];

	bool							m_face_change_temp;

	Point GetPoint(int colorset, bool bl_skin_color);
	int	GetColor(int _x, int _y, bool bl_skin_color);
	bool	ChangeColor(int _x, int _y);

//	void	ResetFaceList(int * p_face_list, int selected_face);

//	void	DrawEnergy();
	void	SendNewCharacterToClient();
//	void	ChangeFace();
	void	_ShowCharacterOusters( int _x, int _y, S_SLOT* p_slot, int index, int enable, int dark  );
	void	_ShowCharacterSlayer( int _x, int _y, S_SLOT* p_slot, int index, int enable, int dark );
	void	_ShowCharacterVampire( int _x, int _y, S_SLOT* p_slot, int index, int enable, int dark );

	void	_ShowCharacterACOusters( int _x, int _y, S_SLOT* p_slot, int index, int enable, int dark );
	void	_ShowCharacterACSlayer( int _x, int _y, S_SLOT* p_slot, int index, int enable, int dark );
	void	_ShowCharacterACVampire( int _x, int _y, S_SLOT* p_slot, int index, int enable, int dark );


public:
	C_VS_UI_NEWCHAR();
	~C_VS_UI_NEWCHAR();

//	void	UsedID(bool used);

	void	AcquireDisappear() {}
	void	CancelPushState() { m_pC_button_group->CancelPushState(); }
	void	WindowEventReceiver(id_t event);
	bool	IsPixel(int _x, int _y);
	void	UnacquireMouseFocus();

	void	ShowButtonWidget(C_VS_UI_EVENT_BUTTON * p_button);
	void	ShowButtonDescription(C_VS_UI_EVENT_BUTTON * p_button);
	void	DrawCover(int _x, int _y);
//	void	DrawHP(int _x, int _y, int line);
//	void	DrawMP(int _x, int _y, int line);
//	void	SetEnergy(int cur_val, int max_val, WORD &percent, WORD &line);
	void	Run(id_t id);
	void	ShowCharacter(int _x, int _y, S_SLOT * p_slot, int index, int enable, int dark=false);
	void	Show();
	bool	MouseControl(UINT message, int _x, int _y);
	void	KeyboardControl(UINT message, UINT key, long extra);
	void	Start();
	void	Finish();
	void	Process();
	void	RollDice(bool load = false);
	void	SetError(int error) { m_failed_code = error; }
	void	SetCharacterToThisSlot(int slot, S_SLOT * p_slot);
//	void	SetBoardSpk(C_SPRITE_PACK * p_spk) { m_pC_board_spk = p_spk; }
};

class C_VS_UI_GO_BILING_PAGE;
//-----------------------------------------------------------------------------
// C_VS_UI_CHAR_MANAGER
//
// 
//-----------------------------------------------------------------------------
class C_VS_UI_CHAR_MANAGER : public Window, public Exec, public ButtonVisual, public C_VS_UI_DESC
{
private:
	enum SLOT_POSITION
	{ 
//		HEART_LEFT_X = 66, 
//		HEART_CENTER_X = 290, 
//		HEART_RIGHT_X = 530, 
//		HEART_Y = 120,

		SLOT = 3,
		SLOT_LEFT = 0,
		SLOT_CENTER = 1,
		SLOT_RIGHT = 2,
	};

	enum COMMON_SPK_ID
	{
		BACK,
		BACK_BUTTON,
		BACK_BUTTON_PUSHED,
		BACK_BUTTON_HILIGHTED,
		NEXT_BUTTON,
		NEXT_BUTTON_PUSHED,
		NEXT_BUTTON_HILIGHTED,
	};

	enum CHAR_MANAGER_SPK_ID
	{
		WINDOW,
		WINDOW_HILIGHTED,
		TITLE,
		DELETE_BUTTON,
		DELETE_BUTTON_PUSHED,
		DELETE_BUTTON_HILIGHTED,
		CREATE_BUTTON,
		CREATE_PUSHED,
		CREATE_HILIGHTED,
	};
public :

	enum EXEC_ID
	{
		BACK_ID,
		NEXT_ID,
		DELETE_1_ID,
		DELETE_2_ID,
		DELETE_3_ID,
		CREATE_ID,

		CANNOT_PLAY_ID,
		CANNOT_PLAY_BY_ATTR_ID,
	};
private :
	enum USEGRADE
	{
		CHILD = 0,
		ADULT,
	};

	C_VS_UI_CHAR_DELETE *	m_pC_char_delete;
	C_VS_UI_GO_BILING_PAGE *	m_pC_biling;

	C_SPRITE_PACK				m_common_spk;
	C_SPRITE_PACK				m_image_spk;
	//add by sonic 2006.9.29
	C_SPRITE_PACK				m_title1_spk;
	// end by sonic
	C_SPRITE_PACK*				m_pC_use_grade;

//	C_SPRITE_PACK				m_pC_menu_default;
//	C_SPRITE_PACK				m_pC_menu_select;

	ButtonGroup *				m_pC_button_group;

	C_VS_UI_NEWCHAR *			m_pC_newchar;
//	C_VS_UI_CHAR_INFO * 		m_pC_char_info;

	S_SLOT						m_slot[SLOT];
	int							m_select_heart, m_select_heart_temp;

	enum HELP
	{
		HELP_DEFAULT,
		HELP_BACK,
		HELP_NEXT,
		HELP_X,
		HELP_CREATE,
		HELP_MAX,
	};
	int							m_focused_help;

public:
	C_VS_UI_CHAR_MANAGER();
	~C_VS_UI_CHAR_MANAGER();

	bool	SelectSlot(int n);
//	void	UsedID(bool used)	{ if(m_pC_newchar)m_pC_newchar->UsedID(used); }
//	void	CloseCharInfo();
	void	AcquireDisappear() {}
	void	CancelPushState() { m_pC_button_group->CancelPushState(); }
	void	AcquireFirstSequence();
	void	UnacquireMouseFocus();
	void	WindowEventReceiver(id_t event);
	bool	IsPixel(int _x, int _y);

	void	ShowButtonWidget(C_VS_UI_EVENT_BUTTON * p_button);
	void	ShowButtonDescription(C_VS_UI_EVENT_BUTTON * p_button);
	void	Run(id_t id);
	bool	MouseControl(UINT message, int _x, int _y);
	void	KeyboardControl(UINT message, UINT key, long extra);
	void	Show();
	void	Process();
	void	Start(bool back = false);
	void	Finish();
	void	NewCharacterCreateOk();
	void	NewCharacterCreateFailed(int error);
	void	SetCharacter(int slot, S_SLOT &S_slot);
	void	DeleteCharacter(int slot);
	void	ClearAllCharacter();

};


//-----------------------------------------------------------------------------
// C_VS_UI_SERVER_SELECT
//
// 
//-----------------------------------------------------------------------------
class C_VS_UI_SERVER_SELECT : public Window, public Exec, public ButtonVisual, public C_VS_UI_DESC
{
public:
	enum SERVER_STATUS
	{
		STATUS_VERY_GOOD,
		STATUS_GOOD,
		STATUS_NORMAL,
		STATUS_BAD,
		STATUS_VERY_BAD,
		STATUS_DOWN,
	};

	enum GROUP_STATUS
	{
		STATUS_OPENED,
		STATUS_CLOSED,
	};


private:
	enum COMMON_SPK_ID
	{
		BACK,
		BACK_BUTTON,
		BACK_BUTTON_PUSHED,
		BACK_BUTTON_HILIGHTED,
		NEXT_BUTTON,
		NEXT_BUTTON_PUSHED,
		NEXT_BUTTON_HILIGHTED,
	};

	enum SERVER_SELECT_SPK_ID
	{
		MAIN_WINDOW,
		TITLE_WORLD,
		TITLE_SERVER,
		SCROLL_TAG,
		SCROLL_TAG_HILIGHTED,
		SCROLL_UP_BUTTON,
		SCROLL_UP_PUSHED,
		SCROLL_UP_HILIGHTED,
		SCROLL_DOWN_BUTTON,
		SCROLL_DOWN_PUSHED,
		SCROLL_DOWN_HILIGHTED,
	};

	enum EXEC_ID
	{
		BACK_ID,
		NEXT_ID,
		SCROLL_UP_ID,
		SCROLL_DOWN_ID,
	};

	bool m_bl_group;

	unsigned int				m_scroll;
	bool						m_bl_scrolling;

	enum
	{
		SCROLL_X = 670,
		SCROLL_Y = 220,
		SCROLL_WIDTH = 20,
		SCROLL_HEIGHT = 185,
	};

	C_SPRITE_PACK				m_common_spk;
	C_SPRITE_PACK				m_image_spk;
	C_SPRITE_PACK				m_title1_spk;

	ButtonGroup *				m_pC_button_group;

	std::vector<std::string>	m_server_name;
	std::vector<int>			m_server_id;
	std::vector<int>			m_server_status;

	int							m_focus_server;
	int							m_server_select;

	int							m_server_x;
	int							m_server_y;

	enum HELP
	{
		HELP_DEFAULT,
		HELP_BACK,
		HELP_NEXT,
		HELP_MAX,
	};
	int							m_focused_help;

public:
	C_VS_UI_SERVER_SELECT();
	~C_VS_UI_SERVER_SELECT();

	void	AcquireDisappear() {}
	void	CancelPushState() { m_pC_button_group->CancelPushState(); }
	void	AcquireFirstSequence();
	void	UnacquireMouseFocus();
	void	WindowEventReceiver(id_t event);
	bool	IsPixel(int _x, int _y);

	void	ShowButtonWidget(C_VS_UI_EVENT_BUTTON * p_button);
	void	Run(id_t id);
	bool	MouseControl(UINT message, int _x, int _y);
	void	KeyboardControl(UINT message, UINT key, long extra);
	void	Show();
	void	Process();
	void	Start(bool bGroup);
	void	Finish();

	void	SetServerList(LPSTR *name, int *id, int *status, int size, int default_id);
};



//-----------------------------------------------------------------------------
// class C_VS_UI_OPTION
//
//-----------------------------------------------------------------------------
class C_VS_UI_OPTION : public Window, public Exec, public ButtonVisual, public C_VS_UI_DESC
{
public :

	enum EXEC_ID
	{
		DEFAULT_ID = 0xffff0000,
		SAVE_ID,
		LOAD_ID,
		CLOSE_ID,

		CONTROL_ID,
		GRAPHIC_ID,
		SOUND_ID,
		GAME_ID,

		//CONTROL TAB
		LIST_ID,
	};

	enum EXEC_CHECK_ID
	{
		//CONTROL
		CHECK_CONTROL_TAB,
		CHECK_NORMAL_CHAT = CHECK_CONTROL_TAB,	//옵션만
		CHECK_ENTER_CHAT,						//옵션만
//		CHECK_MOUSE_SPEED,						//메세지??
		CHECK_IFEEL,							//UI처리

		//GRAPHIC
		CHECK_GRAPHIC_TAB,
		CHECK_3D = CHECK_GRAPHIC_TAB,			//옵션만
		CHECK_ALPHA_HPBAR,						//옵션만
		CHECK_BLOOD_DROP,						//옵션만
		CHECK_AUTOHIDE_SMOOTH,					//옵션만
		CHECK_GAMMA,							//UI처리
		CHECK_CHATBOX,							//옵션만
		CHECK_FPS,								//옵션만
		CHECK_DEFAULT_ALPHA,					//옵션만 ? 혹시 다른 처리도-.-
		CHECK_ALPHA_DEPTH,						//옵션만
		CHECK_DO_NOT_SHOW_PERSNALSHOP_MSG,		// 개인상점 스트링 안보기

		//SOUND
		CHECK_SOUND_TAB,
//		CHECK_YELL = CHECK_SOUND_TAB,			//옵션만
		CHECK_SOUND = CHECK_SOUND_TAB,							//메세지
		CHECK_MUSIC,							//메세지
		CHECK_WAV,								//메세지
		CHECK_MIDI,								//메세지

		//GAME
		CHECK_GAME_TAB,
		CHECK_HELP = CHECK_GAME_TAB,			//옵션만
		CHECK_FILTERING,						//옵션만
		CHECK_PRELOAD_MONSTER,
		CHECK_CHAT_WHITE,						//옵션만
		CHECK_TEEN_VERSION,
		CHECK_POPUPCHAT_BY_WHISPER,
		CHECK_DO_NOT_SHOW_WAR_MSG,				// 전쟁 메시지 보지 않기
		CHECK_DO_NOT_SHOW_LAIR_MSG,				// 레어마스터 메시지 보지 않기
		CHECK_DO_NOT_SHOW_HOLY_LAND_MSG,		// 아담의 성지 메시지 보지 않기
		CHECK_SHOW_GAMEMONEY_WITH_HANGUL,		// 게임머니 한글 병행 표시
//		CHECK_NOT_SEND_MY_INFO,			// 아직.
//		CHECK_CHANGE,							//옵션만
//		CHECK_PARTY_INVITE,						//옵션만
//		CHECK_PARTY_REQUEST,					//옵션만


		//VALUE
		CHECK_VALUE_MOUSE_SPEED,
		CHECK_VALUE_GAMMA,
		CHECK_VALUE_SOUND_VOLUME,
		CHECK_VALUE_MUSIC_VOLUME,

		CHECK_MAX,

		CHECK_CONTROL_MAX = 1,
		CHECK_GRAPHIC_MAX = 10,
		CHECK_SOUND_MAX = 2,
		//CHECK_GAME_MAX = 7,
		CHECK_GAME_MAX = 10,
	};

	enum MAIN_SPK
	{
		MAIN_WINDOW,
		HOTKEY_WINDOW,

		TAB_CONTROL,
		TAB_GRAPHIC,
		TAB_SOUND,
		TAB_GAME,
	};

	enum ETC_SPK
	{
		VOLUME_BAR,
		VOLUME_TAG,
		DEFAULT_BUTTON_HILIGHTED,
		DEFAULT_BUTTONHILIGHTED_PUSHED,
		LOAD_BUTTON_HILIGHTED,
		LOAD_BUTTON_HILIGHTED_PUSHED,
		SAVE_BUTTON_HILIGHTED,
		SAVE_BUTTON_HILIGHTED_PUSHED,
		RADIO_BACK_DISABLE,
		RADIO_BACK_ENABLE,
		RADIO_FOCUS,
		CHECK_BACK_DISABLE,
		CHECK_BACK_ENABLE,
		CHECK_FOCUS,
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

	};

	enum CHECK_TYPE
	{
		CHECK_NOT,
		CHECK_CHECK,
		CHECK_CHECK_DISABLE = 0xFFFE,
		CHECK_DISABLE = 0xFFFF,
	};

	enum VALUE_RECT
	{
		RECT_MOUSE_SPEED,
		RECT_GAMMA,
		RECT_SOUND,
		RECT_MUSIC,
		RECT_ALPHA,

		RECT_MAX
	};

	enum TITLE_SPK
	{
		TITLE_MAIN_WINDOW,
		TITLE_HOTKEY_WINDOW,

		TITLE_TAB_CONTROL_HILIGHT,
		TITLE_TAB_GRAPHIC_HILIGHT,
		TITLE_TAB_SOUND_HILIGHT,
		TITLE_TAB_GAME_HILIGHT,

		TITLE_TAB_CONTROL_PUSH,
		TITLE_TAB_GRAPHIC_PUSH,
		TITLE_TAB_SOUND_PUSH,
		TITLE_TAB_GAME_PUSH,

		TITLE_VOLUME_BAR,
		TITLE_VOLUME_TAG,

		TITLE_RADIO_BACK,
		TITLE_RADIO_FOCUS,

		TITLE_CHECK_BACK,
		TITLE_CHECK_FOCUS,

		TITLE_BUTTON_EXIT_HILIGHT,
		TITLE_BUTTON_EXIT_PUSH,
	};
private:
	C_VS_UI_SCROLL_BAR			*m_pC_scroll_bar;
	bool						m_bLBPush;

	int							m_focus_hotkey;

	CHECK_TYPE					m_check[CHECK_MAX];

	C_SPRITE_PACK	*			m_pC_main_spk;
	C_SPRITE_PACK	*			m_pC_etc_spk;

	ButtonGroup *				m_pC_button_group;
	ButtonGroup *				m_pC_control_button_group;
	ButtonGroup *				m_pC_graphic_button_group;
	ButtonGroup *				m_pC_sound_button_group;
	ButtonGroup *				m_pC_game_button_group;

	Rect						m_rt_value[RECT_MAX];
	int							m_value_mouse_speed, m_value_gamma, m_volume_sound, m_volume_music;

	int							m_check_x, m_check_y, m_check_gap;
	int							m_i_selected_tab;
	int							m_vampire_plus_x, m_vampire_plus_y;

	void						SetValue(int _x, enum VALUE_RECT rect_value);

	bool						m_IsTitle;
	int							Tab_X[4];
	int							Tab_Y;
public:
	C_VS_UI_OPTION(bool IsTitle = false);
	~C_VS_UI_OPTION();

	void	AcquireDisappear() {}
	void	CancelPushState() 
	{
		m_pC_button_group->CancelPushState();
		m_pC_control_button_group->CancelPushState();
		m_pC_graphic_button_group->CancelPushState();
		m_pC_sound_button_group->CancelPushState();
		m_pC_game_button_group->CancelPushState();
		m_pC_scroll_bar->CancelPushState();
	}

	void	AcquireFirstSequence();
	void	UnacquireMouseFocus();
	void	WindowEventReceiver(id_t event);
	bool	IsPixel(int _x, int _y);

	void	ShowButtonWidget(C_VS_UI_EVENT_BUTTON * p_button);
	void	Run(id_t id);
	bool	MouseControl(UINT message, int _x, int _y);
	void	KeyboardControl(UINT message, UINT key, long extra);
	void	Show();
	void	Process();
	void	Start();
	void	Finish();

};


//-----------------------------------------------------------------------------
// C_VS_UI_TITLE
//
// 
//-----------------------------------------------------------------------------
class C_VS_UI_TITLE : public Window, public Exec, public ButtonVisual
{
private:
	//---------------------------------------
	// Components
	//---------------------------------------
	C_VS_UI_LOGIN *			m_pC_login;
//	C_VS_UI_NEWUSER *			m_pC_newuser;
	C_VS_UI_CHAR_MANAGER *	m_pC_char_manager;
	C_VS_UI_SERVER_SELECT *		m_pC_server_select;
	

	C_SPRITE_PACK				m_title_spk;
	C_SPRITE_PACK				m_title_menu_default;
	C_SPRITE_PACK				m_title_menu_select;
	C_SPRITE_PACK *				m_pC_credit;
	C_SPRITE_PACK 				m_pC_title_ani;
//	C_SPRITE_PACK				m_pC_symbol_ani;
	C_SPRITE_PACK *				m_pC_use_grade;

#ifndef _LIB
	C_VS_UI_DIALOG *			m_pC_dialog;
#endif

	enum TITLE_MENU
	{ 
		CONNECT = 0,
//		TUTORIAL,
		OPTION,
//		INTRO,
		CREDIT,
		EXIT,

		INVALID_ID,
	};

	enum TITLE_SPK
	{ 
		CONNECT_HILIGHT = 0,
		CONNECT_PUST,
		OPTION_HILIGHT,
		OPTION_PUSH,
		CREDIT_HILIGHT,
		CREDIT_PUSH,
		EXIT_HILIGHT,
		EXIT_PUSH,

	};


	enum USEGRADE
	{
		CHILD = 0,
		ADULT,
	};

	ButtonGroup *				m_pC_button_group;
	bool							m_bl_credit;
	DWORD						m_dw_prev_tickcount;
	DWORD						m_dw_millisec;
	bool	Timer();
	int							m_credit_scroll;

public:
	C_VS_UI_TITLE();
	~C_VS_UI_TITLE();

	// 넷마블용
	void	RunConnect()			{ if(m_pC_login) m_pC_login->Start(); }

	void	RunCannotPlayMessage();
	void	RunCannotPlayByAttrMessage();

	void	SelectCharacter(int n)	{ if(m_pC_char_manager)m_pC_char_manager->SelectSlot(n); }
//	void	UsedID(bool used)	{ if(m_pC_char_manager)m_pC_char_manager->UsedID(used); }

	void	SetServerList(LPSTR *name, int *id, int *status, int size, int default_id)	{ if(m_pC_server_select)m_pC_server_select->SetServerList(name, id, status, size, default_id); }

//	void	CloseUserRegistrationWindow();
//	void	RunNewUserRegistration();
//	void	CloseCharInfo();
	void	CancelPushState() { m_pC_button_group->CancelPushState(); }
	void	WindowEventReceiver(id_t event);
	bool	IsPixel(int _x, int _y);
	void	AcquireMouseFocus();
	void	UnacquireMouseFocus();
	void	AcquireFirstSequence();
	void	AcquireDisappear() {}

	void	ShowButtonWidget(C_VS_UI_EVENT_BUTTON * p_button);
	void	Run(id_t id);
	void	Show();
	void	Process();
	bool	MouseControl(UINT message, int _x, int _y);
	void	KeyboardControl(UINT message, UINT key, long extra);
	void	Start();
	void	Finish();
	void	StartCharacterManager(bool back = false);
	void	StartServerSelect(bool bGroup);
	void	SetCharacter(int slot, S_SLOT &S_slot);
	void	ClearAllCharacter();
	void	NewCharacterCreateOk();
	void	NewCharacterCreateFailed(int error);
	void  DeleteCharacter(int slot);


	//
	// gamemenu option
//	void	RunOption();
//	void	CloseOption();
//	bool	IsRunningOption(){ return (m_pC_option != NULL); }	// by larosel

};

//-----------------------------------------------------------------------------
// C_VS_UI_CHAR_DELETE
//
// 
//-----------------------------------------------------------------------------
class C_VS_UI_GO_BILING_PAGE : public Window, public Exec, public ButtonVisual
{
public :
	enum BILING_MSG_LIST 
	{
		CANNOT_PLAY_MSG = 0,
		CANNOT_PLAY_BY_ATTR_MSG,

		BILING_MSG_MAX,
		BILING_MSG_NULL,
	};
private:
	enum BILING_MENU
	{
		BILING_OK,
		BILING_CANCEL,
	};
	
	ButtonGroup *				m_pC_button_group;
	BILING_MSG_LIST				m_biling_mode;

public:
	C_VS_UI_GO_BILING_PAGE(BILING_MSG_LIST msg);
	~C_VS_UI_GO_BILING_PAGE();

	void	ShowButtonWidget(C_VS_UI_EVENT_BUTTON * p_button);
	void	ShowButtonDescription(C_VS_UI_EVENT_BUTTON * p_button);
	void	UnacquireMouseFocus() { m_pC_button_group->UnacquireMouseFocus(); }
	void	CancelPushState() { m_pC_button_group->CancelPushState(); }
	void	AcquireDisappear() {}
	void	WindowEventReceiver(id_t event);
	bool	IsPixel(int _x, int _y);

	void	Start();
	void	Finish();
	void	Process();
	void	Show();
	void	Run(id_t id);
	bool	MouseControl(UINT message, int _x, int _y);
	void	KeyboardControl(UINT message, UINT key, long extra);	
};

#endif