#pragma once
#include "ShaderResourceBase.h"

struct ID3D11ShaderResourceView;

/// <summary>
/// SRV를 셋팅할 엔트리 포인트 가지고있는 클래스
/// </summary>

namespace TLGraphicsEngine
{
	enum class ShaderType;

	class ShaderResourceBuffer :
	    public ShaderResourceBase
	{
	public:
		ShaderResourceBuffer(const char* name, int bindPoint, int bindCnt);
		virtual ~ShaderResourceBuffer();

	public:

		void Update(ShaderType type, ID3D11ShaderResourceView** srv);
	};
}