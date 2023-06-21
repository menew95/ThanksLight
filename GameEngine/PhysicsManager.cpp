#include "pch.h"
#include "PhysicsManager.h"
#include "SceneManager.h"
#include "ResourceManager.h"

#include "GameObject.h"
#include "Transform.h"
#include "Collider.h"
#include "BoxCollider.h"
#include "SphereCollider.h"
#include "CapsuleCollider.h"

#include "MeshCollider.h"
#include "Mesh.h"
#include "IMesh.h"

#include "../DXTK/Inc/SimpleMath.h"
#include "../DXTK/Inc/SimpleMath.inl"
//#include "../DXTK/Src/SimpleMath.cpp"


TLGameEngine::PhysicsManager::PhysicsManager()
{
	m_phisicsEngine = PhysicsEngine::PhysicsEngineInstance();

	m_phisicsEngine->Initialize();
	m_phisicsEngine->AddScene("TEST");
	m_phisicsEngine->SetCurrentScene("TEST");

	InitPhysics();
}

TLGameEngine::PhysicsManager::~PhysicsManager()
{
	m_phisicsEngine->Finalalize();
	m_phisicsEngine.reset();
	PhysicsEngine::PhysicsEngineRelease();
}

void TLGameEngine::PhysicsManager::AddObject(Collider* collider)
{
	DirectX::SimpleMath::Quaternion worldRot = collider->GetTransform().lock().get()->GetWorldRotateQ();
	DirectX::SimpleMath::Vector3 worldPos = collider->GetTransform().lock().get()->GetWorldPosition();

	auto offset = collider->GetCenter();

	PhysicsEngine::Transform input;

	input.quat[0] = worldRot.x;
	input.quat[1] = worldRot.y;
	input.quat[2] = worldRot.z;
	input.quat[3] = worldRot.w;

	input.vec[0] = worldPos.x + offset.x;
	input.vec[1] = worldPos.y + offset.y;
	input.vec[2] = worldPos.z + offset.z;


	switch (collider->GetColliderType())
	{
	case PhysicsEngine::eGeometryType::BOX:
		{
			BoxCollider* boxCollider = static_cast<BoxCollider*>(collider);

			auto extend = boxCollider->GetSize();

			m_phisicsEngine->CreateBoxActor(collider->GetGameObject()->GetUID(), "Box", boxCollider->GetPhysicsType(), input, { extend.x / 2.0f , extend.y / 2.0f, extend.z / 2.0f }, 1);

			break;
		}
	case PhysicsEngine::eGeometryType::SPHERE:
		{
			SphereCollider* sphereCollider = static_cast<SphereCollider*>(collider);

			auto radius = sphereCollider->GetRaduis();

			m_phisicsEngine->CreateSphereActor(collider->GetGameObject()->GetUID(), "Box", PhysicsEngine::ePhysicsType::eDYNAMIC, input, radius, 1);
			break;
		}
	case PhysicsEngine::eGeometryType::CAPSULE:
		{
			CapsuleCollider* capsuleCollider = static_cast<CapsuleCollider*>(collider);

			DirectX::SimpleMath::Quaternion defaultRot = { 0, 0, 0.7071068f, 0.7071068f };
			defaultRot *= worldRot;

			input.quat[0] = defaultRot.x;
			input.quat[1] = defaultRot.y;
			input.quat[2] = defaultRot.z;
			input.quat[3] = defaultRot.w;

			auto radius = capsuleCollider->GetRaduis();
			auto halfHeight = capsuleCollider->GetHeight();

			m_phisicsEngine->CreateCapsuleActor(collider->GetGameObject()->GetUID(), "Box", PhysicsEngine::ePhysicsType::eDYNAMIC, input, radius, halfHeight/2, 1);
			break;
		}
	case PhysicsEngine::eGeometryType::TRIANGLEMESH:
		{
			MeshCollider* meshCollider = static_cast<MeshCollider*>(collider);

			DirectX::SimpleMath::Quaternion defaultRot = { 0, 0, 0.7071068f, 0.7071068f };
			defaultRot *= worldRot;

			input.quat[0] = defaultRot.x;
			input.quat[1] = defaultRot.y;
			input.quat[2] = defaultRot.z;
			input.quat[3] = defaultRot.w;

			auto meshID = meshCollider->GetMeshID();
			std::shared_ptr<Mesh> meshInfo = ResourceManager::Instance().GetMesh(meshID);

			if (meshInfo)
			{
				auto mesh = meshInfo->GetIMesh();
				
				TLGraphicsEngine::Buffer vertexBuffer;
				TLGraphicsEngine::Buffer indexBuffer;

				mesh->GetVertexBufferInfo(vertexBuffer);
				mesh->GetIndexBufferInfo(indexBuffer);

				std::vector<PhysicsEngine::Vector3> vbuffer;
				vbuffer.reserve(vertexBuffer.bufferCnt);

				for (int vertexIdx = 0; vertexIdx < vertexBuffer.bufferCnt; vertexIdx++)
				{
					void* vertexStartPoint = ((char*)vertexBuffer.buffer) + (vertexIdx * vertexBuffer.bufferSize);
					auto vertexPos = reinterpret_cast<PhysicsEngine::Vector3*>(vertexStartPoint);

					vbuffer.emplace_back(PhysicsEngine::Vector3{vertexPos->vec[0], vertexPos->vec[1], vertexPos->vec[2]});
				}
				
				m_phisicsEngine->CreateConvexMeshActor(collider->GetGameObject()->GetUID(), "Box",  vbuffer.data(), vertexBuffer.bufferCnt, reinterpret_cast<UINT32*>(indexBuffer.buffer), indexBuffer.bufferCnt, PhysicsEngine::ePhysicsType::eDYNAMIC, input, 1);
				break;
			}
			else
			{
				assert(false);
			}

			break;
		}
	default:
		break;
	}

	if (collider->GetTrigger())
	{
		m_phisicsEngine->SetTriggerShape(collider->GetGameObject()->GetUID(), true);
		m_phisicsEngine->SetLayer(collider->GetGameObject()->GetUID(), "TRIGGER");
	}

	SetAngularDamp(collider, 5.f);
	SetLinearDamp(collider, 1.5f);
}

