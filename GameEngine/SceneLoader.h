#pragma once

#include "Component.h"

namespace TLGameEngine
{
class SceneLoader : public Component	
{
public:
	SceneLoader();

	virtual void Awake() override;
	virtual void Update() override;

	int GetLoadSceneIndex() { return m_LoadSceneIndex; }

private:
	int m_LoadSceneIndex;

	BOOST_DESCRIBE_CLASS(TLGameEngine::SceneLoader, (), (), (), (m_LoadSceneIndex))
};
}

