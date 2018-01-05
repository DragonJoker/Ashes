#include "MainFrame.hpp"

#include <GlRenderer/GlCreateRenderer.hpp>
#include <VkRenderer/VkCreateRenderer.hpp>

#include <wx/sizer.h>

#include <numeric>

namespace common
{
	MainFrame::MainFrame( wxString const & name
		, wxString const & rendererName )
		: wxFrame{ nullptr
		, wxID_ANY
		, name + wxT( " (" ) + rendererName + wxT( ")" )
		, wxDefaultPosition
		, WindowSize
		, wxSYSTEM_MENU | wxCAPTION | wxCLOSE_BOX | wxCLIP_CHILDREN | wxRESIZE_BORDER | wxMAXIMIZE_BOX }
		, m_name{ name }
		, m_rendererName{ rendererName }
	{
	}

	void MainFrame::initialise()
	{
		SetClientSize( WindowSize );
		Show( true );

		try
		{
			if ( m_rendererName == wxT( "gl" ) )
			{
				m_renderer = gl_renderer::createRenderer();
			}
			else
			{
				m_renderer = vk_renderer::createRenderer();
			}

			std::cout << "Renderer instance created." << std::endl;
			m_panel = doCreatePanel( WindowSize, *m_renderer );

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

		m_renderer.reset();
	}

	void MainFrame::updateFps( std::chrono::microseconds const & duration )
	{
		++m_frameCount;
		m_framesTimes[m_frameIndex] = duration;
		auto count = std::min( m_frameCount, m_framesTimes.size() );
		auto averageTime = std::accumulate( m_framesTimes.begin()
			, m_framesTimes.begin() + count
			, std::chrono::microseconds{ 0 } ).count() / float( count );
		m_frameIndex = ++m_frameIndex % FrameSamplesCount;
		wxString title;
		auto ms = duration.count() / 1000.0f;
		auto avgms = averageTime / 1000.0f;
		title << wxT( " - (Instant) " ) << ms << wxT( " ms" );
		title << wxT( " - " ) << ( 1000.0f / ms ) << " fps";
		title << wxT( " - (Average) " ) << avgms << wxT( " ms" );
		title << wxT( " - " ) << ( 1000.0f / avgms ) << " fps";
		SetTitle( m_name + wxT( " (" ) + m_rendererName + wxT( ")" ) + title );
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
