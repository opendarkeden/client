//////////////////////////////////////////////////////////////////////////////
// Filename    : GCAddBat.h 
// Written By  : Reiot
// Description :
//////////////////////////////////////////////////////////////////////////////

#ifndef __GC_ADD_BAT_H__
#define __GC_ADD_BAT_H__

#include "Packet.h"
#include "PacketFactory.h"
#include "Types\GuildTypes.h"

//////////////////////////////////////////////////////////////////////////////
// class GCAddBat;
//
// �þ� �ȿ� Bat �� ������ ���, �� ��Ŷ�� Bat ������ ��Ƽ� ���۹޴´�.
//////////////////////////////////////////////////////////////////////////////

class GCAddBat : public Packet 
{
public:
	void read(SocketInputStream & iStream) throw(ProtocolException, Error);
	void write(SocketOutputStream & oStream) const throw(ProtocolException, Error);
	void execute(Player* pPlayer) throw(ProtocolException, Error);
	PacketID_t getPacketID() const throw() { return PACKET_GC_ADD_BAT; }
	PacketSize_t getPacketSize() const throw() 
	{ 
		return szObjectID 
			+ szBYTE + m_Name.size() 
			//+ szSpriteType 
			//+ szColor + szColor
			+ szItemType
			+ szCoord + szCoord + szDir
			+ szHP*2
			+ szGuildID
			+ szColor
			;
	}

	#ifdef __DEBUG_OUTPUT__	
		std::string getPacketName() const throw() { return "GCAddBat"; }
		std::string toString() const throw();
	#endif

public:
	ObjectID_t getObjectID() const throw() { return m_ObjectID; }
	void setObjectID(ObjectID_t creatureID) throw() { m_ObjectID = creatureID; }

	std::string getName() const throw() { return m_Name; }
	void setName(std::string name) throw() { m_Name = name; }

	/*
	// get/set sprite type
	SpriteType_t getSpriteType() const throw() { return m_SpriteType; }
	void setSpriteType(SpriteType_t spriteType) throw() { m_SpriteType = spriteType; }

	// get/set main color
	Color_t getMainColor() const throw() { return m_MainColor; }
	void setMainColor(Color_t color) throw() { m_MainColor = color; }

	// get/set sub color
	Color_t getSubColor() const throw() { return m_SubColor; }
	void setSubColor(Color_t color) throw() { m_SubColor = color; }
	*/

	// get/set X
	Coord_t getX() const throw() { return m_X; }
	void setXYDir(Coord_t x, Coord_t y, Dir_t Dir) throw() { m_X = x; m_Y = y; m_Dir = Dir;}
	
	// get/set Y
	Coord_t getY() const throw() { return m_Y; }

	// get/set Dir
	Dir_t getDir() const throw() { return m_Dir; }

	// get /set MaxHP
	HP_t getMaxHP() const throw() { return m_MaxHP; }
	void setMaxHP(HP_t MaxHP) throw() { m_MaxHP = MaxHP; }

	// get /set CurrentHP
	HP_t getCurrentHP() const throw() { return m_CurrentHP; }
	void setCurrentHP(HP_t CurrentHP) throw() { m_CurrentHP = CurrentHP; }

	// get / set ItemType
    ItemType_t getItemType() const throw() { return m_ItemType; }
    void setItemType(ItemType_t ItemType) throw() { m_ItemType = ItemType; }

	// get/set GuildID
	GuildID_t getGuildID() const throw() { return m_GuildID; }
	void setGuildID(GuildID_t GuildID) throw() { m_GuildID = GuildID; }

	Color_t		getBatColor() const throw() { return m_BatColor; }
	void		setBatColor(WORD set) { m_BatColor = set; }

private:
	ObjectID_t   m_ObjectID;
	std::string       m_Name;       // Bat�� �̸�
	
	/*
	SpriteType_t m_SpriteType; // ��������Ʈ Ÿ��
	Color_t      m_MainColor;  // ���� Į��
	Color_t      m_SubColor;   // ���� Į��
	*/

	// ���� �������� ����
	ItemType_t	 m_ItemType;

	Coord_t      m_X;          // X
	Coord_t      m_Y;          // Y
	Dir_t        m_Dir;        // Direction
	HP_t         m_CurrentHP;  // current hp
	HP_t         m_MaxHP;      // max hp
	GuildID_t	 m_GuildID;    // GuildID
	Color_t		 m_BatColor;
};


//////////////////////////////////////////////////////////////////////////////
// class GCAddBatFactory;
//////////////////////////////////////////////////////////////////////////////

class GCAddBatFactory : public PacketFactory 
{
public:
	Packet* createPacket() throw() { return new GCAddBat(); }

	#ifdef __DEBUG_OUTPUT__	
		std::string getPacketName() const throw() { return "GCAddBat"; }
	#endif

	PacketID_t getPacketID() const throw() { return Packet::PACKET_GC_ADD_BAT; }
	PacketSize_t getPacketMaxSize() const throw() 
	{
		return szObjectID 
			+ szBYTE + 20 
			+ szItemType
			//+ szSpriteType 
			//+ szColor + szColor
			+ szCoord + szCoord + szDir
			+ szHP*2
			+ szGuildID
			+ szColor
			;
	}
};

//////////////////////////////////////////////////////////////////////////////
// class GCAddBatHandler;
//////////////////////////////////////////////////////////////////////////////

class GCAddBatHandler 
{
public:
	static void execute(GCAddBat* pPacket, Player* pPlayer) throw(Error);

};

#endif
