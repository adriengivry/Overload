/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#include "OvAudio/Tracking/SoundTracker.h"

OvAudio::Tracking::SoundTracker::SoundTracker(irrklang::ISound* p_track) :
	m_track(p_track),
	m_soundStopEventBinder(StopEvent)
{
	// m_track->setSoundStopEventReceiver(&m_soundStopEventBinder);
}

OvAudio::Tracking::SoundTracker::~SoundTracker()
{
	// m_track->setSoundStopEventReceiver(nullptr);
}

irrklang::ISound* OvAudio::Tracking::SoundTracker::GetTrack() const
{
	return m_track;
}
