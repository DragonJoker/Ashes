#include "MainFrame.hpp"

#include "RenderPanel.hpp"

namespace vkapp
{
	MainFrame::MainFrame( wxString const & rendererName
		, ashes::RendererList const & renderers )
		: common::MainFrame{ AppName, rendererName, renderers }
	{
	}

	wxPanel * MainFrame::doCreatePanel( wxSize const & size, utils::Instance const & instance )
	{
		return new RenderPanel( this, size, instance );
	}
}
