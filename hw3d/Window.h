#pragma once

namespace Windowing
{
	class Window
	{
	public:
		class Exception : public Hw3DException
		{
			using Hw3DException::Hw3DException;

		public:
			static std::string TranslateErrorCode(HRESULT hr) noexcept;
		};

		class HrException : public Exception
		{
		public:
			HrException(int line, const char* file, HRESULT hr) noexcept;
			const char* what() const noexcept override;
			const char* GetType() const noexcept override;
			HRESULT GetErrorCode() const noexcept;
			std::string GetErrorDescription() const noexcept;

		private:
			HRESULT hr;
		};

		class NoGfxException : public Exception
		{
		public:
			using Exception::Exception;
			const char* GetType() const noexcept override;
		};

	private:
		// singleton manages registration/cleanup of window class
		class WindowClass
		{
		public:
			static const char* GetName() noexcept;
			static HINSTANCE GetInstance() noexcept;

		private:
			WindowClass() noexcept;
			~WindowClass();
			WindowClass(const WindowClass&) = delete;
			WindowClass& operator=(const WindowClass&) = delete;
			static constexpr const char* wndClassName = "Direct3D Engine";
			static WindowClass wndClass;
			HINSTANCE hInst;
		};

	public:
		Window(unsigned int width, unsigned int height, const char* name);
		~Window();
		Window(const Window&) = delete;
		Window& operator=(const Window&) = delete;

		Inputs::Keyboard kbd;
		Inputs::Mouse mouse;

		void SetTitle(const std::string& title) const;
		void EnableCursor() noexcept;
		void DisableCursor() noexcept;
		bool CursorEnabled() const noexcept;
		static std::optional<int> ProcessMessages();
		Graphics& Gfx() const;

	private:
		void ConfineCursor() noexcept;
		void FreeCursor() noexcept;
		void ShowCursor() noexcept;
		void HideCursor() noexcept;
		void EnableImGuiMouse() noexcept;
		void DisableImGuiMouse() noexcept;
		static LRESULT CALLBACK HandleMsgSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
		static LRESULT CALLBACK HandleMsgThunk(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;
		LRESULT HandleMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept;

		ImguiManager imgui{};

		bool cursorEnabled = true;
		int width;
		int height;
		HWND hWnd;
		std::unique_ptr<Graphics> pGfx;
		std::vector<char> rawBuffer;
	};
}
