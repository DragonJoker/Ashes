/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "Command/Commands/GlCommandBase.hpp"

#include "Core/GlContextLock.hpp"
#include "Miscellaneous/GlDeviceMemory.hpp"
#include "RenderPass/GlFrameBuffer.hpp"

#include "ashesgl_api.hpp"

#include <cstring>
#include <algorithm>

using ashes::operator==;
using ashes::operator!=;

namespace ashes::gl
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
			, glScissorArrayv
			, GLuint( cmd.first )
			, GLsizei( cmd.count )
			, cmd.scissors.data() );
	}

	void apply( ContextLock const & context
		, CmdApplyViewports const & cmd )
	{
		glLogCall( context
			, glViewportArrayv
			, GLuint( cmd.first )
			, GLsizei( cmd.count )
			, cmd.viewports.data() );
	}

	void apply( ContextLock const & context
		, CmdApplyDepthRanges const & cmd )
	{
		glLogCall( context
			, glDepthRangeArrayv
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
		glLogCall( context
			, glUseProgram
			, cmd.program );
	}

	void apply( ContextLock const & context
		, CmdUseProgramPipeline const & cmd )
	{
		glLogCall( context
			, glBindProgramPipeline
			, cmd.program );
	}

	void apply( ContextLock const & context
		, CmdGetCompressedTexImage const & cmd )
	{
		glLogCall( context
			, glGetCompressedTexImage
			, cmd.target
			, cmd.level
			, getBufferOffset( cmd.offset ) );
	}

	void apply( ContextLock const & context
		, CmdGetQueryResults const & cmd )
	{
		get( cmd.queryPool )->getResults( context
			, cmd.firstQuery
			, cmd.queryCount
			, cmd.stride
			, 0u
			, cmd.flags
			, getBufferOffset( cmd.bufferOffset ) );
	}

	void apply( ContextLock const & context
		, CmdGetTexImage const & cmd )
	{
		glLogCall( context
			, glGetTexImage
			, cmd.target
			, cmd.level
			, cmd.format
			, cmd.type
			, getBufferOffset( cmd.offset ) );
	}

	void apply( ContextLock const & context
		, CmdCheckFramebuffer const & cmd )
	{
		auto status = glLogNonVoidCall( context
			, glCheckFramebufferStatus
			, GL_FRAMEBUFFER );
		checkCompleteness( context.getDevice(), status );
	}

	void apply( ContextLock const & context
		, CmdCleanupFramebuffer const & cmd )
	{
		if ( ( *cmd.fbo ) != GL_INVALID_INDEX )
		{
			glLogCall( context
				, glDeleteFramebuffers
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
		, CmdBindSrcFramebuffer const & cmd )
	{
		if ( get( get( context.getDevice() )->getBlitSrcFbo() )->getInternal() == GL_INVALID_INDEX )
		{
			glLogCall( context
				, glGenFramebuffers
				, 1
				, &get( get( context.getDevice() )->getBlitSrcFbo() )->getInternal() );
		}

		glLogCall( context
			, glBindFramebuffer
			, cmd.target
			, get( get( context.getDevice() )->getBlitSrcFbo() )->getInternal() );
	}

	void apply( ContextLock const & context
		, CmdBindDstFramebuffer const & cmd )
	{
		if ( get( get( context.getDevice() )->getBlitDstFbo() )->getInternal() == GL_INVALID_INDEX )
		{
			glLogCall( context
				, glGenFramebuffers
				, 1
				, &get( get( context.getDevice() )->getBlitDstFbo() )->getInternal() );
		}

		glLogCall( context
			, glBindFramebuffer
			, cmd.target
			, get( get( context.getDevice() )->getBlitDstFbo() )->getInternal() );
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
			, glBlendEquationSeparatei
			, cmd.index
			, cmd.color
			, cmd.alpha );
	}

	void apply( ContextLock const & context
		, CmdBlendFunc const & cmd )
	{
		glLogCall( context
			, glBlendFuncSeparatei
			, cmd.index
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
		, CmdLogCommand const & cmd )
	{
		logDebug( ( "*** " + std::string{ cmd.value } + " ***" ).c_str() );
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
			, glMinSampleShading
			, cmd.value );
	}

	void apply( ContextLock const & context
		, CmdPatchParameter const & cmd )
	{
		glLogCall( context
			, glPatchParameteri
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
			, glPolygonOffsetClamp
			, cmd.slopeFactor
			, cmd.constantFactor
			, cmd.clamp );
	}

	void apply( ContextLock const & context
		, CmdPopDebugGroup const & cmd )
	{
		glLogEmptyCall( context
			, glPopDebugGroup );
		popDebugBlock();
	}

	void apply( ContextLock const & context
		, CmdPrimitiveRestartIndex const & cmd )
	{
		glLogCall( context
			, glPrimitiveRestartIndex
			, cmd.index );
		
	}

	void apply( ContextLock const & context
		, CmdPushDebugGroup const & cmd )
	{
		pushDebugBlock( cmd.message );
		glLogCall( context
			, glPushDebugGroup
			, cmd.source
			, cmd.id
			, cmd.length
			, cmd.message );
		
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
		, CmdFramebufferTexture1D const & cmd )
	{
		glLogCall( context
			, glFramebufferTexture1D
			, cmd.target
			, cmd.point
			, cmd.texTarget
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
		, CmdFramebufferTexture3D const & cmd )
	{
		glLogCall( context
			, glFramebufferTexture3D
			, cmd.target
			, cmd.point
			, cmd.texTarget
			, cmd.object
			, cmd.mipLevel
			, cmd.slice );
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

	void apply( ContextLock const & context
		, CmdReadPixels const & cmd )
	{
		glLogCall( context
			, glReadPixels
			, cmd.x
			, cmd.y
			, cmd.width
			, cmd.height
			, cmd.format
			, cmd.type
			, nullptr );
	}

	void apply( ContextLock const & context
		, CmdPixelStore const & cmd )
	{
		glLogCall( context
			, glPixelStorei
			, cmd.name
			, cmd.param );
	}

	void apply( ContextLock const & context
		, CmdCompressedTexSubImage1D const & cmd )
	{
		glLogCall( context
			, glCompressedTexSubImage1D
			, cmd.copyTarget
			, cmd.mipLevel
			, cmd.x
			, cmd.width
			, cmd.format
			, cmd.imageSize
			, getBufferOffset( cmd.bufferOffset ) );
	}

	void apply( ContextLock const & context
		, CmdCompressedTexSubImage2D const & cmd )
	{
		glLogCall( context
			, glCompressedTexSubImage2D
			, cmd.copyTarget
			, cmd.mipLevel
			, cmd.x
			, cmd.y
			, cmd.width
			, cmd.height
			, cmd.format
			, cmd.imageSize
			, getBufferOffset( cmd.bufferOffset ) );
	}

	void apply( ContextLock const & context
		, CmdCompressedTexSubImage3D const & cmd )
	{
		glLogCall( context
			, glCompressedTexSubImage3D
			, cmd.copyTarget
			, cmd.mipLevel
			, cmd.x
			, cmd.y
			, cmd.z
			, cmd.width
			, cmd.height
			, cmd.depth
			, cmd.format
			, cmd.imageSize
			, getBufferOffset( cmd.bufferOffset ) );
	}

	void apply( ContextLock const & context
		, CmdCopyBufferSubData const & cmd )
	{
		glLogCall( context
			, glCopyBufferSubData
			, cmd.srcTarget
			, cmd.dstTarget
			, GLintptr( cmd.copy.srcOffset )
			, GLintptr( cmd.copy.dstOffset )
			, GLsizeiptr( cmd.copy.size ) );
	}

	void apply( ContextLock const & context
		, CmdCopyImageSubData const & cmd )
	{
		glLogCall( context
			, glCopyImageSubData
			, cmd.srcName
			, cmd.srcTarget
			, cmd.copy.srcSubresource.mipLevel
			, cmd.copy.srcOffset.x
			, cmd.copy.srcOffset.y
			, ( cmd.copy.srcSubresource.baseArrayLayer
				? GLint( cmd.copy.srcSubresource.baseArrayLayer )
				: GLint( cmd.copy.srcOffset.z ) )
			, cmd.dstName
			, cmd.dstTarget
			, cmd.copy.dstSubresource.mipLevel
			, cmd.copy.dstOffset.x
			, cmd.copy.dstOffset.y
			, ( cmd.copy.dstSubresource.baseArrayLayer
				? GLint( cmd.copy.dstSubresource.baseArrayLayer )
				: GLint( cmd.copy.dstOffset.z ) )
			, GLsizei( cmd.copy.extent.width )
			, GLsizei( cmd.copy.extent.height )
			, ( cmd.copy.srcSubresource.layerCount > 1u
				? GLsizei( cmd.copy.srcSubresource.layerCount )
				: GLsizei( cmd.copy.extent.depth ) ) );
	}

	void apply( ContextLock const & context
		, CmdTexSubImage1D const & cmd )
	{
		glLogCall( context
			, glTexSubImage1D
			, cmd.copyTarget
			, cmd.mipLevel
			, cmd.x
			, cmd.width
			, cmd.format
			, cmd.type
			, getBufferOffset( cmd.bufferOffset ) );
	}

	void apply( ContextLock const & context
		, CmdTexSubImage2D const & cmd )
	{
		glLogCall( context
			, glTexSubImage2D
			, cmd.copyTarget
			, cmd.mipLevel
			, cmd.x
			, cmd.y
			, cmd.width
			, cmd.height
			, cmd.format
			, cmd.type
			, getBufferOffset( cmd.bufferOffset ) );
	}

	void apply( ContextLock const & context
		, CmdTexSubImage3D const & cmd )
	{
		glLogCall( context
			, glTexSubImage3D
			, cmd.copyTarget
			, cmd.mipLevel
			, cmd.x
			, cmd.y
			, cmd.z
			, cmd.width
			, cmd.height
			, cmd.depth
			, cmd.format
			, cmd.type
			, getBufferOffset( cmd.bufferOffset ) );
	}
}
