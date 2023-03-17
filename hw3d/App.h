#pragma once

class App
{
public:
	App();
	~App() = default;

	//Master frame / message loop
	int Go();

private:
	Windowing::Window wnd;
	Hw3DTimer timer;
	float speed_factor = 1.0f;

	Entities::Camera camera;
	Entities::PointLight light;
	Entities::Model nano{wnd.Gfx(), "Models\\nano_textured\\nanosuit.obj"};
	Draw::Sheet sheet{wnd.Gfx(), 3.0f};

	void DoFrame();
	void SpawnSimulationWindow() noexcept;
};
