#pragma once

#include "RenderComponent.h"

namespace TLGameEngine
{

class Mesh;
class Material;

class Sprite : public RenderComponent
{
public:
	Sprite();

	virtual void Awake() override;
	virtual void LateUpdate() override;

private:
	void CreateMesh();

	std::weak_ptr<Transform> m_transform;
};
}

