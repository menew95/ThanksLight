#include "pch.h"
#include "PhysicsInfoHelper.h"

physx::PxTransform PhysicsEngine::TransformToPxTransform(const Transform& value)
{
	physx::PxTransform temp;

	temp.p.x = value.vec[0];
	temp.p.y = value.vec[1];
	temp.p.z = value.vec[2];

	temp.q.x = value.quat[0];
	temp.q.y = value.quat[1];
	temp.q.z = value.quat[2];
	temp.q.w = value.quat[3];

	return temp;
}

PhysicsEngine::Transform PhysicsEngine::PxTransformToTransform(const physx::PxTransform& value)
{
	Transform temp;

	temp.vec[0] = value.p.x;
	temp.vec[1] = value.p.y;
	temp.vec[2] = value.p.z;

	temp.quat[0] = value.q.x;
	temp.quat[1] = value.q.y;
	temp.quat[2] = value.q.z;
	temp.quat[3] = value.q.w;

	return temp;
}

physx::PxVec3 PhysicsEngine::Vector3ToPxVec3(const Vector3& value)
{
	physx::PxVec3 temp;

	temp.x = value.vec[0];
	temp.y = value.vec[1];
	temp.z = value.vec[2];

	return temp;
}

PhysicsEngine::Vector3 PhysicsEngine::Vector3ToPxVec3(const physx::PxVec3& value)
{
	Vector3 temp;

	temp.vec[0] = value.x;
	temp.vec[1] = value.y;
	temp.vec[2] = value.z;

	return temp;
}
