#pragma once

class Hw3DException : public std::exception
{
public:
	Hw3DException(int line, const char* file) noexcept;
	const char* what() const noexcept override;
	virtual const char* GetType() const noexcept;
	int GetLine() const noexcept;
	const std::string& GetFile() const noexcept;
	std::string GetOriginString() const noexcept;

protected:
	//Since what() is declared const, need to make whatBuffer mutable to set it
	mutable std::string whatBuffer;

private:
	int line;
	std::string file;
};