void TLGameEngine::PhysicsManager::ArrangeShape(Collider* collider)
{

}

void TLGameEngine::PhysicsManager::AddForce(Collider* collider, PhysicsEngine::Vector3 force)
{
	m_phisicsEngine->AddForce(collider->GetGameObject()->GetUID(), force);
}

void TLGameEngine::PhysicsManager::AddImpulse(Collider* collider, PhysicsEngine::Vector3 impulse)
{
	m_phisicsEngine->AddImpulse(collider->GetGameObject()->GetUID(), impulse);
}

void TLGameEngine::PhysicsManager::SetTransform(Collider* collider, PhysicsEngine::Transform worldtransform)
{

	if (collider->GetColliderType() == PhysicsEngine::eGeometryType::CAPSULE)
	{
		PhysicsEngine::Transform input;
		auto offset = collider->GetCenter();

		input = worldtransform;

		DirectX::SimpleMath::Quaternion quat(worldtransform.quat[0], worldtransform.quat[1], worldtransform.quat[2], worldtransform.quat[3]);
		DirectX::SimpleMath::Quaternion defaultRot = { 0, 0, 0.7071068f, 0.7071068f };
		defaultRot *= quat;

		if (defaultRot.w > 1.f)
		{
			defaultRot.w = 1.0f;
		}

		input.quat[0] = defaultRot.x;
		input.quat[1] = defaultRot.y;
		input.quat[2] = defaultRot.z;
		input.quat[3] = defaultRot.w;

		input.vec[0] += offset.x;
		input.vec[1] += offset.y;
		input.vec[2] += offset.z;

		m_phisicsEngine->SetTransform(collider->GetGameObject()->GetUID(), input);
	}
	else
	{
		auto offset = collider->GetCenter();

		worldtransform.vec[0] += offset.x;
		worldtransform.vec[1] += offset.y;
		worldtransform.vec[2] += offset.z;

		m_phisicsEngine->SetTransform(collider->GetGameObject()->GetUID(), worldtransform);
	}
}

void TLGameEngine::PhysicsManager::SetMass(Collider* collider, float mass)
{
	m_phisicsEngine->SetMass(collider->GetGameObject()->GetUID(), mass);
}

void TLGameEngine::PhysicsManager::SetLinearDamp(Collider* collider, float damping)
{
	m_phisicsEngine->SetLinearDamping(collider->GetGameObject()->GetUID(), damping);
}

void TLGameEngine::PhysicsManager::SetAngularDamp(Collider* collider, float damping)
{
	m_phisicsEngine->SetAngularDamping(collider->GetGameObject()->GetUID(), damping);
}

