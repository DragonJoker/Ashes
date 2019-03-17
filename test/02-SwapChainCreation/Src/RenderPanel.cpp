#include "RenderPanel.hpp"

#include "Application.hpp"
#include "MainFrame.hpp"

#include <Core/Surface.hpp>
#include <Core/Device.hpp>
#include <Core/Exception.hpp>
#include <Core/RenderingResources.hpp>
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
		m_swapChain = m_device->createSwapChain( *m_commandPool
			, { uint32_t( size.x ), uint32_t( size.y ) } );
		m_swapChain->setClearColour( ashes::ClearColorValue{ 1.0f, 0.8f, 0.4f, 0.0f } );
		m_swapChainReset = m_swapChain->onReset.connect( [this]()
		{
			doResetSwapChain();
		} );
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
		m_commandBuffers = m_swapChain->createCommandBuffers( *m_commandPool );
		m_frameBuffers = m_swapChain->createFrameBuffers( *m_renderPass );
		wxSize size{ GetClientSize() };

		for ( size_t i = 0u; i < m_commandBuffers.size() && result; ++i )
		{
			auto & frameBuffer = *m_frameBuffers[i];
			auto & commandBuffer = *m_commandBuffers[i];

			commandBuffer.begin( ashes::CommandBufferUsageFlag::eSimultaneousUse );
			commandBuffer.beginRenderPass( *m_renderPass
				, frameBuffer
				, { m_swapChain->getClearColour() }
				, ashes::SubpassContents::eInline );
			commandBuffer.endRenderPass();
			commandBuffer.end();
		}

		return result;
	}

	void RenderPanel::doDraw()
	{
		auto resources = m_swapChain->getResources();

		if ( resources )
		{
			auto before = std::chrono::high_resolution_clock::now();
			m_graphicsQueue->submit( *m_commandBuffers[resources->getBackBuffer()]
				, resources->getImageAvailableSemaphore()
				, ashes::PipelineStageFlag::eColourAttachmentOutput
				, resources->getRenderingFinishedSemaphore()
				, &resources->getFence() );
			m_swapChain->present( *resources, *m_presentQueue );

			auto after = std::chrono::high_resolution_clock::now();
			wxGetApp().updateFps( std::chrono::duration_cast< std::chrono::microseconds >( after - before ) );
		}
		else
		{
			m_timer->Stop();
			std::cerr << "Can't render" << std::endl;
		}
	}

	void RenderPanel::doResetSwapChain()
	{
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
		wxSize size = GetClientSize();
		m_swapChain->reset( { uint32_t( size.x ), uint32_t( size.y ) } );
		m_timer->Start( TimerTimeMs );
		event.Skip();
	}
}
