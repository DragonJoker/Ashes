#include "MainFrame.hpp"

#include "RenderPanel.hpp"

#include <wx/sizer.h>

namespace vkapp
{
	MainFrame::MainFrame()
		: wxFrame{ nullptr
		, wxID_ANY
		, AppName
		, wxDefaultPosition
		, WindowSize
		, wxSYSTEM_MENU | wxCAPTION | wxCLOSE_BOX | wxCLIP_CHILDREN | wxRESIZE_BORDER | wxMAXIMIZE_BOX }
	{
		SetClientSize( WindowSize );
		Show( true );

		try
		{
			m_renderer = std::make_unique< renderer::Renderer >();
			std::cout << "Vulkan instance created." << std::endl;
			m_panel = wxMakeWindowPtr< RenderPanel >( this, WindowSize, *m_renderer );

			wxBoxSizer * sizer{ new wxBoxSizer{ wxVERTICAL } };
			sizer->Add( m_panel.get(), wxSizerFlags{ 1 }.Expand() );
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
		SetTitle( AppName + title );
	}
}
