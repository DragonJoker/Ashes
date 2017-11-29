/*
This file belongs to Renderer.
See LICENSE file in root folder
*/
#pragma once

#include "CommandPool.hpp"
#include "Connection.hpp"
#include "Queue.hpp"

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
			, Connection && connection );
		/**
		*\brief
		*	Crée une swap chain.
		*\param[in] size
		*	Les dimensions souhaitées.
		*/
		SwapChainPtr createSwapChain( utils::IVec2 const & size )const;
		/**
		*\brief
		*	Attend que le périphérique soit inactif.
		*/
		void waitIdle()const;
		/**
		*\brief
		*	Le vk::Device.
		*/
		inline vk::LogicalDevice const & getDevice()const
		{
			return m_device;
		}
		/**
		*\return
		*	La file de présentation.
		*/
		inline Queue const & getPresentQueue()const
		{
			return m_presentQueue;
		}
		/**
		*\return
		*	La file de dessin.
		*/
		inline Queue const & getGraphicsQueue()const
		{
			return m_graphicsQueue;
		}
		/**
		*\return
		*	Le pool de tampons de commandes pour la file de présentation.
		*/
		inline CommandPool const & getPresentCommandPool()const
		{
			return m_presentCommandPool;
		}
		/**
		*\return
		*	Le pool de tampons de commandes pour la file de dessin.
		*/
		inline CommandPool const & getGraphicsCommandPool()const
		{
			return m_graphicsCommandPool;
		}

	private:
		vk::LogicalDevice m_device;
		Queue m_presentQueue;
		Queue m_graphicsQueue;
		CommandPool m_presentCommandPool;
		CommandPool m_graphicsCommandPool;
	};
}
