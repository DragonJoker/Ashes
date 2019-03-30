#include "Descriptor/TestDescriptorSet.hpp"

#include "Buffer/TestBuffer.hpp"
#include "Buffer/TestBufferView.hpp"
#include "Core/TestDevice.hpp"
#include "Descriptor/TestDescriptorSetLayout.hpp"
#include "Descriptor/TestDescriptorPool.hpp"
#include "Image/TestSampler.hpp"
#include "Image/TestImageView.hpp"

#include <Ashes/Buffer/UniformBuffer.hpp>

namespace test_renderer
{
	DescriptorSet::DescriptorSet( Device const & device
		, DescriptorPool const & pool
		, DescriptorSetLayout const & layout
		, uint32_t bindingPoint )
		: ashes::DescriptorSet{ pool, bindingPoint }
		, m_pool{ pool }
		, m_layout{ layout }
	{
	}

	DescriptorSet::~DescriptorSet()
	{
	}

	void DescriptorSet::update()const
	{
	}
}
