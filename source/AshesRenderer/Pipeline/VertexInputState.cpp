/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "AshesRenderer/Pipeline/VertexInputState.hpp"

#include "AshesRenderer/Pipeline/VertexLayout.hpp"

namespace ashes
{
	namespace
	{
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
					attribute.location,
					vertexLayout.getBindingSlot(),
					attribute.format,
					attribute.offset
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
