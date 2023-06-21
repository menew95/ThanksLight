#pragma once
#include "Component.h"
namespace TLGameEngine
{
    class Canvas :
        public Component
    {
	public:
		Canvas();
		virtual ~Canvas();

	public:
		virtual void Awake() {};
		virtual void OnEnable() {};
		virtual void OnDisalbe() {};
		virtual void Start() {};
		virtual void Update();
		virtual void LateUpdate() {};
		virtual void FixedUpdate() {};
		virtual void OnDestroy() {};

	public:
		void SetWidth(int value) { m_width = value; }
		void SetHeight(int value) { m_height = value; }
		int GetWidth() { return m_width; }
		int GetHeight() { return m_height; }

	public:
		int m_width;
		int m_height;

		static Canvas* m_canvas;

		BOOST_DESCRIBE_CLASS(TLGameEngine::Canvas, (), (), (), ())
    };
}