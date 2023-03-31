#include "stdafx.h"
#include "string.h"
#include <codecvt>
#include <algorithm>
#pragma warning(push)
#pragma warning(disable:4996)

namespace String
{
	std::string ltrim(const std::string& s)
	{
		size_t start = s.find_first_not_of(WHITESPACE);
		return (start == std::string::npos) ? "" : s.substr(start);
	}

	std::string ltrim(const std::string& s, const char& c)
	{
		size_t start = s.find_first_not_of(c);
		return (start == std::string::npos) ? "" : s.substr(start);
	}

	std::string rtrim(const std::string& s)
	{
		size_t end = s.find_last_not_of(WHITESPACE);
		return (end == std::string::npos) ? "" : s.substr(0, end + 1);
	}

	std::string rtrim(const std::string& s, const char& c)
	{
		size_t end = s.find_last_not_of(c);
		return (end == std::string::npos) ? "" : s.substr(0, end + 1);
	}

	std::string trim(const std::string& s)
	{
		return rtrim(ltrim(s));
	}

	std::string trim(const std::string& s, const char& c)
	{
		return rtrim(ltrim(s, c), c);
	}

	std::string ToNarrow(const wchar_t* str)
	{
		// ReSharper disable CppDeprecatedEntity
		return std::wstring_convert<std::codecvt_utf8<wchar_t>>().to_bytes(str);
	}

	std::wstring ToWide(const std::string& str)
	{
		return std::wstring(str.begin(), str.end());
	}

	std::vector<std::string> TokenizeQuoted(const std::string& input)
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

	std::vector<std::string> Split(const std::string& s, const std::string& delim)
	{
		std::vector<std::string> strings;
		SplitIter(s, delim, std::back_inserter(strings));
		return strings;
	}

	std::string ToLower(const std::string& s)
	{
		std::string output;
		output.resize(s.size());
		std::ranges::transform(s, output.begin(), [](unsigned char c) { return std::tolower(c); });
		return output;
	}

	bool Contains(std::string_view haystack, std::string_view needle)
	{
		return std::ranges::search(haystack, needle).begin() != haystack.end();
	}
}

#pragma warning(pop)
