#include "PhysicsHelpers.h"
#include "Assets.h"

namespace MCK::Physics::PhysicsHelpers
{
	RigidbodyComponent* GetRigidbodyComponent(btRigidBody* btBody)
	{
		return static_cast<RigidbodyComponent*>(btBody->getUserPointer());
	}

	MCK::EntitySystem::ComponentBase* GetDynamicsComponent(const btCollisionObject* dynamicsObject)
	{
		return static_cast<MCK::EntitySystem::ComponentBase*>(dynamicsObject->getUserPointer());
	}

	MCK::EntitySystem::Entity* GetEntity(const btCollisionObject* dynamicsObject)
	{
		MCK::EntitySystem::ComponentBase* correspondingComponent = GetDynamicsComponent(dynamicsObject);
		return correspondingComponent->entity;
	}

	bool IsRigidbody(const btCollisionObject* dynamicsObject)
	{
		return dynamicsObject->getInternalType() == btCollisionObject::CollisionObjectTypes::CO_RIGID_BODY;
	}

	btCollisionShape* CreateBoxShape(btVector3 halfExtents)
	{
		btBoxShape* box = new btBoxShape(halfExtents);
		return static_cast<btCollisionShape*>(box);
	}

	btCollisionShape* CreateSphereShape(float radius)
	{
		btSphereShape* sphere = new btSphereShape(radius);
		return static_cast<btCollisionShape*>(sphere);
	}

	btCollisionShape* CreateCapsuleShape(float width, float height)
	{
		btCapsuleShape* capsule = new btCapsuleShape(width, height);
		return static_cast<btCollisionShape*>(capsule);
	}

	btCollisionShape* CreateMeshColliderShape(MCK::AssetType::Mesh* mesh)
	{
		if (mesh == nullptr)
			return nullptr;

		// Setup the indexed mesh
		btIndexedMesh btMesh;
		btMesh.m_numTriangles = mesh->NumIndices() / 3;
		btMesh.m_triangleIndexBase = (const unsigned char*)mesh->vertexIndices.data();
		btMesh.m_triangleIndexStride = 3 * sizeof(GLuint);

		btMesh.m_numVertices = mesh->NumVertices();
		btMesh.m_vertexBase = (const unsigned char*)mesh->vertexPositions.data();
		btMesh.m_vertexStride = 3 * sizeof(float);

		btTriangleIndexVertexArray* vertArray = new btTriangleIndexVertexArray();
		vertArray->addIndexedMesh(btMesh);

		btBvhTriangleMeshShape* meshColliderShape = new btBvhTriangleMeshShape(vertArray, false);
		return meshColliderShape;
	}

	void InitialiseCollider(CreateCollisionShapeInfo shapeInfo, btCollisionShape*& collisionShape)
	{
		switch (shapeInfo.colliderType)
		{
		case (Box):
			collisionShape = CreateBoxShape(btVector3(shapeInfo.width, shapeInfo.height, shapeInfo.depth));
			break;
		case (Sphere):
			collisionShape = CreateSphereShape(shapeInfo.radius);
			break;
		case (Capsule):
			collisionShape = CreateCapsuleShape(shapeInfo.width, shapeInfo.height);
			break;
		case (Mesh):
			collisionShape = CreateMeshColliderShape(shapeInfo.mesh);
		};
	}
}

