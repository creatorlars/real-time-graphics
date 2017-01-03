#pragma once

// Windows
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX

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

// CSTDLIB
#include <cstdio>
#include <cstdint>

// libs
#include "libs\AntTweakBar.h"

// Globals
bool constexpr FULL_SCREEN = false;
bool constexpr VSYNC_ENABLED = true;
float constexpr SCREEN_DEPTH = 1000.f;
float constexpr SCREEN_NEAR = .1f;
unsigned constexpr WINDOW_WIDTH = 800;
unsigned constexpr WINDOW_HEIGHT = 600;