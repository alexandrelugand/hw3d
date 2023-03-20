#pragma once

// Forward declarations
class DxgiInfoManager;
class GDIPlusManager;
class ImguiManager;

// Renderer
class Graphics;

// Windows
namespace Windowing
{
	class Window;
	class ModelWindow;
}

// Bind
namespace Bind
{
	class Bindable;
	class Codex;
	class ColorCBuf;
	template <typename C>
	class ConstantBuffer;
	template <typename C>
	class VertexConstantBuffer;
	template <typename C>
	class PixelConstantBuffer;
	class IndexBuffer;
	class InputLayout;
	class PixelShader;
	class Texture;
	class Topology;
	class TransformCBuf;
	class VertexBuffer;
	class VertexShader;
	class Sampler;
	class Blender;
	class Rasterizer;
}

// Draw
namespace Draw
{
	class Drawable;
	template <class T>
	class DrawableObject;
	class ColorDrawable;
	class Asset;
	class Box;
	class Cylinder;
	class pyramid;
	class Sheet;
	class SkinnedBox;
	class SolidSphere;
}

// Inputs
namespace Inputs
{
	class Keyboard;
	class Mouse;
}

// Vertex layout
namespace Dvtx
{
	struct RGBAColor;
	class VertexLayout;
	class Vertex;
	class ConstVertex;
	class VertexBufferDescriptor;
}

// Entities
namespace Entities
{
	class Mesh;
	class Node;
	class Model;
	class Camera;
	class PointLight;
}

// Namespaces
using namespace Microsoft::WRL;
using namespace DirectX;
using namespace std::chrono;
using namespace std::string_literals;
