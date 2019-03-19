#include "MainFrame.hpp"

#include "RenderPanel.hpp"

namespace vkapp
{
	MainFrame::MainFrame( wxString const & rendererName
		, utils::RendererFactory & factory )
		: common::MainFrame{ AppName, rendererName, factory }
	{
	}

	common::RenderPanel * MainFrame::doCreatePanel( wxSize const & size, ashes::Instance const & instance )
	{
		return new RenderPanel{ this, size };
	}
}
