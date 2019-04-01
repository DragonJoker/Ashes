/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "Utils/UtilsSwapChain.hpp"

#include <Ashes/Command/Queue.hpp>
#include <Ashes/Core/Device.hpp>
#include <Ashes/Core/Exception.hpp>
#include <Ashes/Core/SwapChain.hpp>
#include <Ashes/Image/Image.hpp>
#include <Ashes/Miscellaneous/SwapChainCreateInfo.hpp>
#include <Ashes/RenderPass/FrameBuffer.hpp>
#include <Ashes/RenderPass/FrameBufferAttachment.hpp>
#include <Ashes/RenderPass/RenderPass.hpp>

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

		ashes::SurfaceFormat doSelectFormat( ashes::Surface const & surface )
		{
			ashes::SurfaceFormat result;
			auto formats = surface.getFormats();
			// Si la liste de formats ne contient qu'une entr�e VK_FORMAT_UNDEFINED,
			// la surface n'a pas de format préféré. Sinon, au moins un format supporté
			// sera renvoyé.
			if ( formats.size() == 1u && formats[0].format == ashes::Format::eUndefined )
			{
				result.format = ashes::Format::eR8G8B8A8_UNORM;
				result.colorSpace = ashes::ColorSpace::eSRGBNonLinear;
			}
			else
			{
				assert( formats.size() > 1u );
				auto it = std::find_if( formats.begin()
					, formats.end()
					, []( ashes::SurfaceFormat const & lookup )
					{
						return lookup.format == ashes::Format::eR8G8B8A8_UNORM;
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

		ashes::PresentMode doSelectPresentMode( ashes::Surface const & surface )
		{
			auto presentModes = surface.getPresentModes();
			// Si le mode boîte aux lettres est disponible, on utilise celui-là, car c'est celui avec le
			// minimum de latence dans tearing.
			// Sinon, on essaye le mode IMMEDIATE, qui est normalement disponible, et est le plus rapide
			// (bien qu'il y ait du tearing). Sinon on utilise le mode FIFO qui est toujours disponible.
			ashes::PresentMode result{ ashes::PresentMode::eFifo };

			for ( auto mode : presentModes )
			{
				if ( mode == ashes::PresentMode::eMailbox )
				{
					result = mode;
					break;
				}

				if ( ( result != ashes::PresentMode::eMailbox )
					&& ( mode == ashes::PresentMode::eImmediate ) )
				{
					result = mode;
				}
			}

			return result;
		}

		ashes::SwapChainCreateInfo doGetSwapChainCreateInfo( ashes::Surface const & surface
			, ashes::Extent2D const & size )
		{
			ashes::Extent2D swapChainExtent{};
			auto surfaceCaps = surface.getCapabilities();

			// width et height valent soient tous les deux -1 ou tous les deux autre chose que -1.
			if ( surfaceCaps.currentExtent.width == uint32_t( -1 ) )
			{
				// Si les dimensions de la surface sont indéfinies, elles sont initialisées
				// aux dimensions des images requises.
				swapChainExtent = size;
			}
			else
			{
				// Si les dimensions de la surface sont définies, alors les dimensions de la swap chain
				// doivent correspondre.
				swapChainExtent = surfaceCaps.currentExtent;
			}

			// Parfois, les images doivent être transformées avant d'être présentées (lorsque l'orientation
			// du périphérique est différente de l'orientation par défaut, par exemple).
			// Si la transformation spécifiée est différente de la transformation par défaut, le moteur de 
			// présentation va transformer l'image lors de la présentation. Cette opération peut avoir un
			// impact sur les performances sur certaines plateformes.
			// Ici, nous ne voulons aucune transformation, donc si la transformation identité est supportée,
			// nous l'utilisons, sinon nous utiliserons la même transformation que la transformation courante.
			ashes::SurfaceTransformFlag preTransform{};

			if ( checkFlag( surfaceCaps.supportedTransforms, ashes::SurfaceTransformFlag::eIdentity ) )
			{
				preTransform = ashes::SurfaceTransformFlag::eIdentity;
			}
			else
			{
				preTransform = surfaceCaps.currentTransform;
			}

			auto presentMode = doSelectPresentMode( surface );
			auto surfaceFormat = doSelectFormat( surface );
			return ashes::SwapChainCreateInfo
			{
				ashes::SwapChainCreateFlag::eNone,
				std::ref( surface ),
				doGetImageCount( surface ),
				surfaceFormat.format,
				surfaceFormat.colorSpace,
				swapChainExtent,
				1u,
				surfaceCaps.supportedUsageFlags,
				ashes::SharingMode::eExclusive,
				{},
				preTransform,
				ashes::CompositeAlphaFlag::eOpaque,
				presentMode,
				false,
				ashes::nullopt
			};
		}

		ashes::ImageViewPtr doCloneView( ashes::ImageView const & view )
		{
			return view.getImage().createView(
				{
					view.getType(),
					view.getFormat(),
					view.getComponentMapping(),
					view.getSubResourceRange(),
				} );
		}
	}

	SwapChain::SwapChain( ashes::Device const & device
		, ashes::CommandPool const & commandPool
		, ashes::SurfacePtr surface
		, ashes::Extent2D const & size )
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

	void SwapChain::reset( ashes::Extent2D const & size )
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
			auto attaches = doPrepareAttaches( uint32_t( i ), renderPass.getAttachments(), nullptr );
			result[i] = static_cast< ashes::RenderPass const & >( renderPass ).createFrameBuffer( m_swapChain->getDimensions()
				, std::move( attaches ) );
		}

		return result;
	}

	ashes::FrameBufferPtrArray SwapChain::createFrameBuffers( ashes::RenderPass const & renderPass
		, ashes::ImageViewPtr depthStencilView )const
	{
		ashes::FrameBufferPtrArray result;
		result.resize( m_swapChainImages.size() );

		for ( size_t i = 0u; i < result.size(); ++i )
		{
			auto attaches = doPrepareAttaches( uint32_t( i ), renderPass.getAttachments(), depthStencilView );
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

		if ( resources.waitRecord( ashes::FenceTimeout ) )
		{
			uint32_t imageIndex{ 0u };
			auto res = m_swapChain->acquireNextImage( std::numeric_limits< uint64_t >::max()
				, resources.getImageAvailableSemaphore()
				, imageIndex );

			if ( doCheckNeedReset( ashes::Result( res )
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

	ashes::FrameBufferAttachmentArray SwapChain::doPrepareAttaches( uint32_t backBuffer
		, ashes::AttachmentDescriptionArray const & attaches
		, ashes::ImageViewPtr depthStencilView )const
	{
		ashes::FrameBufferAttachmentArray result;

		for ( auto & attach : attaches )
		{
			auto & image = *m_swapChainImages[backBuffer];

			if ( !ashes::isDepthOrStencilFormat( attach.format ) )
			{
				result.emplace_back( attach
					, image.createView( ashes::ImageViewType::e2D
						, m_swapChain->getFormat() ) );
			}
			else
			{
				assert( depthStencilView
					&& "Asked for a depth stencil attachment in RenderPass, but no depth stencil view provided." );
				result.emplace_back( attach
					, std::move( depthStencilView ) );
			}
		}

		return result;
	}

	bool SwapChain::doCheckNeedReset( ashes::Result errCode
		, bool acquisition
		, char const * const action )
	{
		bool result{ false };

		switch ( errCode )
		{
		case ashes::Result::eSuccess:
			result = true;
			break;

		case ashes::Result::eErrorOutOfDate:
			if ( !acquisition )
			{
				doResetSwapChain();
			}
			else
			{
				result = true;
			}
			break;

		case ashes::Result::eSubOptimal:
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
		m_swapChain = m_device.createSwapChain( doGetSwapChainCreateInfo( *m_surface, m_dimensions ) );
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
