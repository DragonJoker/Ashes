#pragma once

#include "Prerequisites.hpp"

#include <Ashes/Core/Instance.hpp>

#include <wx/frame.h>

namespace common
{
	class MainFrame
		: public wxFrame
	{
	public:
		MainFrame( wxString const & name
			, wxString const & rendererName
			, RendererFactory & factory );
		virtual ~MainFrame();

		void initialise();
		void cleanup();

	private:
		virtual RenderPanel * doCreatePanel( wxSize const & size, ashes::Instance const & instance ) = 0;

		wxDECLARE_EVENT_TABLE();
		void onClose( wxCloseEvent & event );
		void onTimer( wxTimerEvent & event );

	private:
		wxTimer * m_timer{ nullptr };
		wxString m_name;
		wxString m_rendererName;
		ashes::InstancePtr m_instance;
		RendererFactory & m_factory;
		RenderPanel * m_panel{ nullptr };
	};
}
