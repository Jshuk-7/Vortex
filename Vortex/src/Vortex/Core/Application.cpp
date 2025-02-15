#include "vxpch.h"
#include "Application.h"

#include "Vortex/Input/Input.h"

#include "Vortex/Events/KeyEvent.h"

#include "Vortex/Audio/AudioSystem.h"

#include "Vortex/Renderer/Renderer.h"
#include "Vortex/Renderer/Font/Font.h"
#include "Vortex/Renderer/ParticleSystem/ParticleSystem.h"

#include "Vortex/Physics/3D/Physics.h"
#include "Vortex/Physics/2D/Physics2D.h"

#include "Vortex/Network/Networking.h"

#include "Vortex/UI/UISystem.h"

#include "Vortex/System/SystemManager.h"

#include "Vortex/Utils/FileSystem.h"
#include "Vortex/Utils/Random.h"
#include "Vortex/Utils/Time.h"

extern bool g_ApplicationRunning;

namespace Vortex {

	Application::Application(const ApplicationProperties& props)
		: m_Properties(props)
	{
		VX_PROFILE_FUNCTION();

		VX_CORE_ASSERT(!s_Instance, "Application already exists!");
		s_Instance = this;

		if (!m_Properties.WorkingDirectory.empty())
		{
			FileSystem::SetWorkingDirectory(m_Properties.WorkingDirectory);
		}
		else
		{
			m_Properties.WorkingDirectory = FileSystem::GetWorkingDirectory().string();
		}

		WindowProperties windowProps;
		windowProps.Size.x = m_Properties.WindowWidth;
		windowProps.Size.y = m_Properties.WindowHeight;
		windowProps.Title = m_Properties.Name;
		windowProps.Maximized = m_Properties.MaximizeWindow;
		windowProps.Decorated = m_Properties.WindowDecorated;
		windowProps.Resizeable = m_Properties.WindowResizable;
		windowProps.VSync = m_Properties.VSync;

		// We need to set the app's graphics api before we create the window
		Renderer::SetGraphicsAPI(m_Properties.GraphicsAPI);

		m_Window = Window::Create(windowProps);

		if (!m_Properties.MaximizeWindow)
		{
			m_Window->CenterWindow();
		}

		m_Window->SetEventCallback(VX_BIND_CALLBACK(Application::OnEvent));

		Networking::Init();
		Renderer::Init();
		SystemManager::RegisterSystem<UISystem>();
		SystemManager::RegisterAssetSystem<ParticleSystem>();
		Physics::Init();
		SystemManager::RegisterAssetSystem<AudioSystem>();
		Random::Init();
		Font::Init();
		Input::Init();

		if (m_Properties.EnableGUI)
		{
			m_GuiLayer = new GuiLayer();
			PushOverlay(m_GuiLayer);
		}

		m_Running = true;
	}

	Application::~Application()
	{
		VX_PROFILE_FUNCTION();

		// Note:
		// We have to explicitly destroy the layer stack before
		// the engine's submodules are shutdown. This is mainly because
		// the Audio system needs to outlive the lifetime of the layer stack.
		// It is also just a good idea to destroy all the layers before submodules
		// get shutdown because something in the layers code may rely on a submodule
		m_Layers.~LayerStack();

		Input::Shutdown();
		SystemManager::UnRegisterSystem<UISystem>();
		SystemManager::UnRegisterAssetSystem<AudioSystem>();
		Physics::Shutdown();
		Font::Shutdown();
		SystemManager::UnRegisterAssetSystem<ParticleSystem>();
		Renderer::Shutdown();
		Networking::Shutdown();
	}

	void Application::PushLayer(Layer* layer)
	{
		VX_PROFILE_FUNCTION();

		layer->OnAttach();
		m_Layers.PushLayer(layer);
	}

	void Application::PushOverlay(Layer* overlay)
	{
		VX_PROFILE_FUNCTION();

		overlay->OnAttach();
		m_Layers.PushOverlay(overlay);
	}

	void Application::Close()
	{
		m_Running = false;
		g_ApplicationRunning = false;
	}

	void Application::OnEvent(Event& e)
	{
		VX_PROFILE_FUNCTION();

		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(VX_BIND_CALLBACK(Application::OnWindowCloseEvent));
		dispatcher.Dispatch<WindowResizeEvent>(VX_BIND_CALLBACK(Application::OnWindowResizeEvent));

		Input::OnEvent(e);

		for (auto it = m_Layers.rbegin(); it != m_Layers.rend(); ++it)
		{
			if (e.Handled)
				break;

			(*it)->OnEvent(e);
		}
	}

	void Application::AddModule(const SubModule& submodule)
	{
		m_ModuleLibrary.Add(submodule);
	}

	void Application::RemoveModule(const SubModule& submodule)
	{
		const std::string& moduleName = submodule.GetName();
		m_ModuleLibrary.Remove(moduleName);
	}

	const ModuleLibrary& Application::GetModuleLibrary() const
	{
		return m_ModuleLibrary;
	}

	i32 Application::Run()
	{
		VX_PROFILE_FUNCTION();

		i32 exitCode = 0;

		while (m_Running)
		{
			VX_PROFILE_SCOPE("Application Loop");

			m_FrameTime.Clear();

			const float currentTime = Time::GetTime();
			m_FrameTime.DeltaTime = currentTime - m_LastFrameTimeStamp;
			Time::SetDeltaTime(m_FrameTime.DeltaTime);
			m_LastFrameTimeStamp = currentTime;

			if (!m_MainThreadPreUpdateFunctionQueue.empty())
			{
				m_MainThreadPreUpdateFunctionQueue.execute();
				m_MainThreadPreUpdateFunctionQueue.clear();
			}

			if (!m_ApplicationMinimized)
			{
				VX_PROFILE_SCOPE("LayerStack OnUpdate");

				for (Layer* layer : m_Layers)
				{
					layer->OnUpdate(m_FrameTime.DeltaTime);
				}
			}

			if (m_Properties.EnableGUI)
			{
				m_GuiLayer->BeginFrame();

				{
					VX_PROFILE_SCOPE("LayerStack OnGuiRender");

					for (Layer* layer : m_Layers)
					{
						layer->OnGuiRender();
					}
				}

				m_GuiLayer->EndFrame();
			}

			Input::ResetChangesForNextFrame();

			m_Window->OnUpdate();

			if (!m_MainThreadPostUpdateFunctionQueue.empty())
			{
				m_MainThreadPostUpdateFunctionQueue.execute();
				m_MainThreadPostUpdateFunctionQueue.clear();
			}
		}

		return exitCode;
	}

	bool Application::OnWindowCloseEvent(WindowCloseEvent& e)
	{
		Close();

		return false;
	}

	bool Application::OnWindowResizeEvent(WindowResizeEvent& e)
	{
		VX_PROFILE_FUNCTION();

		const bool zeroWidth = e.GetWidth() == 0;
		const bool zeroHeight = e.GetHeight() == 0;
		
		m_ApplicationMinimized = zeroWidth || zeroHeight;
		if (m_ApplicationMinimized)
		{
			return false;
		}

		Viewport viewport;
		viewport.TopLeftXPos = 0;
		viewport.TopLeftYPos = 0;
		viewport.Width = e.GetWidth();
		viewport.Height = e.GetHeight();

		Renderer::OnWindowResize(viewport);

		return false;
	}

}
