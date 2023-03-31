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

	Rgph::BlurOutlineRenderGraph rg{wnd.Gfx()};

	Entities::Camera camera{{-13.5f, 6.0f, 3.5f}, 0.0f, PI / 2.0f};
	Entities::PointLight light{wnd.Gfx()};

	Draw::SkinnedBox cube{wnd.Gfx(), 6.0f, {0.0f, 4.0f, -7.5f}};
	Draw::SkinnedBox cube2{wnd.Gfx(), 6.0f, {20.0f, 4.0f, -7.5f}};
	Draw::Box box{wnd.Gfx(), {1.0f, 0.4f, 0.4f}, 6.0f, {10.0f, 2.0f, 0.0f}};
	Draw::Asset asset{wnd.Gfx(), {0.4f, 0.4f, 1.0f}, 1.0f, {15.0f, 2.0f, 0.0f}};
	Draw::Cylinder cylinder{wnd.Gfx(), {0.4f, 1.0f, 0.4f}, 6.0f, {20.0f, 2.0f, 0.0f}};
	Draw::Pyramid pyramid{wnd.Gfx(), {1.0f, 1.0f, 0.4f}, 6.0f, {25.0f, 2.0f, 0.0f}};
	Draw::Sheet sheet{wnd.Gfx(), 6.0f, {30.0f, 2.0f, 0.0f}};
	Entities::Model goblin{wnd.Gfx(), "Goblin", "Models\\gobber\\GoblinX.obj", 3.0f, {5.0f, 4.0f, 0.0f}, {0.0f, PI / 2.0f, 0.0f}};
	Entities::Model sponza{wnd.Gfx(), "Sponza", "Models\\sponza\\sponza.obj", 1.0f / 20.0f};
	Entities::Model nano{wnd.Gfx(), "Nano", "Models\\nano_textured\\nanosuit.obj", 2.0f, {27.f, -0.56f, 1.7f}, {0.0f, PI / 2.0f, 0.0f}};

	void HandleInput(float dt);
	void DoFrame(float dt);
	void SpawnSimulationWindow() noexcept;

	Probes::DrawableProbe drawableProbe{};
	Probes::HierarchyModelProbe modelProbe{};
};
