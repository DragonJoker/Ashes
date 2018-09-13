#include "Core/VkSwapChain.hpp"

#include "Command/VkCommandBuffer.hpp"
#include "Command/VkCommandPool.hpp"
#include "Command/VkQueue.hpp"
#include "Core/VkBackBuffer.hpp"
#include "Core/VkDevice.hpp"
#include "Core/VkPhysicalDevice.hpp"
#include "Core/VkRenderer.hpp"
#include "Image/VkTexture.hpp"
#include "RenderPass/VkFrameBuffer.hpp"
#include "RenderPass/VkRenderPass.hpp"
#include "Sync/VkImageMemoryBarrier.hpp"
#include "Sync/VkSemaphore.hpp"

#include <Core/Exception.hpp>
#include <Core/RenderingResources.hpp>
#include <RenderPass/FrameBufferAttachment.hpp>

namespace vk_renderer
{
	SwapChain::SwapChain( Device const & device
		, ashes::Extent2D const & size )
		: ashes::SwapChain{ device, size }
		, m_device{ device }
		, m_surface{ device.getPresentSurface() }
	{
		// On choisit le format de la surface.
		doSelectFormat( static_cast< PhysicalDevice const & >( m_device.getPhysicalDevice() ) );

		// On crée la swap chain.
		doCreateSwapChain();

		// Puis les tampons d'images.
		doCreateBackBuffers();

		m_renderingResources.resize( 3 );

		for ( auto & resource : m_renderingResources )
		{
			resource = std::make_unique< ashes::RenderingResources >( device );
		}
	}

	SwapChain::~SwapChain()
	{
		m_backBuffers.clear();
		m_device.vkDestroySwapchainKHR( m_device, m_swapChain, nullptr );
	}

	void SwapChain::reset( ashes::Extent2D const & size )
	{
		m_dimensions = size;
		m_device.updateSurfaceCapabilities();
		doResetSwapChain();
	}

	void SwapChain::createDepthStencil( ashes::Format format )
	{
		m_depthStencil = m_device.createTexture(
			{
				0u,
				ashes::TextureType::e2D,
				format,
				ashes::Extent3D{ getDimensions().width, getDimensions().height, 1u },
				1u,
				1u,
				ashes::SampleCountFlag::e1,
				ashes::ImageTiling::eOptimal,
				ashes::ImageUsageFlag::eDepthStencilAttachment,
				ashes::SharingMode::eExclusive,
				{},
				ashes::ImageLayout::eUndefined,
			} );
		m_depthStencil->bindMemory( m_device.allocateMemory( m_depthStencil->getMemoryRequirements()
			, ashes::MemoryPropertyFlag::eDeviceLocal ) );
		m_depthStencilView = m_depthStencil->createView( ashes::TextureViewType::e2D
			, format );
	}

	ashes::FrameBufferAttachmentArray SwapChain::doPrepareAttaches( uint32_t backBuffer
		, ashes::AttachmentDescriptionArray const & attaches )const
	{
		ashes::FrameBufferAttachmentArray result;

		for ( auto & attach : attaches )
		{
			if ( !ashes::isDepthOrStencilFormat( attach.format ) )
			{
				result.emplace_back( attach, m_backBuffers[backBuffer]->getView() );
			}
			else
			{
				assert( m_depthStencilView );
				result.emplace_back( attach, *m_depthStencilView );
			}
		}

		return result;
	}

	ashes::FrameBufferPtrArray SwapChain::createFrameBuffers( ashes::RenderPass const & renderPass )const
	{
		ashes::FrameBufferPtrArray result;
		result.resize( m_backBuffers.size() );
		auto & surfaceCaps = m_device.getSurfaceCapabilities();

		for ( size_t i = 0u; i < result.size(); ++i )
		{
			auto attaches = doPrepareAttaches( uint32_t( i ), renderPass.getAttachments() );
			result[i] = static_cast< RenderPass const & >( renderPass ).createFrameBuffer( { surfaceCaps.currentExtent.width, surfaceCaps.currentExtent.height }
				, std::move( attaches ) );
		}

		return result;
	}

	ashes::CommandBufferPtrArray SwapChain::createCommandBuffers()const
	{
		ashes::CommandBufferPtrArray result;
		result.resize( m_backBuffers.size() );

		for ( auto & commandBuffer : result )
		{
			commandBuffer = std::make_unique< CommandBuffer >( m_device
				, static_cast< CommandPool const & >( m_device.getGraphicsCommandPool() )
				, true );
		}

		return result;
	}

