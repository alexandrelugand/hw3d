// ReSharper disable CppClangTidyClangDiagnosticUnusedFunction
#pragma once
#include <codecvt>
#pragma warning(push)
#pragma warning(disable:4996)
static const char* W2N(const wchar_t* str)
{
	// ReSharper disable CppDeprecatedEntity
	return std::wstring_convert<std::codecvt_utf8<wchar_t>>().to_bytes(str).c_str(); // NOLINT(clang-diagnostic-deprecated-declarations, clang-diagnostic-return-stack-address)
}

static std::wstring N2W(const std::string& str)
{
	return std::wstring(str.begin(), str.end());
}
#pragma warning(pop)
