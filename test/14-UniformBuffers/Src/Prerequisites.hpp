#pragma once

#include <Prerequisites.hpp>

namespace vkapp
{
	struct TexturedVertexData
	{
		utils::Vec4 position;
		utils::Vec2 uv;
	};
	class RenderPanel;
	class MainFrame;
	class Application;

	static wxString const AppName{ common::makeName( TEST_ID, wxT( TEST_NAME ) ) };
}
