#include "Pipeline/D3D11VertexInputState.hpp"

namespace ashes::D3D11_NAMESPACE
{
	std::vector< D3D11_INPUT_ELEMENT_DESC > convert( VkPipelineVertexInputStateCreateInfo const & state
		, InputLayout const & inputLayout )
	{
		std::vector< D3D11_INPUT_ELEMENT_DESC > result;

		for ( auto attribute : makeArrayView( state.pVertexAttributeDescriptions, state.vertexAttributeDescriptionCount ) )
		{
			auto vertexBindingDescriptions = makeArrayView( state.pVertexBindingDescriptions, state.vertexBindingDescriptionCount );
			auto bindingIt = std::find_if( vertexBindingDescriptions.begin()
				, vertexBindingDescriptions.end()
				, [&attribute]( VkVertexInputBindingDescription const & lookup )
				{
					return attribute.binding == lookup.binding;
				} );
			auto inputDescIt = std::find_if( inputLayout.begin()
				, inputLayout.end()
				, [attribute]( InputElementDesc const & lookup )
				{
					return attribute.location == lookup.BaseSemanticIndex;
				} );

			if ( inputDescIt != inputLayout.end()
				&& bindingIt != vertexBindingDescriptions.end() )
			{
				auto & inputDesc = *inputDescIt;
				auto & binding = *bindingIt;
				result.push_back( D3D11_INPUT_ELEMENT_DESC
					{
						inputDesc.SemanticName.c_str(),
						inputDesc.SemanticIndex,
						getBufferFormat( attribute.format ),
						attribute.binding,
						attribute.offset,
						getInputClassification( binding.inputRate ),
						( binding.inputRate == VK_VERTEX_INPUT_RATE_INSTANCE
							? 1u
							: 0u )
					} );
			}
		}

		return result;
	}
}
