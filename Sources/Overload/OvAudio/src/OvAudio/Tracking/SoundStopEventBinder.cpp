/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#include "OvAudio/Tracking/SoundStopEventBinder.h"

OvAudio::Tracking::SoundStopEventBinder::SoundStopEventBinder(OvTools::Eventing::Event<>& p_soundFinishedEvent) : SoundFinishedEvent(p_soundFinishedEvent)
{
}

void OvAudio::Tracking::SoundStopEventBinder::OnSoundStopped(irrklang::ISound * sound, irrklang::E_STOP_EVENT_CAUSE reason, void * userData)
{
	SoundFinishedEvent.Invoke();
}
