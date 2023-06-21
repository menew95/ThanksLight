#pragma once

#include "RenderComponent.h"

namespace TLGameEngine
{
class Mesh;
class Material;

class MeshRenderer : public RenderComponent
{
public:
	MeshRenderer();

	virtual void Awake() override;
	virtual void LateUpdate() override;

private:

	std::shared_ptr<Transform> m_transform;

	BOOST_DESCRIBE_CLASS(TLGameEngine::MeshRenderer, (), (), (), ())
};
}

