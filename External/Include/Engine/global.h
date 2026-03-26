#pragma once

#include <Windows.h>

// STL
#include <vector>
#include <list>
#include <map>
#include <set>
#include <queue>
#include <string>
#include <algorithm>
#include <thread>
#include <functional>  // std::function, std::bind


using std::vector;
using std::list;
using std::map;
using std::set;
using std::queue;
using std::make_pair;
using std::string;
using std::wstring;
using std::pair;
using std::unordered_map;

#include <filesystem>
using namespace std::filesystem;

#include "singleton.h"

// DirectX 11 라이브러리 설치
// 비주얼 스튜디오 설치 시 함께 연동됨
#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>	// 수학 관련
#include <DirectXPackedVector.h>	// 수학 관련 추가
#include <dxgidebug.h>

using namespace DirectX;
using namespace DirectX::PackedVector;

#pragma comment(lib, "d3d11")
#pragma comment(lib, "d3dcompiler")
#pragma comment(lib, "dxguid")



// ComPtr
#include <wrl.h>
using namespace Microsoft::WRL;

// Directx Tex
#include <DirectxTex/DirectxTex.h>

#ifdef _DEBUG
#pragma comment(lib, "DirectxTex\\DirectXTex_debug")
#else
#pragma comment(lib, "DirectxTex\\DirectXTex")
#endif






// FMOD
#include <FMOD/fmod.h>
#include <FMOD/fmod.hpp>
#include <FMOD/fmod_codec.h>

#ifdef _DEBUG
#pragma comment(lib, "FMOD/fmodL64_vc.lib")
#else
#pragma comment(lib, "FMOD/fmod64_vc.lib")
#endif


#include "SimpleMath.h"

#include "enum.h"
#include "define.h"
#include "struct.h"
#include "func.h"
#include "Ptr.h"

#include "struct_ptr.h"

