#include "stdafx.h"
#include "Hw3DException.h"

Hw3DException::Hw3DException(int line, const char* file) noexcept
	:
	line(line),
	file(file)
{
}

const char* Hw3DException::what() const noexcept
{
	std::ostringstream oss;
	oss << GetType() << std::endl
		<< GetOriginString();
	whatBuffer = oss.str();
	return whatBuffer.c_str();
}

const char* Hw3DException::GetType() const noexcept
{
	return "HW3D Exception";
}

int Hw3DException::GetLine() const noexcept
{
	return line;
}

const std::string& Hw3DException::GetFile() const noexcept
{
	return file;
}

std::string Hw3DException::GetOriginString() const noexcept
{
	std::ostringstream oss;
	oss << "[File] " << file << std::endl
		<< "[Line] " << line;
	return oss.str();
}
