#pragma once

namespace PhysicsEngine
{
	physx::PxTransform TransformToPxTransform(const Transform& value);
	Transform PxTransformToTransform(const physx::PxTransform& value);

	physx::PxVec3 Vector3ToPxVec3(const Vector3& value);
	Vector3 Vector3ToPxVec3(const physx::PxVec3& value);


}