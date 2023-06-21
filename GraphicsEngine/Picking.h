#pragma once
#include "DX11Define.h"
namespace TLGraphicsEngine
{
	class Mesh;
	class EngineCamera;

	/// <summary>
	/// ��ŷ Ŭ����
	/// </summary>
	class Picking
	{
	public:
		Picking();
		~Picking();

	public:
		/// <summary>
		/// NDC �������� Ray�� Mesh�� Bounding Box�� Picking�� �˻��Ѵ�.
		/// </summary>
		/// <param name="mesh">Mesh</param>
		/// <param name="meshWorld">Mesh World Matrix</param>
		/// <param name="x">NDC Mosue Pos X</param>
		/// <param name="y">NDC Mosue Pos Y</param>
		/// <param name="engineCamera">Engine Camera</param>
		/// <returns>��ŷ ����</returns>
		bool CheckPicking(Mesh* mesh, DirectX::XMMATRIX meshWorld, float x, float y, EngineCamera* engineCamera);
	};
}

