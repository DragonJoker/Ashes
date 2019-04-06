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
			, ashes::RendererList const & renderers );
		virtual ~MainFrame();

		void initialise();
		void cleanup();

	private:
		virtual RenderPanel * doCreatePanel( wxSize const & size, utils::Instance const & instance ) = 0;

		wxDECLARE_EVENT_TABLE();
		void onClose( wxCloseEvent & event );
		void onTimer( wxTimerEvent & event );

	private:
		wxTimer * m_timer{ nullptr };
		wxString m_name;
		wxString m_rendererName;
		utils::InstancePtr m_instance;
		utils::InstanceFactory & m_renderers;
		RenderPanel * m_panel{ nullptr };
	};
}
