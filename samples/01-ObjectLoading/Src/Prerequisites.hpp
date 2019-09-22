#pragma once

#include <Prerequisites.hpp>

namespace vkapp
{
	static wxString const AppName{ common::makeName( SAMPLE_ID, wxT( "ObjectLoading" ) ) };
	static wxString const AppDesc = wxT( "Object Loading" );

	class RenderPanel;
	class MainFrame;
	class Application;
}
