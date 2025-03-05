//-----------------------------------------------------------------------------
// PacketFunction2.h
//-----------------------------------------------------------------------------
// 임시로.. 헤더 때문에 생기는 컴파일을 줄일려고 분리했다. - -
//-----------------------------------------------------------------------------

#ifndef __PACKETFUNCTION2_H__
#define __PACKETFUNCTION2_H__

class MActionResultNode;
class MCreature;

extern MActionResultNode*	CreateActionResultNode(MCreature* pCreature, int skillID, BYTE grade = 0);

#endif