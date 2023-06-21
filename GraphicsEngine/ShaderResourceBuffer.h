#pragma once
#include "ShaderResourceBase.h"

struct ID3D11ShaderResourceView;

/// <summary>
/// SRV�� ������ ��Ʈ�� ����Ʈ �������ִ� Ŭ����
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