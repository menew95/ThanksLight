#pragma once
namespace TLGameEngine
{
	class GameObject;

	struct KeyFrame
	{
		/*float m_FrameRate;
		bool m_HasPos = false;
		Vector3 m_Position;
		bool m_HasRot = false;
		Vector3 m_Rotate;
		bool m_HasScale = false;
		Vector3 m_Scale;*/
		float m_FrameRate;
		DirectX::SimpleMath::Vector4 m_Data;
	};

	class AnimationSnap
	{
	public:
		AnimationSnap();
		virtual ~AnimationSnap();
		
	public:
		GameObject* m_TargetGameObject = nullptr;

		float maxFrameRate = 0;
		int posFrame = 0;
		std::vector<KeyFrame> m_posKeyFrameList;
		int rotFrame = 0;
		std::vector<KeyFrame> m_rotKeyFrameList;

		std::vector<KeyFrame> m_KeyFrameList;

	public:
		void AddKeyFrame(KeyFrame keyFrame);
		void Setting(GameObject* targetObject);

		float Play(float frameRate);
		void Next();
	};
}

