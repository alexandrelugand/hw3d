#pragma once

namespace String
{
	const std::string WHITESPACE = " \n\r\t\f\v";
	extern std::string ltrim(const std::string& s);
	extern std::string ltrim(const std::string& s, const char& c);
	extern std::string rtrim(const std::string& s);
	extern std::string rtrim(const std::string& s, const char& c);
	extern std::string trim(const std::string& s);
	extern std::string trim(const std::string& s, const char& c);
	extern std::string ToNarrow(const wchar_t* str);
	extern std::wstring ToWide(const std::string& str);
	extern std::vector<std::string> TokenizeQuoted(const std::string& input);

	template <class Iter>
	constexpr void SplitIter(const std::string& s, const std::string& delim, Iter out)
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

	extern std::vector<std::string> Split(const std::string& s, const std::string& delim);
	extern std::string ToLower(const std::string& s);
	extern bool Contains(std::string_view haystack, std::string_view needle);
}
