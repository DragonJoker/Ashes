#pragma once

#include "Prerequisites.hpp"
#include <ashespp/Core/RendererList.hpp>

#include <wx/app.h>

namespace common
{
	class App
		: public wxApp
	{
	protected:
		explicit App( wxString const & name );

	public:
		bool OnInit()override;
		int OnExit()override;

		wxString const & getInstanceName()const noexcept
		{
			return m_rendererName;
		}

	protected:
		wxString const & getName()const noexcept
		{
			return m_name;
		}

		bool isAllocated()const noexcept
		{
			return m_allocated;
		}

		MainFrame & getMainFrame()const noexcept
		{
			assert( m_mainFrame );
			return *m_mainFrame;
		}

		ashes::RendererList const & getRenderers()const noexcept
		{
			return m_renderers;
		}

	private:
		bool doParseCommandLine();
		virtual MainFrame * doCreateMainFrame( wxString const & rendererName ) = 0;

	private:
		wxString m_name{};
		wxString m_rendererName{};
		bool m_allocated{};
		MainFrame * m_mainFrame{};
		std::unique_ptr< std::streambuf > m_cout{};
		std::unique_ptr< std::streambuf > m_cerr{};
		std::unique_ptr< std::streambuf > m_clog{};
		ashes::RendererList m_renderers{};
	};
}
