#include "MainFrame.hpp"
#include "Application.hpp"

#include <wx/sizer.h>

#include <numeric>

namespace common
{
	MainFrame::MainFrame( wxString const & name
		, wxString const & rendererName
		, utils::InstanceFactory & factory )
		: wxFrame{ nullptr
		, wxID_ANY
		, name + wxT( " (" ) + rendererName + wxT( ")" )
		, wxDefaultPosition
		, WindowSize
		, wxSYSTEM_MENU | wxCAPTION | wxCLOSE_BOX | wxCLIP_CHILDREN | wxRESIZE_BORDER | wxMAXIMIZE_BOX }
		, m_name{ name }
		, m_rendererName{ rendererName }
		, m_factory{ factory }
	{
	}

	void MainFrame::initialise()
	{
		SetClientSize( WindowSize );
		Show( true );

		try
		{
			ashes::ApplicationInfo config
			{
				m_name.ToStdString(),
				ashes::makeVersion( 1, 0, 0 ),
				"Ashes",
				ashes::makeVersion( 1, 0, 0 ),
				ashes::API_VERSION_1_0,
			};
			m_instance = std::make_unique< utils::Instance >( m_factory
				, m_rendererName.ToStdString()
				, config );

			std::cout << "Instance instance created." << std::endl;
			m_panel = doCreatePanel( WindowSize, *m_instance );

			wxBoxSizer * sizer{ new wxBoxSizer{ wxVERTICAL } };
			sizer->Add( m_panel, wxSizerFlags{ 1 }.Expand() );
			sizer->SetSizeHints( this );
			SetSizer( sizer );

			std::cout << "Main frame initialised successfully." << std::endl;
		}
		catch ( std::exception & p_exc )
		{
			wxMessageBox( p_exc.what()
				, wxMessageBoxCaptionStr
				, wxICON_ERROR );
		}
	}

	void MainFrame::cleanup()
	{
		if ( m_panel )
		{
			m_panel->Destroy();
		}

		m_instance.reset();
	}

	void MainFrame::updateFps( std::chrono::microseconds const & durationGpu
		, std::chrono::microseconds const & durationCpu )
	{
		++m_frameCount;
		m_gpuFramesTimes[m_frameIndex] = durationGpu;
		m_cpuFramesTimes[m_frameIndex] = durationCpu;
		m_frameIndex = ++m_frameIndex % FrameSamplesCount;
		auto count = std::min( m_frameCount, m_cpuFramesTimes.size() );

		std::stringstream title;
#ifndef NDEBUG
		title << " Debug";
#endif
		auto averageCpuTime = std::accumulate( m_cpuFramesTimes.begin()
			, m_cpuFramesTimes.begin() + count
			, std::chrono::microseconds{ 0 } ).count() / float( count );
		auto cpuMs = durationCpu.count() / 1000.0f;
		auto cpuAvgms = averageCpuTime / 1000.0f;
		title << " [CPU: I " << std::setw( 6 ) << std::setprecision( 4 ) << cpuMs << "ms";
		title << " (" << std::setw( 5 ) << int( 1000.0f / cpuMs ) << "fps)";
		title << ", A " << std::setw( 6 ) << std::setprecision( 4 ) << cpuAvgms << "ms";
		title << " (" << std::setw( 5 ) << int( 1000.0f / cpuAvgms ) << "fps)";

		auto averageGpuTime = std::accumulate( m_gpuFramesTimes.begin()
			, m_gpuFramesTimes.begin() + count
			, std::chrono::microseconds{ 0 } ).count() / float( count );
		auto gpuMs = durationGpu.count() / 1000.0f;
		auto gpuAvgms = averageGpuTime / 1000.0f;
		title << " [GPU: I " << std::setw( 6 ) << std::setprecision( 4 ) << gpuMs << "ms";
		title << ", A " << std::setw( 6 ) << std::setprecision( 4 ) << gpuAvgms << "ms";

		SetTitle( m_name + wxT( " (" ) + m_rendererName + wxT( ")" ) + wxString( title.str() ) );
	}

	void MainFrame::updateFps( std::chrono::microseconds const & duration )
	{
		++m_frameCount;
		m_cpuFramesTimes[m_frameIndex] = duration;
		auto count = std::min( m_frameCount, m_cpuFramesTimes.size() );
		auto averageTime = std::accumulate( m_cpuFramesTimes.begin()
			, m_cpuFramesTimes.begin() + count
			, std::chrono::microseconds{ 0 } ).count() / float( count );
		m_frameIndex = ++m_frameIndex % FrameSamplesCount;
		std::stringstream title;
		auto ms = duration.count() / 1000.0f;
		auto avgms = averageTime / 1000.0f;
#ifndef NDEBUG
		title << " - Debug";
#endif
		title << " - (Inst) " << std::setw( 6 ) << std::setprecision( 4 ) << ms << " ms";
		title << " - " << std::setw( 5 ) << int( 1000.0f / ms ) << " fps";
		title << " - (Avg) " << std::setw( 6 ) << std::setprecision( 4 ) << avgms << " ms";
		title << " - " << std::setw( 5 ) << int( 1000.0f / avgms ) << " fps";
		SetTitle( m_name + wxT( " (" ) + m_rendererName + wxT( ")" ) + wxString( title.str() ) );
	}

	wxBEGIN_EVENT_TABLE( MainFrame, wxFrame )
		EVT_CLOSE( MainFrame::OnClose )
	wxEND_EVENT_TABLE()

	void MainFrame::OnClose( wxCloseEvent & event )
	{
		cleanup();
		event.Skip( true );
	}
}
