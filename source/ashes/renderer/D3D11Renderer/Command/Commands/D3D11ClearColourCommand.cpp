/*
This file belongs to Ashes.
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

	ClearColourCommand::ClearColourCommand( VkDevice device
		, VkImage image
		, ArrayView< VkImageSubresourceRange const > ranges
		, VkClearColorValue const & colour )
		: CommandBase{ device }
		, m_image{ image }
		, m_ranges{ ranges.begin(), ranges.end() }
		, m_colour{ colour }
		, m_views{ createViews( getDevice(), m_image, m_ranges ) }
	{
	}

	ClearColourCommand::ClearColourCommand( ClearColourCommand  const & rhs )
		: CommandBase{ rhs }
		, m_image{ rhs.m_image }
		, m_ranges{ rhs.m_ranges }
		, m_colour{ rhs.m_colour }
		, m_views{ createViews( getDevice(), m_image, m_ranges ) }
	{
	}

	ClearColourCommand & ClearColourCommand::operator=( ClearColourCommand  const & rhs )
	{
		for ( auto const & view : m_views )
		{
			deallocate( view, get( getDevice() )->getAllocationCallbacks() );
		}

		CommandBase::operator=( rhs );
		m_image = rhs.m_image;
		m_ranges = rhs.m_ranges;
		m_colour = rhs.m_colour;
		m_views = createViews( getDevice(), m_image, m_ranges );

		return *this;
	}

	ClearColourCommand::~ClearColourCommand()noexcept
	{
		for ( auto const & view : m_views )
		{
			deallocate( view, get( getDevice() )->getAllocationCallbacks() );
		}
	}

	void ClearColourCommand::apply( Context const & context )const
	{
		for ( auto const & view : m_views )
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
