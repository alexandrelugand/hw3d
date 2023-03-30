#include "stdafx.h"
#include "RenderGraphCompileException.h"

namespace Rgph
{
	RenderGraphCompileException::RenderGraphCompileException(std::string message, int line, const char* file) noexcept
		: Hw3DException(line, file), message(std::move(message))
	{
	}

	const char* RenderGraphCompileException::what() const noexcept
	{
		std::ostringstream oss;
		oss << Hw3DException::what() << std::endl;
		oss << "[Message]" << std::endl;
		oss << message;
		whatBuffer = oss.str();
		return whatBuffer.c_str();
	}

	const char* RenderGraphCompileException::GetType() const noexcept
	{
		return "Render Graph Compile Exception";
	}

	const std::string& RenderGraphCompileException::GetMessageA() const noexcept
	{
		return message;
	}
}
