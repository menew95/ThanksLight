#pragma once
#include "Component.h"
namespace TLGameEngine
{
	class Text;
	class Image;
	class PostProcess;

	class DebugManager :
	    public Component
	{
	public:
		DebugManager();
		~DebugManager();

	public:
		void Awake();
		void Start();
		void LateUpdate();


	public:
		std::shared_ptr<Transform> m_cameraTransform;
		std::shared_ptr<PostProcess> m_postProcess;

		std::shared_ptr<Text> m_FrameInfo;
		std::shared_ptr<Text> m_CameraInfo;
		std::shared_ptr<Text> m_SceneInfo;
		std::shared_ptr<Text> m_PostProcessInfo;
		std::shared_ptr<Text> m_PostProcessInfo2;

		bool m_debugOn = false;
		bool onOffBloom = true;
		bool onOffTone = true;
		float min = 1.0f;
		float max = 1.26f;
		float exposure = 2.0f;
	};
}

