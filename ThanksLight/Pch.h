#pragma once
#ifdef _DEBUG
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
// Replace _NORMAL_BLOCK with _CLIENT_BLOCK if you want the
// allocations to be of _CLIENT_BLOCK type
#else
#define DBG_NEW new

#endif // _DEBUG

#include <memory>
#include <vector>
#include <functional>
#include <Windows.h>
#include <cassert>
#include <tchar.h>

#include "../GameEngine/pch.h"

using namespace TLGameEngine;