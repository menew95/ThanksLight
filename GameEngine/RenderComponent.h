#pragma once

#include "Component.h"

namespace TLGameEngine
{
struct ConstantData
{
	Matrix world;
	Matrix worldView;
	Matrix worldViewProj;
	Vector3 viewPosition;
	float padding;
};

class Mesh;
class Material;

class RenderComponent abstract : public Component
{
public:
	RenderComponent();

	void SetMesh(std::shared_ptr<Mesh> mesh) { m_mesh = mesh; }

	/// <summary>
	/// 이원택 변경
	/// 하나의 매쉬안에 인덱스 버퍼를 여러개 둬서 서브 매쉬 형태로 변경됨
	/// 하나의 메쉬를 서브매쉬 개수 만큼 메테리얼 설정 가능하도록 변경
	/// </summary>
	/// <param name="material"></param>
	void SetMaterial(std::shared_ptr<Material> material, int i) { if(i < m_materials.size()) m_materials[i] = material; }
	void AddMaterial(std::shared_ptr<Material> material) { m_materials.push_back(material); }

protected:
	std::shared_ptr<Mesh> m_mesh;
	std::vector<std::shared_ptr<Material>> m_materials;

#pragma region 렌더쪽으로 던질 데이터, 임시로 만들어주긴 했지만 동기화 관리를 해 줄 다른 매니저가 필요할 듯
	ConstantData m_data;
#pragma endregion
};
}
