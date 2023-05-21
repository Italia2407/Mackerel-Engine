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

		if (data.collidedEntity->HasTag("death"))
		{
			rigidbody->SetPosition(startPosition);
			std::cout << "Info: Player died - Respawning" << std::endl;
		}
		else
			lastGroundTime = TimeManager::GetUpTime();
	}

	void ExamplePlayerController::OnCreate()
	{
		transform = entity->GetComponent<TransformComponent>();
		rigidbody = entity->GetComponent<Physics::RigidbodyComponent>();
		input = entity->GetComponent<InputComponent>();
		skinnedMesh = entity->GetComponent<SkinnedMeshRendererComponent>();

		if (entity->childEntities.size() > 0)
			skinnedMesh = entity->childEntities[0]->GetComponent<SkinnedMeshRendererComponent>();

		playerCollisionCallback = std::bind(&ExamplePlayerController::OnPlayerCollision, this, std::placeholders::_1);
		receipt = rigidbody->onCollisionHandler.Register(playerCollisionCallback);

		rigidbody->SetCharacter();
		rigidbody->SetMass(4);
		rigidbody->DisableRotation();

		startPosition = rigidbody->GetPosition();
	}

	void ExamplePlayerController::OnUpdate()
	{
		Eigen::Vector2f movInput = input->Direction();

		// Add Move Force
		Eigen::Vector3f moveForce
			= Eigen::Vector3f(input->Direction().x(), 0, input->Direction().y())
			* moveForceMag;
		
		/* Player rotation */
		if (moveForce.squaredNorm() > 0.001f)
		{
			/* rotation angles of facing direction and movement direction */
			float true_rotation_angle = atan2(smoothMoveVector.z(), -smoothMoveVector.x()) - atan2(1.0f, 0.0f);
			float target_rotation_angle = atan2(movInput.y(), -movInput.x()) - atan2(1.0f, 0.0f);

			/* convert to quaternions */
			Eigen::Quaternionf true_q = Eigen::Quaternionf(Eigen::AngleAxisf(true_rotation_angle, Eigen::Vector3f(0.0f, 1.0f, 0.0f)));
			Eigen::Quaternionf target_q = Eigen::Quaternionf(Eigen::AngleAxisf(target_rotation_angle, Eigen::Vector3f(0.0f, 1.0f, 0.0f)));

			/* interpolate 'em to get smooth movement */
			float smooth_t = turnRate * static_cast<float>(TimeManager::getScaledFrameTime());
			Eigen::Quaternionf smooth_q = true_q.slerp(smooth_t, target_q);
			smoothMoveVector = smooth_q * Eigen::Vector3f(0.0f, 0.0f, 1.0f);

			/* apply the new facing direction */
			transform->Rotation() = smooth_q;
		}

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

			if (skinnedMesh)
			{
				if (abs(velocity.y()) < 0.2f && (TimeManager::GetUpTime() - lastGroundTime < 0.5))
					skinnedMesh->PlayAnimation("idle", 0.0f, true, true, true);
			}
		}
		else
		{
			if (skinnedMesh)
			{
				if (abs(velocity.y()) < 0.2f && (TimeManager::GetUpTime() - lastGroundTime < 0.5))
					skinnedMesh->PlayAnimation("run", 0.0f, true, true, true);
			}
		}
	
		// Jump
		if ((input->JumpPressed() || input->JumpHeld()) && (TimeManager::GetUpTime() - lastGroundTime < 0.5) && velocity.y() < 0.8)
		{
			velocity.y() = jumpVel;
			rigidbody->SetLinearVelocity(velocity);

			if (skinnedMesh)
			{
				skinnedMesh->PlayAnimation("jump", 0.0f, true, true, true);
			}
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

