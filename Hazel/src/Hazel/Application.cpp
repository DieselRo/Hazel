//Application.cpp

#include "Application.h"

#include "Hazel/Events/ApplicationEvent.h"
#include "Hazel/Log.h"

namespace Hazel {

	Application::Application()
	{
	}


	Application::~Application()
	{
	}

	void Application::Run()
	{
		WindowResizeEvent e(1280, 720);
		if (e.IsInCategory(EventCategoryApplication))
		{
			::Hazel::Log::GetClientLogger()->trace("{0}", e);
		}
		if (e.IsInCategory(EventCategoryInput))
		{
			::Hazel::Log::GetClientLogger()->trace("{0}", e);
		}

		while (true);
	}

}