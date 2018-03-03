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
		template< AttributeFormat Fmt >
		struct FormatHelper;

		template<>
		struct FormatHelper< AttributeFormat::eMat2f >
		{
			static AttributeFormat constexpr SubFormat = AttributeFormat::eVec2f;
			static uint32_t constexpr SubCount = 2u;
			static uint32_t constexpr SubSize = 4u;
		};

		template<>
		struct FormatHelper< AttributeFormat::eMat3f >
		{
			static AttributeFormat constexpr SubFormat = AttributeFormat::eVec3f;
			static uint32_t constexpr SubCount = 3u;
			static uint32_t constexpr SubSize = 9u;
		};

		template<>
		struct FormatHelper< AttributeFormat::eMat4f >
		{
			static AttributeFormat constexpr SubFormat = AttributeFormat::eVec4f;
			static uint32_t constexpr SubCount = 4u;
			static uint32_t constexpr SubSize = 16u;
		};

		template< AttributeFormat Fmt >
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
				switch ( attribute.getFormat() )
				{
				case renderer::AttributeFormat::eMat4f:
					doFillSubAttributes< renderer::AttributeFormat::eMat4f >( attribute
						, vertexLayout.getBindingSlot()
						, result.vertexAttributeDescriptions );
					break;
				case renderer::AttributeFormat::eMat3f:
					doFillSubAttributes< renderer::AttributeFormat::eMat3f >( attribute
						, vertexLayout.getBindingSlot()
						, result.vertexAttributeDescriptions );
					break;
				case renderer::AttributeFormat::eMat2f:
					doFillSubAttributes< renderer::AttributeFormat::eMat2f >( attribute
						, vertexLayout.getBindingSlot()
						, result.vertexAttributeDescriptions );
					break;
				default:
					result.vertexAttributeDescriptions.push_back(
					{
						attribute.getLocation(),
						vertexLayout.getBindingSlot(),
						attribute.getFormat(),
						attribute.getOffset(),
					} );
					break;
				}
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
