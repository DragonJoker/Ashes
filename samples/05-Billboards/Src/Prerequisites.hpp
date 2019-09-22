#pragma once

#include <Prerequisites.hpp>

namespace vkapp
{
	static wxString const AppName{ common::makeName( SAMPLE_ID, wxT( "Billboards" ) ) };
	static wxString const AppDesc = wxT( "Billboards" );

	class RenderPanel;
	class MainFrame;
	class Application;
}
