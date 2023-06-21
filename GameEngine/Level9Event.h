#pragma once
#include "Component.h"

namespace TLGameEngine
{
	class InteractiveObject;
	class BoxCollider;
	class MeshRenderer;
	class Material;

	class Level9Event : 
		public Component
	{
	public:
		Level9Event();

	public:
		virtual void Awake() override;
		virtual void Start() override;
		virtual void Update() override;

	private:
		std::weak_ptr<InteractiveObject> m_FitfallObj;
		std::vector<std::weak_ptr<BoxCollider>> m_colliders;
		std::vector<std::weak_ptr<MeshRenderer>> m_wallMeshs;
		std::weak_ptr<MeshRenderer> m_wallFloor;

		std::weak_ptr<Material> m_WallMaterial;
		std::weak_ptr<Material> m_FloorMaterial;

	private:
		FileID m_FitfallObject;

		std::vector<FileID> m_FitfallColliders;
		std::vector<FileID> m_FitfallWallMeshs;
		FileID m_FitfallFloorMesh;

		FileInfo m_WallMaterialID;
		FileInfo m_FloorMaterialID;

		BOOST_DESCRIBE_CLASS(TLGameEngine::Level9Event, (), (), (), (m_FitfallObject, m_FitfallColliders, m_FitfallWallMeshs, m_FitfallFloorMesh, m_WallMaterialID, m_FloorMaterialID))
	};
}
