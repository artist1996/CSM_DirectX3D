#pragma once

#include <Windows.h>
#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include <DirectXPackedVector.h>

#include <wrl.h>
using namespace Microsoft::WRL;

#pragma comment(lib, "d3d11")
#pragma comment(lib, "d3dcompiler")

// DirectxTex
#include <DirectxTex/DirectXTex.h>

#ifdef _DEBUG
#pragma comment(lib, "DirectxTex//DirectXTex_debug.lib")
#else
#pragma comment(lib, "DirectxTex//DirectXTex.lib")
#endif

#include "SimpleMath.h"
using namespace DirectX;
using namespace DirectX::SimpleMath;

#include <string>
using std::string;
using std::wstring;

#include <vector>
using std::vector;

#include <list>
using std::list;

#include <map>
using std::map;
using std::make_pair;

#include <typeinfo>

#include <filesystem>
using namespace std::filesystem;

// FMOD
#include <FMOD/fmod.h>
#include <FMOD/fmod.hpp>
#include <FMOD/fmod_codec.h>

#ifdef _DEBUG
#pragma comment(lib, "FMOD/fmodL64_vc.lib")
#else
#pragma comment(lib, "FMOD/fmod64_vc.lib")
#endif

// Fbx Loader
#include <FBXLoader/fbxsdk.h>
#ifdef _DEBUG
#pragma comment(lib, "FBXLoader/x64/debug/libfbxsdk-md.lib")
#else
#pragma comment(lib, "FBXLoader/x64/release/libfbxsdk-md.lib")
#endif

// Recast
#include <Recast/Recast.h>
#include <Recast/RecastAlloc.h>
#include <Recast/RecastAssert.h>
#pragma comment(lib, "Recast/x64/Recast.lib")

// Detour
#include <Detour/DetourAlloc.h>
#include <Detour/DetourAssert.h>
#include <Detour/DetourCommon.h>
#include <Detour/DetourMath.h>
#include <Detour/DetourNavMesh.h>
#include <Detour/DetourNavMeshBuilder.h>
#include <Detour/DetourNavMeshQuery.h>
#include <Detour/DetourNode.h>
#include <Detour/DetourStatus.h>
#include <Detour/DetourCrowd.h>
#include <Detour/DetourLocalBoundary.h>
#include <Detour/DetourObstacleAvoidance.h>
#include <Detour/DetourPathCorridor.h>
#include <Detour/DetourPathQueue.h>
#include <Detour/DetourProximityGrid.h>

#pragma comment(lib, "Detour/x64/Detour.lib")
#pragma comment(lib, "DetourCrowd/x64/DetourCrowd.lib");


#include <random>

#include "singleton.h"
#include "define.h"
#include "enum.h"
#include "struct.h"
#include "func.h"
#include "Ptr.h"