	ashes::RenderingResources * SwapChain::getResources()
	{
		auto & resources = *m_renderingResources[m_resourceIndex];
		m_resourceIndex = ( m_resourceIndex + 1 ) % m_renderingResources.size();

		if ( resources.waitRecord( ashes::FenceTimeout ) )
		{
			uint32_t backBuffer{ 0u };
			auto res = m_device.vkAcquireNextImageKHR( m_device
				, m_swapChain
				, std::numeric_limits< uint64_t >::max()
				, static_cast< Semaphore const & >( resources.getImageAvailableSemaphore() )
				, VK_NULL_HANDLE
				, &backBuffer );

			if ( doCheckNeedReset( res, true, "Swap chain image acquisition" ) )
			{
				resources.setBackBuffer( backBuffer );
				return &resources;
			}

			return nullptr;
		}

		return nullptr;
	}

	void SwapChain::present( ashes::RenderingResources & resources )
	{
		auto res = static_cast< Queue const & >( m_device.getPresentQueue() ).presentBackBuffer( { *this }
			, { resources.getBackBuffer() }
			, { static_cast< Semaphore const & >( resources.getRenderingFinishedSemaphore() ) } );
		doCheckNeedReset( res, false, "Image presentation" );
		resources.setBackBuffer( ~0u );
	}

	uint32_t SwapChain::doGetImageCount()
	{
		auto & surfaceCaps = m_device.getSurfaceCapabilities();
		uint32_t desiredNumberOfSwapChainImages{ surfaceCaps.minImageCount + 1 };

		if ( ( surfaceCaps.maxImageCount > 0 ) &&
			( desiredNumberOfSwapChainImages > surfaceCaps.maxImageCount ) )
		{
			// L'application doit s'initialiser avec moins d'images que voulu.
			desiredNumberOfSwapChainImages = surfaceCaps.maxImageCount;
		}

		return desiredNumberOfSwapChainImages;
	}


	void SwapChain::doSelectFormat( VkPhysicalDevice gpu )
	{
		auto & formats = m_device.getConnection().getSurfaceFormats();
		// Si la liste de formats ne contient qu'une entrée VK_FORMAT_UNDEFINED,
		// la surface n'a pas de format préféré. Sinon, au moins un format supporté
		// sera renvoyé.
		if ( formats.size() == 1u && formats[0].format == ashes::Format::eUndefined )
		{
			m_format = ashes::Format::eR8G8B8A8_UNORM;
		}
		else
		{
			assert( formats.size() > 1u );
			m_format = formats[0].format;
		}

		m_colorSpace = convert( formats[0].colorSpace );
	}

	VkPresentModeKHR SwapChain::doSelectPresentMode()
	{
		auto & presentModes = m_device.getConnection().getPresentModes();
		// Si le mode bo�te aux lettres est disponible, on utilise celui-là, car c'est celui avec le
		// minimum de latence dans tearing.
		// Sinon, on essaye le mode IMMEDIATE, qui est normalement disponible, et est le plus rapide
		// (bien qu'il y ait du tearing). Sinon on utilise le mode FIFO qui est toujours disponible.
		VkPresentModeKHR swapchainPresentMode{ VK_PRESENT_MODE_FIFO_KHR };

		for ( auto mode : presentModes )
		{
			if ( mode == ashes::PresentMode::eMailbox )
			{
				swapchainPresentMode = VK_PRESENT_MODE_MAILBOX_KHR;
				break;
			}

			if ( ( swapchainPresentMode != VK_PRESENT_MODE_MAILBOX_KHR ) &&
				( mode == ashes::PresentMode::eImmediate ) )
			{
				swapchainPresentMode = VK_PRESENT_MODE_IMMEDIATE_KHR;
			}
		}

		return swapchainPresentMode;
	}

