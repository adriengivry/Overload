/**
* @project: Overload
* @author: Overload Tech.
* @licence: MIT
*/

#include "OvAudio/Entities/AudioListener.h"

OvTools::Eventing::Event<OvAudio::Entities::AudioListener&>	OvAudio::Entities::AudioListener::CreatedEvent;
OvTools::Eventing::Event<OvAudio::Entities::AudioListener&>	OvAudio::Entities::AudioListener::DestroyedEvent;

OvAudio::Entities::AudioListener::AudioListener() :
	m_transform(new OvMaths::FTransform()),
	m_internalTransform(true)
{
	Setup();
}

OvAudio::Entities::AudioListener::AudioListener(OvMaths::FTransform& p_transform) :
	m_transform(&p_transform),
	m_internalTransform(false)
{
	Setup();
}

OvAudio::Entities::AudioListener::~AudioListener()
{
	DestroyedEvent.Invoke(*this);
}

OvMaths::FTransform& OvAudio::Entities::AudioListener::GetTransform()
{
	return *m_transform;
}

void OvAudio::Entities::AudioListener::SetEnabled(bool p_enable)
{
	m_enabled = p_enable;
}

bool OvAudio::Entities::AudioListener::IsEnabled() const
{
	return m_enabled;
}

void OvAudio::Entities::AudioListener::Setup()
{
	CreatedEvent.Invoke(*this);
}
