#include "stdafx.h"
#include "App.h"

#pragma comment(lib, "assimp-vc143-mt.lib")

App::App(const std::string& commandLine)
	: wnd(1280u, 1024u, "DirectX Engine"),
	  scriptCommander(String::TokenizeQuoted(commandLine)),
	  light(wnd.Gfx())
{
	wnd.Gfx().SetProjection(XMMatrixPerspectiveLH(1.0f, 3.0f / 4.0f, 0.5f, 400.0f));
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

	//nano.Draw(gfx);
	//wall.Draw(gfx);
	//gobber.Draw(gfx);
	sponza.Draw(gfx);
	box.Draw(gfx);
	box.DrawOutline(gfx);


	light.Draw(gfx);

	while (const auto e = wnd.kbd.ReadKey())
	{
		if (!e->IsPress())
			continue;

		if (e->IsPress() && e->GetCode() == VK_F1)
		{
			if (wnd.CursorEnabled()) // NOLINT(bugprone-branch-clone)
			{
				wnd.DisableCursor();
				wnd.mouse.EnableRaw();
			}
			else
			{
				wnd.EnableCursor();
				wnd.mouse.DisableRaw();
			}
		}

		if (e->IsPress() && e->GetCode() == VK_F11)
		{
			if (gfx.IsWireframeEnabled())
			{
				gfx.DisableWireframe();
			}
			else
			{
				gfx.EnableWireframe();
			}
		}

		if (e->IsPress() && e->GetCode() == VK_F12)
		{
			if (gfx.IsImGuiEnabled())
			{
				gfx.DisableImGui();
			}
			else
			{
				gfx.EnableImGui();
			}
		}
	}

	if (!wnd.CursorEnabled())
	{
		if (wnd.kbd.KeyIsPressed(VK_SHIFT))
		{
			dt *= 4;
		}
		if (wnd.kbd.KeyIsPressed('Z'))
		{
			camera.Translate({0.0f, 0.0f, dt});
		}
		if (wnd.kbd.KeyIsPressed('S'))
		{
			camera.Translate({0.0f, 0.0f, -dt});
		}
		if (wnd.kbd.KeyIsPressed('Q'))
		{
			camera.Translate({-dt, 0.0f, 0.0f});
		}
		if (wnd.kbd.KeyIsPressed('D'))
		{
			camera.Translate({dt, 0.0f, 0.0f});
		}
		if (wnd.kbd.KeyIsPressed(VK_SPACE))
		{
			camera.Translate({0.0f, dt, 0.0f});
		}
		if (wnd.kbd.KeyIsPressed(VK_CONTROL))
		{
			camera.Translate({0.0f, -dt, 0.0f});
		}
	}

	while (const auto delta = wnd.mouse.ReadRawDelta())
	{
		if (!wnd.CursorEnabled())
		{
			camera.Rotate(static_cast<float>(delta->x), static_cast<float>(delta->y));
		}
	}
	if (gfx.IsImGuiEnabled())
	{
		//Camera / light windows
		SpawnSimulationWindow();
		camera.SpawnControlWindow();
		light.SpawnControlWindow();
		//nano.ShowWindow(gfx, "Nanosuit");
		//wall.ShowWindow(gfx, "Wall");
		//gobber.ShowWindow(gfx, "Goblin");
		box.SpawnControlWindow();
		sponza.ShowWindow(wnd.Gfx(), "Sponza");
	}

	// Present
	gfx.EndFrame();
}

void App::SpawnSimulationWindow() noexcept
{
	if (ImGui::Begin("Simulation"))
	{
		ImGui::SliderFloat("Speed Factor", &speed_factor, 0.0f, 6.0f, "%.4f");
		ImGui::Text("%.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		ImGui::Text("Status: %s", speed_factor == 0.0f ? "PAUSED" : "RUNNING");
		ImGui::Text("Cursor: %s", wnd.CursorEnabled() ? "ENABLED" : "DISABLED");
		ImGui::Text("Wireframe: %s", wnd.Gfx().IsWireframeEnabled() ? "ENABLED" : "DISABLED");

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
