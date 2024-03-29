#pragma once

#include <Prerequisites.hpp>

namespace vkapp
{
	struct VertexData
	{
		utils::Vec4 position;
		utils::Vec4 colour;
	};

	static wxString const AppName{ common::makeName( TEST_ID, wxT( TEST_NAME ) ) };

	class RenderPanel;
	class MainFrame;
	class Application;
}
