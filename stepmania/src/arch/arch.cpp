/*
 * This file provides functions to create driver objects.
 */

#include "../stdafx.h"
#include "../RageLog.h"
#include "../RageUtil.h"

#include "../PrefsManager.h"
#include "arch.h"

/* Load default drivers. */
#include "arch_default.h"

/* Override them with arch-specific drivers, as available. */
#if defined(WIN32)
#include "arch_Win32.h"
#endif

LoadingWindow *MakeLoadingWindow() { return new ARCH_LOADING_WINDOW; }
ErrorDialog *MakeErrorDialog() { return new ARCH_ERROR_DIALOG; }
ArchHooks *MakeArchHooks() { return new ARCH_HOOKS; }

/* Err, this is ugly--breaks arch encapsulation. Hmm. */
RageSoundDriver *MakeRageSoundDriver(CString drivers)
{
	CStringArray DriversToTry;
	split(drivers, ",", DriversToTry, true);

	CString Driver;
	RageSoundDriver *ret = NULL;

	for(unsigned i = 0; ret == NULL && i < DriversToTry.size(); ++i)
	{
		try {
			Driver = DriversToTry[i];
			LOG->Trace("Initializing driver: %s", DriversToTry[i].GetString());

#if defined(WIN32)
			if(DriversToTry[i] == "DirectSound") ret = new RageSound_DSound;
			else if(DriversToTry[i] == "DirectSound-sw") ret = new RageSound_DSound_Software;
			else if(DriversToTry[i] == "WaveOut") ret = new RageSound_WaveOut;
#else
#warn No sound drivers defined!
			if(0) ;
#endif
			else if(DriversToTry[i] == "Null") ret = new RageSound_Null;
			else
				LOG->Warn("Unknown sound driver name: %s", DriversToTry[i].GetString());
		}
		catch(const RageException &e) {
			LOG->Info("Couldn't load driver %s: %s", DriversToTry[i].GetString(), e.what());
		}
	}

	if(ret)
		LOG->Info("Sound driver: %s", Driver.GetString());
	
	return ret;
}

/*
 * Copyright (c) 2002 by the person(s) listed below.  All rights reserved.
 *
 * Glenn Maynard
 */
