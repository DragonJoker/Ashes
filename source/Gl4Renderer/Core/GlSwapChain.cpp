#include "Core/GlSwapChain.hpp"

#include "Core/GlDevice.hpp"
#include "Image/GlImage.hpp"
#include "Image/GlImageView.hpp"
#include "RenderPass/GlFrameBuffer.hpp"

namespace gl_renderer
{
	namespace
	{
		std::unique_ptr< Image > createImage( Device const & device
			, ashes::Format format
			, ashes::Extent2D dimensions )
		{
			auto result = std::make_unique< Image >( device
				, format
				, std::move( dimensions ) );
			auto requirements = result->getMemoryRequirements();
			uint32_t deduced = deduceMemoryType( requirements.memoryTypeBits
				, ashes::MemoryPropertyFlag::eDeviceLocal );
			result->bindMemory( device.allocateMemory( { requirements.size, deduced } ) );
			return result;
		}

		std::unique_ptr< ImageView > createImageView( Device const & device
			, Image const & image
			, ashes::Format format )
		{
			return std::make_unique< ImageView >( device
				, image
				, ashes::ImageViewCreateInfo
				{
					ashes::ImageViewType::e2D,
					format,
					ashes::ComponentMapping{},
					ashes::ImageSubresourceRange{ ashes::ImageAspectFlag::eColour },
				} );
		}
	}

	SwapChain::SwapChain( Device const & device
		, ashes::SwapChainCreateInfo createInfo )
		: ashes::SwapChain{ device, std::move( createInfo ) }
		, m_device{ device }
		, m_image{ createImage( device, getFormat(), getDimensions() ) }
		, m_view{ createImageView( device, *m_image, getFormat() ) }
	{
		auto context = m_device.getContext();
		glLogCall( context
			, glGenFramebuffers
			, 1
			, &m_fbo );
		glLogCall( context
			, glBindFramebuffer
			, GL_FRAMEBUFFER
			, m_fbo );
		glLogCall( context
			, glFramebufferTexture2D
			, GL_FRAMEBUFFER
			, GL_ATTACHMENT_POINT_COLOR0
			, GL_TEXTURE_2D
			, m_view->getInternal()
			, 0u );
		checkCompleteness( context->glCheckFramebufferStatus( GL_FRAMEBUFFER ) );
		glLogCall( context
			, glBindFramebuffer
			, GL_FRAMEBUFFER
			, 0 );
	}

	SwapChain::~SwapChain()
	{
		auto context = m_device.getContext();
		glLogCall( context
			, glDeleteFramebuffers
			, 1
			, &m_fbo );
	}

	ashes::ImagePtrArray SwapChain::getImages()const
	{
		ashes::ImagePtrArray result;
		result.emplace_back( std::make_unique< Image >( m_device, *m_image ) );
		return result;
	}

	ashes::Result SwapChain::acquireNextImage( uint64_t timeout
		, ashes::Semaphore const * semaphore
		, ashes::Fence const * fence
		, uint32_t & imageIndex )const
	{
		imageIndex = 0u;
		return ashes::Result::eSuccess;
	}

	void SwapChain::present( uint32_t imageIndex )const
	{
		auto context = m_device.getContext();
		glLogCall( context
			, glBindFramebuffer
			, GL_READ_FRAMEBUFFER
			, m_fbo );
		glLogCall( context
			, glBindFramebuffer
			, GL_DRAW_FRAMEBUFFER
			, 0 );
		glLogCall( context
			, glBlitFramebuffer
			, 0, 0, getDimensions().width, getDimensions().height
			, 0, 0, getDimensions().width, getDimensions().height
			, GL_COLOR_BUFFER_BIT, GL_FILTER_NEAREST );
		glLogCall( context
			, glBindFramebuffer
			, GL_READ_FRAMEBUFFER
			, 0 );
		context->swapBuffers();
	}
}
