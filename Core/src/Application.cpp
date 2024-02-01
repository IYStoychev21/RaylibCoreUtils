#include "pch.h"
#include "Application.h"

namespace Core {
	Application::Application()
	{
		assert(!s_Instance, "Application already exists");

		s_Instance = this;
	}

	void Application::Init(glm::vec2 size, std::string title)
	{
		m_Window.reset(Window::CreateWindow(size.x, size.y, title));
		m_Window->Init();

#ifndef IMGUI_DISABLED
		rlImGuiSetup(true);
#endif
	}

	void Application::Run()
	{
#ifndef IMGUI_DISABLED
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
#endif
	
		for(auto layer : m_LayerStack)
		{ 
			layer->OnAttach();
		}

		while (m_IsRunning)
		{
			ShouldWindowClose();

#ifdef IMGUI_DISABLED
			BeginDrawing();
#endif

			for(auto layer : m_LayerStack)
			{ 
				layer->OnUpdate();
			}

#ifndef IMGUI_DISABLED
			BeginDrawing();

			rlImGuiBegin();

			ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());

			for(auto layer : m_LayerStack)
			{ 
				layer->OnImGuiRender();
			}

			rlImGuiEnd();
#endif

			EndDrawing();
		}

		for(auto layer : m_LayerStack)
		{ 
			layer->OnDetach();
		}
		
#ifndef IMGUI_DISABLED
		rlImGuiShutdown();
#endif
		
		CloseWindow();
	}

	void Application::ShouldWindowClose()
	{
		if (WindowShouldClose())
		{
			m_IsRunning = false;
		}
	}
}