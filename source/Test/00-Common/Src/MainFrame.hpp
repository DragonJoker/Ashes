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
		virtual ~MainFrame() = default;

		void initialise();
		void cleanup();
		void updateFps( std::chrono::microseconds const & durationGpu
			, std::chrono::microseconds const & durationCpu );
		void updateFps( std::chrono::microseconds const & duration );

	private:
		virtual wxPanel * doCreatePanel( wxSize const & size, ashes::Renderer const & renderer ) = 0;

		wxDECLARE_EVENT_TABLE();
		void OnClose( wxCloseEvent & event );

	private:
		static size_t constexpr FrameSamplesCount = 1000;
		wxString m_name;
		wxString m_rendererName;
		ashes::RendererPtr m_renderer;
		RendererFactory & m_factory;
		wxPanel * m_panel{ nullptr };
		std::array< std::chrono::microseconds, FrameSamplesCount > m_cpuFramesTimes;
		std::array< std::chrono::microseconds, FrameSamplesCount > m_gpuFramesTimes;
		uint32_t m_frameIndex{ 0 };
		size_t m_frameCount{ 0 };
	};
}
