#pragma once

#include <Prerequisites.hpp>

namespace vkapp
{
	struct TexturedVertexData
	{
		utils::Vec4 position;
		utils::Vec2 uv;
	};

	static wxString const AppName = wxT( "22-SpecialisationConstants" );

	class RenderPanel;
	class MainFrame;
	class Application;
}
