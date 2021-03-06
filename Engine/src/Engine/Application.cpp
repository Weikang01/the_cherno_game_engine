#include "engine_pch.h"
#include "Application.h"

#include "Engine/Renderer/Renderer.h"
#include "Engine/Input.h"
#include "Engine/ImGui/ImGuiLayer.h"
#include <GLFW/glfw3.h>
#include <Engine/Core/Timestep.h>


namespace Engine
{
	Application* Application::s_instance = nullptr;

	Application::Application()
	{
		EG_CORE_ASSERT(!s_instance, "Application already exists!");
		s_instance = this;

		m_window = std::unique_ptr<Window>(Window::Create());
		m_window->SetEventCallback(ENGINE_BIND_EVENT_FN(Application::OnEvent));

		Renderer::Init();

		m_imGuiLayer = new ImGuiLayer();
		PushLayer(m_imGuiLayer);
	}

	Application::~Application()
	{
	}

	void Application::OnEvent(Event& evnt)
	{
		EventDispatcher dispatcher(evnt);
		dispatcher.Dispatch<WindowCloseEvent>(ENGINE_BIND_EVENT_FN(Application::OnWindowClose));
		dispatcher.Dispatch<WindowResizeEvent>(ENGINE_BIND_EVENT_FN(Application::OnWindowResize));

		for (auto it = m_layerStack.end(); it != m_layerStack.begin();)
		{
			(*--it)->OnEvent(evnt);
			if (evnt.m_Handled)
				break;
		}
	}

	void Application::PushLayer(Layer* layer)
	{
		m_layerStack.PushLayer(layer);
		layer->OnAttach();
	}

	void Application::PushOverlay(Layer* overlay)
	{
		m_layerStack.PushOverlay(overlay);
		overlay->OnAttach();
	}

	bool Application::OnWindowClose(WindowCloseEvent& evnt)
	{
		m_running = false;
		return true;
	}

	bool Application::OnWindowResize(WindowResizeEvent& evnt)
	{
		if (evnt.GetWidth() == 0 && evnt.GetHeight() == 0)
		{
			m_minimized = true;
			return false;
		}
		m_minimized = false;
		Renderer::OnWindowResize(evnt.GetWidth(), evnt.GetHeight());
		return false;
	}

	void Application::Run()
	{
		while (m_running)
		{
			float time = glfwGetTime();
			Timestep timestep = time - m_lastFrameTime;
			m_lastFrameTime = time;

			if (!m_minimized)
			{
				for (Layer* layer : m_layerStack)
					layer->OnUpdate(timestep);
			}
			m_imGuiLayer->begin();
			for (Layer* layer : m_layerStack)
				layer->OnImGuiRender();
			m_imGuiLayer->end();

			m_window->OnUpdate();
		}
	}
}