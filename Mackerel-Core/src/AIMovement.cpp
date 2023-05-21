#include "AIMovement.h"
#include "RigidbodyComponent.h"

namespace MCK::EntitySystem
{
	void AIMovement::OnCreate()
	{
		transform = entity->GetComponent<TransformComponent>();
		rigidbody = entity->GetComponent<Physics::RigidbodyComponent>();

		startPos = rigidbody->GetPosition();
		rigidbody->SetCharacter();
		rigidbody->SetMass(2);
		rigidbody->DisableRotation();
		isReturning = false;
		reachThreshold = 1.5f;
	}

	void AIMovement::OnUpdate()
	{
		Eigen::Vector2f movInput = calcInput();

		// Add Move Force
		Eigen::Vector3f moveForce = Eigen::Vector3f(movInput.x(), 0, movInput.y()) * moveForceMag;

		/* rotation */
		if (moveForce.squaredNorm() > 0.001f)
		{
			/* rotation angles of facing direction and movement direction */
			float true_rotation_angle = atan2(smoothMoveVector.z(), -smoothMoveVector.x()) - atan2(1.0f, 0.0f);
			float target_rotation_angle = atan2(movInput.y(), -movInput.x()) - atan2(1.0f, 0.0f);

			/* convert to quaternions */
			Eigen::Quaternionf true_q = Eigen::Quaternionf(Eigen::AngleAxisf(true_rotation_angle, Eigen::Vector3f(0.0f, 1.0f, 0.0f)));
			Eigen::Quaternionf target_q = Eigen::Quaternionf(Eigen::AngleAxisf(target_rotation_angle, Eigen::Vector3f(0.0f, 1.0f, 0.0f)));

			/* interpolate 'em to get smooth movement */
			float smooth_t = turnRate * static_cast<float>(MCK::TimeManager::getScaledFrameTime());
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

		rigidbody->SetLinearVelocity(moveForce);

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
	}

	Eigen::Vector2f AIMovement::calcInput() {
		Eigen::Vector3f currentPos = rigidbody->GetPosition();
		Eigen::Vector2f movInput(0, 0);

		if (isReturning) {
			if ((currentPos - startPos).norm() <= reachThreshold) {
				isReturning = false;
				movInput = Eigen::Vector2f(endPos.x() - currentPos.x(), endPos.z() - currentPos.z()).normalized();
			}
			else {
				movInput = Eigen::Vector2f(startPos.x() - currentPos.x(), startPos.z() - currentPos.z()).normalized();
			}
		}
		else {
			if ((currentPos - endPos).norm() <= reachThreshold) {
				isReturning = true;
				movInput = Eigen::Vector2f(startPos.x() - currentPos.x(), startPos.z() - currentPos.z()).normalized();
			}
			else {
				movInput = Eigen::Vector2f(endPos.x() - currentPos.x(), endPos.z() - currentPos.z()).normalized();
			}
		}

		std::cout << "Pos: " << currentPos.y() << std::endl;

		return movInput;
	}

	void AIMovement::setEndPosition(Eigen::Vector3f target)
	{
		endPos = target;
	}

	bool AIMovement::Deserialise(json data)
	{
		return true;
	}
}
