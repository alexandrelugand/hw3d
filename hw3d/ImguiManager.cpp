#include "stdafx.h"
#include "ImguiManager.h"

ImguiManager::ImguiManager()
{
	if (!std::filesystem::exists("imgui.ini") && std::filesystem::exists("imgui_default.ini"))
		std::filesystem::copy_file("imgui_default.ini", "imgui.ini");

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui::StyleColorsDark();
}

ImguiManager::~ImguiManager()
{
	ImGui::DestroyContext();
}
