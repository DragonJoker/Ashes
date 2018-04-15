#pragma once

#include <Prerequisites.hpp>

namespace vkapp
{
	struct NonTexturedVertexData
	{
		utils::Vec3 position;
	};

	struct TexturedVertexData
	{
		utils::Vec4 position;
		utils::Vec2 uv;
	};

	static wxString const AppName = wxT( "12-TextureCube" );

	class RenderPanel;
	class MainFrame;
	class Application;
}
