#pragma once

#include <Prerequisites.hpp>

namespace vkapp
{
	struct VertexData
	{
		renderer::Vec4 position;
	};

	struct TexturedVertexData
	{
		renderer::Vec4 position;
		renderer::Vec2 uv;
	};

	static wxString const AppName = wxT( "14-RenderToCube" );

	class RenderPanel;
	class MainFrame;
	class Application;
}
