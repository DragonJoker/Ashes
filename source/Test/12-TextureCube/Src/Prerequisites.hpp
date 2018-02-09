#pragma once

#include <Prerequisites.hpp>

namespace vkapp
{
	struct TexturedVertexData
	{
		renderer::Vec4 position;
		renderer::Vec2 uv;
	};

	static wxString const AppName = wxT( "12-TextureCube" );

	class RenderPanel;
	class MainFrame;
	class Application;
}