	void SwapChain::doCreateSwapChain()
	{
		VkExtent2D swapChainExtent{};
		auto & surfaceCaps = m_device.getSurfaceCapabilities();

		// width et height valent soient tous les deux -1 ou tous les deux autre chose que -1.
		if ( surfaceCaps.currentExtent.width == uint32_t( -1 ) )
		{
			// Si les dimensions de la surface sont indéfinies, elles sont initialisées
			// aux dimensions des images requises.
			swapChainExtent = convert( m_dimensions );
		}
		else
		{
			// Si les dimensions de la surface sont définies, alors les dimensions de la swap chain
			// doivent correspondre.
			swapChainExtent = convert( surfaceCaps.currentExtent );
		}

		// Parfois, les images doivent être transformées avant d'être présentées (lorsque l'orientation
		// du périphérique est différente de l'orientation par défaut, par exemple).
		// Si la transformation spécifiée est différente de la transformation par défaut, le moteur de 
		// présentation va transformer l'image lors de la présentation. Cette opération peut avoir un
		// impact sur les performances sur certaines plateformes.
		// Ici, nous ne voulons aucune transformation, donc si la transformation identité est supportée,
		// nous l'utilisons, sinon nous utiliserons la même transformation que la transformation courante.
		VkSurfaceTransformFlagBitsKHR preTransform{};

		if ( checkFlag( surfaceCaps.supportedTransforms, ashes::SurfaceTransformFlag::eIdentity ) )
		{
			preTransform = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR;
		}
		else
		{
			preTransform = convert( surfaceCaps.currentTransform );
		}

		VkSwapchainKHR oldSwapChain{ VK_NULL_HANDLE };
		m_presentMode = convert( doSelectPresentMode() );

		VkSwapchainCreateInfoKHR createInfo
		{
			VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR,
			nullptr,
			0,                                                                        // flags
			m_surface,                                                                // surface
			doGetImageCount(),                                                        // minImageCount
			convert( m_format ),                                                      // imageFormat
			m_colorSpace,                                                             // imageColorSpace
			swapChainExtent,                                                          // imageExtent
			1,                                                                        // imageArrayLayers
			VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,                                      // imageUsage
			VK_SHARING_MODE_EXCLUSIVE,                                                // imageSharingMode
			0,                                                                        // queueFamilyIndexCount
			nullptr,                                                                  // pQueueFamilyIndices
			preTransform,                                                             // preTransform
			VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR,                                        // compositeAlpha
			convert( m_presentMode ),                                                 // presentMode
			true,                                                                     // clipped
			oldSwapChain,                                                             // oldSwapchain
		};
		DEBUG_DUMP( createInfo );
		auto res = m_device.vkCreateSwapchainKHR( m_device
			, &createInfo
			, nullptr
			, &m_swapChain );
		checkError( res, "Swap chain creation" );

		// On supprime la précédente swap chain au cas où il y en avait une.
		if ( oldSwapChain != VK_NULL_HANDLE )
		{
			m_device.vkDestroySwapchainKHR( m_device
				, oldSwapChain
				, nullptr );
		}
	}

	void SwapChain::doCreateBackBuffers()
	{
		// On récupère les images de la swapchain.
		uint32_t imageCount{ 0u };
		auto res = m_device.vkGetSwapchainImagesKHR( m_device
			, m_swapChain
			, &imageCount
			, nullptr );
		checkError( res, "Swap chain images count retrieval" );

		std::vector< VkImage > swapChainImages( imageCount );
		res = m_device.vkGetSwapchainImagesKHR( m_device
			, m_swapChain
			, &imageCount
			, &swapChainImages[0] );
		checkError( res, "Swap chain images retrieval" );

		// Et on crée des BackBuffers à partir de ces images.
		m_backBuffers.reserve( imageCount );
		uint32_t index{ 0u };

		for ( auto image : swapChainImages )
		{
			auto texture = std::make_unique< Texture >( m_device
				, m_format
				, m_dimensions
				, image );
			auto & ref = *texture;
			m_backBuffers.emplace_back( std::make_unique< BackBuffer >( m_device
				, std::move( texture )
				, index++
				, m_format
				, ref ) );
		}
	}

	bool SwapChain::doCheckNeedReset( VkResult errCode
		, bool acquisition
		, char const * const action )
	{
		bool result{ false };

		switch ( errCode )
		{
		case VK_SUCCESS:
			result = true;
			break;

		case VK_ERROR_OUT_OF_DATE_KHR:
			if ( !acquisition )
			{
				doResetSwapChain();
			}
			else
			{
				result = true;
			}
			break;

		case VK_SUBOPTIMAL_KHR:
			doResetSwapChain();
			break;

		default:
			checkError( errCode, action );
			break;
		}

		return result;
	}

	void SwapChain::doResetSwapChain()
	{
		m_device.waitIdle();
		auto colour = m_clearColour;
		m_backBuffers.clear();
		m_device.vkDestroySwapchainKHR( m_device, m_swapChain, nullptr );
		m_renderingResources.clear();
		// On choisit le format de la surface.
		doSelectFormat( static_cast< PhysicalDevice const & >( m_device.getPhysicalDevice() ) );
		// On crée la swap chain.
		doCreateSwapChain();
		// Puis les tampons d'images.
		doCreateBackBuffers();

		m_renderingResources.resize( 3 );

		for ( auto & resource : m_renderingResources )
		{
			resource = std::make_unique< ashes::RenderingResources >( m_device );
		}

		onReset();
	}
}
