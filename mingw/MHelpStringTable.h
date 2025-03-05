//---------------------------------------------------------------------------
// MHelpStringTable.h
//---------------------------------------------------------------------------

#ifndef __MHELPSTRINGTABLE_H__
#define __MHELPSTRINGTABLE_H__

#include "MStringArray.h"
#include "MHelpDef.h"

class MHelpStringTable : public MStringArray {
	public :
		MHelpStringTable();
		~MHelpStringTable();

		//------------------------------------------------------
		// Init
		//------------------------------------------------------
		void		Init( int size );

		//------------------------------------------------------
		// Displayed
		//------------------------------------------------------
		void		ClearDisplayed();
		bool		IsDisplayed(HELP_OUTPUT ho)		{ return m_Displayed[ho]; }

		//-------------------------------------------------------
		// Reference
		//-------------------------------------------------------		
		MString&	operator [] (int type);
		MString&	Get(int type);

		//-------------------------------------------------------
		// File I/O
		//-------------------------------------------------------
		//void			SaveToFile(ofstream& file);
		void			LoadFromFile(ifstream& file);

	protected :
		CTypeTable<bool>		m_Displayed;
};

extern MHelpStringTable*	g_pHelpStringTable;

#endif
