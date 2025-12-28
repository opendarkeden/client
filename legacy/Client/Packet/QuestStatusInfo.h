#ifndef __QUEST_STATUS_INFO_H__
#define __QUEST_STATUS_INFO_H__

#include "Types.h"

#include "Packet.h"
#include "SocketInputStream.h"
#include "SocketOutputStream.h"

#include <list>
#include <VECTOR>
#define MAX_MISSION_NUM	100

// ui에서 사용될 구조체 - string이 추가되어 있다.
struct UI_GMissionInfo{
	std::string		szMissionTitle;
	std::string		m_StrArg;
	DWORD			m_NumArg;
	BYTE			bStatus;
	BYTE			bCondition;	// 어느 조건에 있는가 0 : Happen, 1 : Complete, 2 : Fail, 3 : Reward
	WORD			bIndex;		// 해당 조건의 몇번째 element인가
	DWORD			dwTimeLimit;// 시간 제한 퀘스트일 경우 처음 남음 시간 세팅 
};
struct UI_GQuestInfo{
	DWORD			dwQuestID;
	BYTE			bStatus;
	std::string		szQuestTitle;
	std::string		szQuestDescription;
	std::vector<UI_GMissionInfo*> vMissionList;
};
// ui에서 사용될 구조체
struct MissionInfo
{
	enum Status
	{
		HIDE = 0,			// 보여주지 않음
		CURRENT,			// 지금 해야 되는 미션 (수행중)
		SUCCESS,			// 이미 성공한 미션
		FAIL,				// 실패
	};

	BYTE	m_Condition;	// 어느 조건에 있는가 0 : Happen, 1 : Complete, 2 : Fail, 3 : Reward
	WORD	m_Index;		// 해당 조건의 몇번째 element인가
	BYTE	m_Status;		// 현재 상태

	string	m_StrArg;		// 찍어줄 문자열
	DWORD	m_NumArg;		// 찍어줄 숫자

	void read(SocketInputStream& iStream) throw(ProtocolException, Error)
	{
		iStream.read(m_Condition);
		iStream.read(m_Index);
		iStream.read(m_Status);

		BYTE szSTR;
		iStream.read(szSTR);
		if ( szSTR != 0 ) iStream.read(m_StrArg, szSTR);

		iStream.read(m_NumArg);
	}

	void write(SocketOutputStream& oStream) const throw(ProtocolException, Error)
	{
		oStream.write(m_Condition);
		oStream.write(m_Index);
		oStream.write(m_Status);

		BYTE szSTR = m_StrArg.size();
		oStream.write( szSTR );
		if ( szSTR != 0 ) oStream.write( m_StrArg );

		oStream.write( m_NumArg );
	}

	PacketSize_t getSize() const { return szBYTE + szWORD + szBYTE + szBYTE + m_StrArg.size() + szDWORD; }
	static PacketSize_t getMaxSize() { return szBYTE + szWORD + szBYTE + szBYTE + 255 + szDWORD; }
};

class QuestStatusInfo
{
public:
	enum Status
	{
		CANNOT = 0,		// 아직 불가능
		CAN_ACCEPT,		// 수행가능
		DOING,			// 수행중
		SUCCESS,		// 성공 (아직 보상받지 않음)
		COMPLETE,		// 완료 (보상받음, 재수행 불가능)
		FAIL,			// 실패 (재수행 불가능)
		CAN_REPLAY,		// 재수행 가능
	};

	void read(SocketInputStream& iStream) throw(ProtocolException, Error);
	void write(SocketOutputStream& oStream) const throw(ProtocolException, Error);
	PacketSize_t getSize() const;
	static PacketSize_t getMaxSize();
	DWORD GetQuestID() {return m_QuestID;}
	BYTE  GetStatus() {return m_Status;}
	const std::list<MissionInfo>& GetMission() const {return m_Missions;}
private:
	DWORD	m_QuestID;
	BYTE	m_Status;	// 위의 enum 참조
	std::list<MissionInfo>	m_Missions;
};

#endif
