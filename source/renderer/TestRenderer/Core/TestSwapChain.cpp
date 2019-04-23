#include "Core/TestSwapChain.hpp"

#include "Core/TestDevice.hpp"
#include "Core/TestPhysicalDevice.hpp"
#include "Core/TestInstance.hpp"
#include "Image/TestImage.hpp"

#include <Ashes/Miscellaneous/MemoryRequirements.hpp>

namespace test_renderer
{
	SwapChain::SwapChain( Device const & device
		, ashes::SwapChainCreateInfo createInfo )
		: ashes::SwapChain{ device, std::move( createInfo ) }
		, m_device{ device }
	{
	}

	SwapChain::~SwapChain()
	{
	}

	ashes::ImagePtrArray SwapChain::getImages()const
	{
		ashes::ImagePtrArray result;
		result.emplace_back( std::make_unique< Image >( m_device
			, getFormat()
			, getDimensions() ) );
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
}
