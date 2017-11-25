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

#include <Utils/UtilsSignal.hpp>

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
		*\brief
		*	Réinitialise la swap chain.
		*/
		void reset( utils::IVec2 const & size );
		/**
		*\brief
		*	Crée les tampons d'image des back buffers, compatibles avec la passe de rendu donnée.
		*\param[in] renderPass
		*	La passe de rendu.
		*\return
		*	Les tampons d'images.
		*/
		FrameBufferPtrArray createFrameBuffers( RenderPass const & renderPass )const;
		/**
		*\brief
		*	Crée les tampons d'image des back buffers, compatibles avec la passe de rendu donnée.
		*\param[in] renderPass
		*	La passe de rendu.
		*\return
		*	Les tampons d'images.
		*/
		CommandBufferPtrArray createCommandBuffers()const;
		/**
		*\brief
		*	Enregistre des commandes de pré-rendu.
		*\param[in] index
		*	L'indice de l'image.
		*\param[in] commandBuffer
		*	Le tampon de commandes recevant les commandes.
		*/
		void preRenderCommands( uint32_t index
			, CommandBuffer const & commandBuffer )const;
		/**
		*\brief
		*	Enregistre des commandes de post-rendu.
		*\param[in] index
		*	L'indice de l'image.
		*\param[in] commandBuffer
		*	Le tampon de commandes recevant les commandes.
		*/
		void postRenderCommands( uint32_t index
			, CommandBuffer const & commandBuffer )const;
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
		*\brief
		*	Définit la couleur de vidage de la swapchain.
		*\param[in] value
		*	La nouvelle valeur.
		*/
		inline void setClearColour( utils::RgbaColour const & value )
		{
			m_swapChain->setClearColour( convert( value ) );
		}
		/**
		*\brief
		*	Définit la couleur de vidage de la swapchain.
		*\param[in] value
		*	La nouvelle valeur.
		*/
		inline utils::RgbaColour getClearColour()const
		{
			return convert( m_swapChain->getClearColour() );
		}
		/**
		*\return
		*	Les dimensions de la swap chain.
		*/
		inline utils::IVec2 getDimensions()const
		{
			return { m_swapChain->getWidth(), m_swapChain->getHeight() };
		}
		/**
		*\return
		*	Les format des pixels de la swap chain.
		*/
		inline utils::PixelFormat getFormat()const
		{
			return convert( m_swapChain->getFormat() );
		}
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
		void doResetSwapChain();

	public:
		//! Le signal levé lorsque la swap chain est recréée.
		using OnResetFunc = std::function< void() >;
		using OnReset = utils::Signal< OnResetFunc >;
		OnReset onReset;

	private:
		Device const & m_device;
		utils::IVec2 m_dimensions;
		vk::SwapChainPtr m_swapChain;
		//! Les ressources de rendu liées à la swap chain.
		std::vector< RenderingResourcesPtr > m_renderingResources;
		//! L'indice de la ressource de rendu active.
		mutable size_t m_resourceIndex{ 0 };
	};
}
