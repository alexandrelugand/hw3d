#pragma once

// Forward declarations
class DxgiInfoManager;
class GDIPlusManager;
class ImguiManager;

// Renderer
class GraphicsResource;
class Graphics;

class Technique;
class FrameCommander;
class Material;
class BlurPack;

// Windows
namespace Windowing
{
	class Window;
}

// Bind
namespace Bind
{
	class BufferResource;
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
	class DynamicCBuf;
	class DepthStencil;
	class RenderTarget;
}

// Draw
namespace Draw
{
	class Drawable;
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

// Dynamic Vertex layout
namespace Dvtx
{
	struct RGBAColor;
	class VertexLayout;
	class Vertex;
	class ConstVertex;
	class VertexBufferDescriptor;
}

// Dynamic Constant buffer
namespace Dcb
{
	class LayoutElement;
	class Layout;
	class RawLayout;
	class CookedLayout;
	class ConstElementRef;
	class ElementRef;
	class Buffer;
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

// Render graph
namespace Rgph
{
	class Pass;
	class Sink;
	class Source;
	class BindingPass;
	class Job;
	class Step;
	class RenderQueuePass;
	class RenderGraph;
}

namespace Probes
{
	class TechniqueProbe;
	class ModelProbe;
	class DrawableProbe;
	class HierarchyModelProbe;
	class MeshTechniqueProbe;
}

// Namespaces
using namespace Microsoft::WRL;
using namespace DirectX;
using namespace std::chrono;
using namespace std::string_literals;
