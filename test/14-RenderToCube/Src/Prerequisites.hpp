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

	static wxString const AppName = wxT( "14-RenderToCube" );

	class RenderPanel;
	class MainFrame;
	class Application;
}
