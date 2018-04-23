/**
*\file
*	Renderer.h
*\author
*	Sylvain Doremus
*/
#pragma once

#include "GlRendererPrerequisites.hpp"

#include <Core/Renderer.hpp>

namespace gl_renderer
{
	class Renderer
		: public renderer::Renderer
	{
	public:
		/**
		*\brief
		*	Constructeur, initialise l'instance de Vulkan.
		*/
		Renderer( Configuration const & configuration );
		/**
		*\brief
		*	Crée le périphérique logique.
		*\param[in] connection
		*	La connection avec la fenêtre.
		*/
		renderer::DevicePtr createDevice( renderer::ConnectionPtr && connection )const override;
		/**
		*\brief
		*	Constructeur.
		*\param[in] renderer
		*	L'instance de Renderer.
		*\param[in] deviceIndex
		*	L'indice du périphérique physique.
		*\param[in] handle
		*	Le descripteur de la fenêtre.
		*/
		renderer::ConnectionPtr createConnection( uint32_t deviceIndex
			, renderer::WindowHandle && handle )const override;
		/**
		*\copydoc	renderer::Renderer::frustum
		*/
		renderer::Mat4 frustum( float left
			, float right
			, float bottom
			, float top
			, float zNear
			, float zFar )const override;
		/**
		*\copydoc	renderer::Renderer::perspective
		*/
		renderer::Mat4 perspective( float radiansFovY
			, float aspect
			, float zNear
			, float zFar )const override;
		/**
		*\copydoc	renderer::Renderer::ortho
		*/
		renderer::Mat4 ortho( float left
			, float right
			, float bottom
			, float top
			, float zNear
			, float zFar )const override;
	};
}
