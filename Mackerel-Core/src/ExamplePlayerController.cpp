#include "ExamplePlayerController.h"
#include "InputComponent.h"
#include "TimeManager.h"

using namespace MCK::EntitySystem;

namespace MCK::ExamplePlayer
{
	void ExamplePlayerController::OnCreate()
	{
		transform = entity->GetComponent<TransformComponent>();
		rigidbody = entity->GetComponent<Physics::RigidbodyComponent>();
		input = entity->GetComponent<InputComponent>();
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
		if (input->JumpPressed())
		{
			velocity.y() = jumpVel;
			rigidbody->SetLinearVelocity(velocity);
		}


	}

	void ExamplePlayerController::OnDestroy() 
	{

	}

	TypeInfoRef ExamplePlayerController::GetType()
	{
		return typeid(ExamplePlayerController);
	}

	bool ExamplePlayerController::Deserialise(json data)
	{
		return true;
	}
}

