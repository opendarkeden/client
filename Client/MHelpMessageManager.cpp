#include "Client_PCH.h"
#include "MHelpMessageManager.h"

#define dSTRING_LEN 2048

MHelpMessage::MHelpMessage()
{
	m_messageType = MESSAGETYPE_NORMAL;
	m_strKeyword = "";
	m_strEvent = "";
	for(int race = 0; race < RACE_MAX; race++)
	{
		m_strDetail[race] = "";
		m_strTitle[race] = "";
		m_iSender[race] = -1;
		m_iLevelMax[race] = -1;
		m_iLevelLow[race] = -1;
		m_iDomainMax[race] = -1;
		m_iDomainLow[race] = -1;
		m_iAttrMax[race] = -1;
		m_iAttrLow[race] = -1;
		
	}
}

MHelpMessage::~MHelpMessage()
{
}

MHelpMessageManager::MHelpMessageManager()
{
	m_KeyCnt = -1;
	m_SenderCnt = -1; // 갯수
	LoadHelpMessageRpk("helpmessage.txt");
}

MHelpMessageManager:: ~MHelpMessageManager()
{
}

void MHelpMessageManager::LoadFromFile(const char * filename)
{	
	ifstream file(filename, ios::binary| ios::nocreate);
	if(!file)
	{
		return;
	}
	else
	{
		LoadFromFile(file);
		file.close();
	}

}

