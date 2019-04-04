/*
This file belongs to GlInstance.
See LICENSE file in root folder.
*/
#include "Command/Commands/GlGenerateMipmapsCommand.hpp"

#include "Image/GlImage.hpp"

#include "ashesgl4_api.hpp"

namespace ashes::gl4
{
	GenerateMipmapsCommand::GenerateMipmapsCommand( VkDevice device
		, VkImage texture )
		: CommandBase{ device }
		, m_texture{ get( texture )->getInternal() }
		, m_target{ convert( get( texture )->getType(), get( texture )->getArrayLayers() ) }
	{
	}

	void GenerateMipmapsCommand::apply( ContextLock const & context )const
	{
		glLogCommand( "GenerateMipmapsCommand" );
		glLogCall( context
			, glBindTexture
			, m_target
			, m_texture );
		glLogCall( context
			, glGenerateMipmap
			, m_target );
		glLogCall( context
			, glBindTexture
			, m_target
			, 0u );
	}

	CommandPtr GenerateMipmapsCommand::clone()const
	{
		return std::make_unique< GenerateMipmapsCommand >( *this );
	}
}
