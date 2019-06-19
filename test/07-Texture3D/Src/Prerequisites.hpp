#pragma once

#include <Prerequisites.hpp>

namespace vkapp
{
	struct UVVertexData
	{
		utils::Vec4 position;
		utils::Vec2 uv;
	};

	struct UVWVertexData
	{
		utils::Vec4 position;
		utils::Vec3 uvw;
	};

	static wxString const AppName = wxT( "07-Texture3D" );

	class RenderPanel;
	class MainFrame;
	class Application;
}
