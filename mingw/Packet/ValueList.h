//////////////////////////////////////////////////////////////////////////////
// Filename    : ValueList.h 
// Written By  : 
// Description :
//////////////////////////////////////////////////////////////////////////////

#ifndef __VALUE_LIST_H__
#define __VALUE_LIST_H__

#include "Types.h"
#include "Exception.h"
#include "Packet.h"

#ifdef __GAME_CLIENT__
#include "Assert.h"
#endif

//////////////////////////////////////////////////////////////////////////////
// class ValueList
//////////////////////////////////////////////////////////////////////////////
template <class T>
class ValueList 
{
public:
    void read (SocketInputStream & iStream) throw (ProtocolException, Error);
    void write (SocketOutputStream & oStream) const throw (ProtocolException, Error);

	PacketSize_t 	getPacketSize () const throw()		{ return szBYTE + sizeof(T) * m_Values.size(); }
	static uint 	getPacketMaxSize() throw() 	{ return szBYTE + sizeof(T) * 255; }

#ifdef __GAME_CLIENT__
	std::string toString () const throw ();
	std::list<T>&					GetList() { return m_Values; }
	std::list<T>::const_iterator		Begin() { return m_Values.begin(); }
	std::list<T>::const_iterator		End() { return m_Values.end(); }
	bool						IsEmpty() { return m_Values.empty(); }
#endif

public:
	int getSize() const throw() 			{ return m_Values.size(); }

	void addValue(const T& info) throw() 	{ m_Values.push_back(info); }

	T popValue() throw() 
	{ 
		const T info = m_Values.front(); 
		m_Values.pop_front(); 
		return info; 
	}

private:
	std::list<T> m_Values;

};


//////////////////////////////////////////////////////////////////////////////
// 입력스트림(버퍼)으로부터 데이타를 읽어서 패킷을 초기화한다.
//////////////////////////////////////////////////////////////////////////////
template <class T>
void ValueList<T>::read ( SocketInputStream & iStream ) 
	 throw ( ProtocolException , Error )
{
	__BEGIN_TRY
		
	BYTE numValue;
	iStream.read( numValue );

	T info;

	for( int i = 0; i < numValue; i++ ) 
	{
		iStream.read( info );
		m_Values.push_back( info );
	}

	__END_CATCH
}

		    
//////////////////////////////////////////////////////////////////////////////
// 출력스트림(버퍼)으로 패킷의 바이너리 이미지를 보낸다.
//////////////////////////////////////////////////////////////////////////////
template <class T>
void ValueList<T>::write ( SocketOutputStream & oStream ) 
     const throw ( ProtocolException , Error )
{
	__BEGIN_TRY
		
	BYTE numValue = m_Values.size();
	oStream.write( numValue );

	std::list<T>::const_iterator itr = m_Values.begin();
    for (; itr!= m_Values.end(); itr++) 
	{
		oStream.write( *itr );
	}

	__END_CATCH
}


#ifdef __DEBUG_OUTPUT__

//////////////////////////////////////////////////////////////////////////////
// get packet's debug std::string
//////////////////////////////////////////////////////////////////////////////
template <class T>
std::string ValueList<T>::toString () 
	const throw ()
{
	__BEGIN_TRY

	StringStream msg;

	msg << "Values(";

	std::list<T>::const_iterator itr = m_Values.begin();

	for ( ; itr!= m_Values.end() ; itr++ ) {
		const T& info = *itr;
		msg << (int)info << ",";
	}

	msg << ")";

	return msg.toString();

	__END_CATCH
}


#endif

#endif
