#pragma once
#include "Forwards.h"

enum CullMode
{
	None = 0,
	Front,
	Back
};

class Graphics
{
	friend class Bind::Bindable;

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

	Graphics(HWND hWnd, unsigned int width, unsigned int height);
	Graphics(const Graphics&) = delete;
	Graphics& operator=(const Graphics&) = delete;
	~Graphics() = default;

	void BeginFrame(float red, float green, float blue) const noexcept;
	void EndFrame();

	void DrawIndexed(UINT uint) noexcpt;

	void SetProjection(FXMMATRIX proj) noexcept;
	XMMATRIX GetProjection() const noexcept;

	void SetCamera(FXMMATRIX cam) noexcept;
	XMMATRIX GetCamera() const noexcept;

	void SetCullMode(const CullMode& cullMode) const;

	void EnableImGui() noexcept;
	void DisableImGui() noexcept;
	bool IsImGuiEnabled() const noexcept;

private:
	void InitRasterizerState();
	bool imguiEnabled = true;
	XMMATRIX projection{};
	XMMATRIX camera{};

	ComPtr<ID3D11Device> pDevice = nullptr;
	ComPtr<IDXGISwapChain> pSwapChain = nullptr;
	ComPtr<ID3D11DeviceContext> pContext = nullptr;
	ComPtr<ID3D11RenderTargetView> pTarget = nullptr;
	ComPtr<ID3D11DepthStencilView> pDSV = nullptr;

	ComPtr<ID3D11RasterizerState> m_wire_frame_state = nullptr;
	ComPtr<ID3D11RasterizerState> m_cull_front_state = nullptr;
	ComPtr<ID3D11RasterizerState> m_cull_back_state = nullptr;
	ComPtr<ID3D11RasterizerState> m_cull_none_state = nullptr;

#ifndef NDEBUG
	DxgiInfoManager infoManager;
#endif
};
