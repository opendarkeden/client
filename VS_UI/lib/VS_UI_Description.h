/*-----------------------------------------------------------------------------

	VS_UI_Description.h

	All description.

	2000.12.09. KJTINC

-----------------------------------------------------------------------------*/

#ifndef __VS_UI_DESCRIPTION_H__
#define __VS_UI_DESCRIPTION_H__

#pragma warning(disable:4786)

#include "VS_UI_Descriptor.h"

// descriptor id
const id_t DID_ITEM = 1;
const id_t DID_SKILL = 2;
const id_t DID_HELP = 3;
const id_t DID_SKILLTREE = 4;
const id_t DID_INFO = 5;
const id_t DID_STRINGS = 6;
const id_t DID_BLOOD_BIBLE_STATUS = 7;
const id_t DID_MULTILINE_INFO = 8;
void	g_InstallDescriptor();
#endif