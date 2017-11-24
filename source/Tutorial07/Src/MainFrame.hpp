#pragma once

#include "Prerequisites.hpp"

#include <Renderer/Renderer.hpp>

#include <wx/frame.h>

namespace vkapp
{
	class MainFrame
		: public wxFrame
	{
	public:
		MainFrame();

		void updateFps( std::chrono::microseconds const & duration );

	private:
		renderer::RendererPtr m_renderer;
		wxWindowPtr< RenderPanel > m_panel;
	};
}
