//----------------------------------------------------------------------
// MRequestMode.h
//----------------------------------------------------------------------

#ifndef __MREQUESTMODE_H__
#define __MREQUESTMODE_H__

class MRequestMode {
	public :
		enum REQUEST_MODE
		{
			REQUEST_NULL,
			REQUEST_TRADE,
			REQUEST_PARTY,
			REQUEST_INFO,
		};

	public :
		MRequestMode();
		~MRequestMode();

		bool			IsRequestMode() const			{ return m_RequestMode!=REQUEST_NULL; }
		REQUEST_MODE	GetRequestMode() const			{ return m_RequestMode; }
		bool			IsRequestTrade() const			{ return m_RequestMode==REQUEST_TRADE; }
		bool			IsRequestParty() const			{ return m_RequestMode==REQUEST_PARTY; }
		bool			IsRequestInfo() const			{ return m_RequestMode==REQUEST_INFO; }
		void			SetRequestMode(REQUEST_MODE rm)	{ m_RequestMode = rm; }
		void			UnSetRequestMode()				{ m_RequestMode = REQUEST_NULL; }

	public :
		REQUEST_MODE	m_RequestMode;
};

#endif