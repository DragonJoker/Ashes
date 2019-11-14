#include "RenderPanel.hpp"

#include "Application.hpp"
#include "MainFrame.hpp"

#include <util/Transform.hpp>

#include <ashespp/Core/Surface.hpp>
#include <ashespp/Core/Device.hpp>
#include <ashespp/Image/Image.hpp>
#include <ashespp/Image/ImageView.hpp>
#include <ashespp/RenderPass/FrameBuffer.hpp>
#include <ashespp/RenderPass/RenderPass.hpp>

#include <ashes/common/Exception.hpp>

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

		ashes::SwapChainCreateInfo doGetSwapChainCreateInfo( ashes::Device const & device
			, ashes::Surface const & surface
			, VkExtent2D const & size )
		{
			VkExtent2D swapChainExtent{};
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
				swapChainExtent,
				1u,
				VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
				VK_SHARING_MODE_EXCLUSIVE,
				{},
				preTransform,
				VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR,
				presentMode,
				VK_TRUE,
				nullptr
			};
		}
	}

	RenderPanel::RenderPanel( wxWindow * parent
		, wxSize const & size
		, utils::Instance const & instance )
		: wxPanel{ parent, wxID_ANY, wxDefaultPosition, size }
		, m_timer{ new wxTimer{ this, int( Ids::RenderTimer ) } }
	{
		try
		{
			doCreateSurface( instance );
			std::cout << "Surface created." << std::endl;
			doCreateDevice( instance );
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
			m_device->getDevice().waitIdle();
			m_presentQueue.reset();
			m_graphicsQueue.reset();
			m_commandBuffers.clear();
			m_renderingResources.clear();
			m_commandPool.reset();
			m_frameBuffers.clear();
			m_renderPass.reset();
			m_swapChainImages.clear();
			m_swapChain.reset();
			m_device.reset();
		}
	}

	void RenderPanel::doCreateSurface( utils::Instance const & instance )
	{
		auto handle = common::makeWindowHandle( *this );
		auto & gpu = instance.getPhysicalDevice( 0u );
		m_surface = instance.getInstance().createSurface( gpu
			, std::move( handle ) );
	}

	void RenderPanel::doCreateDevice( utils::Instance const & instance )
	{
		m_device = std::make_unique< utils::Device >( instance.getInstance()
			, *m_surface );
		m_graphicsQueue = m_device->getDevice().getQueue( m_device->getGraphicsQueueFamily(), 0u );
		m_presentQueue = m_device->getDevice().getQueue( m_device->getPresentQueueFamily(), 0u );
		m_commandPool = m_device->getDevice().createCommandPool( m_device->getGraphicsQueueFamily()
			, VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT | VK_COMMAND_POOL_CREATE_TRANSIENT_BIT );
	}

	void RenderPanel::doCreateSwapChain()
	{
		wxSize size{ GetClientSize() };
		m_swapChain = m_device->getDevice().createSwapChain( doGetSwapChainCreateInfo( m_device->getDevice()
			, *m_surface
			, { uint32_t( size.x ), uint32_t( size.y ) } ) );
		m_swapChainImages = m_swapChain->getImages();
		m_clearColour = VkClearColorValue{ 1.0f, 0.8f, 0.4f, 0.0f };
		doCreateRenderingResources();
	}

	void RenderPanel::doCreateRenderPass()
	{
		ashes::VkAttachmentDescriptionArray attaches
		{
			{
				0u,
				m_swapChain->getFormat(),
				VK_SAMPLE_COUNT_1_BIT,
				VK_ATTACHMENT_LOAD_OP_CLEAR,
				VK_ATTACHMENT_STORE_OP_STORE,
				VK_ATTACHMENT_LOAD_OP_DONT_CARE,
				VK_ATTACHMENT_STORE_OP_DONT_CARE,
				VK_IMAGE_LAYOUT_UNDEFINED,
				VK_IMAGE_LAYOUT_PRESENT_SRC_KHR,
			}
		};
		ashes::SubpassDescriptionArray subpasses;
		subpasses.emplace_back( ashes::SubpassDescription
			{
				0u,
				VK_PIPELINE_BIND_POINT_GRAPHICS,
				{},
				{ { 0u, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL } },
				{},
				ashes::nullopt,
				{},
			} );
		ashes::VkSubpassDependencyArray dependencies
		{
			{
				VK_SUBPASS_EXTERNAL,
				0u,
				VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT,
				VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
				VK_ACCESS_MEMORY_READ_BIT,
				VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT | VK_ACCESS_COLOR_ATTACHMENT_READ_BIT,
				VK_DEPENDENCY_BY_REGION_BIT,
			},
			{
				0u,
				VK_SUBPASS_EXTERNAL,
				VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
				VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT,
				VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT | VK_ACCESS_COLOR_ATTACHMENT_READ_BIT,
				VK_ACCESS_MEMORY_READ_BIT,
				VK_DEPENDENCY_BY_REGION_BIT,
			}
		};
		ashes::RenderPassCreateInfo createInfo
		{
			0u,
			std::move( attaches ),
			std::move( subpasses ),
			std::move( dependencies ),
		};
		m_renderPass = m_device->getDevice().createRenderPass( std::move( createInfo ) );
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

			commandBuffer.begin( VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT );
			commandBuffer.beginRenderPass( *m_renderPass
				, frameBuffer
				, { { m_clearColour } }
				, VK_SUBPASS_CONTENTS_INLINE );
			commandBuffer.endRenderPass();
			commandBuffer.end();
		}

		return result;
	}

	void RenderPanel::doCreateRenderingResources()
	{
		for ( uint32_t i = 0u; i < uint32_t( m_swapChainImages.size() ); ++i )
		{
			m_renderingResources.emplace_back( std::make_unique< RenderingResources >( m_device->getDevice().createSemaphore()
				, m_device->getDevice().createSemaphore()
				, m_device->getDevice().createFence( VK_FENCE_CREATE_SIGNALED_BIT )
				, m_commandPool->createCommandBuffer()
				, 0u ) );
		}
	}

	void RenderPanel::doCreateFrameBuffers()
	{
		m_frameBuffers.resize( m_swapChainImages.size() );
		m_views.resize( m_frameBuffers.size() );

		for ( size_t i = 0u; i < m_frameBuffers.size(); ++i )
		{
			auto attaches = doPrepareAttaches( uint32_t( i ), m_views[i] );
			m_frameBuffers[i] = m_renderPass->createFrameBuffer( m_swapChain->getDimensions()
				, std::move( attaches ) );
		}
	}

	void RenderPanel::doCreateCommandBuffers()
	{
		m_commandBuffers.resize( m_swapChainImages.size() );

		for ( auto & commandBuffer : m_commandBuffers )
		{
			commandBuffer = m_commandPool->createCommandBuffer();
		}
	}

	ashes::ImageViewCRefArray RenderPanel::doPrepareAttaches( uint32_t backBuffer
		, ashes::ImageViewArray & views )const
	{
		views.clear();

		for ( auto & attach : m_renderPass->getAttachments() )
		{
			views.push_back( m_swapChainImages[backBuffer].createView( VkImageViewCreateInfo
				{
						VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
						nullptr,
						0u,
						m_swapChainImages[backBuffer],
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

		ashes::ImageViewCRefArray attaches;

		for ( auto & view : views )
		{
			attaches.emplace_back( view );
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
				, VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT
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
		bool res = resources.fence->wait( ashes::MaxTimeout ) == ashes::WaitResult::eSuccess;

		if ( res )
		{
			resources.fence->reset();
			uint32_t imageIndex{ 0u };
			auto res = m_swapChain->acquireNextImage( ashes::MaxTimeout
				, *resources.imageAvailableSemaphore
				, imageIndex );

			if ( doCheckNeedReset( VkResult( res )
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
			auto res = m_presentQueue->present( ashes::SwapChainCRefArray{ { std::ref( *m_swapChain ) } }
				, ashes::UInt32Array{ { resources.imageIndex } }
				, ashes::SemaphoreCRefArray{ { std::ref( *resources.finishedRenderingSemaphore ) } } );
		}
		catch ( ashes::Exception & exc )
		{
			doCheckNeedReset( exc.getResult()
				, false
				, "Image presentation" );
		}

		resources.imageIndex = ~0u;
	}

	bool RenderPanel::doCheckNeedReset( VkResult errCode
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

	void RenderPanel::doResetSwapChain()
	{
		m_device->getDevice().waitIdle();
		m_frameBuffers.clear();
		m_commandBuffers.clear();
		m_renderingResources.clear();
		m_swapChainImages.clear();
		m_swapChain.reset();
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
