/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "Command/Commands/GlCommandBase.hpp"

#include "Core/GlContextLock.hpp"
#include "Miscellaneous/GlDeviceMemory.hpp"

#include "ashesgl3_api.hpp"

#include <cstring>
#include <algorithm>

using ashes::operator==;
using ashes::operator!=;

namespace ashes::gl3
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
		, CmdApplyScissors const & cmd )
	{
		glLogCall( context
			, glScissorArrayv_ARB
			, GLuint( cmd.first )
			, GLsizei( cmd.count )
			, cmd.scissors.data() );
	}

	void apply( ContextLock const & context
		, CmdApplyViewports const & cmd )
	{
		glLogCall( context
			, glViewportArrayv_ARB
			, GLuint( cmd.first )
			, GLsizei( cmd.count )
			, cmd.viewports.data() );
	}

	void apply( ContextLock const & context
		, CmdApplyDepthRanges const & cmd )
	{
		glLogCall( context
			, glDepthRangeArrayv_ARB
			, GLuint( cmd.first )
			, GLsizei( cmd.count )
			, cmd.depthRanges.data() );
	}

	void apply( ContextLock const & context
		, CmdApplyScissor const & cmd )
	{
		glLogCall( context
			, glScissor
			, GLint( cmd.scissor.offset.x )
			, GLint( cmd.scissor.offset.y )
			, GLsizei( cmd.scissor.extent.width )
			, GLsizei( cmd.scissor.extent.height ) );
	}

	void apply( ContextLock const & context
		, CmdApplyViewport const & cmd )
	{
		glLogCall( context
			, glViewport
			, GLint( cmd.viewport.x )
			, GLint( cmd.viewport.y )
			, GLsizei( cmd.viewport.width )
			, GLsizei( cmd.viewport.height ) );
		glLogCall( context
			, glDepthRange
			, GLclampf( cmd.viewport.minDepth )
			, GLclampf( cmd.viewport.maxDepth ) );
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
		glLogCall( context
			, glUseProgram
			, cmd.program );
	}

	void apply( ContextLock const & context
		, CmdInitFramebuffer const & cmd )
	{
		if ( ( *cmd.fbo ) == GL_INVALID_INDEX )
		{
			glLogCall( context
				, glGenFramebuffers
				, 1
				, cmd.fbo );
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
		, CmdBlendConstants const & cmd )
	{
		glLogCall( context
			, glBlendColor
			, cmd.blendConstant0
			, cmd.blendConstant1
			, cmd.blendConstant2
			, cmd.blendConstant3 );
	}

	void apply( ContextLock const & context
		, CmdBlendEquation const & cmd )
	{
		glLogCall( context
			, glBlendEquationSeparate
			, cmd.color
			, cmd.alpha );
	}

	void apply( ContextLock const & context
		, CmdBlendFunc const & cmd )
	{
		glLogCall( context
			, glBlendFuncSeparate
			, cmd.colorSrc
			, cmd.colorDst
			, cmd.alphaSrc
			, cmd.alphaDst );
	}

	void apply( ContextLock const & context
		, CmdColorMask const & cmd )
	{
		glLogCall( context
			, glColorMaski
			, cmd.index
			, cmd.r
			, cmd.g
			, cmd.b
			, cmd.a );
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
		, CmdCullFace const & cmd )
	{
		glLogCall( context
			, glCullFace
			, cmd.value );
	}

	void apply( ContextLock const & context
		, CmdDepthFunc const & cmd )
	{
		glLogCall( context
			, glDepthFunc
			, cmd.value );
	}

	void apply( ContextLock const & context
		, CmdDepthMask const & cmd )
	{
		glLogCall( context
			, glDepthMask
			, cmd.value );
	}

	void apply( ContextLock const & context
		, CmdDepthRange const & cmd )
	{
		glLogCall( context
			, glDepthRange
			, cmd.min
			, cmd.max );
	}

	void apply( ContextLock const & context
		, CmdFrontFace const & cmd )
	{
		glLogCall( context
			, glFrontFace
			, cmd.value );
	}

	void apply( ContextLock const & context
		, CmdLineWidth const & cmd )
	{
		glLogCall( context
			, glLineWidth
			, cmd.value );
	}

	void apply( ContextLock const & context
		, CmdLogicOp const & cmd )
	{
		glLogCall( context
			, glLogicOp
			, cmd.value );
	}

	void apply( ContextLock const & context
		, CmdMinSampleShading const & cmd )
	{
		glLogCall( context
			, glMinSampleShading_ARB
			, cmd.value );
	}

	void apply( ContextLock const & context
		, CmdPatchParameter const & cmd )
	{
		glLogCall( context
			, glPatchParameteri_ARB
			, cmd.param
			, cmd.value );
	}

	void apply( ContextLock const & context
		, CmdPolygonMode const & cmd )
	{
		glLogCall( context
			, glPolygonMode
			, GL_CULL_MODE_FRONT_AND_BACK
			, cmd.value );
	}

	void apply( ContextLock const & context
		, CmdPolygonOffset const & cmd )
	{
		glLogCall( context
			, glPolygonOffsetClampEXT
			, cmd.constantFactor
			, cmd.slopeFactor
			, cmd.clamp );
	}

	void apply( ContextLock const & context
		, CmdPrimitiveRestartIndex const & cmd )
	{
		glLogCall( context
			, glPrimitiveRestartIndex
			, cmd.index );
		
	}

	void apply( ContextLock const & context
		, CmdStencilFunc const & cmd )
	{
		glLogCall( context
			, glStencilFuncSeparate
			, cmd.face
			, cmd.op
			, cmd.ref
			, cmd.compMask );
	}

	void apply( ContextLock const & context
		, CmdStencilMask const & cmd )
	{
		glLogCall( context
			, glStencilMaskSeparate
			, cmd.face
			, cmd.mask );
	}

	void apply( ContextLock const & context
		, CmdStencilOp const & cmd )
	{
		glLogCall( context
			, glStencilOpSeparate
			, cmd.face
			, cmd.fail
			, cmd.depthFail
			, cmd.pass );
	}

	void apply( ContextLock const & context
		, CmdFramebufferTexture const & cmd )
	{
		glLogCall( context
			, glFramebufferTexture
			, cmd.target
			, cmd.point
			, cmd.object
			, cmd.mipLevel );
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
		, CmdFramebufferTextureLayer const & cmd )
	{
		glLogCall( context
			, glFramebufferTextureLayer
			, cmd.target
			, cmd.point
			, cmd.object
			, cmd.mipLevel
			, cmd.arrayLayer );
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

	void apply( ContextLock const & context
		, CmdUpdateBuffer const & cmd )
	{
		void * data;

		if ( VK_SUCCESS == get( cmd.memory )->lock( context, cmd.memoryOffset, cmd.dataSize, 0u, &data ) )
		{
			std::memcpy( data, cmd.pData, cmd.dataSize );
			get( cmd.memory )->flush( context, cmd.memoryOffset, cmd.dataSize );
			get( cmd.memory )->unlock( context );
		}
	}

	void apply( ContextLock const & context
		, CmdFillBuffer const & cmd )
	{
		uint32_t * data;

		if ( VK_SUCCESS == get( cmd.memory )->lock( context, cmd.memoryOffset, cmd.dataSize, 0u, reinterpret_cast< void ** >( &data ) ) )
		{
			std::fill_n( data, cmd.dataSize / sizeof( uint32_t ), cmd.data );
			get( cmd.memory )->flush( context, cmd.memoryOffset, cmd.dataSize );
			get( cmd.memory )->unlock( context );
		}
	}
}
