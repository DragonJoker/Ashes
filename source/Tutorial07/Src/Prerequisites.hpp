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
	*@~french
	*@brief
	*	Les données d'un sommet.
	*@~english
	*@brief
	*	The data for one vertex.
	*/
	struct VertexData
	{
		float x, y, z, w;
		float r, g, b, a;
	};
	/**
	*@~french
	*@brief
	*	Les données d'un sommet texturé.
	*@~english
	*@brief
	*	The data for one textured vertex.
	*/
	struct TexturedVertexData
	{
		utils::Vec4 position;
		utils::Vec2 uv;
	};
	/**
	*@~french
	*@brief
	*	Cr�e une vk::Connection associant une fen�tre et un GPU.
	*@param[in] window
	*	Fen�tre utilis�e pour r�cup�rer les informations d�pendantes de la plateforme cible.
	*@param[in] vulkan
	*	L'instance Vulkan.
	*@param[in] gpu
	*	Le GPU choisi pour associer Vulkan � la fen�tre.
	*@~english
	*@brief
	*	Creates a vk::Connection binding a window to a GPU.
	*@param[in] window
	*	The window used to retrieve the platform dependant informations.
	*@param[in] vulkan
	*	The Vulkan instance.
	*@param[in] gpu
	*	The GPU chosen to bind the window and the GPU.
	*/
	renderer::Connection makeConnection( wxWindow * window
		, renderer::Renderer const & vulkan );
	/**
	*@~french
	*@brief
	*	Cr�e un pointeur intelligent sur une fen�tre.
	*@~french
	*@brief
	*	Creates a smart pointer to a window.
	*/
	template< typename T, typename ... Params >
	inline wxWindowPtr< T > wxMakeWindowPtr( Params && ... params )
	{
		return wxWindowPtr< T >( new T( std::forward< Params >( params )... ) );
	}
	/**
	*@~french
	*@brief
	*	Crée une matrice de projection orthographique.
	*@param[in] left, right
	*	Les bornes gauche et droite.
	*@param[in] bottom, top
	*	Les bornes basse et haute.
	*@param[in] near, far
	*	Les bornes proche et éloignée.
	*@return
	*	Un tableau contenant la matrice résultat.
	*@~english
	*@brief
	*	Create an orthographic projection matrix.
	*@param[in] left, right
	*	The left and right bounds.
	*@param[in] bottom, top
	*	The top and bottom bounds.
	*@param[in] near, far
	*	The near and far bounds.
	*@return
	*	An array containing the resulting matrix.
	*/
	std::array< float, 16 > getOrthographicProjection( float left
		, float right
		, float top
		, float bottom
		, float near
		, float far );

	class Application;
	class MainFrame;
	class RenderingResources;
	class RenderPanel;

	using RenderingResourcesPtr = std::unique_ptr< RenderingResources >;
}
