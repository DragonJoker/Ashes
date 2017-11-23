/*
This file belongs to VkLib.
See LICENSE file in root folder
*/
#pragma once

#include "Connection.hpp"

#include <VkLib/LogicalDevice.hpp>

namespace renderer
{
	/**
	*\brief
	*	Classe contenant les informations liées au GPU logique.
	*/
	class Device
	{
	public:
		/**
		*\brief
		*	Constructeur.
		*\param[in] renderer
		*	L'instance de Renderer.
		*\param[in] connection
		*	La connection à l'application.
		*/
		Device( Renderer const & renderer
			, Connection & connection );
		/**
		*\brief
		*	Crée une swap chain.
		*\param[in] size
		*	Les dimensions souhaitées.
		*/
		SwapChainPtr createSwapChain( utils::IVec2 const & size )const;
		/**
		*\~french
		*\brief
		*	Conversion implicite vers VkDevice.
		*\~english
		*\brief
		*	VkDevice implicit cast operator.
		*/
		inline vk::LogicalDevice const & getDevice()const
		{
			return m_device;
		}

	private:
		vk::LogicalDevice m_device;
	};
}
