#pragma once

#include "Component.h"
#include "..\Physics\Export\PhysicsInfo.h"

namespace TLGameEngine
{
class Narration;
class BoxCollider;

class NarrationTrigger : public Component, public PhysicsEngine::IEventTrigger
{
public:
	NarrationTrigger();
	virtual ~NarrationTrigger();

	virtual void Awake() override;
	virtual void Update() override;


	virtual void OnTriggerEnter(std::string name) override;
	virtual void OnTriggerPersist(std::string name) {};
	virtual void OnTriggerExit(std::string name) override;

private:
	bool m_isAutoTrigger = false;
	std::shared_ptr<Narration> m_narration;
	std::shared_ptr<BoxCollider> m_collider;

	FileID m_Player;
	float m_AutoTriggerTime;
	int m_index;
	BOOST_DESCRIBE_CLASS(TLGameEngine::NarrationTrigger, (), (), (), (m_AutoTriggerTime, m_Player, m_index))
};
}

