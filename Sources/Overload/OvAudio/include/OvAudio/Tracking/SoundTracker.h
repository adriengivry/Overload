/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#pragma once

#include <irrklang/irrKlang.h>


#include "OvAudio/Tracking/SoundStopEventBinder.h"

namespace OvAudio::Tracking
{
	/**
	* Track a playing sound and allow the modification of its settings
	*/
	class SoundTracker
	{
	public:
		/**
		* Constructor
		* @param p_track
		*/
		SoundTracker(irrklang::ISound* p_track);

		/**
		* Destructor
		*/
		~SoundTracker();

		/**
		* Returns the tracked sound instance
		*/
		irrklang::ISound* GetTrack() const;

	public:
		/**
		* FinishedEvent is called when the track get stopped/finished
		*/
		OvTools::Eventing::Event<> StopEvent;

	private:
		irrklang::ISound* const m_track = nullptr;

	private:
		SoundStopEventBinder m_soundStopEventBinder;
	};
}