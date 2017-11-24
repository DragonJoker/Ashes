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
		static size_t constexpr FrameSamplesCount = 1000;
		renderer::RendererPtr m_renderer;
		wxWindowPtr< RenderPanel > m_panel;
		std::array< std::chrono::microseconds, FrameSamplesCount > m_framesTimes;
		uint32_t m_frameIndex{ 0 };
		size_t m_frameCount{ 0 };
	};
}
