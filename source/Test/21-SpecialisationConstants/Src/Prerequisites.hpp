#pragma once

#include <Prerequisites.hpp>

namespace vkapp
{
	struct TexturedVertexData
	{
		renderer::Vec4 position;
		renderer::Vec2 uv;
	};

	static wxString const AppName = wxT( "21-SpecialisationConstants" );

	class RenderPanel;
	class MainFrame;
	class Application;
}
