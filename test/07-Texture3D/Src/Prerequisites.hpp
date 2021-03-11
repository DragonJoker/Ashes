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

	static wxString const AppName{ common::makeName( TEST_ID, wxT( TEST_NAME ) ) };

	class RenderPanel;
	class MainFrame;
	class Application;
}
