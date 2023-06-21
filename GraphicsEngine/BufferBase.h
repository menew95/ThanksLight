#pragma once

#include "ShaderResourceBase.h"

struct ID3D11Buffer;
struct ID3D11DeviceContext;

/// <summary>
/// Buffer Base
/// vertex, index, const buffer들의 부모 클래스
/// </summary>

namespace TLGraphicsEngine
{
	enum class RegisterType;

	class BufferBase : public ShaderResourceBase
	{
	public:
		BufferBase() = delete;
		BufferBase(const char* name, int bindPoint, int bindCnt, int size, RegisterType type);
		virtual ~BufferBase() = default;

	public:
		int m_Size = 0;

		ComPtr<ID3D11Buffer> m_Buffer = nullptr;

	public:
		int GetSize();
		ID3D11Buffer* GetBuffer();
		ID3D11Buffer* const* GetBufferRef();
	};
}