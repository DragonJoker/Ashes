#pragma once

#include <Prerequisites.hpp>

namespace vkapp
{
	static wxString const AppName{ common::makeName( SAMPLE_ID, wxT( "NormalMapping" ) ) };
	static wxString const AppDesc = wxT( "Normal Mapping" );

	class RenderPanel;
	class MainFrame;
	class Application;
}
