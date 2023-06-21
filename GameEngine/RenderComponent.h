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
	/// �̿��� ����
	/// �ϳ��� �Ž��ȿ� �ε��� ���۸� ������ �ּ� ���� �Ž� ���·� �����
	/// �ϳ��� �޽��� ����Ž� ���� ��ŭ ���׸��� ���� �����ϵ��� ����
	/// </summary>
	/// <param name="material"></param>
	void SetMaterial(std::shared_ptr<Material> material, int i) { if(i < m_materials.size()) m_materials[i] = material; }
	void AddMaterial(std::shared_ptr<Material> material) { m_materials.push_back(material); }

protected:
	std::shared_ptr<Mesh> m_mesh;
	std::vector<std::shared_ptr<Material>> m_materials;

#pragma region ���������� ���� ������, �ӽ÷� ������ֱ� ������ ����ȭ ������ �� �� �ٸ� �Ŵ����� �ʿ��� ��
	ConstantData m_data;
#pragma endregion
};
}
