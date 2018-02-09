#pragma once

#include <Prerequisites.hpp>

namespace vkapp
{
	struct VertexData
	{
		renderer::Vec4 position;
		renderer::Vec4 colour;
	};

	static wxString const AppName = wxT( "03-VertexLayout" );

	class RenderPanel;
	class MainFrame;
	class Application;
}
