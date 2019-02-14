#pragma once

#include <Core/Connection.hpp>
#include <Core/Renderer.hpp>

#include <Factory.hpp>
#include <Quaternion.hpp>

#pragma warning( disable: 4996 )
#include <wx/wx.h>
#include <wx/windowptr.h>

#include <array>
#include <chrono>

namespace common
{
	static wxSize const WindowSize{ 800, 600 };
	using RendererFactory = utils::Factory< ashes::Renderer
		, std::string
		, ashes::RendererPtr
		, std::function< ashes::RendererPtr( ashes::Renderer::Configuration const & ) > >;
	/**
	*\~english
	*\brief
	*	An image data.
	*\~french
	*\brief
	*	Les données d'une image.
	*/
	struct ImageData
	{
		//!\~english	The image dimensions.
		//!\~french		Les dimensions de l'image.
		ashes::Extent3D size;
		//!\~english	The image buffer.
		//!\~french		Le tampon de l'image.
		ashes::ByteArray data;
		//!\~english	The image pixel format.
		//!\~french		Les format des pixels de l'image.
		ashes::Format format;
	};
	/**
	*\~french
	*\brief
	*	Crée une ashes::Connection associant une fenêtre et un GPU.
	*\param[in] window
	*	Fenêtre utilisée pour récupérer les informations dépendantes de la plateforme cible.
	*\param[in] vulkan
	*	L'instance Vulkan.
	*\param[in] gpu
	*	Le GPU choisi pour associer Vulkan à la fenêtre.
	*\~english
	*\brief
	*	Creates a ashes::Connection binding a window to a GPU.
	*\param[in] window
	*	The window used to retrieve the platform dependant informations.
	*\param[in] vulkan
	*	The Vulkan instance.
	*\param[in] gpu
	*	The GPU chosen to bind the window and the GPU.
	*/
	ashes::ConnectionPtr makeConnection( wxWindow * window
		, ashes::Renderer const & renderer );

	class Application;
	class MainFrame;
}
