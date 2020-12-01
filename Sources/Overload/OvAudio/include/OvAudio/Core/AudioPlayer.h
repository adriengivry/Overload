/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#pragma once


#include "OvAudio/Core/AudioEngine.h"
#include "OvAudio/Resources/Sound.h"

namespace OvAudio::Core
{
	/**
	* Handle sounds playback
	*/
	class AudioPlayer
	{
	public:
		/**
		* Constructor
		* @param p_audioEngine
		*/
		AudioPlayer(AudioEngine& p_audioEngine);

		/**
		* Play a sound in 2D and return a SoundTracker if tracking is asked
		* @param p_sound
		* @param p_autoPlay
		* @param p_looped
		* @param p_track
		*/
		std::unique_ptr<Tracking::SoundTracker> PlaySound(const Resources::Sound& p_sound, bool p_autoPlay = true, bool p_looped = false, bool p_track = false);

		/**
		* Play a sound in 3D and return a SoundTracker if tracking is asked
		* @param p_sound
		* @param p_autoPlay
		* @param p_looped
		* @param p_position
		* @param p_track
		*/
		std::unique_ptr<Tracking::SoundTracker> PlaySpatialSound(const Resources::Sound& p_sound, bool p_autoPlay = true, bool p_looped = false, const OvMaths::FVector3& p_position = {0.0f, 0.0f, 0.0f}, bool p_track = false);

	private:
		AudioEngine& m_audioEngine;
	};
}