void MHelpMessageManager::SaveToFile(const char * filename)
{
	ofstream file(filename, ios::binary);
	if(file)
	{
		SaveToFile(file);
		file.close();
	}
}
// HelpMessageRpk 파일 로드 
bool MHelpMessageManager::LoadHelpMessageRpk(const char *helprpkfilename)
{
	m_pack_file.SetRAR(RPK_HELP, RPK_PASSWORD);
	if(!m_pack_file.IsSet())return false;
	if(!m_pack_file.Open(helprpkfilename)){	return false;}

	MHelpMessage message;
	MString		 m_sender;
	char sztemp[dSTRING_LEN];
	std::string temp,temp2,tempend;
	char seps[] = " ";
	char *token;
	int  levtemp[7],i,Sendercnt;
	bool detail_flag = false;
	char * iskeyword = NULL,* isMessagetype = NULL,* isEvent = NULL ,* isTitle = NULL;
	char * isLevel = NULL,	* isDetail= NULL,   * isDetailEnd= NULL,* isSender = NULL;
	m_pack_file.GetString(sztemp, dSTRING_LEN);
	Sendercnt = atoi(sztemp);
	m_pack_file.GetString(sztemp, dSTRING_LEN);

	for(i = 0; i<Sendercnt; i++)		// 보내는 사람 숫자를 센다
	{
		m_pack_file.GetString(sztemp, dSTRING_LEN);
		temp = sztemp;		
		int nCharToDel2 = 0;
		int len2 = temp.length();
		for (std::string::reverse_iterator itor = temp.rbegin(); itor != temp.rend(); itor++) // 보내는 사람뒤에 공백이나 개행처리
		{
			char c = *itor;
			if (c=='\r')
				nCharToDel2++;
			else break;
		}
		temp.erase(len2 - nCharToDel2, len2);
		m_sender = temp.c_str();
		MHelpMessageManager::Instance().addSender(m_sender);
		temp = "";

	}
	m_pack_file.GetString(sztemp, dSTRING_LEN);
	MHelpMessageManager::Instance().m_KeyCnt = atoi(sztemp);
	int current_race = 0;
	while(m_pack_file.GetString(sztemp, dSTRING_LEN))
	{
		temp = sztemp;
		char c;
		int nCharToDel = 0;
		if(isDetail)
		{
			
		}
		else
		{
			for (std::string::iterator itr = temp.begin(); itr != temp.end(); itr++) // 앞에 공백이나 탭이있을경우
			{
				c = *itr;
				if (c == ' ' || c == '\t')
					nCharToDel++;
				else break;
			}
			
			temp.erase(0, nCharToDel);			//공백이나 탭만큼 삭제해준다
		}

		nCharToDel = 0;
		int len = temp.length();
		for (std::string::reverse_iterator itor = temp.rbegin(); itor != temp.rend(); itor++)
		{
			c = *itor;
			if (c=='\r')
				nCharToDel++;
			else break;
		} 
		// 뒤쪽 개행처리공백처리
		if(isTitle)
			temp.erase(len - nCharToDel, len);
		else
			temp.replace( temp.begin()+len-nCharToDel, temp.begin() + len, "\r\n");
		
		if(iskeyword)		// 키워드를 읽었을떄
		{	
			temp.erase(temp.length()-2 ,2);
			message.m_strKeyword  = temp.c_str();
			iskeyword = NULL;
			temp  = "";
		}
		if(isMessagetype) // 메세지 타입
		{
			message.m_messageType = atoi(sztemp);
			isMessagetype = NULL;
		}
		if(isEvent)  // 이벤트 
		{
			temp.erase(temp.length()-2 ,2);
			message.m_strEvent  = temp.c_str();
			isEvent = NULL;
			temp = "";
		}
		if(isTitle) // 타이틀일 경우
		{
			message.m_strTitle[current_race]  = temp.c_str();
			isTitle = NULL; 
			temp = "";
		}
		if(isSender) // 보내는 사람일 경우
		{
			message.m_iSender[current_race]  = atoi(sztemp);	
			isSender = NULL;
		}	
		if(isLevel) // 레벨
		{
			char levline[dSTRING_LEN];
			memset(levline,0,dSTRING_LEN);
			memcpy(levline, temp.c_str(), temp.length());
			token = strtok(levline, seps);							// 레벨
			int cnt = 0;
			while(token!=NULL)
			{
				levtemp[cnt++]=atoi(token);
				token = strtok( NULL, seps );
			}
			message.m_iLevelLow[current_race] = levtemp[0];
			message.m_iLevelMax[current_race] = levtemp[1];
			message.m_iDomainLow[current_race]= levtemp[2];
			message.m_iDomainMax[current_race]= levtemp[3];
			message.m_iAttrLow[current_race]=   levtemp[4];
			message.m_iAttrMax[current_race]=   levtemp[5];
            isLevel = NULL;
		}
		isDetailEnd = strstr(temp.c_str(), "{End}"); 

		if(isDetail)
		{
			detail_flag = true;
			if(isDetailEnd) // 내용의 마지막일경우 처리
			{
				if(temp2.length() > 0)
					temp2.erase(temp2.length()-2 ,2);
					message.m_strDetail[current_race] = temp2.c_str();
				isDetail = NULL;
				temp2 = "";
				current_race++;
				isDetailEnd = NULL;
				detail_flag = false;
			}
			else // 내용이 태그가 들어오면 End 가 들어올떄까지 계속 detail 에 저장한다
			{
				temp2 += temp.c_str();
			}
		}

		iskeyword = strstr(temp.c_str(),"[===KeyWord===]");					
		isMessagetype = strstr(temp.c_str(),"[===MessageType===]");
		isEvent   = strstr(temp.c_str(),"[===Event===]");
		isTitle  = strstr(temp.c_str(),"[==Title==]");
		isSender = strstr(temp.c_str(),"[==Sender==]");
		isLevel  = strstr(temp.c_str(),"[==Level 조건표==]");
		if(detail_flag == false) 
		{
			isDetail = strstr(temp.c_str(),"[==Detail==]");
		}
		temp = "";
		if(current_race == 3)
		{
			MHelpMessageManager::Instance().addMessage(message);
			current_race = 0;
		}
	}

	m_pack_file.Release();
	return true;
}


