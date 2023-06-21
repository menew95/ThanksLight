#include "Export/IGraphicsEngine.h"
#include "GraphicsEngine.h"


GRAPHIC_CLASS_DECLSPEC void TLGraphicsEngine::GraphicsEngineRelease(IGraphicsEngine* engine)
{
	if (engine != nullptr)
	{
		delete engine;
		engine = nullptr;
	}
}

GRAPHIC_CLASS_DECLSPEC TLGraphicsEngine::IGraphicsEngine* TLGraphicsEngine::GraphicsEngineInstance(eEngineType engineType)
{
	switch (engineType)
	{
		case eEngineType::DirectX11:
			return TLGraphicsEngine::GraphicsEngine::Instance();
		default:
			return nullptr;
	}
}
