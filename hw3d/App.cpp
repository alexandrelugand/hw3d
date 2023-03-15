#include "stdafx.h"
#include "App.h"

#pragma comment(lib, "assimp-vc143-mt.lib")

App::App()
	: wnd(1280u, 1024u, "DirectX Engine"), light(wnd.Gfx())
{
	wnd.Gfx().SetProjection(XMMatrixPerspectiveLH(1.0f, 3.0f / 4.0f, 0.5f, 40.0f));
}

int App::Go()
{
	while (true)
	{
		if (wnd.kbd.KeyIsPressed(VK_ESCAPE))
			PostQuitMessage(0);

		// process all messages pending, but to not block for new messages
		if (const auto exitCode = Windowing::Window::ProcessMessages())
		{
			// if return optional has value, means we're quitting so return exit code
			return *exitCode;
		}

		// execute the game logic
		DoFrame();
	}
}

void App::DoFrame()
{
	auto dt = timer.Mark() * speed_factor;
	auto& gfx = wnd.Gfx();

	gfx.BeginFrame(0.07f, 0.0f, 0.12f);
	gfx.SetCamera(camera.GetMatrix());
	light.Bind(gfx, gfx.GetCamera());

	nano.Draw(gfx);
	light.Draw(gfx);

	if (gfx.IsImGuiEnabled())
	{
		//Camera / light windows
		SpawnSimulationWindow();
		camera.SpawnControlWindow();
		light.SpawnControlWindow();
		nano.ShowWindow();
	}

	// Present
	gfx.EndFrame();
}

void App::SpawnSimulationWindow() noexcept
{
	if (ImGui::Begin("Simulation Speed"))
	{
		ImGui::SliderFloat("Speed Factor", &speed_factor, 0.0f, 6.0f, "%.4f");
		ImGui::Text("%.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		ImGui::Text("Status: %s", speed_factor == 0.0f ? "PAUSED" : "RUNNING");
		if (ImGui::Button("Reset"))
			speed_factor = 1.0f;

		if (speed_factor == 0.0f)
		{
			if (ImGui::Button("Resume"))
				speed_factor = 1.0f;
		}
		else
		{
			if (ImGui::Button("Pause"))
				speed_factor = 0.0f;
		}
	}
	ImGui::End();
}
