/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "Command/Commands/GlGenerateMipmapsCommand.hpp"

#include "Image/GlImage.hpp"

#include "ashesgl_api.hpp"

namespace ashes::gl
{
	void apply( ContextLock const & context
		, CmdGenerateMipmaps const & cmd )
	{
		glLogCall( context
			, glGenerateMipmap
			, cmd.target );
	}

	//*************************************************************************

	void buildGenerateMipmapsCommand( VkImage texture
		, CmdList & list )
	{
		glLogCommand( "GenerateMipmapsCommand" );
		auto target = convert( get( texture )->getDevice()
			, get( texture )->getType()
			, get( texture )->getArrayLayers()
			, get( texture )->getCreateFlags() );
		list.push_back( makeCmd< OpType::eBindTexture >( target
				, get( texture )->getInternal() ) );
		list.push_back( makeCmd< OpType::eGenerateMipmaps >( target ) );
		list.push_back( makeCmd< OpType::eBindTexture >( target
			, 0u ) );
	}
}
