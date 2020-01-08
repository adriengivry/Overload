/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#pragma once

#include <irrklang/irrKlang.h>
#include <irrklang/ik_ISoundStopEventReceiver.h>

#include <OvTools/Eventing/Event.h>

namespace OvAudio::Tracking
{
	/**
	* This class is an interface between irrklang sound stop event and OvTools event
	*/
	class SoundStopEventBinder : public irrklang::ISoundStopEventReceiver
	{
	public:
		/**
		* Constructor
		* @param p_soundFinishedEvent
		*/
		SoundStopEventBinder(OvTools::Eventing::Event<>& p_soundFinishedEvent);

		/**
		* Called when the sound is stopped
		* @param sound
		* @param reason
		* @param userData
		*/
		virtual void OnSoundStopped(irrklang::ISound* sound, irrklang::E_STOP_EVENT_CAUSE reason, void* userData) override;

	private:
		OvTools::Eventing::Event<>& SoundFinishedEvent;
	};
}