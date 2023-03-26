#pragma once

namespace Entities
{
	class ModelException : public Hw3DException
	{
	public:
		ModelException(int line, const char* file, std::string note) noexcept;
		const char* what() const noexcept override;
		const char* GetType() const noexcept override;
		const std::string& GetNote() const noexcept;

	private:
		std::string note;
	};
}
