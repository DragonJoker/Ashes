#pragma once

#include <Prerequisites.hpp>

namespace vkapp
{
	static wxString const AppName{ common::makeName( SAMPLE_ID, wxT( "ObjectLighting" ) ) };
	static wxString const AppDesc = wxT( "Object Lighting" );

	class RenderPanel;
	class MainFrame;
	class Application;
}
