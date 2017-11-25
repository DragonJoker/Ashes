#include "DescriptorSetLayoutBinding.hpp"
#include "UniformBuffer.hpp"

namespace renderer
{
	template< typename T >
	void DescriptorSet::createBinding( DescriptorSetLayoutBinding const & layoutBinding
		, UniformBuffer< T > const & uniformBuffer )
	{
		m_descriptorSet->createBinding( layoutBinding.getBinding()
			, uniformBuffer.getUbo() );
	}
}
