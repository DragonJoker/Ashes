#include "MainFrame.hpp"

#include "RenderPanel.hpp"

namespace vkapp
{
	MainFrame::MainFrame( wxString const & rendererName
		, ashes::RendererList const & renderers )
		: common::MainFrame{ AppName, rendererName, renderers }
	{
	}

	wxWindowPtr< common::RenderPanel > MainFrame::doCreatePanel( wxSize const & size, utils::Instance const & instance )
	{
		return common::wxMakeWindowDerivedPtr< common::RenderPanel, RenderPanel >( this, size, getRendererName().ToStdString() );
	}
}
