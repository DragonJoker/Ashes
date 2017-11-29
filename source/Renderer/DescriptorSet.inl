#include "DescriptorSetLayoutBinding.hpp"
#include "UniformBuffer.hpp"

namespace renderer
{
	template< typename T >
	UniformBufferBinding< T > DescriptorSet::createBinding( DescriptorSetLayoutBinding const & layoutBinding
		, UniformBuffer< T > const & uniformBuffer
		, uint32_t offset )
	{
		uint32_t constexpr size = uint32_t( sizeof( T ) );
		offset = uniformBuffer.getUbo().getOffset( offset, size );
		auto binding = layoutBinding.getBinding();
		binding.setCount( 1u );
		m_descriptorSet->createBinding( binding
			, uniformBuffer.getUbo()
			, offset
			, size );
		return UniformBufferBinding< T >
		{
			layoutBinding,
			uniformBuffer,
			offset
		};
	}
}
