#pragma once

class App
{
public:
	App();

	//Master frame / message loop
	int Go();

private:
	Window wnd;
	Hw3DTimer timer;
	float speed_factor = 1.0f;
	static constexpr size_t nDrawables = 180;

	std::vector<std::unique_ptr<class Drawable>> drawables;
	std::vector<class Box*> boxes;

	Camera camera;
	PointLight light;

	std::optional<int> comboBoxIndex;
	std::set<int> boxControlIds;

	void DoFrame();
	void SpawnSimulationWindow() noexcept;
	void SpawnBoxWindowManagerWindow() noexcept;
	void SpawnBoxWindows() noexcept;
};
