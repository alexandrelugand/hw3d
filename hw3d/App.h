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
	ScriptCommander scriptCommander;

	Hw3DTimer timer;
	float speed_factor = 1.0f;

	Entities::Camera camera;
	Entities::PointLight light;
	//Entities::Model nano{wnd.Gfx(), "Models\\nano_textured\\nanosuit.obj"};
	//Entities::Model wall{wnd.Gfx(), "Models\\brick_wall\\brick_wall.obj"};
	//Entities::Model gobber{wnd.Gfx(), "Models\\gobber\\GoblinX.obj"};
	Entities::Model sponza{wnd.Gfx(), "Models\\sponza\\sponza.obj", 1.0f / 20.0f};

	void DoFrame();
	void SpawnSimulationWindow() noexcept;
};
