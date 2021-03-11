#pragma once

#include "Prerequisites.hpp"

#include <util/UtilsInstance.hpp>

#include <wx/frame.h>

namespace common
{
	class MainFrame
		: public wxFrame
	{
	public:
		MainFrame( wxString const & name
			, wxString const & rendererName
			, ashes::RendererList const & renderers
			, wxSize const & size = WindowSize );
		virtual ~MainFrame() = default;

		void initialise();
		void cleanup();
		void updateFps( std::chrono::microseconds const & durationGpu
			, std::chrono::microseconds const & durationCpu );
		void updateFps( std::chrono::microseconds const & duration );

	private:
		virtual wxPanel * doCreatePanel( wxSize const & size, utils::Instance const & instance ) = 0;

		wxDECLARE_EVENT_TABLE();
		void OnClose( wxCloseEvent & event );
		void OnKeyUp( wxKeyEvent & event );

	private:
		static size_t constexpr FrameSamplesCount = 100;
		std::string m_name;
		wxString m_rendererName;
		utils::InstancePtr m_instance;
		ashes::RendererList const & m_renderers;
		wxPanel * m_panel{ nullptr };
		std::array< std::chrono::microseconds, FrameSamplesCount > m_cpuFramesTimes;
		std::array< std::chrono::microseconds, FrameSamplesCount > m_gpuFramesTimes;
		uint32_t m_frameIndex{ 0 };
		size_t m_frameCount{ 0 };
		wxSize m_defaultSize;
	};
}
