/*
This file belongs to RendererLib.
See LICENSE file in root folder.
*/
#include "VertexInputState.hpp"

#include "Pipeline/VertexLayout.hpp"

namespace renderer
{
	namespace
	{
		template< Format Fmt >
		inline void doFillSubAttributes( Attribute const & attribute
			, uint32_t bindingSlot
			, VertexInputAttributeDescriptionArray & result )
		{
			uint32_t offset = attribute.getOffset();
			uint32_t location = attribute.getLocation();

			for ( auto i = 0u; i < FormatHelper< Fmt >::SubCount; ++i )
			{
				result.push_back(
				{
					location,
					bindingSlot,
					FormatHelper< Fmt >::SubFormat,
					offset,
				} );
				++location;
				offset += FormatHelper< Fmt >::SubSize;
			}
		}

		void doFill( VertexLayout const & vertexLayout
			, VertexInputState & result )
		{
			result.vertexBindingDescriptions.push_back(
			{
				vertexLayout.getBindingSlot(),
				vertexLayout.getStride(),
				vertexLayout.getInputRate()
			} );

			for ( auto const & attribute : vertexLayout )
			{
				result.vertexAttributeDescriptions.push_back(
				{
					attribute.getLocation(),
					vertexLayout.getBindingSlot(),
					attribute.getFormat(),
					attribute.getOffset(),
				} );
			}

		}
	}

	VertexInputState VertexInputState::create( VertexLayoutCRefArray const & vertexLayouts )
	{
		VertexInputState result;

		for ( auto const & vb : vertexLayouts )
		{
			doFill( vb.get(), result );
		}

		return result;
	}

	VertexInputState VertexInputState::create( VertexLayout const & vertexLayout )
	{
		VertexInputState result;
		doFill( vertexLayout, result );
		return result;
	}
}
