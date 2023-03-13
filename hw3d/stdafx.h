#pragma once

// Disable unnecessary preprocessor macros
#define FULL_WINTARD
#include "WinPreprocessor.h"

// STL
#include <chrono>
#include <random>
#include <exception>
#include <stdexcept>
#include <memory>
#include <vector>
#include <array>
#include <string>
#include <algorithm>
#include <sstream>
#include <bitset>
#include <optional>
#include <queue>
#include <iomanip>
#include <set>
#include <unordered_map>

// Windows
#pragma warning(disable:4265)
#include <wrl.h>
#pragma warning(default:4265)

namespace Gdiplus
{
	using std::min;
	using std::max;
}

#include <gdiplus.h>
#include <assert.h>

// DirectX
#include <d3d11.h>
#include <DirectXMath.h>
#include <dxgidebug.h>
#include <d3dcompiler.h>

// ImGUI
#include <imgui/imgui.h>
#include <imgui/imgui_impl_dx11.h>
#include <imgui/imgui_impl_win32.h>

// Assimp
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

// HW3D
#include "dxerr.h"
#include "Hw3DMath.h"
#include "Hw3DTimer.h"
#include "Hw3DException.h"
#include "WindowsMessageMap.h"
#include "GDIPlusManager.h"
#include "ImguiManager.h"
#include "DxgiInfoManager.h"
#include "ConditionalNoexcept.h"
#include "WindowsThrowMacros.h"
#include "GraphicsThrowMacros.h"
#include "Keyboard.h"
#include "Mouse.h"
#include "Graphics.h"
#include "Window.h"
#include "Bindable.h"
#include "Drawable.h"
#include "DrawableBase.h"
#include "ColorDrawable.h"
#include "TestObject.h"
#include "ConstantBuffer.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "InputLayout.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "Topology.h"
#include "TransformCbuf.h"
#include "ColorCBuf.h"
#include "VertexDescriptor.h"
#include "IndexedTriangleList.h"
#include "Camera.h"
#include "PointLight.h"
#include "Sampler.h"
#include "Surface.h"
#include "Texture.h"

// Forward declarations
class DxgiInfoManager;
class GDIPlusManager;
class ImguiManager;

class Graphics;
class Window;

class Bindable;
class BindableBase;
class Drawable;
template <class T>
class DrawableBase;
class ColorDrawable;
template <class T>
class TestObject;

class Camera;
class PointLight;

class Keyboard;
class Mouse;

template <typename C>
class ConstantBuffer;
template <typename C>
class VertexConstantBuffer;
template <typename C>
class PixelConstantBuffer;

class VertexBuffer;
class IndexBuffer;
class InputLayout;
class VertexShader;
class PixelShader;
class Topology;
class TransformCBuf;
class ColorCBuf;

class VertexDescriptor;
class Vertex;
class ConstVertex;
class VertexBufferDescriptor;

template <class T>
class IndexedTriangleList;

class Sampler;
class Surface;
class Texture;

// Namespaces
using namespace Microsoft::WRL;
using namespace DirectX;
using namespace std::chrono;
using namespace std::string_literals;
