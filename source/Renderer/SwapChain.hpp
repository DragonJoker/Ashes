/**
*\file
*	Renderer.h
*\author
*	Sylvain Doremus
*/
#pragma once

#include "RendererPrerequisites.hpp"
#include "RenderingResources.hpp"

#include <VkLib/SwapChain.hpp>

namespace renderer
{
	class SwapChain
	{
	public:
		/**
		*\brief
		*	Constructeur.
		*/
		SwapChain( Device const & device
			, utils::IVec2 const & size );
		/**
		*\return
		*	Récupère les ressources de rendu actives.
		*/
		RenderingResources * getResources();
		/**
		*\return
		*	Récupère les ressources de rendu par défaut.
		*/
		RenderingResources const & getDefaultResources()const
		{
			return *m_renderingResources[0];
		}
		/**
		*\return
		*	Présente les ressources de rendu.
		*/
		void present( RenderingResources & resources );
		/**
		*\return
		*	La swap chain de vulkan.
		*/
		inline vk::SwapChain const & getSwapChain()const
		{
			return *m_swapChain;
		}

	private:
		bool doCheckNeedReset( VkResult errCode
			, bool acquisition
			, char const * const action );
		void SwapChain::doResetSwapChain();

	private:
		Device const & m_device;
		utils::IVec2 m_dimensions;
		vk::SwapChainPtr m_swapChain;
		vk::BackBuffer * m_backBuffer{ nullptr };
		//! Les ressources de rendu liées à la swap chain.
		std::vector< RenderingResourcesPtr > m_renderingResources;
		//! L'indice de la ressource de rendu active.
		mutable size_t m_resourceIndex{ 0 };
	};
}
