#pragma once

#include <Prerequisites.hpp>

namespace vkapp
{
	struct TexturedVertexData
	{
		renderer::Vec4 position;
		float uv;
	};

	static wxString const AppName = wxT( "08-TextureBuffer" );

	class RenderPanel;
	class MainFrame;
	class Application;
}
