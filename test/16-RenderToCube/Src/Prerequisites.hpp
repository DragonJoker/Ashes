#pragma once

#include <Prerequisites.hpp>

namespace vkapp
{
	struct VertexData
	{
		utils::Vec4 position;
	};

	struct TexturedVertexData
	{
		utils::Vec4 position;
		utils::Vec2 uv;
	};

	static wxString const AppName{ common::makeName( TEST_ID, wxT( TEST_NAME ) ) };

	class RenderPanel;
	class MainFrame;
	class Application;
}
