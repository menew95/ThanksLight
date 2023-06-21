#include "pch.h"
#include "AnimationSnap.h"
#include "GameObject.h"
#include "Transform.h"
#include "../DXTK/Inc/SimpleMath.h"

TLGameEngine::AnimationSnap::AnimationSnap()
{

}

TLGameEngine::AnimationSnap::~AnimationSnap()
{

}

void TLGameEngine::AnimationSnap::AddKeyFrame(KeyFrame keyFrame)
{
	m_KeyFrameList.push_back(keyFrame);
}

void TLGameEngine::AnimationSnap::Setting(GameObject* targetObject)
{
	m_TargetGameObject = targetObject;
}

float TLGameEngine::AnimationSnap::Play(float frameRate)
{
	if (m_TargetGameObject == nullptr)
	{
		return 0;
	}
	auto targetTransform = m_TargetGameObject->GetComponent<Transform>();

	if (frameRate > maxFrameRate)
	{
		posFrame = 0;
		rotFrame = 0;
		frameRate -= maxFrameRate;
		if (frameRate > maxFrameRate)
		{
			int temp = (int)(frameRate / maxFrameRate);

			frameRate -= (temp * maxFrameRate);
		}
	}

	// pos
	if(posFrame < (int)m_posKeyFrameList.size() - 2)
	{
		DirectX::XMFLOAT3 pre, next;

		if (frameRate > m_posKeyFrameList[posFrame + 1].m_FrameRate)
		{
			posFrame++;
		}
		//posFrame = 0;
		pre = 
		{
			m_posKeyFrameList[posFrame].m_Data.x,
			m_posKeyFrameList[posFrame].m_Data.y,
			m_posKeyFrameList[posFrame].m_Data.z
		};

		next =
		{
			m_posKeyFrameList[posFrame + 1].m_Data.x,
			m_posKeyFrameList[posFrame + 1].m_Data.y,
			m_posKeyFrameList[posFrame + 1].m_Data.z
		};

		float alpha = (frameRate - m_posKeyFrameList[posFrame].m_FrameRate)
			/ (m_posKeyFrameList[posFrame + 1].m_FrameRate - m_posKeyFrameList[posFrame].m_FrameRate);

		if (alpha < 0.f)
		{
			alpha = 0.0f;
		}
		else if(alpha > 1.0f)
		{
			alpha = 1.0f;
		}
		
		{
			using namespace DirectX;

			XMFLOAT3 result;
			XMVECTOR x1 = XMLoadFloat3(&pre);
			XMVECTOR x2 = XMLoadFloat3(&next);
			XMVECTOR X = XMVectorLerp(x1, x2, alpha);
			XMStoreFloat3(&result, X);

			SimpleMath::Vector3 pos = { result.x, result.y, result.z };
			targetTransform->SetLocalPosition(pos);
		}
	}

	// rot
	if (rotFrame < (int)m_rotKeyFrameList.size() - 2)
	{
		DirectX::SimpleMath::Quaternion preQ, nextQ;

		if (frameRate > m_rotKeyFrameList[rotFrame + 1].m_FrameRate)
		{
			rotFrame++;
		}

		preQ = 
		{ 
			m_rotKeyFrameList[rotFrame].m_Data.x,
			m_rotKeyFrameList[rotFrame].m_Data.y,
			m_rotKeyFrameList[rotFrame].m_Data.z,
			m_rotKeyFrameList[rotFrame].m_Data.w
		};

		nextQ =
		{
			m_rotKeyFrameList[rotFrame + 1].m_Data.x,
			m_rotKeyFrameList[rotFrame + 1].m_Data.y,
			m_rotKeyFrameList[rotFrame + 1].m_Data.z,
			m_rotKeyFrameList[rotFrame + 1].m_Data.w
		};

		float alpha = (frameRate - m_rotKeyFrameList[rotFrame].m_FrameRate)
			/ (m_rotKeyFrameList[rotFrame + 1].m_FrameRate - m_rotKeyFrameList[rotFrame].m_FrameRate);

		if (alpha < 0.f)
		{
			alpha = 0.0f;
		}
		else if (alpha > 1.0f)
		{
			alpha = 1.0f;
		}
		DirectX::SimpleMath::Quaternion result = DirectX::SimpleMath::Quaternion::Slerp(preQ, nextQ, alpha);

		targetTransform->SetLocalRotateQ(result);
	}

	return frameRate;
}

void TLGameEngine::AnimationSnap::Next()
{
	if (m_TargetGameObject == nullptr)
	{
		return;
	}
	auto targetTransform = m_TargetGameObject->GetComponent<Transform>();


	// pos
	if (m_posKeyFrameList.size() != 0)
	{
		DirectX::XMFLOAT3 pre;

		posFrame++;

		if (posFrame == m_posKeyFrameList.size())
		{
			posFrame = 0;
		}
		//posFrame = 0;
		pre =
		{
			m_posKeyFrameList[posFrame].m_Data.x,
			m_posKeyFrameList[posFrame].m_Data.y,
			m_posKeyFrameList[posFrame].m_Data.z
		};

		DirectX::SimpleMath::Vector3 pos = { pre.x, pre.y, pre.z };
		targetTransform->SetLocalPosition(pos);
	}
	// rot
	if (m_rotKeyFrameList.size() != 0)
	{
		DirectX::SimpleMath::Quaternion preQ;

		rotFrame++;

		if (rotFrame == m_rotKeyFrameList.size())
		{
			rotFrame = 0;
		}

		preQ =
		{
			m_rotKeyFrameList[rotFrame].m_Data.x,
			m_rotKeyFrameList[rotFrame].m_Data.y,
			m_rotKeyFrameList[rotFrame].m_Data.z,
			m_rotKeyFrameList[rotFrame].m_Data.w
		};

		targetTransform->SetLocalRotateQ(preQ);
	}
}
