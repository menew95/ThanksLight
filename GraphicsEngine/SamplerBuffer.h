#pragma once
#include "ShaderResourceBase.h"

/// <summary>
/// Sampler Resource
/// </summary>

struct ID3D11SamplerState;

namespace TLGraphicsEngine
{
	enum class ShaderType;

	class SamplerBuffer :
	    public ShaderResourceBase
	{
	public:
		SamplerBuffer(const char* name, int bindPoint, int bindCnt);
		virtual ~SamplerBuffer();

	private:
		/// <summary>
		/// ���̴����� �޾ƿ� ���÷� �̸��� ���� ���÷�������Ʈ Ŭ�������� �̹� ������
		/// ���÷�������Ʈ�� ã�ƿ�
		/// </summary>
		ComPtr<ID3D11SamplerState> m_pSamplerState;
		
	public:

		void Update(ShaderType type);

		// �ܺο��� ���÷��� �޾Ƽ� ���� ������ ��� ��뿹���� ���� ����
		void Update(ShaderType type, ID3D11SamplerState* sampler);
	};
}