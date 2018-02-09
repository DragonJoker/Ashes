#pragma once

#include <Prerequisites.hpp>

namespace vkapp
{
	struct UVVertexData
	{
		renderer::Vec4 position;
		renderer::Vec2 uv;
	};

	struct UVWVertexData
	{
		renderer::Vec4 position;
		renderer::Vec3 uvw;
	};

	static wxString const AppName = wxT( "11-Texture3D" );

	class RenderPanel;
	class MainFrame;
	class Application;
}
