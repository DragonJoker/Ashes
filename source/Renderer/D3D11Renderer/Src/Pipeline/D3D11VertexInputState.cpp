#include "D3D11RendererPrerequisites.hpp"

namespace d3d11_renderer
{
	std::vector< D3D11_INPUT_ELEMENT_DESC > convert( renderer::VertexInputState const & state
		, std::list< std::string > & strings )
	{
		std::vector< D3D11_INPUT_ELEMENT_DESC > result;
		UINT index = 0u;

		for ( auto attribute : state.vertexAttributeDescriptions )
		{
			auto & binding = state.vertexBindingDescriptions[attribute.binding];
			auto semanticName = attribute.semantic.name;
			strings.emplace_back( std::move( semanticName ) );
			result.push_back( D3D11_INPUT_ELEMENT_DESC
				{
					strings.back().c_str(),
					attribute.semantic.index,
					convert( attribute.format ),
					attribute.binding,
					attribute.offset,
					convert( binding.inputRate ),
					( binding.inputRate == renderer::VertexInputRate::eInstance 
						? 1u
						: 0u )
				} );
		}

		return result;
	}
}
