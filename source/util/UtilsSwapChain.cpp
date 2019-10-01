/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "util/UtilsSwapChain.hpp"

#include <ashespp/Command/Queue.hpp>
#include <ashespp/Core/Device.hpp>
#include <ashespp/Core/SwapChain.hpp>
#include <ashespp/Image/Image.hpp>
#include <ashespp/Image/ImageView.hpp>
#include <ashespp/Image/ImageViewCreateInfo.hpp>
#include <ashespp/RenderPass/FrameBuffer.hpp>
#include <ashespp/RenderPass/RenderPass.hpp>

#include <ashes/common/Exception.hpp>
#include <ashes/common/Format.hpp>

namespace utils
{
	namespace
	{
		uint32_t doGetImageCount( ashes::Surface const & surface )
		{
			auto surfaceCaps = surface.getCapabilities();
			uint32_t desiredNumberOfSwapChainImages{ surfaceCaps.minImageCount + 1 };

			if ( ( surfaceCaps.maxImageCount > 0 ) &&
				( desiredNumberOfSwapChainImages > surfaceCaps.maxImageCount ) )
			{
				// L'application doit s'initialiser avec moins d'images que voulu.
				desiredNumberOfSwapChainImages = surfaceCaps.maxImageCount;
			}

			return desiredNumberOfSwapChainImages;
		}

		VkSurfaceFormatKHR doSelectFormat( ashes::Surface const & surface )
		{
			VkSurfaceFormatKHR result;
			auto formats = surface.getFormats();
			// Si la liste de formats ne contient qu'une entr�e VK_FORMAT_UNDEFINED,
			// la surface n'a pas de format préféré. Sinon, au moins un format supporté
			// sera renvoyé.
			if ( formats.size() == 1u && formats[0].format == VK_FORMAT_UNDEFINED )
			{
				result.format = VK_FORMAT_R8G8B8A8_UNORM;
				result.colorSpace = VK_COLORSPACE_SRGB_NONLINEAR_KHR;
			}
			else
			{
				assert( !formats.empty() );
				auto it = std::find_if( formats.begin()
					, formats.end()
					, []( VkSurfaceFormatKHR const & lookup )
					{
						return lookup.format == VK_FORMAT_R8G8B8A8_UNORM;
					} );

				if ( it != formats.end() )
				{
					result = *it;
				}
				else
				{
					result = formats[0];
				}
			}

			return result;
		}

		VkPresentModeKHR doSelectPresentMode( ashes::Surface const & surface )
		{
			auto presentModes = surface.getPresentModes();
			// Si le mode boîte aux lettres est disponible, on utilise celui-là, car c'est celui avec le
			// minimum de latence dans tearing.
			// Sinon, on essaye le mode IMMEDIATE, qui est normalement disponible, et est le plus rapide
			// (bien qu'il y ait du tearing). Sinon on utilise le mode FIFO qui est toujours disponible.
			VkPresentModeKHR result{ VK_PRESENT_MODE_FIFO_KHR };

			for ( auto mode : presentModes )
			{
				if ( mode == VK_PRESENT_MODE_MAILBOX_KHR )
				{
					result = mode;
					break;
				}

				if ( ( result != VK_PRESENT_MODE_MAILBOX_KHR )
					&& ( mode == VK_PRESENT_MODE_IMMEDIATE_KHR ) )
				{
					result = mode;
				}
			}

			return result;
		}

		ashes::SwapChainCreateInfo doGetSwapChainCreateInfo( ashes::Surface const & surface
			, VkSurfaceCapabilitiesKHR const & surfaceCaps )
		{
			// Parfois, les images doivent être transformées avant d'être présentées (lorsque l'orientation
			// du périphérique est différente de l'orientation par défaut, par exemple).
			// Si la transformation spécifiée est différente de la transformation par défaut, le moteur de 
			// présentation va transformer l'image lors de la présentation. Cette opération peut avoir un
			// impact sur les performances sur certaines plateformes.
			// Ici, nous ne voulons aucune transformation, donc si la transformation identité est supportée,
			// nous l'utilisons, sinon nous utiliserons la même transformation que la transformation courante.
			VkSurfaceTransformFlagBitsKHR preTransform{};

			if ( ashes::checkFlag( surfaceCaps.supportedTransforms, VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR ) )
			{
				preTransform = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR;
			}
			else
			{
				preTransform = surfaceCaps.currentTransform;
			}

			auto presentMode = doSelectPresentMode( surface );
			auto surfaceFormat = doSelectFormat( surface );
			return ashes::SwapChainCreateInfo
			{
				0u,
				surface,
				doGetImageCount( surface ),
				surfaceFormat.format,
				surfaceFormat.colorSpace,
				surfaceCaps.currentExtent,
				1u,
				surfaceCaps.supportedUsageFlags,
				VK_SHARING_MODE_EXCLUSIVE,
				{},
				preTransform,
				VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR,
				presentMode,
				false,
				nullptr
			};
		}

