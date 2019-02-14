#pragma once

#include <Prerequisites.hpp>

namespace vkapp
{
	struct VertexData
	{
		utils::Vec4 position;
		utils::Vec4 colour;
	};

	static wxString const AppName = wxT( "04-StagingBuffer" );

	class RenderPanel;
	class MainFrame;
	class Application;
}
