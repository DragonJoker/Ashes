/**
*\file
*	Instance.h
*\author
*	Sylvain Doremus
*/
#pragma once

#include "TestRendererPrerequisites.hpp"

#include <Ashes/Core/Instance.hpp>
#include <Utils/DynamicLibrary.hpp>

namespace test_renderer
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
		*\brief
		*	Destructeur.
		*/
		~Instance();
		/**
		*\brief
		*	Crée le périphérique logique.
		*\param[in] connection
		*	La connection avec la fenêtre.
		*/
		ashes::DevicePtr createDevice( ashes::ConnectionPtr connection
			, ashes::DeviceQueueCreateInfoArray queueCreateInfos
			, ashes::StringArray enabledLayers
			, ashes::StringArray enabledExtensions
			, ashes::PhysicalDeviceFeatures enabledFeatures )const override;
		/**
		*\brief
		*	Crée une connection.
		*\param[in] deviceIndex
		*	L'indice du périphérique physique.
		*\param[in] handle
		*	Le descripteur de la fenêtre.
		*/
		ashes::ConnectionPtr createConnection( ashes::PhysicalDevice const & gpu
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
	};
}
