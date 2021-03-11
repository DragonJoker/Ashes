#pragma once

#include <Prerequisites.hpp>

namespace vkapp
{
	static wxString const AppName{ common::makeName( TEST_ID, wxT( TEST_NAME ) ) };

	class Application;
	class MainFrame;
	class RenderingResources;
	class RenderPanel;

	using RenderingResourcesPtr = std::unique_ptr< RenderingResources >;
}
