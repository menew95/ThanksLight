#pragma once
#include "Transform.h"

namespace TLGameEngine
{
struct Rect
{
	float x;
	float y;
	float width;
	float height;

	operator RECT()
	{
		return {static_cast<LONG>(x), static_cast<LONG>(y), static_cast<LONG>(x + width), static_cast<LONG>(y + height)};
	}
};

	class RectTransform :
	    public Transform
	{
	public:
		RectTransform();
		~RectTransform();

	public:
		virtual void Start() override;
		virtual void Update() override;

	public:
		Vector2 GetPivot() { return m_Pivot; }
		void SetPivot(Vector2 value) { m_Pivot = value; }

		float GetWidth() { return m_SizeDelta.x; }
		void SetWidth(float value) { m_SizeDelta.x = value; }

		float GetHeight() { return m_SizeDelta.y; }
		void SetPivot(float value) { m_SizeDelta.y = value; }

		void SetResolution(Vector2 resolution) { m_resolution = resolution; m_rect.width = resolution.x; m_rect.height = resolution.y; }

		Rect GetRect() { return m_rect; }
		void SetRect(Rect rect) { m_rect = rect; }

		void SetAnchoredPosition(Vector2 position) { m_AnchoredPosition = position; }
		Vector2 GetAnchoredPosition() { return m_AnchoredPosition; }
	private:
		Rect m_rect = { 0, 0, 0 , 0 };
		Vector2 m_resolution;

		Vector2 m_AnchorMin = { 0.5f, 0.5f };
		Vector2 m_AnchorMax = { 0.5f, 0.5f };
		Vector2 m_AnchoredPosition = { 0, 0 };
		Vector2 m_SizeDelta;	//width, height
		Vector2 m_Pivot = { 0, 0 };

		BOOST_DESCRIBE_CLASS(TLGameEngine::RectTransform, (), (), (m_LocalPosition, m_LocalRotation, m_LocalScale, m_Father, m_Children, m_GameObject), (m_SizeDelta, m_AnchorMin, m_AnchorMax, m_AnchoredPosition, m_Pivot))
	};
}

