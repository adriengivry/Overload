/**
* @project: Overload
* @author: Overload Tech.
* @restrictions: This software may not be resold, redistributed or otherwise conveyed to a third party.
*/

#include "OvAudio/Tracking/SoundStopEventBinder.h"

OvAudio::Tracking::SoundStopEventBinder::SoundStopEventBinder(OvTools::Eventing::Event<>& p_soundFinishedEvent) : SoundFinishedEvent(p_soundFinishedEvent)
{
}

void OvAudio::Tracking::SoundStopEventBinder::OnSoundStopped(irrklang::ISound * sound, irrklang::E_STOP_EVENT_CAUSE reason, void * userData)
{
	SoundFinishedEvent.Invoke();
}