		ashes::SwapChainCreateInfo doGetSwapChainResetInfo( ashes::Surface const & surface
			, VkExtent2D const & size )
		{
			auto surfaceCaps = surface.getCapabilities();
			surfaceCaps.currentExtent.width = std::min( size.width, surfaceCaps.maxImageExtent.width );
			surfaceCaps.currentExtent.height = std::min( size.height, surfaceCaps.maxImageExtent.height );
			surfaceCaps.currentExtent.width = std::max( surfaceCaps.currentExtent.width, surfaceCaps.minImageExtent.width );
			surfaceCaps.currentExtent.height = std::max( surfaceCaps.currentExtent.height, surfaceCaps.minImageExtent.height );
			return doGetSwapChainCreateInfo( surface, surfaceCaps );
		}

		ashes::SwapChainCreateInfo doGetSwapChainCreateInfo( ashes::Surface const & surface
			, VkExtent2D const & size )
		{
			auto surfaceCaps = surface.getCapabilities();

			// width et height valent soient tous les deux -1 ou tous les deux autre chose que -1.
			if ( surfaceCaps.currentExtent.width == uint32_t( -1 ) )
			{
				// Si les dimensions de la surface sont indéfinies, elles sont initialisées
				// aux dimensions des images requises.
				surfaceCaps.currentExtent = size;
			}
			else
			{
				// Si les dimensions de la surface sont définies, alors les dimensions de la swap chain
				// doivent correspondre.
			}

			return doGetSwapChainCreateInfo( surface, surfaceCaps );
		}

		ashes::ImageView doCloneView( ashes::ImageView const & view )
		{
			return view.image->createView( ashes::ImageViewCreateInfo
				{
					0u,
					*view.image,
					view->viewType,
					view->format,
					view->components,
					view->subresourceRange,
				} );
		}
	}

	SwapChain::SwapChain( ashes::Device const & device
		, ashes::CommandPool const & commandPool
		, ashes::SurfacePtr surface
		, VkExtent2D const & size )
		: m_device{ device }
		, m_commandPool{ commandPool }
		, m_surface{ std::move( surface ) }
		, m_dimensions{ size }
		, m_swapChain{ device.createSwapChain( doGetSwapChainCreateInfo( *m_surface, size ) ) }
		, m_swapChainImages{ m_swapChain->getImages() }
	{
		for ( uint32_t i = 0u; i < uint32_t( m_swapChainImages.size() ); ++i )
		{
			m_renderingResources.emplace_back( std::make_unique< RenderingResources >( m_device
				, *m_swapChain
				, m_commandPool ) );
		}
	}

	SwapChain::~SwapChain()
	{
		m_renderingResources.clear();
		m_swapChainImages.clear();
		m_swapChain.reset();
	}

	void SwapChain::reset( VkExtent2D const & size )
	{
		m_dimensions = size;
		doResetSwapChain();
	}

	ashes::FrameBufferPtrArray SwapChain::createFrameBuffers( ashes::RenderPass const & renderPass )const
	{
		ashes::FrameBufferPtrArray result;
		result.resize( m_swapChainImages.size() );

		for ( size_t i = 0u; i < result.size(); ++i )
		{
			auto attaches = doPrepareAttaches( uint32_t( i )
				, renderPass.getAttachments()
				, nullptr );
			result[i] = static_cast< ashes::RenderPass const & >( renderPass ).createFrameBuffer( m_swapChain->getDimensions()
				, std::move( attaches ) );
		}

		return result;
	}

