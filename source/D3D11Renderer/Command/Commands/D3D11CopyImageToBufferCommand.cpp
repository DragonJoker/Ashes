/*
This file belongs to GlInstance.
See LICENSE file in root folder.
*/
#include "Command/Commands/D3D11CopyImageToBufferCommand.hpp"

#include "Buffer/D3D11Buffer.hpp"
#include "Core/D3D11Device.hpp"
#include "Image/D3D11Image.hpp"
#include "Image/D3D11ImageView.hpp"

#include <Ashes/Image/ImageSubresourceRange.hpp>
#include <Ashes/Miscellaneous/BufferImageCopy.hpp>

namespace d3d11_renderer
{
	namespace
	{
		std::vector< ImageViewPtr > createViews( Device const & device
			, ashes::Image const & texture
			, ashes::BufferImageCopyArray const & copies )
		{
			std::vector< ImageViewPtr > result;
			ashes::ImageType type = texture.getType();
			ashes::ImageViewType viewType;

			if ( type == ashes::ImageType::e3D )
			{
				viewType = ashes::ImageViewType::e3D;
			}
			else if ( type == ashes::ImageType::e2D )
			{
				viewType = ashes::ImageViewType::e2D;
			}
			else if ( type == ashes::ImageType::e1D )
			{
				viewType = ashes::ImageViewType::e1D;
			}

			for ( auto & copy : copies )
			{
				ashes::ImageViewCreateInfo createInfo{};
				createInfo.viewType = viewType;
				createInfo.format = texture.getFormat();
				createInfo.subresourceRange.aspectMask = ashes::getAspectMask( createInfo.format );
				createInfo.subresourceRange.baseArrayLayer = copy.imageSubresource.baseArrayLayer;
				createInfo.subresourceRange.layerCount = copy.imageSubresource.layerCount;
				createInfo.subresourceRange.baseMipLevel = copy.imageSubresource.mipLevel;
				createInfo.subresourceRange.levelCount = 1u;
				result.emplace_back( std::make_unique< ImageView >( device
					, static_cast< Image const & >( texture )
					, createInfo ) );
			}

			return result;
		}
	}

	CopyImageToBufferCommand::CopyImageToBufferCommand( Device const & device
		, ashes::BufferImageCopyArray const & copyInfo
		, ashes::Image const & src
		, ashes::BufferBase const & dst )
		: CommandBase{ device }
		, m_src{ static_cast< Image const & >( src ) }
		, m_dst{ static_cast< Buffer const & >( dst ) }
		, m_copyInfo{ copyInfo }
		, m_format{ getSRVFormat( m_src.getFormat() ) }
		, m_views{ createViews( m_device, m_src, m_copyInfo ) }
	{
	}

	CopyImageToBufferCommand::CopyImageToBufferCommand( CopyImageToBufferCommand const & rhs )
		: CommandBase{ rhs.m_device }
		, m_src{ rhs.m_src }
		, m_dst{ rhs.m_dst }
		, m_copyInfo{ rhs.m_copyInfo }
		, m_format{ rhs.m_format }
		, m_views{ createViews( m_device, m_src, m_copyInfo ) }
	{
	}

	void CopyImageToBufferCommand::apply( Context const & context )const
	{
		for ( size_t i = 0; i < m_views.size(); ++i )
		{
			applyOne( m_copyInfo[i]
				, *m_views[i] );
		}
	}

	void CopyImageToBufferCommand::applyOne( ashes::BufferImageCopy const & copyInfo
		, ImageView const & view )const
	{
	}

	CommandPtr CopyImageToBufferCommand::clone()const
	{
		return std::make_unique< CopyImageToBufferCommand >( *this );
	}
}
