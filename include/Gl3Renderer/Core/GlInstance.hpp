/**
*\file
*	Instance.h
*\author
*	Sylvain Doremus
*/
#pragma once

#include "Gl3Renderer/GlRendererPrerequisites.hpp"

#include <Ashes/Core/Instance.hpp>

namespace gl_renderer
{
	class Instance
		: public ashes::Instance
	{
	public:
		/**
		*\brief
		*	Constructeur, initialise l'instance de Vulkan.
		*/
		Instance( Configuration const & configuration );
		/**
		*\copydoc	ashes::Instance::createDevice
		*/
		ashes::DevicePtr createDevice( ashes::SurfacePtr surface
			, ashes::DeviceCreateInfo createInfos )const override;
		/**
		*\copydoc	ashes::Instance::createSurface
		*/
		ashes::SurfacePtr createSurface( ashes::PhysicalDevice const & gpu
			, ashes::WindowHandle handle )const override;
		/**
		*\copydoc	ashes::Instance::frustum
		*/
		std::array< float, 16 > frustum( float left
			, float right
			, float bottom
			, float top
			, float zNear
			, float zFar )const override;
		/**
		*\copydoc	ashes::Instance::perspective
		*/
		std::array< float, 16 > perspective( float radiansFovY
			, float aspect
			, float zNear
			, float zFar )const override;
		/**
		*\copydoc	ashes::Instance::ortho
		*/
		std::array< float, 16 > ortho( float left
			, float right
			, float bottom
			, float top
			, float zNear
			, float zFar )const override;

		inline bool isSPIRVSupported()const
		{
			return m_spirvSupported;
		}

	private:
		bool m_spirvSupported;
	};
}
