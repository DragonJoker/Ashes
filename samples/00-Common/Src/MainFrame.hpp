#pragma once

#include "Prerequisites.hpp"
#include "RenderPanel.hpp"

#include <ashespp/Core/Instance.hpp>
#include <ashespp/Core/RendererList.hpp>

#include <wx/frame.h>
#include <wx/windowptr.h>

namespace common
{
	class MainFrame
		: public wxFrame
	{
	public:
		MainFrame( wxString const & name
			, wxString const & rendererName
			, ashes::RendererList const & renderers );

		void initialise();
		void cleanup();

		wxString const & getRendererName()const
		{
			return m_rendererName;
		}

	private:
		virtual wxWindowPtr< RenderPanel > doCreatePanel( wxSize const & size, utils::Instance const & instance ) = 0;

		wxDECLARE_EVENT_TABLE();
		void onClose( wxCloseEvent & event );
		void onTimer( wxTimerEvent & event );
		void onKeyUp( wxKeyEvent & event );

	private:
		wxTimer m_timer{};
		wxString m_name;
		wxString m_rendererName;
		utils::InstancePtr m_instance;
		ashes::RendererList const & m_renderers;
		wxWindowPtr< RenderPanel > m_panel{ nullptr };
	};
}
