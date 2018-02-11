#pragma once

#include "Prerequisites.hpp"

#include <Core/Renderer.hpp>

#include <wx/frame.h>

namespace common
{
	class MainFrame
		: public wxFrame
	{
	public:
		MainFrame( wxString const & name
			, wxString const & rendererName
			, RendererFactory & factory );
		virtual ~MainFrame();

		void initialise();
		void cleanup();
		void updateFps( std::chrono::microseconds const & durationGpu
			, std::chrono::microseconds const & durationCpu );
		void updateFps( std::chrono::microseconds const & duration );

	private:
		virtual RenderPanel * doCreatePanel( wxSize const & size, renderer::Renderer const & renderer ) = 0;

		wxDECLARE_EVENT_TABLE();
		void onClose( wxCloseEvent & event );
		void onTimer( wxTimerEvent & event );
		void onSize( wxSizeEvent & event );

	private:
		static size_t constexpr FrameSamplesCount = 1000;
		wxTimer * m_timer{ nullptr };
		wxString m_name;
		wxString m_rendererName;
		renderer::RendererPtr m_renderer;
		RendererFactory & m_factory;
		RenderPanel * m_panel{ nullptr };
		std::array< std::chrono::microseconds, FrameSamplesCount > m_cpuFramesTimes;
		std::array< std::chrono::microseconds, FrameSamplesCount > m_gpuFramesTimes;
		uint32_t m_frameIndex{ 0 };
		size_t m_frameCount{ 0 };
	};
}
