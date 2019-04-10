/*
This file belongs to GlInstance.
See LICENSE file in root folder.
*/
#include "Command/Commands/GlCopyImageCommand.hpp"

#include "Image/GlImage.hpp"
#include "Image/GlImageView.hpp"

#include "ashesgl4_api.hpp"

namespace ashes::gl4
{
	void apply( ContextLock const & context
		, CmdCopyImageSubData const & cmd )
	{
		glLogCall( context
			, glCopyImageSubData
			, cmd.dstName
			, cmd.srcTarget
			, cmd.copy.srcSubresource.mipLevel
			, cmd.copy.srcOffset.x
			, cmd.copy.srcOffset.y
			, ( cmd.copy.srcSubresource.baseArrayLayer
				? int32_t( cmd.copy.srcSubresource.baseArrayLayer )
				: cmd.copy.srcOffset.z )
			, cmd.dstName
			, cmd.dstTarget
			, cmd.copy.dstSubresource.mipLevel
			, cmd.copy.dstOffset.x
			, cmd.copy.dstOffset.y
			, ( cmd.copy.dstSubresource.baseArrayLayer
				? int32_t( cmd.copy.dstSubresource.baseArrayLayer )
				: cmd.copy.dstOffset.z )
			, cmd.copy.extent.width
			, cmd.copy.extent.height
			, cmd.copy.extent.depth );
	}

	void buildCopyImageCommand( VkImageCopy copyInfo
		, VkImage src
		, VkImage dst
		, CmdList & list )
	{
		glLogCommand( "CopyImageCommand" );
		auto srcTarget = convert( get( src )->getType()
			, get( src )->getArrayLayers() );
		auto dstTarget = convert( get( dst )->getType()
			, get( dst )->getArrayLayers() );
		list.push_back( makeCmd< OpType::eBindTexture >( srcTarget
			, get( src )->getInternal() ) );
		list.push_back( makeCmd< OpType::eBindTexture >( dstTarget
			, get( dst )->getInternal() ) );
		list.push_back( makeCmd< OpType::eCopyImageSubData >( get( src )->getInternal()
			, srcTarget
			, get( dst )->getInternal()
			, dstTarget
			, std::move( copyInfo ) ) );
		list.push_back( makeCmd< OpType::eBindTexture >( srcTarget
			, 0u ) );
		list.push_back( makeCmd< OpType::eBindTexture >( dstTarget
			, 0u ) );
	}
}
