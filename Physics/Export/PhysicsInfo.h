#pragma once
#include <string>
#include "../dllDefine.h"


namespace PhysicsEngine
{
	struct PHYSICS_ENGINE Transform
	{
		float vec[3];
		float quat[4];
	};
	struct PHYSICS_ENGINE Vector3
	{
		Vector3() :
			vec{ 0.f,0.f,0.f } {};

		Vector3(float x, float y, float z)
			:vec{ x, y, z } {};


		float vec[3];
	};
	struct PHYSICS_ENGINE Vector4
	{
		float vec[4];
	};

	enum class PHYSICS_ENGINE eGeometryType
	{
		SPHERE,
		PLANE,
		CAPSULE,
		BOX,
		CONVEXMESH,
		TRIANGLEMESH,
		HEIGHTFIELD,
		GEOMETRY_COUNT,	//!< internal use only!
		INVALID = -1		//!< internal use only!
	};
	enum class PHYSICS_ENGINE ePhysicsType
	{
		eSTATTIC = 1,
		eDYNAMIC = 1 << 1,
		ePhysicsType_COUNT = 0,
		INVAILD = -1
	};
	struct PHYSICS_ENGINE PhysicsAixsLock
	{
		PhysicsAixsLock() = default;
		PhysicsAixsLock(bool linearX, bool linearY, bool linearZ, bool angularX, bool angularY, bool angularZ) :
			m_linearX(linearX),
			m_linearY(linearY),
			m_linearZ(linearZ),
			m_anguarX(angularX),
			m_anguarY(angularY),
			m_anguarZ(angularZ)
		{

		}
		;

		bool m_linearX = false;
		bool m_linearY = false;
		bool m_linearZ = false;
		bool m_anguarX = false;
		bool m_anguarY = false;
		bool m_anguarZ = false;
	};
	struct PHYSICS_ENGINE RayCastHit
	{
		std::string m_objectName;
		float m_distance=0;
	};

	class PHYSICS_ENGINE IEventTrigger
	{
	public:
		virtual void OnTriggerEnter(std::string name) abstract;
		virtual void OnTriggerPersist(std::string name) abstract;
		virtual void OnTriggerExit (std::string name) abstract;
	};

	class PHYSICS_ENGINE IEventCollider
	{
	public:
		virtual void OnCollisionEnter(std::string name) abstract;
		virtual void OnCollisionPersist(std::string name) abstract;
		virtual void OnCollisionExit   (std::string name) abstract;
	};

}