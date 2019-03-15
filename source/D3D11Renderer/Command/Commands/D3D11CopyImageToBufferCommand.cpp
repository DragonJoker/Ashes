/*
This file belongs to GlInstance.
See LICENSE file in root folder.
*/
#include "Command/Commands/D3D11CopyImageToBufferCommand.hpp"

#include "Buffer/D3D11Buffer.hpp"
#include "Core/D3D11Device.hpp"
#include "Image/D3D11Texture.hpp"
#include "Image/D3D11TextureView.hpp"

#include <Ashes/Image/ImageSubresourceRange.hpp>
#include <Ashes/Miscellaneous/BufferImageCopy.hpp>

namespace d3d11_renderer
{
	namespace
	{
		std::vector< TextureViewPtr > createViews( Device const & device
			, ashes::Texture const & texture
			, ashes::BufferImageCopyArray const & copies )
		{
			std::vector< TextureViewPtr > result;
			ashes::TextureType type = texture.getType();
			ashes::TextureViewType viewType;

			if ( type == ashes::TextureType::e3D )
			{
				viewType = ashes::TextureViewType::e3D;
			}
			else if ( type == ashes::TextureType::e2D )
			{
				viewType = ashes::TextureViewType::e2D;
			}
			else if ( type == ashes::TextureType::e1D )
			{
				viewType = ashes::TextureViewType::e1D;
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
				result.emplace_back( std::make_unique< TextureView >( device
					, static_cast< Texture const & >( texture )
					, createInfo ) );
			}

			return result;
		}
	}

	CopyImageToBufferCommand::CopyImageToBufferCommand( Device const & device
		, ashes::BufferImageCopyArray const & copyInfo
		, ashes::Texture const & src
		, ashes::BufferBase const & dst )
		: CommandBase{ device }
		, m_src{ static_cast< Texture const & >( src ) }
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
		, TextureView const & view )const
	{
	}

	CommandPtr CopyImageToBufferCommand::clone()const
	{
		return std::make_unique< CopyImageToBufferCommand >( *this );
	}
}
