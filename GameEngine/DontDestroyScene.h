#pragma once
#include "Scene.h"
namespace TLGameEngine
{
	class DontDestroyScene :
	    public Scene
	{
	public:
		DontDestroyScene();
		~DontDestroyScene();

	public:
		virtual void Load();
		
	};
}

