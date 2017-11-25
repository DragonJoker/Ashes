#pragma once

#include <Renderer/Connection.hpp>

#include <Utils/Mat4.hpp>

#pragma warning( disable: 4996 )
#include <wx/wx.h>
#include <wx/windowptr.h>

#include <array>
#include <chrono>

namespace vkapp
{
	static wxSize const WindowSize{ 800, 600 };
	static wxString const AppName{ wxT( "MainApp" ) };
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
		float x, y, z, w;
		float r, g, b, a;
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
		utils::Vec4 position;
		utils::Vec2 uv;
	};
	/**
	*\~french
	*\brief
	*	Crée une vk::Connection associant une fenêtre et un GPU.
	*\param[in] window
	*	Fenêtre utilis�e pour r�cup�rer les informations dépendantes de la plateforme cible.
	*\param[in] vulkan
	*	L'instance Vulkan.
	*\param[in] gpu
	*	Le GPU choisi pour associer Vulkan � la fenêtre.
	*\~english
	*\brief
	*	Creates a vk::Connection binding a window to a GPU.
	*\param[in] window
	*	The window used to retrieve the platform dependant informations.
	*\param[in] vulkan
	*	The Vulkan instance.
	*\param[in] gpu
	*	The GPU chosen to bind the window and the GPU.
	*/
	renderer::Connection makeConnection( wxWindow * window
		, renderer::Renderer const & vulkan );
	/**
	*\~french
	*\brief
	*	Crée un pointeur intelligent sur une fenêtre.
	*\~french
	*\brief
	*	Creates a smart pointer to a window.
	*/
	template< typename T, typename ... Params >
	inline wxWindowPtr< T > wxMakeWindowPtr( Params && ... params )
	{
		return wxWindowPtr< T >( new T( std::forward< Params >( params )... ) );
	}

	class Application;
	class MainFrame;
	class RenderingResources;
	class RenderPanel;

	using RenderingResourcesPtr = std::unique_ptr< RenderingResources >;
}