void MHelpMessageManager::LoadFromFile(class ifstream &file)
{
	MHelpMessage message;
	MString		 m_sender;
	char sztemp[dSTRING_LEN];
	std::string temp,temp2,tempend;
	char seps[] = " ";
	char *token;
	int  levtemp[7];
	int i;
	bool detail_flag = false;
	char * iskeyword = NULL,* isMessagetype = NULL,* isEvent = NULL ,* isTitle = NULL;
	char * isLevel = NULL,	* isDetail= NULL,   * isDetailEnd= NULL,* isSender = NULL;

	file.getline(sztemp, dSTRING_LEN);			
	m_SenderCnt = atoi(sztemp);
	file.getline(sztemp, dSTRING_LEN);			// 한줄 건너서 읽기// 
	for(i = 0; i<m_SenderCnt; i++)		// 보내는 사람 숫자를 센다
	{
		file.getline(sztemp,dSTRING_LEN);		
		temp = sztemp;		
		int nCharToDel2 = 0;
		int len2 = temp.length();
		for (std::string::reverse_iterator itor = temp.rbegin(); itor != temp.rend(); itor++) // 보내는 사람뒤에 공백이나 개행처리
		{
			char c = *itor;
			if (c=='\r')
				nCharToDel2++;
			else break;
		}
		temp.erase(len2 - nCharToDel2, len2);
		m_sender = temp.c_str();
		MHelpMessageManager::Instance().addSender(m_sender);
		temp = "";

	}
	file.getline(sztemp,dSTRING_LEN); 
	MHelpMessageManager::Instance().m_KeyCnt = atoi(sztemp);
	int current_race = 0;
	while(file.getline(sztemp,dSTRING_LEN))
	{
		temp = sztemp;
		char c;
		int nCharToDel = 0;
	/*	for (std::string::iterator itr = temp.begin(); itr != temp.end(); itr++) // 앞에 공백이나 탭이있을경우
		{
			c = *itr;
			if (c == ' ' || c == '\t')
				nCharToDel++;
			else break;
		}
		temp.erase(0, nCharToDel);			//공백이나 탭만큼 삭제해준다*/

		if(isDetail)
		{
			
		}
		else
		{
			for (std::string::iterator itr = temp.begin(); itr != temp.end(); itr++) // 앞에 공백이나 탭이있을경우
			{
				c = *itr;
				if (c == ' ' || c == '\t')
					nCharToDel++;
				else break;
			}
			
			temp.erase(0, nCharToDel);			//공백이나 탭만큼 삭제해준다
		}

		nCharToDel = 0;

		int len = temp.length();
		for (std::string::reverse_iterator itor = temp.rbegin(); itor != temp.rend(); itor++)
		{
			c = *itor;
			if (c=='\r')
				nCharToDel++;
			else break;
		} 
		// 뒤쪽 개행처리공백처리
		if(isTitle)
			temp.erase(len - nCharToDel, len);
		else
			temp.replace( temp.begin()+len-nCharToDel, temp.begin() + len, "\r\n");
		
		if(iskeyword)		// 키워드를 읽었을떄
		{	
			temp.erase(temp.length()-2 ,2);
			message.m_strKeyword  = temp.c_str();
			iskeyword = NULL;
			temp  = "";
		}
		if(isMessagetype) // 메세지 타입
		{
			message.m_messageType = atoi(sztemp);
			isMessagetype = NULL;
		}
		if(isEvent)  // 이벤트 
		{
			temp.erase(temp.length()-2 ,2);
			message.m_strEvent  = temp.c_str();
			isEvent = NULL;
			temp = "";
		}
		if(isTitle) // 타이틀일 경우	
		{
			message.m_strTitle[current_race]  = temp.c_str();
			isTitle = NULL; 
			temp = "";
		}
		if(isSender) // 보내는 사람일 경우
		{
			message.m_iSender[current_race]  = atoi(sztemp);	
			isSender = NULL;
		}	
		if(isLevel) // 레벨
		{
			char levline[100];
			memset(levline,0,100);
			memcpy(levline, temp.c_str(), 100);
			token = strtok(levline, seps);							// 레벨
			int cnt = 0;
			while(token!=NULL)
			{
				levtemp[cnt++]=atoi(token);
				token = strtok( NULL, seps );
			}
			message.m_iLevelLow[current_race] = levtemp[0];
			message.m_iLevelMax[current_race] = levtemp[1];
			message.m_iDomainLow[current_race]= levtemp[2];
			message.m_iDomainMax[current_race]= levtemp[3];
			message.m_iAttrLow[current_race]=   levtemp[4];
			message.m_iAttrMax[current_race]=   levtemp[5];
            isLevel = NULL;
		}
		isDetailEnd = strstr(temp.c_str(), "{End}"); 

		if(isDetail)
		{
			detail_flag = true;
			if(isDetailEnd) // 내용의 마지막일경우 처리
			{
				if(temp2.length() > 0)
					temp2.erase(temp2.length()-2 ,2);
					message.m_strDetail[current_race] = temp2.c_str();
				isDetail = NULL;
				temp2 = "";
				current_race++;
				isDetailEnd = NULL;
				detail_flag = false;
			}
			else // 내용이 태그가 들어오면 End 가 들어올떄까지 계속 detail 에 저장한다
			{
				temp2 += temp.c_str();
			}
		}

		iskeyword = strstr(temp.c_str(),"[===KeyWord===]");					
		isMessagetype = strstr(temp.c_str(),"[===MessageType===]");
		isEvent   = strstr(temp.c_str(),"[===Event===]");
		isTitle  = strstr(temp.c_str(),"[==Title==]");
		isSender = strstr(temp.c_str(),"[==Sender==]");
		isLevel  = strstr(temp.c_str(),"[==Level 조건표==]");
		if(detail_flag == false) 
		{
			isDetail = strstr(temp.c_str(),"[==Detail==]");
		}
		temp = "";
		if(current_race == 3)
		{
			MHelpMessageManager::Instance().addMessage(message);
			current_race = 0;
		}
	}
	file.close();
}

