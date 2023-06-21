#pragma once

#define WIN32_LEAN_AND_MEAN             // 거의 사용되지 않는 내용을 Windows 헤더에서 제외합니다.
// Windows 헤더 파일
#include <windows.h>
#include <wrl.h>
#ifdef _DEBUG
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#define NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
// Replace _NORMAL_BLOCK with _CLIENT_BLOCK if you want the
// allocations to be of _CLIENT_BLOCK type
#else
#define NEW new

#endif // _DEBUG

#include <cassert>
#include <ctime>
#include <string>
#include <iostream>
#include <ostream>
#include <sstream>
#include <fstream>
#include <io.h>

#include <winerror.h>
#include <tchar.h>

#include <algorithm>
#include <iterator>
#include <vector>
#include <stdexcept>

#include <vector>
#include <map>
#include <unordered_map>
#include <set>
#include <queue>
#include <algorithm>

#include <memory>
#include <wrl.h>