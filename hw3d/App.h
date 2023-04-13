#pragma once

class App
{
public:
	App(const std::string& commandLine);
	~App() = default;

	//Master frame / message loop
	int Go();

private:
	Windowing::Window wnd;
	ScriptCommander sc;

	Hw3DTimer timer;
	float speed_factor = 1.0f;
	bool savingDepth{};

	Rgph::BlurOutlineRenderGraph rg{wnd.Gfx()};

	Entities::CameraContainer cameras;
	Entities::PointLight light{wnd.Gfx(), {-9.0f, 15.8f, -3.4f}, PI / 180.0f * 12.0f, PI / 180.0f * 90.0f};

	Draw::SkinnedBox cube{wnd.Gfx(), 6.0f, {-14.0f, 4.0f, 0.0f}};
	Draw::SkinnedBox cube2{wnd.Gfx(), 6.0f, {-14.0f, 4.0f, -7.5f}};
	Entities::Model goblin{wnd.Gfx(), "Goblin", "Models\\gobber\\GoblinX.obj", 3.0f, {5.0f, 4.0f, 0.0f}, {0.0f, PI / 2.0f, 0.0f}};
	Entities::Model sponza{wnd.Gfx(), "Sponza", "Models\\sponza\\sponza.obj", 1.0f / 20.0f};
	Entities::Model nano{wnd.Gfx(), "Nano", "Models\\nano_textured\\nanosuit.obj", 2.0f, {27.f, -0.56f, 1.7f}, {0.0f, PI / 2.0f, 0.0f}};

	void HandleInput(float dt);
	void DoFrame(float dt);
	void SpawnSimulationWindow() noexcept;

	Probes::DrawableProbe drawableProbe{};
	Probes::HierarchyModelProbe modelProbe{};
};
