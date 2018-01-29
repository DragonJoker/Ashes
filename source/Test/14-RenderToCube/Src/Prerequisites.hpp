#pragma once

#include <Prerequisites.hpp>

namespace vkapp
{
	/**
	*\~french
	*\brief
	*	Les données d'un sommet non texturé.
	*\~english
	*\brief
	*	The data for one non textured vertex.
	*/
	struct VertexData
	{
		renderer::Vec4 position;
	};
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
		renderer::Vec4 position;
		renderer::Vec2 uv;
	};

	static wxString const AppName = wxT( "14-RenderToCube" );

	class RenderPanel;
	class MainFrame;
	class Application;
}
