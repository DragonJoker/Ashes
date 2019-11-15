#include "Pipeline/D3D11DynamicStates.hpp"

#include "Core/D3D11Device.hpp"

#include "ashesd3d11_api.hpp"

namespace ashes::d3d11
{
	namespace
	{
		template<typename T>
		void doHashCombine( size_t & seed, T const & v )
		{
			const uint64_t kMul = 0x9ddfea08eb382d69ULL;

			std::hash< T > hasher;
			uint64_t a = ( hasher( v ) ^ seed ) * kMul;
			a ^= ( a >> 47 );

			uint64_t b = ( seed ^ a ) * kMul;
			b ^= ( b >> 47 );

			seed = static_cast< std::size_t >( b * kMul );
		}
	}

	DynamicStates::DynamicStates( VkDevice device
		, VkPipelineDynamicStateCreateInfo const * createInfo )
		: m_dynamicState{ ( createInfo
			? deepCopy( *createInfo, m_dynamicStates )
			: VkPipelineDynamicStateCreateInfo{} ) }
	{
	}

	DynamicStates::~DynamicStates()
	{
	}
}
