//-----------------------------------------------------------------------------
// MEffectManager.h
//-----------------------------------------------------------------------------

#ifndef __MEFFECTMANAGER_H__
#define __MEFFECTMANAGER_H__

#pragma warning(disable:4786)

#include <list>
class MEffect;

class MEffectManager {
	public :
		// Effect list
		typedef std::list<MEffect*>						EFFECT_LIST;

	public :
		MEffectManager();
		virtual ~MEffectManager();

		//------------------------------------------------------
		// Release
		//------------------------------------------------------
		virtual void		Release();

		//------------------------------------------------------
		// Add
		//------------------------------------------------------
		virtual void		AddEffect(MEffect* pEffect);

		//------------------------------------------------------
		// Update
		//------------------------------------------------------
		virtual void		Update() = 0;

		//------------------------------------------------------
		// list
		//------------------------------------------------------
		int					GetSize() const			{ return m_listEffect.size(); }
		EFFECT_LIST::const_iterator GetEffects()	{ return m_listEffect.begin(); }

	protected :
		EFFECT_LIST			m_listEffect;
};

#endif

