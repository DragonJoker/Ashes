/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include "D3D11Renderer/D3D11RendererPrerequisites.hpp"

#include <Ashes/Core/Connection.hpp>

namespace d3d11_renderer
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
		*\param[in] renderer
		*	L'instance de Renderer.
		*\param[in] deviceIndex
		*	L'indice du périphérique physique.
		*\param[in] handle
		*	Le descripteur de la fenêtre.
		*/
		Connection( Renderer const & renderer
			, uint32_t deviceIndex
			, ashes::WindowHandle && handle );
		/**
		*\brief
		*	Destructeur, détruit la surface KHR.
		*/
		~Connection();
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
		Renderer const & m_renderer;
		PhysicalDevice const & m_gpu;
		uint32_t m_graphicsQueueFamilyIndex{ std::numeric_limits< uint32_t >::max() };
		uint32_t m_computeQueueFamilyIndex{ std::numeric_limits< uint32_t >::max() };
		uint32_t m_presentQueueFamilyIndex{ std::numeric_limits< uint32_t >::max() };
	};
}
