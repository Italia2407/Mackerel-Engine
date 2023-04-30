#include "AudioListener.h"

namespace MCK::EntitySystem
{
	/**
	 * Get references to audioEngine and transform component.
	 * 
	 */
	void AudioListener::OnCreate()
	{
		// get a reference to the audio engine
		audioEngine = &entity->scene->audioEngine;

		// get a reference to a transform componenet
		transform = entity->GetComponent<MCK::EntitySystem::TransformComponent>();
	}

	/**
	 * Update the listener transform by passing information from the 
	 *	transform component to the audio engine.
	 * 
	 */
	void AudioListener::OnUpdate()
	{
		audioEngine->UpdateTransform(transform->Position(), transform->Rotation());
	}	
	
	void AudioListener::OnDestroy()
	{

	}

	bool AudioListener::Deserialise(json data)
	{
		return true;
	}
}
