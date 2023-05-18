#include "ExamplePlayerController.h"
#include "InputComponent.h"
#include "TimeManager.h"
#include <iostream>

using namespace MCK::EntitySystem;

namespace MCK::ExamplePlayer
{
	void ExamplePlayerController::OnPlayerCollision(MCK::Physics::CollisionData data)
	{
		// test collision
		std::cout << "Collision" << std::endl;

		if (data.collidedEntity->HasTag("death"))
		{
			rigidbody->SetPosition(startPosition);
			std::cout << "Player died: Respawning" << std::endl;
		}
		else
			lastGroundTime = TimeManager::GetUpTime();
	}

	void ExamplePlayerController::OnCreate()
	{
		transform = entity->GetComponent<TransformComponent>();
		rigidbody = entity->GetComponent<Physics::RigidbodyComponent>();
		input = entity->GetComponent<InputComponent>();

		playerCollisionCallback = std::bind(&ExamplePlayerController::OnPlayerCollision, this, std::placeholders::_1);
		receipt = rigidbody->onCollisionHandler.Register(playerCollisionCallback);

		startPosition = rigidbody->GetPosition();
	}

	void ExamplePlayerController::OnUpdate()
	{
		Eigen::Vector2f movInput = input->Direction();

		// Add Move Force
		Eigen::Vector3f moveForce
			= Eigen::Vector3f(input->Direction().x(), 0, input->Direction().y())
			* moveForceMag;

		rigidbody->AddCentralForce(moveForce);
		
		// Apply damping
		Eigen::Vector3f velocity = rigidbody->GetLinearVelocity();

		Eigen::Vector3f dampingForce
			= Eigen::Vector3f(-velocity.x(), 0, -velocity.z()) * horizontalDamping;

		rigidbody->AddCentralForce(dampingForce);

		// Apply deceleration
		if (movInput.norm() < 0.3f)
		{
			Eigen::Vector3f planearVelocity = Eigen::Vector3f(velocity.x(), 0, velocity.z());
			Eigen::Vector3f decel = -planearVelocity.normalized();
			decel = decel * deceleration * TimeManager::getFrameTime();

			if (decel.norm() > planearVelocity.norm())
			{
				velocity = Eigen::Vector3f(0, velocity.y(), 0);
			}
			else
			{
				velocity += decel;
			}

			rigidbody->SetLinearVelocity(velocity);
		}
	
		// Jump
		if ((input->JumpPressed() || input->JumpHeld()) && (TimeManager::GetUpTime() - lastGroundTime < 0.5) && velocity.y() < 0.8)
		{
			velocity.y() = jumpVel;
			rigidbody->SetLinearVelocity(velocity);
		}
	}

	void ExamplePlayerController::OnDestroy() 
	{
		rigidbody->onCollisionHandler.Deregister(receipt);
	}

	bool ExamplePlayerController::Deserialise(json data)
	{
		return true;
	}
}

