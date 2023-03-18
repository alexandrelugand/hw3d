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
	//Entities::Model nano{wnd.Gfx(), "Models\\nano_textured\\nanosuit.obj"};
	//Entities::Model wall{wnd.Gfx(), "Models\\brick_wall\\brick_wall.obj"};
	//Draw::Sheet sheet{wnd.Gfx(), 3.0f};
	Entities::Model gobber{wnd.Gfx(), "Models\\gobber\\GoblinX2.obj"};

	void DoFrame();
	void SpawnSimulationWindow() noexcept;
};
