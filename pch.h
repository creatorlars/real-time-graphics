#pragma once

// Windows
#define NOMINMAX

#pragma comment(lib, "winmm.lib")

#include <windows.h>
#include <wrl/client.h>

template<typename T>
using ComPtr = Microsoft::WRL::ComPtr<T>;

// DirectX
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")

#include <d3d11.h>
#include <directxmath.h>
using namespace DirectX;

// STL
#include <fstream>
#include <sstream>
#include <algorithm>
#include <string>
#include <iterator>
#include <vector>
#include <numeric>
#include <memory>
#include <limits>
#include <tuple>
#include <array>
#include <iostream>
#include <functional>
#include <chrono>
#include <random>
using namespace std::literals;

// CSTDLIB
#include <cstdio>
#include <cstdint>

// libs
#include "libs\AntTweakBar.h"

// Globals
float constexpr SCREEN_DEPTH_FAR = 1000.f;
float constexpr SCREEN_DEPTH_NEAR = .1f;