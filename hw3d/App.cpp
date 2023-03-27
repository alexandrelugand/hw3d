#include "stdafx.h"
#include "App.h"

#pragma comment(lib, "assimp-vc143-mt.lib")

App::App(const std::string& commandLine)
	: wnd(1280u, 1024u, "DirectX Engine"),
	  sc(String::TokenizeQuoted(commandLine)),
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
	light.Bind(wnd.Gfx(), camera.GetMatrix());
	light.Submit(fc);

	goblin.Submit(fc);
	sponza.Submit(fc);
	box.Submit(fc);
	box2.Submit(fc);
	box3.Submit(fc);
	asset.Submit(fc);
	cylinder.Submit(fc);
	pyramid.Submit(fc);
	sheet.Submit(fc);

	fc.Execute(gfx);

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

	// Mesh techniques window
	class Tp : public TechniqueProbe
	{
	public:
		void OnSetTechnique() override
		{
			ImGui::TextColored({0.4f, 1.0f, 0.6f, 1.0f}, pTech->GetName().c_str());
			bool active = pTech->IsActive();
			ImGui::Checkbox(("Tech Active##"s + std::to_string(techIdx)).c_str(), &active);
			pTech->SetActive(active);
		}

		bool OnVisitBuffer(Dcb::Buffer& buf) override
		{
			bool dirty = false;
			const auto dcheck = [&dirty](bool changed) { dirty = dirty || changed; };
			auto tag = [tagScratch = std::string{}, tagString = "##" + std::to_string(bufIdx)]
			(const char* label) mutable
			{
				tagScratch = label + tagString;
				return tagScratch.c_str();
			};

			if (const auto v = buf["scale"]; v.Exists())
			{
				dcheck(ImGui::SliderFloat(tag("Scale"), &v, 1.0f, 2.0f, "%.3f"));
			}
			if (const auto v = buf["offset"]; v.Exists())
			{
				dcheck(ImGui::SliderFloat(tag("offset"), &v, 0.0f, 1.0f, "%.3f"));
			}
			if (auto v = buf["materialColor"]; v.Exists())
			{
				dcheck(ImGui::ColorPicker3(tag("Color"), reinterpret_cast<float*>(&static_cast<XMFLOAT3&>(v))));
			}
			if (auto v = buf["specularColor"]; v.Exists())
			{
				dcheck(ImGui::ColorPicker3(tag("Spec. Color"), reinterpret_cast<float*>(&static_cast<XMFLOAT3&>(v))));
			}
			if (const auto v = buf["specularGloss"]; v.Exists())
			{
				dcheck(ImGui::SliderFloat(tag("Glossiness"), &v, 1.0f, 100.0f, "%.1f"));
			}
			if (const auto v = buf["specularWeight"]; v.Exists())
			{
				dcheck(ImGui::SliderFloat(tag("Spec. Weight"), &v, 0.0f, 2.0f));
			}
			if (const auto v = buf["useSpecularMap"]; v.Exists())
			{
				dcheck(ImGui::Checkbox(tag("Spec. Map Enable"), &v));
			}
			if (const auto v = buf["useNormalMap"]; v.Exists())
			{
				dcheck(ImGui::Checkbox(tag("Normal Map Enable"), &v));
			}
			if (const auto v = buf["normalMapWeight"]; v.Exists())
			{
				dcheck(ImGui::SliderFloat(tag("Normal Map Weight"), &v, 0.0f, 2.0f));
			}
			return dirty;
		}
	};

	class Mp : ModelProbe
	{
	public:
		void SpawnWindow(Entities::Model& model)
		{
			ImGui::Begin((model.GetName() + "##"s + std::to_string(model.GetId())).c_str());
			ImGui::Columns(2, nullptr, true);
			model.Accept(*this);

			ImGui::NextColumn();
			if (pSelectedNode != nullptr)
			{
				bool dirty = false;
				const auto dcheck = [&dirty](bool changed) { dirty = dirty || changed; };
				auto& tf = ResolveTransform();

				ImGui::TextColored({0.4f, 1.0f, 0.6f, 1.0f}, "Translation");
				dcheck(ImGui::SliderFloat("X", &tf.x, -60.f, 60.f));
				dcheck(ImGui::SliderFloat("Y", &tf.y, -60.f, 60.f));
				dcheck(ImGui::SliderFloat("Z", &tf.z, -60.f, 60.f));
				ImGui::TextColored({0.4f, 1.0f, 0.6f, 1.0f}, "Orientation");
				dcheck(ImGui::SliderAngle("X-rot", &tf.xRot, -180.0f, 180.0f));
				dcheck(ImGui::SliderAngle("Y-rot", &tf.yRot, -180.0f, 180.0f));
				dcheck(ImGui::SliderAngle("Z-rot", &tf.zRot, -180.0f, 180.0f));
				if (dirty)
				{
					pSelectedNode->SetAppliedTransform(
						XMMatrixRotationX(tf.xRot) *
						XMMatrixRotationY(tf.yRot) *
						XMMatrixRotationZ(tf.zRot) *
						XMMatrixTranslation(tf.x, tf.y, tf.z)
					);
				}
			}
			if (pSelectedNode != nullptr)
			{
				Tp probe;
				pSelectedNode->Accept(probe);
			}
			ImGui::End();
		}

	protected:
		bool PushNode(Entities::Node& node) override
		{
			// if there is no selected node, set selectedId to an impossible value
			const int selectedId = (pSelectedNode == nullptr) ? -1 : pSelectedNode->GetId();
			// build up flags for current node
			const auto node_flags = ImGuiTreeNodeFlags_OpenOnArrow
				| ((node.GetId() == selectedId) ? ImGuiTreeNodeFlags_Selected : 0)
				| (node.HasChildren() ? 0 : ImGuiTreeNodeFlags_Leaf);
			// render this node
			const auto expanded = ImGui::TreeNodeEx(
				reinterpret_cast<void*>(static_cast<intptr_t>(node.GetId())),
				node_flags, node.GetName().c_str()
			);
			// processing for selecting node
			if (ImGui::IsItemClicked())
			{
				// used to change the highlighted node on selection change
				struct Probe : public TechniqueProbe
				{
					void OnSetTechnique() override
					{
						if (pTech->GetName() == "Outline")
						{
							pTech->SetActive(highlighted);
						}
					}

					bool highlighted = false;
				} probe;

				// remove highlight on prev-selected node
				if (pSelectedNode != nullptr)
				{
					pSelectedNode->Accept(probe);
				}
				// add highlight to newly-selected node
				probe.highlighted = true;
				node.Accept(probe);

				pSelectedNode = &node;
			}
			// signal if children should also be recursed
			return expanded;
		}

		void PopNode(Entities::Node& node) override
		{
			ImGui::TreePop();
		}

	private:
		Entities::Node* pSelectedNode = nullptr;

		struct TransformParameters
		{
			float xRot = 0.0f;
			float yRot = 0.0f;
			float zRot = 0.0f;
			float x = 0.0f;
			float y = 0.0f;
			float z = 0.0f;
		};

		std::unordered_map<int, TransformParameters> transformParams{};

		TransformParameters& ResolveTransform() noexcept
		{
			const auto id = pSelectedNode->GetId();
			auto i = transformParams.find(id);
			if (i == transformParams.end())
			{
				return LoadTransform(id);
			}
			return i->second;
		}

		TransformParameters& LoadTransform(int id) noexcept
		{
			const auto& applied = pSelectedNode->GetAppliedTransform();
			const auto angles = ExtractEulerAngles(applied);
			const auto translation = ExtractTranslation(applied);
			TransformParameters tp;
			tp.zRot = angles.z;
			tp.xRot = angles.x;
			tp.yRot = angles.y;
			tp.x = translation.x;
			tp.y = translation.y;
			tp.z = translation.z;
			return transformParams.insert({id, {tp}}).first->second;
		}
	};
	static Mp modelProbe;

	if (gfx.IsImGuiEnabled())
	{
		//Camera / light windows
		SpawnSimulationWindow();
		camera.SpawnControlWindow();
		light.SpawnControlWindow();
		//nano.ShowWindow(gfx, "Nanosuit");
		//wall.ShowWindow(gfx, "Wall");
		//gobber.ShowWindow(gfx, "Goblin");
		//sponza.ShowWindow(wnd.Gfx(), "Sponza");
		box.SpawnControlWindow();
		box2.SpawnControlWindow();
		box3.SpawnControlWindow();
		asset.SpawnControlWindow();
		cylinder.SpawnControlWindow();
		pyramid.SpawnControlWindow();
		sheet.SpawnControlWindow();

		modelProbe.SpawnWindow(goblin);
		modelProbe.SpawnWindow(sponza);

		fc.ShowWindows(wnd.Gfx());
	}

	// Present
	gfx.EndFrame();
	fc.Reset();
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
