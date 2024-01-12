/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "Command/Commands/GlClearDepthStencilCommand.hpp"

#include "Core/GlContextStateStack.hpp"
#include "Image/GlImageView.hpp"
#include "Miscellaneous/GlCallLogger.hpp"

#include "ashesgl_api.hpp"

namespace ashes::gl
{
	static void clearDepthStencilCommand( VkDevice device
		, VkImage image
		, GlFormat format
		, GlType type
		, VkClearDepthStencilValue value
		, ArrayView< VkImageSubresourceRange const > ranges
		, CmdList & list )
	{
		for ( auto range : ranges )
		{
			if ( range.levelCount == RemainingArrayLayers )
			{
				range.levelCount = ashes::getMaxMipCount( get( image )->getDimensions() );
			}

			if ( range.layerCount == RemainingArrayLayers )
			{
				range.layerCount = get( device )->getLimits().maxImageArrayLayers;
			}

			for ( uint32_t level = range.baseMipLevel;
				level < range.baseMipLevel + range.levelCount;
				++level )
			{
				list.push_back( makeCmd< OpType::eClearTexDepthStencil >( get( image )->getInternal()
					, level
					, format
					, type
					, value ) );
			}
		}
	}

	static void clearStencilCommand( VkDevice device
		, VkImage image
		, GlFormat format
		, GlType type
		, VkClearDepthStencilValue value
		, ArrayView< VkImageSubresourceRange const > ranges
		, CmdList & list )
	{
		for ( auto range : ranges )
		{
			if ( range.levelCount == RemainingArrayLayers )
			{
				range.levelCount = ashes::getMaxMipCount( get( image )->getDimensions() );
			}

			if ( range.layerCount == RemainingArrayLayers )
			{
				range.layerCount = get( device )->getLimits().maxImageArrayLayers;
			}

			for ( uint32_t level = range.baseMipLevel;
				level < range.baseMipLevel + range.levelCount;
				++level )
			{
				list.push_back( makeCmd< OpType::eClearTexStencil >( get( image )->getInternal()
					, level
					, format
					, type
					, int32_t( value.stencil ) ) );
			}
		}
	}

	static void clearDepthCommand( VkDevice device
		, VkImage image
		, GlFormat format
		, GlType type
		, VkClearDepthStencilValue value
		, ArrayView< VkImageSubresourceRange const > ranges
		, CmdList & list )
	{
		for ( auto range : ranges )
		{
			if ( range.levelCount == RemainingArrayLayers )
			{
				range.levelCount = ashes::getMaxMipCount( get( image )->getDimensions() );
			}

			if ( range.layerCount == RemainingArrayLayers )
			{
				range.layerCount = get( device )->getLimits().maxImageArrayLayers;
			}

			for ( uint32_t level = range.baseMipLevel;
				level < range.baseMipLevel + range.levelCount;
				++level )
			{
				list.push_back( makeCmd< OpType::eClearTexDepth >( get( image )->getInternal()
					, level
					, format
					, type
					, value.depth ) );
			}
		}
	}

	void buildClearDepthStencilCommand( VkDevice device
		, ContextStateStack & stack
		, VkImage image
		, [[maybe_unused]] VkImageLayout imageLayout
		, VkClearDepthStencilValue value
		, ArrayView< VkImageSubresourceRange const > ranges
		, CmdList & list )
	{
		glLogCommand( list, "ClearDepthStencilCommand" );
		stack.applyDisableBlend( list );

		if ( get( getInstance( device ) )->hasClearTexImage() )
		{
			auto format = get( image )->getUnpackFormat();
			auto type = get( image )->getUnpackType();

			if ( isDepthStencilFormat( get( image )->getFormatVk() ) )
			{
				clearDepthStencilCommand( device, image, format, type, value, ranges, list );
			}
			else if ( isStencilFormat( get( image )->getFormatVk() ) )
			{
				clearStencilCommand( device, image, format, type, value, ranges, list );
			}
			else if ( isDepthFormat( get( image )->getFormatVk() ) )
			{
				clearDepthCommand( device, image, format, type, value, ranges, list );
			}
		}
		else
		{
			std::cerr << "Unsupported command : ClearDepthStencilCommand" << std::endl;
		}
	}
}
