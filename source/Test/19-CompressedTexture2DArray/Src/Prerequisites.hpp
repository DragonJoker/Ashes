#pragma once

#include <Prerequisites.hpp>

namespace vkapp
{
	struct TexturedVertexData
	{
		utils::Vec4 position;
		utils::Vec2 uv;
	};

	static wxString const AppName = wxT( "19-CompressedTexture2DArray" );

	class RenderPanel;
	class MainFrame;
	class Application;
}
