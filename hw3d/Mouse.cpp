#include "stdafx.h"
#include "Mouse.h"

namespace Inputs
{
	std::pair<int, int> Mouse::GetPos() const noexcept
	{
		return {x, y};
	}

	int Mouse::GetPosX() const noexcept
	{
		return x;
	}

	int Mouse::GetPosY() const noexcept
	{
		return y;
	}

	bool Mouse::IsInWindow() const noexcept
	{
		return isInWindow;
	}

	bool Mouse::LeftIsPressed() const noexcept
	{
		return leftIsPressed;
	}

	bool Mouse::RightIsPressed() const noexcept
	{
		return rightIsPressed;
	}

	std::optional<Mouse::Event> Mouse::Read() noexcept
	{
		if (buffer.size() > 0u)
		{
			Event e = buffer.front();
			buffer.pop();
			return e;
		}
		return {};
	}

	std::optional<Mouse::RawDelta> Mouse::ReadRawDelta() noexcept
	{
		if (rawDeltaBuffer.empty())
		{
			return std::nullopt;
		}

		const RawDelta d = rawDeltaBuffer.front();
		rawDeltaBuffer.pop();
		return d;
	}

	void Mouse::Flush() noexcept
	{
		buffer = std::queue<Event>();
	}

	void Mouse::EnableRaw() noexcept
	{
		rawEnabled = true;
	}

	void Mouse::DisableRaw() noexcept
	{
		rawEnabled = false;
	}

	bool Mouse::RawEnabled() const noexcept
	{
		return rawEnabled;
	}

	void Mouse::OnMouseMove(int newx, int newy) noexcept
	{
		x = newx;
		y = newy;

		buffer.push(Event(Event::Type::Move, *this));
		TrimBuffer();
	}

	void Mouse::OnMouseLeave() noexcept
	{
		isInWindow = false;
		buffer.push(Event(Event::Type::Leave, *this));
		TrimBuffer();
	}

	void Mouse::OnMouseEnter() noexcept
	{
		isInWindow = true;
		buffer.push(Event(Event::Type::Enter, *this));
		TrimBuffer();
	}

	void Mouse::OnRawDelta(int dx, int dy) noexcept
	{
		rawDeltaBuffer.push({dx, dy});
		TrimRawInputBuffer();
	}

	void Mouse::OnLeftPressed(int x, int y) noexcept
	{
		leftIsPressed = true;

		buffer.push(Event(Event::Type::LPress, *this));
		TrimBuffer();
	}

	void Mouse::OnLeftReleased(int x, int y) noexcept
	{
		leftIsPressed = false;

		buffer.push(Event(Event::Type::LRelease, *this));
		TrimBuffer();
	}

	void Mouse::OnRightPressed(int x, int y) noexcept
	{
		rightIsPressed = true;

		buffer.push(Event(Event::Type::RPress, *this));
		TrimBuffer();
	}

	void Mouse::OnRightReleased(int x, int y) noexcept
	{
		rightIsPressed = false;

		buffer.push(Event(Event::Type::RRelease, *this));
		TrimBuffer();
	}

	void Mouse::OnWheelUp(int x, int y) noexcept
	{
		buffer.push(Event(Event::Type::WheelUp, *this));
		TrimBuffer();
	}

	void Mouse::OnWheelDown(int x, int y) noexcept
	{
		buffer.push(Event(Event::Type::WheelDown, *this));
		TrimBuffer();
	}

	void Mouse::TrimBuffer() noexcept
	{
		while (buffer.size() > bufferSize)
		{
			buffer.pop();
		}
	}

	void Mouse::TrimRawInputBuffer() noexcept
	{
		while (rawDeltaBuffer.size() > bufferSize)
		{
			rawDeltaBuffer.pop();
		}
	}

	void Mouse::OnWheelDelta(int x, int y, int delta) noexcept
	{
		wheelDeltaCarry += delta;
		// generate events for every 120 
		while (wheelDeltaCarry >= WHEEL_DELTA)
		{
			wheelDeltaCarry -= WHEEL_DELTA;
			OnWheelUp(x, y);
		}
		while (wheelDeltaCarry <= -WHEEL_DELTA)
		{
			wheelDeltaCarry += WHEEL_DELTA;
			OnWheelDown(x, y);
		}
	}
}
