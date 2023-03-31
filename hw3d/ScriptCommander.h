#pragma once

class ScriptCommander
{
public:
	class Exception : public Hw3DException
	{
	public:
		Exception(int line, const char* file, const std::string& script = "", const std::string& message = "") noexcept;
		const char* what() const noexcept override;
		const char* GetType() const noexcept override;

	private:
		std::string script;
		std::string message;
	};

	ScriptCommander(const std::vector<std::string>& args);
	void Publish(std::string path) const;
};
