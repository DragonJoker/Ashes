#include "MainFrame.hpp"
#include "Application.hpp"

#include <wx/sizer.h>

#ifdef min
#	undef min
#	undef max
#	undef mabs
#endif

#include <algorithm>
#include <numeric>

namespace common
{
	MainFrame::MainFrame( wxString const & name
		, wxString const & rendererName
		, ashes::RendererList const & renderers )
		: wxFrame{ nullptr
		, wxID_ANY
		, name + wxT( " (" ) + rendererName + wxT( ")" )
		, wxDefaultPosition
		, WindowSize
		, wxSYSTEM_MENU | wxCAPTION | wxCLOSE_BOX | wxCLIP_CHILDREN | wxRESIZE_BORDER | wxMAXIMIZE_BOX }
		, m_name{ name.ToStdString() }
		, m_rendererName{ rendererName }
		, m_renderers{ renderers }
	{
	}

	void MainFrame::initialise()
	{
		SetClientSize( WindowSize );
		SetMinClientSize( WindowSize );
		Show( true );

		try
		{
			m_instance = std::make_unique< utils::Instance >( m_renderers
				, m_rendererName.ToStdString()
				, ashes::ApplicationInfo
				{
					m_name.c_str(),
					ashes::makeVersion( 1, 0, 0 ),
					"Ashes",
					ashes::makeVersion( 1, 0, 0 ),
					VK_API_VERSION_1_0,
				} );

			std::cout << "Instance created." << std::endl;
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
		auto cpuAvgMs = averageCpuTime / 1000.0f;
		auto cpuMs = durationCpu.count() / 1000.0f;
		title << " [CPU: I " << std::setw( 6 ) << std::setprecision( 4 ) << cpuMs << " ms";
		title << ", A " << std::setw( 6 ) << std::setprecision( 4 ) << cpuAvgMs << " ms";
		title << " (" << std::setw( 5 ) << int( 1000.0f / cpuAvgMs ) << " fps)";

		auto averageGpuTime = std::accumulate( m_gpuFramesTimes.begin()
			, m_gpuFramesTimes.begin() + count
			, std::chrono::microseconds{ 0 } ).count() / float( count );
		auto gpuAvgMs = averageGpuTime / 1000.0f;
		auto gpuMs = durationGpu.count() / 1000.0f;
		title << " [GPU: I " << std::setw( 6 ) << std::setprecision( 4 ) << gpuMs << " ms";
		title << ", A " << std::setw( 6 ) << std::setprecision( 4 ) << gpuAvgMs << " ms";
		title << " (" << std::setw( 5 ) << int( 1000.0f / gpuAvgMs ) << " fps)";

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
		EVT_KEY_UP( MainFrame::OnKeyUp )
	wxEND_EVENT_TABLE()

	void MainFrame::OnClose( wxCloseEvent & event )
	{
		cleanup();
		event.Skip( true );
	}

	void MainFrame::OnKeyUp( wxKeyEvent & event )
	{
		if ( event.GetRawKeyCode() == WXK_ESCAPE )
		{
			Close( true );
		}

		event.Skip( true );
	}
}
