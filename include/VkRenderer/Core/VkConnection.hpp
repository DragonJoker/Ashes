/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include "VkRenderer/VkRendererPrerequisites.hpp"

#include <Ashes/Core/Connection.hpp>

namespace vk_renderer
{
	/**
	*\brief
	*	Encapsulation de la connexion à une application dépendant de la plateforme.
	*/
	class Connection
		: public ashes::Connection
	{
	public:
		/**
		*\brief
		*	Constructeur.
		*\param[in] instance
		*	L'instance.
		*\param[in] deviceIndex
		*	L'indice du périphérique physique.
		*\param[in] handle
		*	Le descripteur de la fenêtre.
		*/
		Connection( Instance const & instance
			, ashes::PhysicalDevice const & gpu
			, ashes::WindowHandle handle );
		/**
		*\brief
		*	Destructeur, détruit la surface KHR.
		*/
		~Connection();
		/**
		*\copydoc	ashes::Connection:getSurfaceSupport
		*/
		bool getSurfaceSupport( uint32_t queueFamilyIndex )const override;
		void updateSurfaceCapabilities();
		/**
		*\~french
		*\return
		*	La surface de présentation.
		*\~english
		*\return
		*	The presentation surface.
		*/
		inline auto getPresentSurface()const
		{
			return m_presentSurface;
		}
		/**
		*\~french
		*\return
		*	L'index du type de file graphique.
		*\~english
		*\return
		*	The graphic queue's family index.
		*/
		inline auto getGraphicsQueueFamilyIndex()const
		{
			return m_graphicsQueueFamilyIndex;
		}
		/**
		*\~french
		*\return
		*	L'index du type de file de calcul.
		*\~english
		*\return
		*	The compute queue's family index.
		*/
		inline auto getComputeQueueFamilyIndex()const
		{
			return m_computeQueueFamilyIndex;
		}
		/**
		*\~french
		*\return
		*	L'index du type de file de présentation.
		*\~english
		*\return
		*	The presentation queue's family index.
		*/
		inline auto getPresentQueueFamilyIndex()const
		{
			return m_graphicsQueueFamilyIndex;
		}
		/**
		*\~french
		*\return
		*	Le périphérique physique.
		*\~english
		*\return
		*	The physical device.
		*/
		inline PhysicalDevice const & getGpu()const
		{
			return m_gpu;
		}

	private:
		void doCreatePresentSurface();
		void doRetrieveSurfaceCapabilities();
		void doRetrieveSurfaceFormats();
		void doRetrievePresentModes();

	private:
		Instance const & m_instance;
		PhysicalDevice const & m_gpu;
		VkSurfaceKHR m_presentSurface{ VK_NULL_HANDLE };
		uint32_t m_graphicsQueueFamilyIndex{ std::numeric_limits< uint32_t >::max() };
		uint32_t m_computeQueueFamilyIndex{ std::numeric_limits< uint32_t >::max() };
		uint32_t m_presentQueueFamilyIndex{ std::numeric_limits< uint32_t >::max() };
	};
}
