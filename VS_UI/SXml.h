//////////////////////////////////////////////////////////////////////////////
/// \file XML.h
/// \author excel96
/// \date 2003.7.25
///
/// \todo
/// \bug
/// \warning
//////////////////////////////////////////////////////////////////////////////

#pragma once

//#pragma warning (push, 1)
//	#include <xercesc/util/PlatformUtils.hpp>
//	#include <xercesc/sax2/XMLReaderFactory.hpp>
//	#include <xercesc/sax2/SAX2XMLReader.hpp>
//	#include <xercesc/sax2/Attributes.hpp>
//	#include <xercesc/sax2/DefaultHandler.hpp>
//#pragma warning (pop)


#pragma warning(disable:4786)

#ifdef PLATFORM_WINDOWS
#include <windows.h>
#else
#include "../../basic/Platform.h"
#include <unistd.h>
#endif
#include <map>
#include <vector>
#include <string>
#include <fstream>
using std::map;
using std::vector;
using std::string;

//XERCES_CPP_NAMESPACE_USE

//////////////////////////////////////////////////////////////////////////////
/// \class XMLUtil
/// \brief
//////////////////////////////////////////////////////////////////////////////

class XMLUtil
{
public:
	static string trim(const string& str);
	static void filelog(char* fmt, ...);
	static string WideCharToString(const wchar_t * wstr, int wstrlen = -1);
};


//////////////////////////////////////////////////////////////////////////////
/// \class StrX
/// \brief 
//////////////////////////////////////////////////////////////////////////////

//class StrX
//{
//private:
//	char* m_pCSTR; ///< ���� ���ڿ� ����
//
//public:
//	StrX(const XMLCh* const toTranscode) { m_pCSTR = XMLString::transcode(toTranscode); }
//	~StrX() { XMLString::release(&m_pCSTR); }
//
//	const char* c_str() const { return m_pCSTR; }
//};

//////////////////////////////////////////////////////////////////////////////
/// \class XMLAttribute
/// \brief 
//////////////////////////////////////////////////////////////////////////////
class XMLAttribute
{
private:
	string	m_Name;
	string	m_Value;

public:
	XMLAttribute( IN const string &name, IN const string &value );
	virtual ~XMLAttribute();

	OUT const char*		GetName() const;
	OUT const char*		ToString() const;
	OUT const int		ToInt() const;
	OUT const DWORD		ToHex() const;
	OUT const bool		ToBool() const;
};


//////////////////////////////////////////////////////////////////////////////
/// \class XMLTree
/// \brief 
//////////////////////////////////////////////////////////////////////////////

class XMLTree
{
private:
	typedef map<string, XMLAttribute *> ATTRIBUTES_MAP;
	typedef vector<XMLAttribute *> ATTRIBUTES_VECTOR;
	
	typedef map<string, XMLTree*> CHILDREN_MAP;
	typedef vector<XMLTree *> CHILDREN_VECTOR;

	string     m_Name;        ///< ����� �̸�
	string     m_Text;        ///< ��忡 ���� �ؽ�Ʈ
	XMLTree*   m_pParent;     ///< �θ� ����� ������

	ATTRIBUTES_MAP m_AttributesMap;  ///< ������ �ִ� �Ӽ�����
	ATTRIBUTES_VECTOR m_AttributesVector;  ///< ������ �ִ� �Ӽ�����
	CHILDREN_MAP   m_ChildrenMap;    ///< �ڽ� ����
	CHILDREN_VECTOR   m_ChildrenVector;    ///< �ڽ� ����

public:
	XMLTree();
	XMLTree( IN const string& name );
	virtual ~XMLTree();

public:
	OUT const string& GetName() const;
	void SetName( IN const string& name );

	OUT const string& GetText() const;
	void SetText( IN const string& text );


	OUT const XMLTree* GetParent() const;
	void SetParent( IN XMLTree* pParent );

	void AddAttribute( IN const string& name, IN const string& value );
	OUT const XMLAttribute *GetAttribute( IN const string& name ) const;

	OUT XMLTree* AddChild( IN const string& name );
	OUT XMLTree* AddChild( IN XMLTree *pChild );
	OUT XMLTree* AddChildOnlyVector( IN XMLTree *pChild );
	OUT const XMLTree* GetChild( IN const string& name ) const;
	OUT const XMLTree* GetChild( IN size_t index ) const;
	OUT const XMLTree* GetChildByAttr( IN size_t index , IN const string& name ) const;
	OUT const size_t GetChildCount() const;

	void Release();

	void Save(const char* pFilename);

private:
	void Save(std::ofstream& file, size_t indent);
};

//////////////////////////////////////////////////////////////////////////////
/// \class XMLTreeGenerator
/// \brief 
//////////////////////////////////////////////////////////////////////////////

//class XMLTreeGenerator : public DefaultHandler
//{
//private:
//	XMLTree* m_pRoot;   ///< �ֻ��� ���
//	XMLTree* m_pBuffer; ///< XML �Ľ̿� �ӽ� ���
//
//
//public:
//	XMLTreeGenerator( IN XMLTree* pTree );
//	virtual ~XMLTreeGenerator();
//
//
//public:
//	// Handlers for the SAX ContentHandler interface
//	void startElement( IN const XMLCh* const uri, IN const XMLCh* const localname, IN const XMLCh* const qname, IN const Attributes& attrs );
//	void endElement( IN const XMLCh* const uri, IN const XMLCh* const localname, IN const XMLCh* const qname );
//	void characters( IN const XMLCh* const chars, IN const unsigned int length );
//	void ignorableWhitespace( IN const XMLCh* const, IN const unsigned int ) {}
//	void resetDocument();
//
//	// Handlers for the SAX ErrorHandler interface
//	void warning( IN const SAXParseException& e );
//	void error( IN const SAXParseException& e );
//	void fatalError( IN const SAXParseException& e );
//
//
//private:
//	XMLTreeGenerator( IN const XMLTreeGenerator& ) {}
//	XMLTreeGenerator& operator = ( IN const XMLTreeGenerator& ) { return *this; }
//};

//////////////////////////////////////////////////////////////////////////////
/// \class XMLParser
/// \brief 
//////////////////////////////////////////////////////////////////////////////

class XMLParser
{
private:
//	DefaultHandler* m_pHandler;


public:
	XMLParser();
	virtual ~XMLParser();


public:
	/// \brief ������ ��ġ�� �ִ� ���� �Ǵ� �� ������ �Ľ��Ѵ�.
//	void parseURL(const char* pURL);

	/// \brief �μ��� �Ѱ����� ���ڿ��� XML ������ �����ϰ� �Ľ��Ѵ�.
	char* parse(char* buffer, XMLTree *pTree, bool IsUseOnlyVector = false);
};

