/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "Command/Commands/D3D11ClearDepthStencilCommand.hpp"

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
					, get( device )->getAllocationCallbacks()
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

	ClearDepthStencilCommand::ClearDepthStencilCommand( VkDevice device
		, VkImage image
		, ArrayView< VkImageSubresourceRange const > ranges
		, VkClearDepthStencilValue value )
		: CommandBase{ device }
		, m_image{ image }
		, m_ranges{ ranges.begin(), ranges.end() }
		, m_value{ value }
		, m_flags{ ( isDepthFormat( get( image )->getFormat() )
				? D3D11_CLEAR_DEPTH
				: 0u )
			| ( isStencilFormat( get( image )->getFormat() )
				? D3D11_CLEAR_STENCIL
				: 0u ) }
		, m_views{ createViews( m_device, m_image, m_ranges ) }
	{
	}

	ClearDepthStencilCommand::ClearDepthStencilCommand( ClearDepthStencilCommand const & rhs )
		: CommandBase{ rhs.m_device }
		, m_image{ rhs.m_image }
		, m_ranges{ rhs.m_ranges }
		, m_value{ rhs.m_value }
		, m_flags{ rhs.m_flags }
		, m_views{ createViews( m_device, m_image, m_ranges ) }
	{
	}

	ClearDepthStencilCommand::~ClearDepthStencilCommand()
	{
		for ( auto view : m_views )
		{
			deallocate( view, get( m_device )->getAllocationCallbacks() );
		}
	}

	void ClearDepthStencilCommand::apply( Context const & context )const
	{
		for ( auto & view : m_views )
		{
			context.context->ClearDepthStencilView( get( view )->getDepthStencilView()
				, m_flags
				, m_value.depth
				, m_value.stencil );
		}
	}

	CommandPtr ClearDepthStencilCommand::clone()const
	{
		return std::make_unique< ClearDepthStencilCommand >( *this );
	}
}
