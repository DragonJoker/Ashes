/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "Command/Commands/GlCommandBase.hpp"

#include "Core/GlContextLock.hpp"
#include "Miscellaneous/GlDeviceMemory.hpp"

#include "ashesgl4_api.hpp"

namespace ashes::gl4
{
	void apply( ContextLock const & context
		, CmdEnable const & cmd )
	{
		glLogCall( context
			, glEnable
			, cmd.value );
	}
	
	void apply( ContextLock const & context
		, CmdDisable const & cmd )
	{
		glLogCall( context
			, glDisable
			, cmd.value );
	}

	void apply( ContextLock const & context
		, CmdApplyScissor const & cmd )
	{
		auto & save = context->getCurrentScissor();

		if ( save != cmd.scissor )
		{
			glLogCall( context
				, glScissor
				, cmd.scissor.offset.x
				, cmd.scissor.offset.y
				, cmd.scissor.extent.width
				, cmd.scissor.extent.height );
			context->setCurrentScissor( cmd.scissor );
		}
	}

	void apply( ContextLock const & context
		, CmdApplyViewport const & cmd )
	{
		auto & save = context->getCurrentViewport();

		if ( save != cmd.viewport )
		{
			glLogCall( context
				, glViewport
				, GLint( cmd.viewport.x )
				, GLint( cmd.viewport.y )
				, GLsizei( cmd.viewport.width )
				, GLsizei( cmd.viewport.height ) );
			glLogCall( context
				, glDepthRange
				, cmd.viewport.minDepth
				, cmd.viewport.maxDepth );
			context->setCurrentViewport( cmd.viewport );
		}
	}

	void apply( ContextLock const & context
		, CmdDrawBuffer const & cmd )
	{
		glLogCall( context
			, glDrawBuffer
			, cmd.target );
	}

	void apply( ContextLock const & context
		, CmdDrawBuffers const & cmd )
	{
		glLogCall( context
			, glDrawBuffers
			, GLsizei( cmd.count )
			, cmd.targets.data() );
	}

	void apply( ContextLock const & context
		, CmdUseProgram const & cmd )
	{
		if ( cmd.program != context->getCurrentProgram() )
		{
			glLogCall( context
				, glUseProgram
				, cmd.program );
			context->setCurrentProgram( cmd.program );
		}
	}

	void apply( ContextLock const & context
		, CmdBindFramebuffer const & cmd )
	{
		if ( cmd.fbo != VK_NULL_HANDLE )
		{
			glLogCall( context
				, glBindFramebuffer
				, cmd.target
				, get( cmd.fbo )->getInternal() );
		}
		else
		{
			glLogCall( context
				, glBindFramebuffer
				, cmd.target
				, 0u );
		}

		context->setCurrentFramebuffer( cmd.fbo );
	}

	void apply( ContextLock const & context
		, CmdBindBuffer const & cmd )
	{
		glLogCall( context
			, glBindBuffer
			, cmd.target
			, cmd.name );
	}

	void apply( ContextLock const & context
		, CmdBindTexture const & cmd )
	{
		glLogCall( context
			, glBindTexture
			, cmd.type
			, cmd.name );
	}

	void apply( ContextLock const & context
		, CmdClearColour const & cmd )
	{
		glLogCall( context
			, glClearBufferfv
			, GL_CLEAR_TARGET_COLOR
			, cmd.colourIndex
			, cmd.color.float32 );
	}

	void apply( ContextLock const & context
		, CmdClearDepth const & cmd )
	{
		glLogCall( context
			, glDepthMask
			, GL_TRUE );
		glLogCall( context
			, glClearBufferfv
			, GL_CLEAR_TARGET_DEPTH
			, 0u
			, &cmd.depth );
	}

	void apply( ContextLock const & context
		, CmdClearStencil const & cmd )
	{
		glLogCall( context
			, glDepthMask
			, GL_TRUE );
		glLogCall( context
			, glClearBufferiv
			, GL_CLEAR_TARGET_STENCIL
			, 0u
			, &cmd.stencil );
	}

	void apply( ContextLock const & context
		, CmdClearDepthStencil const & cmd )
	{
		glLogCall( context
			, glDepthMask
			, GL_TRUE );
		glLogCall( context
			, glClearBufferfi
			, GL_CLEAR_TARGET_DEPTH_STENCIL
			, 0u
			, cmd.depthStencil.depth
			, cmd.depthStencil.stencil );
	}

	void apply( ContextLock const & context
		, CmdFramebufferTexture2D const & cmd )
	{
		glLogCall( context
			, glFramebufferTexture2D
			, cmd.target
			, cmd.point
			, cmd.texTarget
			, cmd.object
			, cmd.mipLevel );
	}

	void apply( ContextLock const & context
		, CmdActiveTexture const & cmd )
	{
		glLogCall( context
			, glActiveTexture
			, GlTextureUnit( GL_TEXTURE0 + cmd.binding ) );
	}

	void apply( ContextLock const & context
		, CmdUploadMemory const & cmd )
	{
		get( cmd.memory )->upload( context, 0u, WholeSize );
	}

	void apply( ContextLock const & context
		, CmdDownloadMemory const & cmd )
	{
		get( cmd.memory )->download( context, 0u, WholeSize );
	}

	void apply( ContextLock const & context
		, CmdReadBuffer const & cmd )
	{
		glLogCall( context
			, glReadBuffer
			, cmd.point );
	}
}
