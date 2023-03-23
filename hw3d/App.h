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
	Draw::SkinnedBox box{wnd.Gfx(), 6.0f, {22.5f, 10.0f, -7.5f}};
	Entities::Model sponza{wnd.Gfx(), "Models\\sponza\\sponza.obj", 1.0f / 20.0f};

	void DoFrame();
	void SpawnSimulationWindow() noexcept;
};