void MHelpMessageManager::SaveToFile(class ofstream &file)
{
	int i;
	size_t strlen = 0;
	file << m_SenderCnt << "\r\n";
	file << "[===Sender===]" << "\r\n";

	for(i = 0; i< m_SenderCnt; i++)
	{
		const MString& strSender = MHelpMessageManager::Instance().getSender(i);
		file << strSender << "\r\n";
	}
	m_KeyCnt = this->getMessageSize();
	file <<  m_KeyCnt << "\r\n";
	for(i=0 ; i< m_KeyCnt; i++)
	{
		const MHelpMessage& message = MHelpMessageManager::Instance().getMessage(i);
		file << "[===KeyWord===]" << "\r\n" ;
		file <<  message.m_strKeyword  << "\r\n";
		file << "[===MessageType===]" << "\r\n" ;
		file << "    " << message.m_messageType << "\r\n";
		file << "	[===Event===]" << "\r\n" ;
		strlen =  message.m_strEvent.GetLength();
		if( strlen > 0 )
		{
			file << "	  " <<  message.m_strEvent  << "\r\n";
		}
		else
		{
		file <<  "\r\n" ;
		}
		// 메세지 타입 넣어야함
		for(int j = 0; j<3; j++)
		{

			switch(j)
			{
			case 0:	file << "		#SLAYERPAGE" << "\r\n"; break;
			case 1:	file << "		#VAMPIREPAGE" << "\r\n"; break;
			case 2:	file << "		#OUTSTERSPAGE" << "\r\n"; break;
			}
			file << "		[==Title==]" << "\r\n" ;
			strlen =  message.m_strTitle[j].GetLength();
			if( strlen > 0 )
			{
				file << "		   " <<   message.m_strTitle[j]<< "\r\n";
			}
			else
			{
			file <<  "\r\n" ;
			}
			file << "		[==Sender==]" << "\r\n";
			file << "		   " << message.m_iSender[j] << "\r\n";
			file << "		[==Level 조건표==]" << "\r\n" ;
			file << "		   " << message.m_iLevelLow[j] << " " << message.m_iLevelMax[j] << " " <<
				message.m_iDomainLow[j]<< " " << message.m_iDomainMax[j] << " " <<
				message.m_iAttrLow[j] << " "  << message.m_iAttrLow[j] << "\r\n";
			file << "		[==Detail==]" << "\r\n" ;
			strlen =  message.m_strDetail[j].GetLength();
			if( strlen > 0 )
				file <<  message.m_strDetail[j] << "\r\n" ;
			file << "{End}" << "\r\n" ;
		}
	}
	file.close();
}