void TLGameEngine::PhysicsManager::SetAxisLock(Collider* collider, PhysicsEngine::PhysicsAixsLock lockInfo)
{
	m_phisicsEngine->SetAxisLock(collider->GetGameObject()->GetUID(), lockInfo);
}

void TLGameEngine::PhysicsManager::SetMaxLinearVelocity(Collider* collider, float vel)
{
	m_phisicsEngine->SetMaxLinearVelocity(collider->GetGameObject()->GetUID(), vel);
}

void TLGameEngine::PhysicsManager::SetMaxAngularVelocity(Collider* collider, float vel)
{
	m_phisicsEngine->SetMaxAngularVelocity(collider->GetGameObject()->GetUID(), vel);
}

void TLGameEngine::PhysicsManager::SetLinearVelocity(Collider* collider, PhysicsEngine::Vector3 velocity)
{
	m_phisicsEngine->SetLinearVelocity(collider->GetGameObject()->GetUID(), velocity);
}

void TLGameEngine::PhysicsManager::SetAngularVelocity(Collider* collider, PhysicsEngine::Vector3 velocity)
{
	m_phisicsEngine->SetAnguarVelocity(collider->GetGameObject()->GetUID(), velocity);
}

Vector3 TLGameEngine::PhysicsManager::GetAngularVelocity(Collider* collider)
{
	PhysicsEngine::Vector3 ret;

	if (m_phisicsEngine->GetAnguarVelocity(collider->GetGameObject()->GetUID(), ret))
	{
		return { ret.vec[0], ret.vec[1], ret.vec[2] };
	}
	else
	{
		return { 0.f ,0.f , 0.f };
	}
}

Vector3 TLGameEngine::PhysicsManager::GetLinearVelocity(Collider* collider)
{
	PhysicsEngine::Vector3 ret;

	if (m_phisicsEngine->GetLinearVelocity(collider->GetGameObject()->GetUID(), ret))
	{
		return { ret.vec[0], ret.vec[1], ret.vec[2] };
	}
	else
	{
		return { 0.f ,0.f , 0.f };
	}
}

void TLGameEngine::PhysicsManager::SetGravity(Collider* collider, bool flag)
{
	m_phisicsEngine->SetGravity(collider->GetGameObject()->GetUID(), flag);
}

void TLGameEngine::PhysicsManager::SetKinematic(Collider* collider, bool flag)
{
	m_phisicsEngine->SetKinematic(collider->GetGameObject()->GetUID(), flag);
}

void TLGameEngine::PhysicsManager::AddTriggerEvent(Collider* collider, PhysicsEngine::IEventTrigger* callBackClass)
{
	m_phisicsEngine->AddTriggerEvent(collider->GetGameObject()->GetUID(), callBackClass);
}

void TLGameEngine::PhysicsManager::AddColliderEvent(Collider* collider, PhysicsEngine::IEventCollider* callBackClass)
{
	m_phisicsEngine->AddColliderEvent(collider->GetGameObject()->GetUID(), callBackClass);
}
void TLGameEngine::PhysicsManager::DeleteObject(Collider* collider)
{
	m_phisicsEngine->DeleteActor(collider->GetGameObject()->GetUID());
}

void TLGameEngine::PhysicsManager::DisableObject(Collider* collider)
{
	m_phisicsEngine->DisableAcotr(collider->GetGameObject()->GetUID());
}

void TLGameEngine::PhysicsManager::EnableObject(Collider* collider)
{
	m_phisicsEngine->EnableAcotr(collider->GetGameObject()->GetUID());
}

void TLGameEngine::PhysicsManager::Simulate(float dt)
{
	m_phisicsEngine->Simualte(dt);
}

