#include "stdafx.h"
#include "ModelException.h"

namespace Entities
{
	ModelException::ModelException(int line, const char* file, std::string note) noexcept
		: Hw3DException(line, file), note(std::move(note))
	{
	}

	const char* ModelException::what() const noexcept
	{
		std::ostringstream oss;
		oss << Hw3DException::what()
			<< "[Note] " << GetNote();
		whatBuffer = oss.str();
		return whatBuffer.c_str();
	}

	const char* ModelException::GetType() const noexcept
	{
		return "HW3D Model exception";
	}

	const std::string& ModelException::GetNote() const noexcept
	{
		return note;
	}
}
