/*
This file belongs to GlInstance.
See LICENSE file in root folder.
*/
#include "Command/Commands/D3D11ClearColourCommand.hpp"

#include "Image/D3D11ImageView.hpp"

#include "ashesd3d11_api.hpp"

namespace ashes::d3d11
{
	namespace
	{
		VkImageViewArray createViews( VkDevice device
			, VkImage image
			, VkImageSubresourceRangeArray ranges )
		{
			VkImageViewArray results;
			results.resize( ranges.size() );
			uint32_t index = 0u;

			for ( auto & result : results )
			{
				allocate( result
					, nullptr
					, device
					, VkImageViewCreateInfo
					{
						VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
						nullptr,
						0u,
						image,
						VK_IMAGE_VIEW_TYPE_2D,
						get( image )->getFormat(),
						VkComponentMapping{},
						std::move( ranges[index] ),
					} );
				++index;
			}

			return results;
		}
	}

	ClearColourCommand::ClearColourCommand( VkDevice device
		, VkImage image
		, VkImageSubresourceRangeArray ranges
		, VkClearColorValue const & colour )
		: CommandBase{ device }
		, m_image{ image }
		, m_ranges{ std::move( ranges ) }
		, m_colour{ colour }
		, m_views{ createViews( m_device, m_image, m_ranges ) }
	{
	}

	ClearColourCommand::ClearColourCommand( ClearColourCommand  const & rhs )
		: CommandBase{ rhs.m_device }
		, m_image{ rhs.m_image }
		, m_ranges{ rhs.m_ranges }
		, m_colour{ rhs.m_colour }
		, m_views{ createViews( m_device, m_image, m_ranges ) }
	{
	}

	ClearColourCommand::~ClearColourCommand()
	{
		for ( auto view : m_views )
		{
			deallocate( view, nullptr );
		}
	}

	void ClearColourCommand::apply( Context const & context )const
	{
		for ( auto & view : m_views )
		{
			context.context->ClearRenderTargetView( get( view )->getRenderTargetView()
				, m_colour.float32 );
		}
	}

	CommandPtr ClearColourCommand::clone()const
	{
		return std::make_unique< ClearColourCommand >( *this );
	}
}
