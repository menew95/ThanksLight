#pragma once
#include <queue>
#include <DirectXMath.h>

namespace TLGraphicsEngine
{
	class Mesh;
	class Material;

	/// <summary>
	/// ������ �� ������Ʈ�� ������ ����ִ� ����ü
	/// </summary>
	struct RenderObject
	{
		enum class RenderType
		{
			Mesh = 0,
			SkinnedMesh = 1,
			ProjectedMesh = 2,
			OutLine = 3
		};
		RenderType m_RenderType;
		Mesh* m_Mesh;
		std::vector<Material*> m_Mats;
		DirectX::XMFLOAT4X4 m_World;
		float* m_BoneTMList = nullptr;
		int m_Size = 0;

		RenderObject(RenderType renderType,
			Mesh* mesh, std::vector<Material*> mat, DirectX::XMMATRIX worldMatrix, float* boneTMList, int size)
			: m_RenderType(renderType), m_Mesh(mesh), m_Mats(mat)//, m_World(worldMatrix)
			, m_BoneTMList(boneTMList), m_Size(size)
		{
			DirectX::XMStoreFloat4x4(&m_World, worldMatrix);
		}
		RenderObject(RenderType renderType,
			Mesh* mesh, std::vector<Material*> mat, DirectX::XMMATRIX worldMatrix)
			: m_RenderType(renderType), m_Mesh(mesh), m_Mats(mat)//, m_World(worldMatrix)
		{
			DirectX::XMStoreFloat4x4(&m_World, worldMatrix);
		}

		// OutLine�� ���� ������Ʈ
		RenderObject(RenderType renderType,
			Mesh* mesh, DirectX::XMMATRIX worldMatrix)
			: m_RenderType(renderType), m_Mesh(mesh)
		{
			DirectX::XMStoreFloat4x4(&m_World, worldMatrix);
		}
		RenderObject(const RenderObject& renderObj) = default;
		RenderObject(RenderObject&& renderObj) = default;
	};

	struct RenderUIObject
	{
		enum class eRenderUIType
		{
			Text = 0,
			Image = 1,
		};
		eRenderUIType m_eRenderUIType;
		DirectX::XMFLOAT2 m_position;
		std::string m_text;
		RenderUIObject(eRenderUIType renderUIType, float x, float y, const char* text)
			: m_eRenderUIType(renderUIType)
			, m_position({ x, y })
			, m_text(text)
		{

		}

		RenderUIObject(const RenderUIObject& renderObj) = default;
		RenderUIObject(RenderUIObject&& renderObj) = default;
	};

	struct GizmoObject
	{
		GizmoObject(Mesh* mesh, DirectX::XMMATRIX worldMatrix, XMFLOAT4 color)
			: m_Mesh(mesh), m_Color(color)
		{
			DirectX::XMStoreFloat4x4(&m_World, worldMatrix);
		}
		Mesh* m_Mesh;
		DirectX::XMFLOAT4X4 m_World;
		DirectX::XMFLOAT4 m_Color;
	};

	/// <summary>
	/// ���� �� ������Ʈ�� ��� �ִ� ť
	/// </summary>
	class RenderQueue
	{
	public:
		RenderQueue();
		~RenderQueue();

	public:
		// �ø� �� ť
		std::queue<RenderObject> m_BegineRenderQueue;

		// �ø� �� ť
		std::queue<RenderObject> m_DrawRenderQueue;


		std::queue<GizmoObject> m_DrawGizmoQueue;
	public:
		void RegistMesh(Mesh* mesh, std::vector<Material*> mat, float WM[16]);
		void RegistProjectedMesh(Mesh* mesh, std::vector<Material*> mat, float WM[16]);
		void RegistSkinnedMesh(Mesh* mesh, std::vector<Material*> mat, float WM[16], float* m_BoneTMList, int size);
		void RegistGizmo(Mesh* mesh, float WM[16], XMFLOAT4 color);
		void RegistOutLine(Mesh* mesh, float WM[16]);
		
		void RegistUI(float x, float y, DirectX::XMFLOAT4 color, TCHAR* text);
	};
}

