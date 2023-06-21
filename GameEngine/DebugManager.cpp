#include "pch.h"
#include "DebugManager.h"
#include "Text.h"
#include "Image.h"
#include "Transform.h"
#include "MyTime.h"
#include "SceneManager.h"
#include "Scene.h"
#include "PostProcess.h"
#include "Camera.h";
#include "Canvas.h"
#include "RectTransform.h"

TLGameEngine::DebugManager::DebugManager()
	: Component(Type::Action)
{

}

TLGameEngine::DebugManager::~DebugManager()
{

}

void TLGameEngine::DebugManager::Awake()
{

}

void TLGameEngine::DebugManager::Start()
{
	m_cameraTransform = Camera::GetMainCamera()->GetComponent<Transform>();
	m_postProcess = Camera::GetMainCamera()->GetComponent<PostProcess>();
}

void TLGameEngine::DebugManager::LateUpdate()
{
	if (Input::Instance().GetKeyDown(VK_F1))
	{
		m_debugOn = !m_debugOn;

		m_FrameInfo->GetComponent<RectTransform>()->SetParent(Canvas::m_canvas->GetComponent<RectTransform>());
		m_CameraInfo->GetComponent<RectTransform>()->SetParent(Canvas::m_canvas->GetComponent<RectTransform>());
		m_SceneInfo->GetComponent<RectTransform>()->SetParent(Canvas::m_canvas->GetComponent<RectTransform>());
		m_PostProcessInfo->GetComponent<RectTransform>()->SetParent(Canvas::m_canvas->GetComponent<RectTransform>());
		m_PostProcessInfo2->GetComponent<RectTransform>()->SetParent(Canvas::m_canvas->GetComponent<RectTransform>());
	
	
		m_FrameInfo->SetIsActive(m_debugOn);
		m_CameraInfo->SetIsActive(m_debugOn);
		m_SceneInfo->SetIsActive(m_debugOn);
		m_PostProcessInfo->SetIsActive(m_debugOn);
		m_PostProcessInfo2->SetIsActive(m_debugOn);

		if (m_debugOn)
		{
			m_FrameInfo->GetComponent<RectTransform>()->SetAnchoredPosition({ -400, 300 });
			m_CameraInfo->GetComponent<RectTransform>()->SetAnchoredPosition({ -400, 250 });
			m_SceneInfo->GetComponent<RectTransform>()->SetAnchoredPosition({ -400, 200 });
			m_PostProcessInfo->GetComponent<RectTransform>()->SetAnchoredPosition({ -400, 150 });
			m_PostProcessInfo2->GetComponent<RectTransform>()->SetAnchoredPosition({ -400, 100 });
		}
	}

	if (!m_debugOn)
		return;

	if (Input::Instance().GetKeyDown(vk_num('1')))
	{
		onOffBloom = !onOffBloom;
		m_postProcess->SetBloom(onOffBloom);
	}

	if (Input::Instance().GetKeyDown(vk_num('2')))
	{
		onOffTone = !onOffTone;
		m_postProcess->SetToneMap(onOffTone);
	}

	if (Input::Instance().GetKeyDown(vk_num('3')))
	{
		min -= 0.1f;
		if (min < 0.f)
			min = 0.1f;

		m_postProcess->SetBloomThreshold(min, max);
	}
	else if (Input::Instance().GetKeyDown(vk_num('4')))
	{
		min += 0.1f;
		if (min > 2.f)
			min = 2.f;
		m_postProcess->SetBloomThreshold(min, max);
	}

	if (Input::Instance().GetKeyDown(vk_num('5')))
	{
		max -= 0.1f;
		if (max > 2.f)
			max = 2.f;
		m_postProcess->SetBloomThreshold(min, max);
	}
	else if (Input::Instance().GetKeyDown(vk_num('6')))
	{
		max += 0.1f;
		if (max > 2.f)
			max = 2.f;
		m_postProcess->SetBloomThreshold(min, max);
	}


	if (Input::Instance().GetKeyDown(vk_num('7')))
	{
		exposure-= 0.1f;
		if (exposure < 
			0.5)
			exposure = 0.5;
		m_postProcess->SetToneMapExposure(exposure);
	}
	else if (Input::Instance().GetKeyDown(vk_num('8')))
	{
		exposure+= 0.1f;
		if (exposure > 5)
			exposure = 5;
		m_postProcess->SetToneMapExposure(exposure);
	}

	std::string debug;

	debug = "Frame : " + std::to_string(Time::Instance().GetFps())
		+ "DeltaTime : " + std::to_string(Time::Instance().GetDeltaTime());
	m_FrameInfo->SetText(debug.c_str());
	
	auto camPos = m_cameraTransform->GetWorldPosition();
	debug = "Camera Position : " + std::to_string(camPos.x)
		+ " , " + std::to_string(camPos.y)
		+ " , " + std::to_string(camPos.z);
	
	m_CameraInfo->SetText(debug.c_str());
	
	debug = "Curr Scene : " + SceneManager::Instance().m_currScene->GetName();
	
	m_SceneInfo->SetText(debug.c_str());
	
	debug = "Bloom : " + std::to_string(m_postProcess->m_onOffBloom)
		+ " Threshold min: " + std::to_string(m_postProcess->m_min)
		+ " max : " + std::to_string(m_postProcess->m_max);
	
	m_PostProcessInfo->SetText(debug.c_str());
	
	debug = " ToneMap : " + std::to_string(m_postProcess->m_onOffTone)
		+ " exposure : " + std::to_string(m_postProcess->m_exposure);
	
	m_PostProcessInfo2->SetText(debug.c_str());
}
