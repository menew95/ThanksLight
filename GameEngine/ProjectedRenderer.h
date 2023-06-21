#pragma once
#include "RenderComponent.h"

namespace TLGameEngine
{
	class BoxCollider;

	class ProjectedRenderer : public RenderComponent
	{
	public:
		ProjectedRenderer();

		virtual void Awake() override;
		virtual void LateUpdate() override;


		void SetRender2D();
	private:
		bool m_isRender2D = false;
		bool m_is2D = false;

		std::weak_ptr<Transform> m_transform;
		std::vector<std::shared_ptr<Material>> m_materials;
		std::weak_ptr<BoxCollider> m_collider;

#pragma region ���������� ���� ������, �ӽ÷� ������ֱ� ������ ����ȭ ������ �� �� �ٸ� �Ŵ����� �ʿ��� ��
		ConstantData m_data;
#pragma endregion


		std::vector<FileInfo> m_materialIDs;
		FileID m_colliderID;
		BOOST_DESCRIBE_CLASS(TLGameEngine::ProjectedRenderer, (), (), (), (m_materialIDs, m_colliderID))
	};

}