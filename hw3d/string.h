#pragma once
#include <codecvt>
#include <algorithm>
#pragma warning(push)
#pragma warning(disable:4996)

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


	static std::string ToNarrow(const wchar_t* str)
	{
		// ReSharper disable CppDeprecatedEntity
		return std::wstring_convert<std::codecvt_utf8<wchar_t>>().to_bytes(str);
	}

	static std::wstring ToWide(const std::string& str)
	{
		return std::wstring(str.begin(), str.end());
	}

	static std::vector<std::string> TokenizeQuoted(const std::string& input)
	{
		std::istringstream stream;
		stream.str(input);
		std::vector<std::string> tokens;
		std::string token;

		while (stream >> std::quoted(token))
		{
			tokens.push_back(std::move(token));
		}
		return tokens;
	}

	template <class Iter>
	void SplitIter(const std::string& s, const std::string& delim, Iter out)
	{
		if (delim.empty())
		{
			*out++ = s;
		}
		else
		{
			size_t a = 0, b = s.find(delim);
			for (; b != std::string::npos;
			       a = b + delim.length(), b = s.find(delim, a))
			{
				*out++ = std::move(s.substr(a, b - a));
			}
			*out++ = std::move(s.substr(a, s.length() - a));
		}
	}

	static std::vector<std::string> Split(const std::string& s, const std::string& delim)
	{
		std::vector<std::string> strings;
		SplitIter(s, delim, std::back_inserter(strings));
		return strings;
	}

	static std::string ToLower(const std::string& s)
	{
		std::string output;
		output.resize(s.size());
		std::ranges::transform(s, output.begin(), [](unsigned char c) { return std::tolower(c); });
		return output;
	}
}

#pragma warning(pop)
