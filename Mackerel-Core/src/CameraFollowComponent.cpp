#include "CameraFollowComponent.h"

namespace MCK::EntitySystem 
{

	void CameraFollowComponent::OnCreate()
	{
		// get reference to the camera component
		camera = entity->GetComponent<MCK::EntitySystem::PerspectiveCamera>();

		// get player object
		player = entity->scene->FindEntityWithTag("Player");

		// get reference to player transform
		transform = player->GetComponent<MCK::EntitySystem::TransformComponent>();

		// define an offset
		offset = Eigen::Vector3f(1.f, 5.5f, 10.5f);

		// set x for smoothing, greater than zero, default to 7
		x = 7.f;

	}

	void CameraFollowComponent::OnUpdate()
	{
		// player position + offset
		Eigen::Vector3f position = transform->Position() + offset;

		// get delta (scaled frame time)
		float delta = (float)TimeManager::getScaledFrameTime();

		// set target position of camera with smoothing
		// (camera + (position + offset) * x * delta)
		//camera->Position() = camera->Position() + position * x * delta;
		camera->Position() = position;
	}

	void CameraFollowComponent::OnDestroy()
	{

	}

	bool CameraFollowComponent::Deserialise(json data)
	{
		return true;
	}

}
