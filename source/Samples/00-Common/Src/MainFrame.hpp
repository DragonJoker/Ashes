#pragma once

#include "Prerequisites.hpp"

#include <Core/Renderer.hpp>

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
		virtual RenderPanel * doCreatePanel( wxSize const & size, renderer::Renderer const & renderer ) = 0;

		wxDECLARE_EVENT_TABLE();
		void onClose( wxCloseEvent & event );
		void onTimer( wxTimerEvent & event );

	private:
		wxTimer * m_timer{ nullptr };
		wxString m_name;
		wxString m_rendererName;
		renderer::RendererPtr m_renderer;
		RendererFactory & m_factory;
		RenderPanel * m_panel{ nullptr };
	};
}
