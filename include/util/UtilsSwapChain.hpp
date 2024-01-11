/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#pragma once

#include "RenderingResources.hpp"

#include <ashespp/Core/SwapChain.hpp>
#include <ashespp/Core/SwapChainCreateInfo.hpp>
#include <ashespp/Image/ImageView.hpp>
#include <ashes/common/Signal.hpp>

namespace utils
{
	/**
	*\~english
	*\brief
	*	Helper class to handle swapchain reset and presentation.
	*\~french
	*\brief
	*	Classe d'aide à la présentation et la réinitialisation de la swapchain.
	*/
	class SwapChain
	{
	public:
		/**
		*\~english
		*\brief
		*	Constructor.
		*\param[in] device
		*	The parent Device.
		*\~french
		*\brief
		*	Constructeur.
		*\param[in] device
		*	Le Device parent.
		*/
		SwapChain( ashes::Device const & device
			, ashes::CommandPool const & commandPool
			, ashes::SurfacePtr surface
			, VkExtent2D const & size );
		~SwapChain();
		/**
		*\~french
		*\brief
		*	Resets the swap chain.
		*\~french
		*\brief
		*	Réinitialise la swap chain.
		*/
		void reset( VkExtent2D const & size );
		/**
		*\~french
		*\brief
		*	Creates the backbuffers' framebuffers, compativle with given render pass.
		*\param[in] renderPass
		*	The render pass.
		*\return
		*	The framebuffers.
		*\~french
		*\brief
		*	Crée les tampons d'image des back buffers, compatibles avec la passe de rendu donnée.
		*\param[in] renderPass
		*	La passe de rendu.
		*\return
		*	Les tampons d'images.
		*/
		ashes::FrameBufferPtrArray createFrameBuffers( ashes::RenderPass const & renderPass )const;
		/**
		*\~french
		*\brief
		*	Creates the backbuffers' framebuffers, compativle with given render pass.
		*\param[in] renderPass
		*	The render pass.
		*\return
		*	The framebuffers.
		*\~french
		*\brief
		*	Crée les tampons d'image des back buffers, compatibles avec la passe de rendu donnée.
		*\param[in] renderPass
		*	La passe de rendu.
		*\return
		*	Les tampons d'images.
		*/
		ashes::FrameBufferPtrArray createFrameBuffers( ashes::RenderPass const & renderPass
			, ashes::Image const & depthImage )const;
		/**
		*\~french
		*\brief
		*	Creates the backbuffers' command buffers.
		*\return
		*	The command buffers.
		*\~french
		*\brief
		*	Crée les tampons de commandes des back buffers.
		*\return
		*	Les tampons de commandes.
		*/
		ashes::CommandBufferPtrArray createCommandBuffers()const;
		/**
		*\~english
		*\return
		*	The active rendering resources.
		*\~french
		*\return
		*	Les ressources de rendu actives.
		*/
		RenderingResources * getResources();
		/**
		*\~english
		*\brief
		*	Presents the swapchain image to the given queue.
		*\param[in] queue
		*	The queue used for presentation.
		*\~french
		*\brief
		*	Présente l'image de la swapchain à la file donnée.
		*\param[in] queue
		*	La file de présentation.
		*/
		void present( RenderingResources & resources
			, ashes::Queue const & queue );

		inline VkFormat getFormat()const
		{
			return m_swapChain->getFormat();
		}

		inline VkExtent2D getDimensions()const
		{
			return m_swapChain->getDimensions();
		}

	private:
		ashes::ImageViewCRefArray doPrepareAttaches( uint32_t backBuffer
			, ashes::VkAttachmentDescriptionArray const & attaches
			, ashes::Image const * depthImage )const;
		bool doCheckNeedReset( VkResult errCode
			, bool acquisition
			, char const * const action );
		void doResetSwapChain();

	public:
		using OnResetFunc = std::function< void() >;
		using OnReset = ashes::Signal< OnResetFunc >;
		//!\~english	The signal raised when the swapchain is reset.
		//!\~french		Le signal levé lorsque la swap chain est recréée.
		OnReset onReset;

	private:
		ashes::Device const & m_device;
		ashes::CommandPool const & m_commandPool;
		ashes::SurfacePtr m_surface;
		VkExtent2D m_dimensions;
		ashes::SwapChainPtr m_swapChain;
		mutable ashes::ImageViewArray m_swapChainImageViews;
		RenderingResourcesArray m_renderingResources;
		mutable size_t m_resourceIndex{ 0u };
	};

	using SwapChainPtr = std::unique_ptr< SwapChain >;
}