	ashes::FrameBufferPtrArray SwapChain::createFrameBuffers( ashes::RenderPass const & renderPass
		, ashes::Image const & depthImage )const
	{
		ashes::FrameBufferPtrArray result;
		result.resize( m_swapChainImages.size() );

		for ( size_t i = 0u; i < result.size(); ++i )
		{
			auto attaches = doPrepareAttaches( uint32_t( i )
				, renderPass.getAttachments()
				, &depthImage );
			result[i] = static_cast< ashes::RenderPass const & >( renderPass ).createFrameBuffer( m_swapChain->getDimensions()
				, std::move( attaches ) );
		}

		return result;
	}

	ashes::CommandBufferPtrArray SwapChain::createCommandBuffers()const
	{
		ashes::CommandBufferPtrArray result;
		result.resize( m_swapChainImages.size() );

		for ( auto & commandBuffer : result )
		{
			commandBuffer = m_commandPool.createCommandBuffer( true );
		}

		return result;
	}

	RenderingResources * SwapChain::getResources()
	{
		auto & resources = *m_renderingResources[m_resourceIndex];
		m_resourceIndex = ( m_resourceIndex + 1 ) % m_renderingResources.size();

		if ( resources.waitRecord( ashes::MaxTimeout ) )
		{
			uint32_t imageIndex{ 0u };
			auto res = m_swapChain->acquireNextImage( std::numeric_limits< uint64_t >::max()
				, resources.getImageAvailableSemaphore()
				, imageIndex );

			if ( doCheckNeedReset( VkResult( res )
				, true
				, "Swap chain image acquisition" ) )
			{
				resources.setImageIndex( imageIndex );
				return &resources;
			}

			return nullptr;
		}

		ashes::Logger::logError( "Couldn't retrieve rendering resources" );
		return nullptr;
	}

	void SwapChain::present( RenderingResources & resources
		, ashes::Queue const & queue )
	{
		try
		{
			auto res = queue.present( { *m_swapChain }
				, { resources.getImageIndex() }
				, { resources.getRenderingFinishedSemaphore() } );
		}
		catch ( ashes::Exception & exc )
		{
			doCheckNeedReset( exc.getResult()
				, false
				, "Image presentation" );
		}

		resources.setImageIndex( ~0u );
	}

	ashes::ImageViewCRefArray SwapChain::doPrepareAttaches( uint32_t backBuffer
		, ashes::VkAttachmentDescriptionArray const & attaches
		, ashes::Image const * depthImage )const
	{
		ashes::ImageViewCRefArray result;

		for ( auto & attach : attaches )
		{
			auto & image = m_swapChainImages[backBuffer];

			if ( !ashes::isDepthOrStencilFormat( attach.format ) )
			{
				m_swapChainImageViews.emplace_back( image.createView( VkImageViewCreateInfo
					{
						VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
						nullptr,
						0u,
						image,
						VK_IMAGE_VIEW_TYPE_2D,
						m_swapChain->getFormat(),
						{},
						{
							ashes::getAspectMask( m_swapChain->getFormat() ),
							0u,
							1u,
							0u,
							1u,
						}
					} ) );
			}
			else
			{
				assert( depthImage
					&& "Asked for a depth stencil attachment in RenderPass, but no depth stencil view provided." );
				m_swapChainImageViews.emplace_back( depthImage->createView( VK_IMAGE_VIEW_TYPE_2D
					, depthImage->getFormat() ) );
			}

			result.emplace_back( m_swapChainImageViews.back() );
		}

		return result;
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
			throw ashes::Exception{ errCode, action };
			break;
		}

		return result;
	}

	void SwapChain::doResetSwapChain()
	{
		m_device.waitIdle();
		m_swapChainImages.clear();
		m_renderingResources.clear();
		m_swapChain.reset();
		m_swapChain = m_device.createSwapChain( doGetSwapChainResetInfo( *m_surface, m_dimensions ) );
		m_swapChainImages = m_swapChain->getImages();

		for ( uint32_t i = 0u; i < uint32_t( m_swapChainImages.size() ); ++i )
		{
			m_renderingResources.emplace_back( std::make_unique< RenderingResources >( m_device
				, *m_swapChain
				, m_commandPool ) );
		}

		onReset();
	}
}
