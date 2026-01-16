//////////////////////////////////////////////////////////////////////
//
// Filename    : GCSystemAvailabilitiesHandler.cc
// Written By  : elca@ewestsoft.com
// Description :
//
//////////////////////////////////////////////////////////////////////

// include files
#include "Client_PCH.h"
#include "GCSystemAvailabilities.h"
#include <bitset>
#include "SystemAvailabilities.h"

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void GCSystemAvailabilitiesHandler::execute ( GCSystemAvailabilities * pGCSystemAvailabilities , Player * pPlayer )
	 throw ( Error )
{
	__BEGIN_TRY
	
	g_pSystemAvailableManager->SetFlag( pGCSystemAvailabilities->getFlag() );
	g_pSystemAvailableManager->SetLimitSkillLevel( pGCSystemAvailabilities->getSkillLimit() );
	g_pSystemAvailableManager->SetOpenDegree( pGCSystemAvailabilities->getDegree() - 1 );

	__END_CATCH
}
