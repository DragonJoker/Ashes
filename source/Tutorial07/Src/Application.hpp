#pragma once

#include "Prerequisites.hpp"

#include <wx/app.h>

namespace vkapp
{
	class App
		: public wxApp
	{
	public:
		bool OnInit()override;
		int OnExit()override;

		void updateFps( std::chrono::microseconds const & duration );

	private:
		bool m_allocated{ false };
		MainFrame * m_mainFrame;
		std::streambuf * m_cout;
		std::streambuf * m_cerr;
		std::streambuf * m_clog;
	};
}

wxDECLARE_APP( vkapp::App );
