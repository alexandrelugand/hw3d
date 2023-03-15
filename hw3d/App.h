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
	int x = 0, y = 0;

	Entities::Camera camera;
	Entities::PointLight light;

	Entities::Model nano{wnd.Gfx(), "Models\\nano_hierarchy.gltf"};

	void DoFrame();

	void SpawnSimulationWindow() noexcept;
};
