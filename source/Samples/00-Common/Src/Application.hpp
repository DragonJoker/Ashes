#pragma once

#include "RendererPlugin.hpp"

#include <Utils/Factory.hpp>

#include <wx/app.h>

namespace common
{
	class App
		: public wxApp
	{
	protected:
		App( wxString const & name );

	public:
		virtual ~App() = default;
		bool OnInit()override;
		int OnExit()override;

		inline wxString const & getRendererName()const
		{
			return m_rendererName;
		}

	private:
		bool doParseCommandLine();
		virtual MainFrame * doCreateMainFrame( wxString const & rendererName ) = 0;

	protected:
		wxString m_name;
		wxString m_rendererName;
		bool m_allocated{ false };
		MainFrame * m_mainFrame{ nullptr };
		std::streambuf * m_cout{ nullptr };
		std::streambuf * m_cerr{ nullptr };
		std::streambuf * m_clog{ nullptr };
		std::vector< RendererPlugin > m_plugins;
		RendererFactory m_factory;
	};
}
