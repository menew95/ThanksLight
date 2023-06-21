#pragma once
#include "BufferBase.h"

/// <summary>
/// Const Buffer
/// </summary>

namespace TLGraphicsEngine
{
	struct CBBase;
	enum class ShaderType;

	class ConstBuffer :
	    public BufferBase
	{
	public: 
		ConstBuffer(const char* name, int size, int bindPoint, int bindCnt);
		virtual ~ConstBuffer();

	public:
		void Update(ShaderType type, CBBase* const cb);
	};
}