#pragma once

#include <Prerequisites.hpp>

namespace vkapp
{
	/**
	*\~french
	*\brief
	*	Les données d'un sommet.
	*\~english
	*\brief
	*	The data for one vertex.
	*/
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
