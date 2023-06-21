#pragma once
#include "Component.h"
#include "..\Physics\Export\PhysicsInfo.h"

namespace TLGameEngine
{

	class Trigger : public Component, public PhysicsEngine::IEventTrigger
	{
	public:
		Trigger();
		virtual ~Trigger();

		virtual void Awake() {};
		virtual void Start();
		virtual void Update() {};

		virtual void OnTriggerEnter(std::string name) abstract;
		virtual void OnTriggerPersist(std::string name) {};
		virtual void OnTriggerExit(std::string name) abstract;

	public:
		bool GetIsTrigger() const { return m_IsTrigger; }
		void SetIsTrigger(bool val) { m_IsTrigger = val; }

	public:


	protected:
		bool m_IsTrigger = false;


	};
}
