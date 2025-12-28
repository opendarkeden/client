//----------------------------------------------------------------------
// CFramePack.cpp
//----------------------------------------------------------------------
#include "Frame_PCH.h"
#include "CFramePack.h"

//----------------------------------------------------------------------
// Info To File
//----------------------------------------------------------------------
/*
void		
CEffectFramePack::InfoToFile(const char* filename)
{
	ofstream file(filename);	

	for (int type=0; type<m_Size; type++)
	{
		//if (m_pData[type].GetSize()==0)
		//{
			//file << "[" << type << "][NONE]" << endl;
		//}
	
		for (int dir=0; dir<m_pData[type].GetSize(); dir++)
		{
			//if (m_pData[type][action][dir].GetSize()==0)
			//{
			//	file << "[" << type << "][" << action << "][" << dir << "][NONE]" << endl;
			//}

			for (int frame=0; frame<m_pData[type][dir].GetSize(); frame++)
			{
				int spriteID = m_pData[type][dir][frame].GetSpriteID();
				int cx = m_pData[type][dir][frame].GetCX();
				int cy = m_pData[type][dir][frame].GetCY();
				file << "[" << type << "][" << dir << "][" << frame << "] = ( " << spriteID << ",  " << cx << ", " << cy << " ) " << endl;
			}
		}	
	}

	file.close();
}
*/

//----------------------------------------------------------------------
// Info To File
//----------------------------------------------------------------------
void		
CCreatureFramePack::InfoToFile(const char* filename)
{
	ofstream file(filename);	

	for (int type=0; type<m_Size; type++)
	{
		//if (m_pData[type].GetSize()==0)
		//{
			//file << "[" << type << "][NONE]" << endl;
		//}

		for (int action=0; action<m_pData[type].GetSize(); action++)
		{
			//if (m_pData[type][action].GetSize()==0)
			//{
			//	file << "[" << type << "][" << action << "][NONE]" << endl;
			//}

			for (int dir=0; dir<m_pData[type][action].GetSize(); dir++)
			{
				//if (m_pData[type][action][dir].GetSize()==0)
				//{
				//	file << "[" << type << "][" << action << "][" << dir << "][NONE]" << endl;
				//}

				for (int frame=0; frame<m_pData[type][action][dir].GetSize(); frame++)
				{
					int spriteID = m_pData[type][action][dir][frame].GetSpriteID();
					int cx = m_pData[type][action][dir][frame].GetCX();
					int cy = m_pData[type][action][dir][frame].GetCY();
					file << "[" << type << "][" << action << "][" << dir << "][" << frame << "] = ( " << spriteID << ",  " << cx << ", " << cy << " ) " << endl;
				}
			}
		}
	}

	file.close();
}