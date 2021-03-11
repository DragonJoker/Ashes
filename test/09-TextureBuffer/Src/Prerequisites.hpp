#pragma once

#include <Prerequisites.hpp>

namespace vkapp
{
	struct TexturedVertexData
	{
		utils::Vec4 position;
		float uv;
	};

	static wxString const AppName{ common::makeName( TEST_ID, wxT( TEST_NAME ) ) };

	class RenderPanel;
	class MainFrame;
	class Application;
}
