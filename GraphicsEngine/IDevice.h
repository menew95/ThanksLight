#pragma once

#include <memory>

struct _GUID;
typedef struct _GUID GUID;

namespace GraphicsEngine
{
	class ViewportContext
	{
		
	};

	class IDevice
	{
	public:
		virtual void Present() abstract;
		virtual void CreateDevice() abstract;


	};
}