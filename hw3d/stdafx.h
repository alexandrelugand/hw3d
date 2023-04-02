#pragma once

// Disable unnecessary preprocessor macros
#define FULL_WINTARD
#include "WinPreprocessor.h"

// STL
#include <cassert>
#include <chrono>
#include <random>
#include <exception>
#include <stdexcept>
#include <memory>
#include <vector>
#include <array>
#include <string>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <bitset>
#include <optional>
#include <queue>
#include <iomanip>
#include <set>
#include <unordered_map>
#include <optional>
#include <iostream>
#include <filesystem>

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
#include <DirectXTex.h>

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

//JSON
#include <json.hpp>

// HW3D
// Utilities
#include "uuid.h"
#include "string.h"
#include "dxerr.h"
#include "Hw3DMath.h"
#include "Hw3DTimer.h"
#include "Hw3DException.h"
#include "ConditionalNoexcept.h"
#include "WindowsThrowMacros.h"
#include "GraphicsThrowMacros.h"
#include "TexturePreprocessor.h"
#include "ScriptCommander.h"

// Managers
#include "WindowsMessageMap.h"
#include "GDIPlusManager.h"
#include "ImguiManager.h"
#include "DxgiInfoManager.h"

// I/O
#include "Keyboard.h"
#include "Mouse.h"

// Renderer
#include "GraphicsResouce.h"
#include "Graphics.h"
#include "PsMaterialConstants.h"
#include "Technique.h"
#include "TechniqueProbe.h"
#include "ModelProbe.h"
#include "Material.h"
#include "Projection.h"

//Windowing
#include "Window.h"

// Bind
#include "BufferResource.h"
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
#include "TransformAllCBuf.h"
#include "TransformCBufScaling.h"
#include "ColorCBuf.h"
#include "Surface.h"
#include "Texture.h"
#include "Sampler.h"
#include "Blender.h"
#include "Rasterizer.h"
#include "LayoutCodex.h"
#include "DynamicCBuf.h"
#include "Stencil.h"
#include "NullPixelShader.h"
#include "DepthStencil.h"
#include "RenderTarget.h"
#include "ShadowCameraCBuf.h"
#include "ShadowSampler.h"

// Dynamic Vertex layout
#include "Color.h"
#include "VertexLayout.h"
#include "Vertex.h"
#include "ConstVertex.h"
#include "VertexBufferDescriptor.h"

// Dynamic Constant buffer
#include "LayoutCodex.h"
#include "DynamicConstant.h"

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
#include "CameraIndicator.h"
#include "Frustum.h"

// Entities
#include "Camera.h"
#include "CameraContainer.h"
#include "PointLight.h"
#include "ModelException.h"
#include "Model.h"
#include "Node.h"
#include "Mesh.h"

// Render graph
#include "Channels.h"
#include "RenderGraphCompileException.h"
#include "Pass.h"
#include "Sink.h"
#include "Source.h"
#include "BindingPass.h"
#include "Job.h"
#include "Step.h"
#include "RenderQueuePass.h"
#include "BlurOutlineDrawingPass.h"
#include "RenderGraph.h"
#include "BufferClearPass.h"
#include "LambertianPass.h"
#include "OutlineMaskGenerationPass.h"
#include "FullscreenPass.h"
#include "HorizontalBlurPass.h"
#include "VerticalBlurPass.h"
#include "WireframePass.h"
#include "ShadowMappingPass.h"
#include "BlurOutlineRenderGraph.h"
#include "ScaleOutlineRenderGraph.h"

// Probes
#include "TransformParameters.h"
#include "MeshTechniqueProbe.h"
#include "TechniqueProbe.h"
#include "HierarchyModelProbe.h"
#include "DrawableProbe.h"
