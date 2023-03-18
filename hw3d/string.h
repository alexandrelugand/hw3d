#pragma once

namespace String
{
	const std::string WHITESPACE = " \n\r\t\f\v";

	static std::string ltrim(const std::string& s)
	{
		size_t start = s.find_first_not_of(WHITESPACE);
		return (start == std::string::npos) ? "" : s.substr(start);
	}

	static std::string ltrim(const std::string& s, const char& c)
	{
		size_t start = s.find_first_not_of(c);
		return (start == std::string::npos) ? "" : s.substr(start);
	}

	static std::string rtrim(const std::string& s)
	{
		size_t end = s.find_last_not_of(WHITESPACE);
		return (end == std::string::npos) ? "" : s.substr(0, end + 1);
	}

	static std::string rtrim(const std::string& s, const char& c)
	{
		size_t end = s.find_last_not_of(c);
		return (end == std::string::npos) ? "" : s.substr(0, end + 1);
	}

	static std::string trim(const std::string& s)
	{
		return rtrim(ltrim(s));
	}

	static std::string trim(const std::string& s, const char& c)
	{
		return rtrim(ltrim(s, c), c);
	}
}
