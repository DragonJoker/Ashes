#include "MainFrame.hpp"
#include "Application.hpp"
#include "RenderPanel.hpp"

#include <wx/sizer.h>

#include <numeric>

namespace common
{
	namespace
	{
		enum class Ids
		{
			RenderTimer = 42
		}	Ids;

		static int const TimerTimeMs = 20;
	}

	MainFrame::MainFrame( wxString const & name
		, wxString const & rendererName
		, RendererFactory & factory )
		: wxFrame{ nullptr
			, wxID_ANY
			, name
			, wxDefaultPosition
			, WindowSize
			, wxSYSTEM_MENU | wxCAPTION | wxCLOSE_BOX | wxCLIP_CHILDREN | wxRESIZE_BORDER | wxMAXIMIZE_BOX }
		, m_name{ name }
		, m_rendererName{ rendererName }
		, m_factory{ factory }
		, m_timer{ new wxTimer{ this, int( Ids::RenderTimer ) } }
	{
	}

	MainFrame::~MainFrame()
	{
		delete m_timer;
	}

	void MainFrame::initialise()
	{
		SetClientSize( WindowSize );
		Show( true );

		try
		{
			renderer::Renderer::Configuration config
			{
				m_name.ToStdString(),
				"RendererLib",
#if !defined( NDEBUG )
				true,
#else
				false,
#endif
			};
			m_renderer = m_factory.create( m_rendererName.ToStdString(), config );

			std::cout << "Renderer instance created." << std::endl;
			m_panel = doCreatePanel( WindowSize, *m_renderer );
			m_panel->initialise( *m_renderer );

			Connect( int( Ids::RenderTimer )
				, wxEVT_TIMER
				, wxTimerEventHandler( MainFrame::onTimer )
				, nullptr
				, this );
			m_timer->Start( TimerTimeMs );

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
		m_timer->Stop();

		if ( m_panel )
		{
			m_panel->Destroy();
		}

		m_renderer.reset();
	}

	wxBEGIN_EVENT_TABLE( MainFrame, wxFrame )
		EVT_CLOSE( MainFrame::onClose )
	wxEND_EVENT_TABLE()

	void MainFrame::onClose( wxCloseEvent & event )
	{
		cleanup();
		event.Skip( true );
	}

	void MainFrame::onTimer( wxTimerEvent & event )
	{
		if ( event.GetId() == int( Ids::RenderTimer ) )
		{
			try
			{
				m_panel->update();
				m_panel->draw();
			}
			catch ( std::exception & exc )
			{
				m_timer->Stop();
				wxMessageBox( exc.what()
					, wxMessageBoxCaptionStr
					, wxICON_ERROR );
			}
		}
	}
}