void TLGameEngine::PhysicsManager::UpdateTransform(Collider* collider)
{
	PhysicsEngine::Transform output;

	if (m_phisicsEngine->GetTransform(collider->GetGameObject()->GetUID(), output))
	{
		if (collider->GetColliderType() == PhysicsEngine::eGeometryType::CAPSULE)
		{
			DirectX::SimpleMath::Vector3 pos(output.vec[0], output.vec[1], output.vec[2]);
			DirectX::SimpleMath::Quaternion quat(output.quat[0], output.quat[1], output.quat[2], output.quat[3]);

			DirectX::SimpleMath::Quaternion defaultRot = { 0, 0, -0.7071068f, 0.7071068f };

			defaultRot *= quat;

			defaultRot.Normalize();

			collider->GetTransform().lock().get()->SetWorldPosition(pos);
			collider->GetTransform().lock().get()->SetWorldRotateQ(defaultRot);
		}
		else
		{
			auto offset = collider->GetCenter();

			//DirectX::SimpleMath::Vector3 pos(output.vec[0], output.vec[1], output.vec[2]);
			DirectX::SimpleMath::Vector3 pos(output.vec[0] - offset.x, output.vec[1] - offset.y, output.vec[2] - offset.z);
			DirectX::SimpleMath::Quaternion quat(output.quat[0], output.quat[1], output.quat[2], output.quat[3]);


			collider->GetTransform().lock().get()->SetWorldPosition(pos);
			collider->GetTransform().lock().get()->SetWorldRotateQ(quat);
		}
	}
}

void TLGameEngine::PhysicsManager::SetColliderLayer(Collider* collider, std::string LayerName)
{
	m_phisicsEngine->SetLayer(collider->GetGameObject()->GetUID(), LayerName);
}

//TLGameEngine::RayCastHit TLGameEngine::PhysicsManager::RayCast(PhysicsEngine::Vector3 origin, PhysicsEngine::Vector3 dir, float distance)
//{
//	PhysicsEngine::RayCastHit rayHitInfo;
//
//	bool ret = m_phisicsEngine->Raycast(origin, dir, distance, rayHitInfo);
//
//	if (!ret)
//	{
//		return TLGameEngine::RayCastHit();
//	}
//
//	std::cout << rayHitInfo.m_objectName << std::endl;
//
//	auto gameObject = SceneManager::Instance().FindObject(rayHitInfo.m_objectName.c_str());
//
//	if (gameObject == nullptr)
//	{
//		return TLGameEngine::RayCastHit();
//	}
//
//	return TLGameEngine::RayCastHit(rayHitInfo.m_objectName.c_str(), rayHitInfo.m_distance, ret, gameObject);
//}

TLGameEngine::RayCastHit TLGameEngine::PhysicsManager::RayCast(PhysicsEngine::Vector3 origin, PhysicsEngine::Vector3 dir, float distance, std::vector<std::string> Filteredlayers, PhysicsEngine::ePhysicsType includeType)
{
	PhysicsEngine::RayCastHit rayHitInfo;

	bool ret = m_phisicsEngine->Raycast(origin, dir, distance, Filteredlayers, includeType, rayHitInfo);

	if (!ret)
	{
		return TLGameEngine::RayCastHit();
	}

	auto gameObject = SceneManager::Instance().FindObject(rayHitInfo.m_objectName.c_str());

	if (gameObject == nullptr)
	{
		return TLGameEngine::RayCastHit();
	} 

	return TLGameEngine::RayCastHit(rayHitInfo.m_objectName.c_str(), rayHitInfo.m_distance, ret, gameObject);
}

void TLGameEngine::PhysicsManager::InitPhysics()
{	

	m_phisicsEngine->DeleteScene("TEST");
	m_phisicsEngine->AddScene("TEST");
	m_phisicsEngine->SetCurrentScene("TEST");

	m_phisicsEngine->AddLayer("PLAYER");
	m_phisicsEngine->AddLayer("HANDEDOBJECT");
	m_phisicsEngine->AddLayer("PROJECTOROBJECT");
	m_phisicsEngine->AddLayer("3DOBJECT");
	m_phisicsEngine->AddLayer("2DOBJECT");
	m_phisicsEngine->AddLayer("WALLOBJECT");
	m_phisicsEngine->AddLayer("TRIGGER");

	m_phisicsEngine->SetLayerFilterData("PLAYER", { "HANDEDOBJECT", "2DOBJECT" });
	m_phisicsEngine->SetLayerFilterData("HANDEDOBJECT", { "PLAYER", "2DOBJECT" });
	m_phisicsEngine->SetLayerFilterData("2DOBJECT", { "PLYAER", "3DOBJECT", "2DOBJECT", "WALLOBJECT"});
	m_phisicsEngine->SetLayerFilterData("WALLOBJECT", { "Default" , "PLAYER", "3DOBJECT", "2DOBJECT", "HANDEDOBJECT"});
	m_phisicsEngine->SetLayerFilterData("Default", { "WALLOBJECT" });

	m_phisicsEngine->Test();
}

