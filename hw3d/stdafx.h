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
#include <optional>

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
#ifdef IM_DEBUG_BREAK
#undef IM_DEBUG_BREAK()
#def IM_DEBUG_BREAK()
#endif

// Assimp
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

// HW3D
// Utilities
#include "utf8.h"
#include "uuid.h"
#include "dxerr.h"
#include "Hw3DMath.h"
#include "Hw3DTimer.h"
#include "Hw3DException.h"
#include "ConditionalNoexcept.h"
#include "WindowsThrowMacros.h"
#include "GraphicsThrowMacros.h"

// Managers
#include "WindowsMessageMap.h"
#include "GDIPlusManager.h"
#include "ImguiManager.h"
#include "DxgiInfoManager.h"

// I/O
#include "Keyboard.h"
#include "Mouse.h"

// Renderer
#include "Graphics.h"

//Windowing
#include "Window.h"
#include "ModelWindow.h"

// Bind
#include "Bindable.h"
#include "Codex.h"
#include "Drawable.h"
#include "ColorDrawable.h"
#include "ConstantBuffer.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "InputLayout.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "Topology.h"
#include "TransformCbuf.h"
#include "ColorCBuf.h"
#include "Surface.h"
#include "Texture.h"
#include "Sampler.h"
#include "Cull.h"

// Dvtx
#include "Color.h"
#include "VertexLayout.h"
#include "Vertex.h"
#include "ConstVertex.h"
#include "VertexBufferDescriptor.h"

// Geometry
#include "IndexedTriangleList.h"

// Draw
#include "DrawableObject.h"
#include "Asset.h"
#include "Box.h"
#include "Cylinder.h"
#include "Pyramid.h"
#include "Sheet.h"
#include "SkinnedBox.h"
#include "SolidSphere.h"

// Entities
#include "Camera.h"
#include "PointLight.h"
#include "Model.h"
#include "Node.h"
#include "Mesh.h"
