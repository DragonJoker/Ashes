#pragma once

#include <Prerequisites.hpp>

namespace vkapp
{
	struct TexturedVertexData
	{
		utils::Vec4 position;
		utils::Vec2 uv;
	};

	static wxString const AppName{ common::makeName( TEST_ID, wxT( "Texture2DMipmaps" ) ) };

	class RenderPanel;
	class MainFrame;
	class Application;
}
