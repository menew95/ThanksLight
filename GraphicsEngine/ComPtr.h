#pragma once

#include <wrl.h>

namespace TLGraphicsEngine
{
	template<typename T>
	using ComPtr = Microsoft::WRL::ComPtr<T>;
}