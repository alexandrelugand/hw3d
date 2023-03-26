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
	FrameCommander fc;

	Hw3DTimer timer;
	float speed_factor = 1.0f;

	Entities::Camera camera;
	Entities::PointLight light;
	Entities::Model goblin{wnd.Gfx(), "Goblin", "Models\\gobber\\GoblinX.obj", 2.0f, {5.0f, 2.0f, 0.0f}, {0.0f, PI / 2.0f, 0.0f}};
	Entities::Model sponza{wnd.Gfx(), "Sponza", "Models\\sponza\\sponza.obj", 1.0f / 20.0f};
	Draw::SkinnedBox box{wnd.Gfx(), 6.0f, {0.0f, 4.0f, -7.5f}};
	Draw::SkinnedBox box2{wnd.Gfx(), 6.0f, {22.5f, 4.0f, -7.5f}};
	Draw::Box box3{wnd.Gfx(), {1.0f, 0.4f, 0.4f}, 6.0f, {10.0f, 2.0f, 0.0f}};
	Draw::Asset asset{wnd.Gfx(), {0.4f, 0.4f, 1.0f}, 1.0f, {15.0f, 2.0f, 0.0f}};
	Draw::Cylinder cylinder{wnd.Gfx(), {0.4f, 1.0f, 0.4f}, 6.0f, {20.0f, 2.0f, 0.0f}};
	Draw::Pyramid pyramid{wnd.Gfx(), {1.0f, 1.0f, 0.4f}, 6.0f, {25.0f, 2.0f, 0.0f}};
	Draw::Sheet sheet{wnd.Gfx(), 6.0f, {30.0f, 2.0f, 0.0f}};

	void DoFrame();
	void SpawnSimulationWindow() noexcept;
};
