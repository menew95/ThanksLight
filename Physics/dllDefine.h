#pragma once

#ifdef PHYSICS_EXPORTS
#define PHYSICS_ENGINE __declspec(dllexport)
#define	PHYSICS_ENGINE_TEMPLATE_EXPORT 
#else
#define PHYSICS_ENGINE __declspec(dllimport)
#define	PHYSICS_ENGINE_TEMPLATE_EXPORT extern
#endif