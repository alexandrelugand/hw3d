#pragma once
using namespace Microsoft::WRL;
using namespace DirectX;

class Graphics
{
	friend class Bindable;

public:
	class Exception : public Hw3DException
	{
		using Hw3DException::Hw3DException;
	};

	class HrException : public Exception
	{
	public:
		HrException(int line, const char* file, HRESULT hr, std::vector<std::string> infoMsgs = {}) noexcept;
		const char* what() const noexcept override;
		const char* GetType() const noexcept override;
		HRESULT GetErrorCode() const noexcept;
		std::string GetErrorString() const noexcept;
		std::string GetErrorDescription() const noexcept;
		std::string GetErrorInfo() const noexcept;

	private:
		HRESULT hr;
		std::string info;
	};

	class InfoException : public Exception
	{
	public:
		InfoException(int line, const char* file, std::vector<std::string> infoMsgs) noexcept;
		const char* what() const noexcept override;
		const char* GetType() const noexcept override;
		std::string GetErrorInfo() const noexcept;

	private:
		std::string info;
	};

	class DeviceRemovedException : public HrException
	{
		using HrException::HrException;

	public:
		const char* GetType() const noexcept override;

	private:
		std::string reason;
	};

	Graphics(HWND hWnd);
	Graphics(const Graphics&) = delete;
	Graphics& operator=(const Graphics&) = delete;
	~Graphics() = default;

	void BeginFrame(float red, float green, float blue) const noexcept;
	void EndFrame();
	void DrawTestCube(float dt, float x, float z);
	void DrawIndexed(UINT uint) noexcpt;
	void SetProjection(FXMMATRIX proj) noexcept;
	XMMATRIX GetProjection() const noexcept;
	void SetCamera(FXMMATRIX cam) noexcept;
	XMMATRIX GetCamera() const noexcept;

	void EnableImGui() noexcept;
	void DisableImGui() noexcept;
	bool IsImGuiEnabled() const noexcept;

private:
	bool imguiEnabled = true;
	XMMATRIX projection;
	XMMATRIX camera;

	ComPtr<ID3D11Device> pDevice;
	ComPtr<IDXGISwapChain> pSwapChain;
	ComPtr<ID3D11DeviceContext> pContext;
	ComPtr<ID3D11RenderTargetView> pTarget;
	ComPtr<ID3D11DepthStencilView> pDSV;

#ifndef NDEBUG
	DxgiInfoManager infoManager;
#endif
};
