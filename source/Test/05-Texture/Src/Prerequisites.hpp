#pragma once

#include <Prerequisites.hpp>

namespace vkapp
{
	/**
	*\~french
	*\brief
	*	Les données d'un sommet texturé.
	*\~english
	*\brief
	*	The data for one textured vertex.
	*/
	struct TexturedVertexData
	{
		utils::Vec4 position;
		utils::Vec2 uv;
	};

	static wxString const AppName = wxT( "05-Texture" );

	class RenderPanel;
	class MainFrame;
	class Application;
}
