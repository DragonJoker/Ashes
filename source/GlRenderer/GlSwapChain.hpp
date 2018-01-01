/**
*\file
*	Renderer.h
*\author
*	Sylvain Doremus
*/
#pragma once

#include "GlClearValue.hpp"
#include "GlRendererPrerequisites.hpp"
#include "GlRenderingResources.hpp"

#include <Renderer/SwapChain.hpp>

#include <Utils/UtilsSignal.hpp>

namespace gl_renderer
{
	class SwapChain
		: public renderer::SwapChain
	{
	public:
		/**
		*\brief
		*	Constructeur.
		*/
		SwapChain( renderer::Device const & device
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
		renderer::FrameBufferPtrArray createFrameBuffers( renderer::RenderPass const & renderPass )const override;
		/**
		*\brief
		*	Crée les tampons d'image des back buffers, compatibles avec la passe de rendu donnée.
		*\param[in] renderPass
		*	La passe de rendu.
		*\return
		*	Les tampons d'images.
		*/
		renderer::CommandBufferPtrArray createCommandBuffers()const override;
		/**
		*\brief
		*	Enregistre des commandes de pré-rendu.
		*\param[in] index
		*	L'indice de l'image.
		*\param[in] commandBuffer
		*	Le tampon de commandes recevant les commandes.
		*/
		void preRenderCommands( uint32_t index
			, renderer::CommandBuffer const & commandBuffer )const override;
		/**
		*\brief
		*	Enregistre des commandes de post-rendu.
		*\param[in] index
		*	L'indice de l'image.
		*\param[in] commandBuffer
		*	Le tampon de commandes recevant les commandes.
		*/
		void postRenderCommands( uint32_t index
			, renderer::CommandBuffer const & commandBuffer )const override;;
		/**
		*\return
		*	Récupère les ressources de rendu actives.
		*/
		renderer::RenderingResources * getResources()override;
		/**
		*\return
		*	Présente les ressources de rendu.
		*/
		void present( renderer::RenderingResources & resources )override;
		/**
		*\brief
		*	Définit la couleur de vidage de la swapchain.
		*\param[in] value
		*	La nouvelle valeur.
		*/
		inline void setClearColour( utils::RgbaColour const & value )override
		{
			m_clearColour = value;
		}
		/**
		*\brief
		*	Définit la couleur de vidage de la swapchain.
		*\param[in] value
		*	La nouvelle valeur.
		*/
		inline utils::RgbaColour getClearColour()const override
		{
			return m_clearColour;
		}
		/**
		*\return
		*	Les dimensions de la swap chain.
		*/
		inline utils::IVec2 getDimensions()const override
		{
			return m_dimensions;
		}
		/**
		*\return
		*	Les format des pixels de la swap chain.
		*/
		inline utils::PixelFormat getFormat()const override
		{
			return m_format;
		}

	private:
		bool doCheckNeedReset( VkResult errCode
			, bool acquisition
			, char const * const action );
		void doResetSwapChain();

	private:
		utils::RgbaColour m_clearColour;
		utils::IVec2 m_dimensions;
		utils::PixelFormat m_format;
	};
}
