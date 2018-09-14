/**
*\file
*	Renderer.h
*\author
*	Sylvain Doremus
*/
#pragma once

#include "D3D11RendererPrerequisites.hpp"

#include <Core/Renderer.hpp>
#include <Utils/DynamicLibrary.hpp>

namespace d3d11_renderer
{
	class Renderer
		: public ashes::Renderer
	{
	public:
		/**
		*\brief
		*	Constructeur, initialise l'instance de Vulkan.
		*/
		Renderer( Configuration const & configuration );
		/**
		*\brief
		*	Destructeur.
		*/
		~Renderer();
		/**
		*\brief
		*	Crée le périphérique logique.
		*\param[in] connection
		*	La connection avec la fenêtre.
		*/
		ashes::DevicePtr createDevice( ashes::ConnectionPtr && connection )const override;
		/**
		*\brief
		*	Crée une connection.
		*\param[in] deviceIndex
		*	L'indice du périphérique physique.
		*\param[in] handle
		*	Le descripteur de la fenêtre.
		*/
		ashes::ConnectionPtr createConnection( uint32_t deviceIndex
			, ashes::WindowHandle && handle )const override;
		/**
		*\copydoc	ashes::Renderer::frustum
		*/
		std::array< float, 16 > frustum( float left
			, float right
			, float bottom
			, float top
			, float zNear
			, float zFar )const override;
		/**
		*\copydoc	ashes::Renderer::perspective
		*/
		std::array< float, 16 > perspective( float radiansFovY
			, float aspect
			, float zNear
			, float zFar )const override;
		/**
		*\copydoc	ashes::Renderer::ortho
		*/
		std::array< float, 16 > ortho( float left
			, float right
			, float bottom
			, float top
			, float zNear
			, float zFar )const override;

		inline IDXGIFactory * getDXGIFactory()const
		{
			return m_factory;
		}

	private:
		void doCreateDXGIFactory();
		void doLoadAdapters();

	private:
		IDXGIFactory * m_factory;
	};
}
