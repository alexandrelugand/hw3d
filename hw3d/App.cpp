#include "stdafx.h"
#include "App.h"

#pragma comment(lib, "assimp-vc143-mt.lib")

App::App(const std::string& commandLine)
	: wnd(1280u, 1024u, "DirectX Engine"),
	  sc(String::TokenizeQuoted(commandLine))
{
	cameras.AddCamera(std::make_unique<Entities::Camera>(wnd.Gfx(), "A", XMFLOAT3{-34.5f, 18.0f, 3.0f}, PI / 180.0f * 17.0f, PI / 2.0f));
	cameras.AddCamera(std::make_unique<Entities::Camera>(wnd.Gfx(), "B", XMFLOAT3{-13.5f, 28.8f, -6.4f}, PI / 180.0f * 13.0f, PI / 180.0f * 61.0f));
	cameras.AddCamera(light.ShareCamera());
	cameras.LinkTechniques(rg);

	light.LinkTechniques(rg);
	cube.LinkTechniques(rg);
	cube2.LinkTechniques(rg);
	goblin.LinkTechniques(rg);
	sponza.LinkTechniques(rg);
	nano.LinkTechniques(rg);

	rg.BindShadowCamera(*light.ShareCamera());
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
		auto dt = timer.Mark() * speed_factor;
		HandleInput(dt);
		DoFrame(dt);
	}
}


void App::HandleInput(float dt)
{
	auto& gfx = wnd.Gfx();
	while (const auto e = wnd.kbd.ReadKey())
	{
		if (!e->IsPress())
			continue;

		switch (e->GetCode())
		{
		case VK_F11:
			{
				if (gfx.IsWireframeEnabled())
					gfx.DisableWireframe();
				else
					gfx.EnableWireframe();
				break;
			}
		case VK_F12:
			{
				if (gfx.IsImGuiEnabled())
					gfx.DisableImGui();
				else
					gfx.EnableImGui();
				break;
			}
		case VK_RETURN:
			{
				savingDepth = true;
				break;
			}
		default: ;
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
			cameras->Translate({0.0f, 0.0f, dt});
		}
		if (wnd.kbd.KeyIsPressed('S'))
		{
			cameras->Translate({0.0f, 0.0f, -dt});
		}
		if (wnd.kbd.KeyIsPressed('Q'))
		{
			cameras->Translate({-dt, 0.0f, 0.0f});
		}
		if (wnd.kbd.KeyIsPressed('D'))
		{
			cameras->Translate({dt, 0.0f, 0.0f});
		}
		if (wnd.kbd.KeyIsPressed(VK_SPACE))
		{
			cameras->Translate({0.0f, dt, 0.0f});
		}
		if (wnd.kbd.KeyIsPressed(VK_CONTROL))
		{
			cameras->Translate({0.0f, -dt, 0.0f});
		}
	}

	if (wnd.mouse.RightIsPressed()) // NOLINT(bugprone-branch-clone)
	{
		wnd.DisableCursor();
		wnd.mouse.EnableRaw();
	}
	else
	{
		wnd.EnableCursor();
		wnd.mouse.DisableRaw();
	}

	while (const auto delta = wnd.mouse.ReadRawDelta())
	{
		if (!wnd.CursorEnabled())
		{
			cameras->Rotate(static_cast<float>(delta->x), static_cast<float>(delta->y));
		}
	}
}

void App::DoFrame(float dt)
{
	auto& gfx = wnd.Gfx();

	gfx.BeginFrame(0.07f, 0.0f, 0.12f);

	light.Bind(gfx, cameras->GetMatrix());
	light.Submit(Chan::main);

	rg.BindMainCamera(cameras.GetActiveCamera());
	cameras.Submit(Chan::main);

	for (const auto c : Chan::Channels)
	{
		sponza.Submit(c);
		cube.Submit(c);
		cube2.Submit(c);
		goblin.Submit(c);
		nano.Submit(c);
	}

	rg.Execute(gfx);

	if (savingDepth)
	{
		rg.DumpShadowMap(gfx, "shadow.png");
		savingDepth = false;
	}

	if (gfx.IsImGuiEnabled())
	{
		//Camera / light windows
		SpawnSimulationWindow();
		cameras.SpawnWindow(gfx);
		light.SpawnControlWindow();

		drawableProbe.SpawnControl(cube);
		drawableProbe.SpawnControl(cube2);
		modelProbe.SpawnWindow(goblin);
		modelProbe.SpawnWindow(sponza);
		modelProbe.SpawnWindow(nano);

		rg.RenderWindows(gfx);
	}

	// Present
	gfx.EndFrame();
	rg.Reset();
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
