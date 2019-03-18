#include "RenderPanel.hpp"

#include "Application.hpp"
#include "MainFrame.hpp"

#include <Core/Surface.hpp>
#include <Core/Device.hpp>
#include <Core/Exception.hpp>
#include <RenderPass/FrameBuffer.hpp>
#include <RenderPass/RenderPass.hpp>
#include <RenderPass/RenderSubpass.hpp>
#include <RenderPass/RenderSubpassState.hpp>

#include <Transform.hpp>

#include <chrono>

namespace vkapp
{
	namespace
	{
		enum class Ids
		{
			This = 41,
			RenderTimer = 42
		};

		static int const TimerTimeMs = 40;

		uint32_t doGetImageCount( ashes::Device const & device )
		{
			auto & surface = device.getSurface();
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

		ashes::SurfaceFormat doSelectFormat( ashes::Device const & device )
		{
			auto & surface = device.getSurface();
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

		ashes::PresentMode doSelectPresentMode( ashes::Device const & device )
		{
			auto & surface = device.getSurface();
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

		ashes::SwapChainCreateInfo doGetSwapChainCreateInfo( ashes::Device const & device
			, ashes::Extent2D const & size )
		{
			ashes::Extent2D swapChainExtent{};
			auto & surface = device.getSurface();
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

			auto presentMode = doSelectPresentMode( device );
			auto surfaceFormat = doSelectFormat( device );
			return ashes::SwapChainCreateInfo
			{
				ashes::SwapChainCreateFlag::eNone,
				std::ref( surface ),
				doGetImageCount( device ),
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
			std::nullopt
			};
		}
	}

	RenderPanel::RenderPanel( wxWindow * parent
		, wxSize const & size
		, ashes::Instance const & instance )
		: wxPanel{ parent, wxID_ANY, wxDefaultPosition, size }
		, m_timer{ new wxTimer{ this, int( Ids::RenderTimer ) } }
	{
		try
		{
			auto surface = doCreateSurface( instance );
			std::cout << "Surface created." << std::endl;
			doCreateDevice( instance, std::move( surface ) );
			std::cout << "Logical device created." << std::endl;
			doCreateSwapChain();
			std::cout << "Swap chain created." << std::endl;
			doCreateRenderPass();
			std::cout << "Render pass created." << std::endl;
			doPrepareFrames();
			std::cout << "Frames prepared." << std::endl;
		}
		catch ( std::exception & )
		{
			doCleanup();
			throw;
		}

		m_timer->Start( TimerTimeMs );

		Connect( int( Ids::RenderTimer ), wxEVT_TIMER, wxTimerEventHandler( RenderPanel::onTimer ), nullptr, this );
		Connect( wxID_ANY, wxEVT_SIZE, wxSizeEventHandler( RenderPanel::onSize ), nullptr, this );
	}

	RenderPanel::~RenderPanel()
	{
		doCleanup();
	}

	void RenderPanel::doCleanup()
	{
		delete m_timer;

		if ( m_device )
		{
			m_device->waitIdle();
			m_commandPool.reset();
			m_presentQueue.reset();
			m_graphicsQueue.reset();
			m_commandBuffers.clear();
			m_renderingResources.clear();
			m_swapChain.reset();
			m_frameBuffers.clear();
			m_renderPass.reset();
			m_device.reset();
		}
	}

	ashes::SurfacePtr RenderPanel::doCreateSurface( ashes::Instance const & instance )
	{
		auto handle = common::makeWindowHandle( *this );
		auto & gpu = instance.getPhysicalDevice( 0u );
		return instance.createSurface( gpu
			, std::move( handle ) );
	}

	void RenderPanel::doInitialiseQueues( ashes::Instance const & instance
		, ashes::Surface const & surface )
	{
		auto & gpu = instance.getPhysicalDevice( 0u );
		// Parcours des propriétés des files, pour vérifier leur support de la présentation.
		std::vector< bool > supportsPresent( static_cast< uint32_t >( gpu.getQueueProperties().size() ) );
		uint32_t i{ 0u };
		m_graphicsQueueFamilyIndex = std::numeric_limits< uint32_t >::max();
		m_presentQueueFamilyIndex = std::numeric_limits< uint32_t >::max();

		for ( auto & present : supportsPresent )
		{
			auto present = surface.getSupport( i );

			if ( gpu.getQueueProperties()[i].queueCount > 0 )
			{
				if ( gpu.getQueueProperties()[i].queueFlags & ashes::QueueFlag::eGraphics )
				{
					// Tout d'abord on choisit une file graphique
					if ( m_graphicsQueueFamilyIndex == std::numeric_limits< uint32_t >::max() )
					{
						m_graphicsQueueFamilyIndex = i;
					}

					// Si une file supporte les graphismes et la présentation, on la préfère.
					if ( present )
					{
						m_graphicsQueueFamilyIndex = i;
						m_presentQueueFamilyIndex = i;
						break;
					}
				}
			}

			++i;
		}

		if ( m_presentQueueFamilyIndex == std::numeric_limits< uint32_t >::max() )
		{
			// Pas de file supportant les deux, on a donc 2 files distinctes.
			for ( size_t i = 0; i < gpu.getQueueProperties().size(); ++i )
			{
				if ( supportsPresent[i] )
				{
					m_presentQueueFamilyIndex = static_cast< uint32_t >( i );
					break;
				}
			}
		}

		// Si on n'en a pas trouvé, on génère une erreur.
		if ( m_graphicsQueueFamilyIndex == std::numeric_limits< uint32_t >::max()
			|| m_presentQueueFamilyIndex == std::numeric_limits< uint32_t >::max() )
		{
			throw ashes::Exception{ ashes::Result::eErrorInitializationFailed
				, "Queue families retrieval" };
		}
	}

	void RenderPanel::doCreateDevice( ashes::Instance const & instance
		, ashes::SurfacePtr surface )
	{
		doInitialiseQueues( instance, *surface );
		m_device = instance.createDevice( std::move( surface )
			, m_graphicsQueueFamilyIndex
			, m_presentQueueFamilyIndex );
		m_graphicsQueue = m_device->getQueue( m_graphicsQueueFamilyIndex, 0u );
		m_presentQueue = m_device->getQueue( m_presentQueueFamilyIndex, 0u );
		m_commandPool = m_device->createCommandPool( m_graphicsQueueFamilyIndex
			, ashes::CommandPoolCreateFlag::eResetCommandBuffer | ashes::CommandPoolCreateFlag::eTransient );
	}

	void RenderPanel::doCreateSwapChain()
	{
		wxSize size{ GetClientSize() };
		m_swapChain = m_device->createSwapChain( doGetSwapChainCreateInfo( *m_device
			, { uint32_t( size.x ), uint32_t( size.y ) } ) );
		m_clearColour = ashes::ClearColorValue{ 1.0f, 0.8f, 0.4f, 0.0f };
		doCreateRenderingResources();
	}

	void RenderPanel::doCreateRenderPass()
	{
		ashes::AttachmentDescriptionArray attaches
		{
			{
				m_swapChain->getFormat(),
				ashes::SampleCountFlag::e1,
				ashes::AttachmentLoadOp::eClear,
				ashes::AttachmentStoreOp::eStore,
				ashes::AttachmentLoadOp::eDontCare,
				ashes::AttachmentStoreOp::eDontCare,
				ashes::ImageLayout::eUndefined,
				ashes::ImageLayout::ePresentSrc,
			}
		};
		ashes::AttachmentReferenceArray subAttaches
		{
			{ 0u, ashes::ImageLayout::eColourAttachmentOptimal }
		};
		ashes::RenderSubpassPtrArray subpasses;
		subpasses.emplace_back( std::make_unique< ashes::RenderSubpass >( ashes::PipelineBindPoint::eGraphics
			, ashes::RenderSubpassState{ ashes::PipelineStageFlag::eColourAttachmentOutput
				, ashes::AccessFlag::eColourAttachmentWrite }
			, subAttaches ) );
		m_renderPass = m_device->createRenderPass( attaches
			, std::move( subpasses )
			, ashes::RenderSubpassState{ ashes::PipelineStageFlag::eBottomOfPipe
				, ashes::AccessFlag::eMemoryRead }
			, ashes::RenderSubpassState{ ashes::PipelineStageFlag::eBottomOfPipe
				, ashes::AccessFlag::eMemoryRead } );
	}

	bool RenderPanel::doPrepareFrames()
	{
		bool result{ true };
		doCreateCommandBuffers();
		doCreateFrameBuffers();

		wxSize size{ GetClientSize() };

		for ( size_t i = 0u; i < m_commandBuffers.size() && result; ++i )
		{
			auto & frameBuffer = *m_frameBuffers[i];
			auto & commandBuffer = *m_commandBuffers[i];

			commandBuffer.begin( ashes::CommandBufferUsageFlag::eSimultaneousUse );
			commandBuffer.beginRenderPass( *m_renderPass
				, frameBuffer
				, { m_clearColour }
				, ashes::SubpassContents::eInline );
			commandBuffer.endRenderPass();
			commandBuffer.end();
		}

		return result;
	}

	void RenderPanel::doCreateRenderingResources()
	{
		for ( uint32_t i = 0u; i < uint32_t( m_swapChain->getImages().size() ); ++i )
		{
			m_renderingResources.emplace_back( std::make_unique< RenderingResources >( m_device->createSemaphore()
				, m_device->createSemaphore()
				, m_device->createFence( ashes::FenceCreateFlag::eSignaled )
				, m_commandPool->createCommandBuffer()
				, 0u ) );
		}
	}

	void RenderPanel::doCreateFrameBuffers()
	{
		m_frameBuffers.resize( m_swapChain->getImages().size() );

		for ( size_t i = 0u; i < m_frameBuffers.size(); ++i )
		{
			auto attaches = doPrepareAttaches( uint32_t( i ) );
			m_frameBuffers[i] = m_renderPass->createBackBuffer( m_swapChain->getDimensions()
				, std::move( attaches ) );
		}
	}

	void RenderPanel::doCreateCommandBuffers()
	{
		m_commandBuffers.resize( m_swapChain->getImages().size() );

		for ( auto & commandBuffer : m_commandBuffers )
		{
			commandBuffer = m_commandPool->createCommandBuffer( true );
		}
	}

	ashes::FrameBufferAttachmentArray RenderPanel::doPrepareAttaches( uint32_t backBuffer )const
	{
		ashes::FrameBufferAttachmentArray attaches;

		for ( auto & attach : m_renderPass->getAttachments() )
		{
			if ( !ashes::isDepthOrStencilFormat( attach.format ) )
			{
				attaches.emplace_back( attach, m_swapChain->getImages()[backBuffer]->getView() );
			}
			else
			{
				attaches.emplace_back( attach, m_swapChain->getDepthStencilView() );
			}
		}

		return attaches;
	}

	void RenderPanel::doDraw()
	{
		auto resources = getResources();

		if ( resources )
		{
			auto before = std::chrono::high_resolution_clock::now();
			m_graphicsQueue->submit( *m_commandBuffers[resources->imageIndex]
				, *resources->imageAvailableSemaphore
				, ashes::PipelineStageFlag::eColourAttachmentOutput
				, *resources->finishedRenderingSemaphore
				, resources->fence.get() );

			try
			{
				auto res = m_presentQueue->present( { *m_swapChain }
					, { resources->imageIndex }
					, { *resources->finishedRenderingSemaphore } );
			}
			catch ( ashes::Exception & exc )
			{
				doCheckNeedReset( exc.getResult()
					, false
					, "Image presentation" );
			}

			resources->imageIndex = ~0u;

			auto after = std::chrono::high_resolution_clock::now();
			wxGetApp().updateFps( std::chrono::duration_cast< std::chrono::microseconds >( after - before ) );
		}
		else
		{
			m_timer->Stop();
			std::cerr << "Can't render" << std::endl;
		}
	}

	RenderPanel::RenderingResources * RenderPanel::getResources()
	{
		auto & resources = *m_renderingResources[m_resourceIndex];
		m_resourceIndex = ( m_resourceIndex + 1 ) % m_renderingResources.size();
		bool res = resources.fence->wait( ashes::FenceTimeout ) == ashes::WaitResult::eSuccess;

		if ( res )
		{
			resources.fence->reset();
			uint32_t imageIndex{ 0u };
			auto res = m_swapChain->acquireNextImage( std::numeric_limits< uint64_t >::max()
				, *resources.imageAvailableSemaphore
				, imageIndex );

			if ( doCheckNeedReset( ashes::Result( res )
				, true
				, "Swap chain image acquisition" ) )
			{
				resources.imageIndex = imageIndex;
				return &resources;
			}

			return nullptr;
		}

		ashes::Logger::logError( "Couldn't retrieve rendering resources" );
		return nullptr;
	}

	void RenderPanel::present( RenderingResources & resources )
	{
		try
		{
			auto res = m_presentQueue->present( { std::ref( *m_swapChain ) }
				, ashes::UInt32Array{ resources.imageIndex }
				, { std::ref( *resources.finishedRenderingSemaphore ) } );
		}
		catch ( ashes::Exception & exc )
		{
			doCheckNeedReset( exc.getResult()
				, false
				, "Image presentation" );
		}

		resources.imageIndex = ~0u;
	}

	bool RenderPanel::doCheckNeedReset( ashes::Result errCode
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

	void RenderPanel::doResetSwapChain()
	{
		m_device->waitIdle();
		doCreateSwapChain();
		doPrepareFrames();
	}

	void RenderPanel::onTimer( wxTimerEvent & event )
	{
		if ( event.GetId() == int( Ids::RenderTimer ) )
		{
			doDraw();
		}
	}

	void RenderPanel::onSize( wxSizeEvent & event )
	{
		m_timer->Stop();
		doResetSwapChain();
		m_timer->Start( TimerTimeMs );
		event.Skip();
	}
}
