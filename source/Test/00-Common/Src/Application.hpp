#pragma once

#include "Prerequisites.hpp"

#include <wx/app.h>

namespace common
{
	class App
		: public wxApp
	{
	public:
		App( wxString const & name );
		bool OnInit()override;
		int OnExit()override;

		void updateFps( std::chrono::microseconds const & duration );

		inline wxString const & getRendererName()const
		{
			return m_rendererName;
		}

	private:
		bool doParseCommandLine();
		virtual MainFrame * doCreateMainFrame( wxString const & rendererName ) = 0;

	private:
		wxString m_name;
		wxString m_rendererName;
		bool m_allocated{ false };
		MainFrame * m_mainFrame;
		std::streambuf * m_cout;
		std::streambuf * m_cerr;
		std::streambuf * m_clog;
	};
}
