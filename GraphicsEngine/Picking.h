#pragma once
#include "DX11Define.h"
namespace TLGraphicsEngine
{
	class Mesh;
	class EngineCamera;

	/// <summary>
	/// 피킹 클래스
	/// </summary>
	class Picking
	{
	public:
		Picking();
		~Picking();

	public:
		/// <summary>
		/// NDC 공간에서 Ray와 Mesh의 Bounding Box의 Picking를 검사한다.
		/// </summary>
		/// <param name="mesh">Mesh</param>
		/// <param name="meshWorld">Mesh World Matrix</param>
		/// <param name="x">NDC Mosue Pos X</param>
		/// <param name="y">NDC Mosue Pos Y</param>
		/// <param name="engineCamera">Engine Camera</param>
		/// <returns>피킹 여부</returns>
		bool CheckPicking(Mesh* mesh, DirectX::XMMATRIX meshWorld, float x, float y, EngineCamera* engineCamera);
	};
}

