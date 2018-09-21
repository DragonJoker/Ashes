/*
This file belongs to GlRenderer.
See LICENSE file in root folder.
*/
#include "D3D11CopyBufferToImageCommand.hpp"

#include "Buffer/D3D11Buffer.hpp"
#include "Image/D3D11Texture.hpp"
#include "Image/D3D11TextureView.hpp"

#include <Image/ImageSubresourceRange.hpp>

namespace d3d11_renderer
{
	namespace
	{
		std::vector< D3D11_BOX > doGetSrcBoxes( ashes::BufferImageCopyArray const & copyInfo )
		{
			std::vector< D3D11_BOX > result;

			for ( auto & copy : copyInfo )
			{
				auto size = copy.bufferImageHeight * copy.bufferRowLength;
				size = size
					? size
					: ( std::max( copy.imageExtent.width, 1u )
						* std::max( copy.imageExtent.height, 1u )
						* std::max( copy.imageExtent.depth, 1u ) );
				result.push_back(
					{
						copy.bufferOffset,
						0u,
						0u,
						copy.bufferOffset + size,
						1u,
						1u
					}
				);
			}

			return result;
		}
	}

	CopyBufferToImageCommand::CopyBufferToImageCommand( Device const & device
		, ashes::BufferImageCopyArray const & copyInfo
		, ashes::BufferBase const & src
		, ashes::Texture const & dst )
		: CommandBase{ device }
		, m_copyInfo{ copyInfo }
		, m_src{ static_cast< Buffer const & >( src ) }
		, m_dst{ static_cast< Texture const & >( dst ) }
		, m_format{ getSRVFormat( m_dst.getFormat() ) }
		, m_srcBoxes{ doGetSrcBoxes( copyInfo ) }
	{
	}

	void CopyBufferToImageCommand::apply( Context const & context )const
	{
		for ( auto i = 0u; i < m_copyInfo.size(); ++i )
		{
			applyOne( context.context
				, m_copyInfo[i]
				, m_srcBoxes[i] );
		}
	}

	CommandPtr CopyBufferToImageCommand::clone()const
	{
		return std::make_unique< CopyBufferToImageCommand >( *this );
	}

	void CopyBufferToImageCommand::applyOne( ID3D11DeviceContext * context
		, ashes::BufferImageCopy const & copyInfo
		, D3D11_BOX const & srcBox )const
	{
	}
}
