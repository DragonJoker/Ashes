/*
This file belongs to Renderer.
See LICENSE file in root folder
*/
#pragma once

#include "RendererPrerequisites.hpp"

#include <VkLib/Connection.hpp>

namespace renderer
{
	/**
	*\brief
	*	Encapsulation de la connexion à une application dépendant de la plateforme.
	*/
	class Connection
	{
	public:
		Connection( Connection const & ) = delete;
		Connection & operator=( Connection const & ) = delete;
		Connection( Connection && rhs ) = default;
		Connection & operator=( Connection && rhs ) = default;

#if defined( VK_USE_PLATFORM_WIN32_KHR )

		/**
		*\brief
		*	Constructeur.
		*\param[in] renderer
		*	L'instance de Renderer.
		*\param[in] hinstance
		*	Le descripteur de l'application.
		*\param[in] hwnd
		*	Le descripteur de la fenêtre.
		*/
		Connection( Renderer const & renderer
			, HINSTANCE hinstance
			, HWND hwnd );

#elif defined( VK_USE_PLATFORM_XCB_KHR )

		/**
		*\brief
		*	Constructeur.
		*\param[in] renderer
		*	L'instance de Renderer.
		*\param[in] connection
		*	Le descripteur de l'application.
		*\param[in] handle
		*	Le descripteur de la fenêtre.
		*/
		Connection( Renderer const & renderer
			, xcb_connection_t * connection
			, xcb_window_t handle );

#elif defined( VK_USE_PLATFORM_XLIB_KHR )

		/**
		*\brief
		*	Constructeur.
		*\param[in] renderer
		*	L'instance de Renderer.
		*\param[in] display
		*	Le descripteur de l'écran.
		*\param[in] window
		*	Le descripteur de la fenêtre.
		*/
		Connection( Renderer const & renderer
			, Display * display
			, Window window );

#endif
		/**
		*\return
		*	La connection vulkan.
		*/
		vk::Connection & getConnection()
		{
			return m_connection;
		}

	private:
		vk::Connection m_connection;
	};
}
