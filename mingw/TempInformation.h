//----------------------------------------------------------------------
// TempInformation.h
//----------------------------------------------------------------------
// Client의 정보 중에서 변할 수 있는 것
//----------------------------------------------------------------------

#ifndef	__TEMPINFORMATION_H__
#define	__TEMPINFORMATION_H__

#include <Windows.h>

class TempInformation {
	public :
		enum TEMP_MODE
		{
			MODE_NULL,
			MODE_SKILL_LEARN,		// (domainType, maxLevel, skillType)
			MODE_SHOP_BUY,			// (shelfType, index, x, y, MShop*)
			MODE_SHOP_SELL,			// (MItem*)
			MODE_SHOP_SELL_ALL_SKULL,		// nothing.. (해골 다 팔기)
			MODE_SHOP_REPAIR,		// (MItem*)
			MODE_SHOP_SILVERING,	// (MItem*)
			MODE_STORAGE_BUY,		// (price)
			MODE_STORAGE_BUY_WAIT,	// (nothing.. - -)

			MODE_TRADE_REQUEST,		// (ID)
			MODE_TRADE_VERIFY_DROP_TO_INVENTORY,		// (ID, x, y, MItem*)
			MODE_TRADE_VERIFY_INSERT_TO_INVENTORY,
			MODE_TRADE_VERIFY_PICKUP_FROM_INVENTORY,	// (ID, x, y, MItem*)
			MODE_TRADE_VERIFY_PICKUP_TO_QUICKSLOT,		// (slot,     MItem*)

			MODE_TRADE_VERIFY_CANCEL,					// OK한거 취소
			MODE_TRADE_VERIFY_CLOSE,					// 완전 닫을 때
			MODE_TRADE_VERIFY_ADD_MONEY,				// (money)
			MODE_TRADE_VERIFY_REMOVE_MONEY,				// (money)
			MODE_TRADE_VERIFY_ADD_ITEM,					// (ID, x, y, MItem*)
			MODE_TRADE_VERIFY_REMOVE_ITEM,				// (ID, x, y, MItem*)

			MODE_PARTY_REQUEST,		// (ID)

			MODE_SKILL_MAKE_ITEM,	// (itemClass, itemType)

			MODE_BONUSPOINT_USE,	// (part(0,1,2=str,dex,int))
	
			MODE_WAIT_RESURRECT,	// UI를 기다리는 경우

			MODE_ITEM_TO_ITEM,		// 어떤 아이템에 별을 쓸려고 했는가?
			MODE_WAIT_BRING_FEE,	// 세금 가져온거 검증 패킷이 올때까지..
			MODE_INPUT_NAME,		// 이름 입력중이면 파라미터에 해당 NPC ID 를.
			MODE_SELECT_QUEST,		// 스킬 선택
			MODE_ITEM_MIXING,		// 아이템 믹싱
			MODE_ITEM_CODE_SHEET,	// 코드 시트에 넣을때 아이템 타입

			MODE_PETITEM_MOVETO_INVENTORY, // 펫 보관함 -> 인벤
			MODE_PETITEM_MOVETO_PETSTORAGE, //  인벤 -> 펫 아이템

			MODE_SMS_ADD_ADDRESS,	// sms 주소 추가
			MODE_SMS_DELETE_ADDRESS,// sms 주소 삭제
			
			MODE_NICKNAME_CHANGE_CUSTOM, // 커스텀 닉네임 변경
			MODE_NICKNAME_SELECT_NICK, // 닉네임 선택 변경

			MAX_MODE
		};

	public :
		TempInformation();
		~TempInformation();

		//--------------------------------------------------------
		// File I/O
		//--------------------------------------------------------
		//void	SaveToFile(const char* filename);
		//bool	LoadFromFile(const char* filename);
		TEMP_MODE		Mode;

	public :
		void			SetMode(TEMP_MODE mode);
		const TEMP_MODE GetMode() const;

		int				Value1;
		int				Value2;
		int				Value3;
		int				Value4;

		int				PartyInviter;	// 검증 없이 처리되는거라서리.. - -;

		void*			pValue;
};


extern	TempInformation*		g_pTempInformation;

#endif

