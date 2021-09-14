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
		, CmdActiveTexture const & cmd )
	{
		glLogCall( context
			, glActiveTexture
			, GlTextureUnit( GL_TEXTURE0 + cmd.binding ) );
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
		, CmdApplyScissors const & cmd )
	{
		glLogCall( context
			, glScissorArrayv
			, GLuint( cmd.first )
			, GLsizei( cmd.count )
			, cmd.scissors.data() );
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
		, CmdApplyViewports const & cmd )
	{
		glLogCall( context
			, glViewportArrayv
			, GLuint( cmd.first )
			, GLsizei( cmd.count )
			, cmd.viewports.data() );
	}

	void apply( ContextLock const & context
		, CmdBeginQuery const & cmd )
	{
		glLogCall( context
			, glBeginQuery
			, GlQueryType( cmd.target )
			, GLuint( cmd.query ) );
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
		, CmdBindBufferRange const & cmd )
	{
		glLogCall( context
			, glBindBufferRange
			, cmd.target
			, cmd.binding
			, cmd.name
			, GLintptr( cmd.offset )
			, GLsizeiptr( cmd.range ) );
	}

	void apply( ContextLock const & context
		, CmdBindContextState const & cmd )
	{
		cmd.stack->apply( context, *cmd.state );
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
		, CmdBindImage const & cmd )
	{
		glLogCall( context
			, glBindImageTexture
			, cmd.binding
			, cmd.name
			, GLint( cmd.baseMipLevel )
			, cmd.layerCount != 0 ? GL_TRUE : GL_FALSE
			, GLint( cmd.baseArrayLayer )
			, GL_ACCESS_TYPE_READ_WRITE
			, cmd.internal );
	}

	void apply( ContextLock const & context
		, CmdBindSampler const & cmd )
	{
		glLogCall( context
			, glBindSampler
			, cmd.binding
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
		, CmdBindVextexArray const & cmd )
	{
		if ( cmd.vao )
		{
			glLogCall( context
				, glBindVertexArray
				, cmd.vao->getVao() );
		}
		else
		{
			glLogCall( context
				, glBindVertexArray
				, 0u );
		}
	}

	void apply( ContextLock const & context
		, CmdBindVextexArrayObject const & cmd )
	{
		glLogCall( context
			, glBindVertexArray
			, cmd.vao );
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
		, CmdBlitFramebuffer const & cmd )
	{
		glLogCall( context
			, glBlitFramebuffer
			, cmd.srcL
			, cmd.srcT
			, cmd.srcR
			, cmd.srcB
			, cmd.dstL
			, cmd.dstT
			, cmd.dstR
			, cmd.dstB
			, cmd.mask
			, cmd.filter );
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
		, CmdClearBack const & cmd )
	{
		glLogCall( context
			, glClear
			, cmd.mask );
	}

	void apply( ContextLock const & context
		, CmdClearBackColour const & cmd )
	{
		glLogCall( context
			, glClearColor
			, cmd.color.float32[0]
			, cmd.color.float32[1]
			, cmd.color.float32[2]
			, cmd.color.float32[3] );
	}

	void apply( ContextLock const & context
		, CmdClearBackDepth const & cmd )
	{
		glLogCall( context
			, glClearDepth
			, cmd.depth );
	}

	void apply( ContextLock const & context
		, CmdClearBackStencil const & cmd )
	{
		glLogCall( context
			, glClearStencil
			, cmd.stencil );
	}

	void apply( ContextLock const & context
		, CmdClearBackDepthStencil const & cmd )
	{
		glLogCall( context
			, glClearDepth
			, cmd.depth );
		glLogCall( context
			, glClearStencil
			, cmd.stencil );
	}

	void apply( ContextLock const & context
		, CmdClearColour const & cmd )
	{
		glLogCall( context
			, glClearBufferfv
			, GL_CLEAR_TARGET_COLOR
			, GLint( cmd.colourIndex )
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
			, 0
			, &cmd.depth );
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
			, 0
			, cmd.depthStencil.depth
			, GLint( cmd.depthStencil.stencil ) );
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
		, CmdClearTexColorF const & cmd )
	{
		glLogCall( context
			, glClearTexImage
			, cmd.name
			, GLint( cmd.mipLevel )
			, cmd.format
			, GL_TYPE_F32
			, cmd.color.data() );
	}

	void apply( ContextLock const & context
		, CmdClearTexColorUI const & cmd )
	{
		glLogCall( context
			, glClearTexImage
			, cmd.name
			, GLint( cmd.mipLevel )
			, cmd.format
			, GL_TYPE_UI32
			, cmd.color.data() );
	}

	void apply( ContextLock const & context
		, CmdClearTexColorSI const & cmd )
	{
		glLogCall( context
			, glClearTexImage
			, cmd.name
			, GLint( cmd.mipLevel )
			, cmd.format
			, GL_TYPE_I32
			, cmd.color.data() );
	}

	void apply( ContextLock const & context
		, CmdClearTexDepth const & cmd )
	{
		glLogCall( context
			, glClearTexImage
			, cmd.name
			, GLint( cmd.mipLevel )
			, cmd.format
			, cmd.type
			, &cmd.depth );
	}

	void apply( ContextLock const & context
		, CmdClearTexDepthStencil const & cmd )
	{
		glLogCall( context
			, glClearTexImage
			, cmd.name
			, GLint( cmd.mipLevel )
			, cmd.format
			, cmd.type
			, &cmd.depthStencil.depth );
	}

	void apply( ContextLock const & context
		, CmdClearTexStencil const & cmd )
	{
		glLogCall( context
			, glClearTexImage
			, cmd.name
			, GLint( cmd.mipLevel )
			, cmd.format
			, cmd.type
			, &cmd.stencil );
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
		, CmdCompressedTexSubImage1D const & cmd )
	{
		glLogCall( context
			, glCompressedTexSubImage1D
			, cmd.copyTarget
			, GLint( cmd.mipLevel )
			, cmd.x
			, GLsizei( cmd.width )
			, cmd.format
			, GLsizei( cmd.imageSize )
			, getBufferOffset( cmd.bufferOffset ) );
	}

	void apply( ContextLock const & context
		, CmdCompressedTexSubImage2D const & cmd )
	{
		glLogCall( context
			, glCompressedTexSubImage2D
			, cmd.copyTarget
			, GLint( cmd.mipLevel )
			, cmd.x
			, cmd.y
			, GLsizei( cmd.width )
			, GLsizei( cmd.height )
			, cmd.format
			, GLsizei( cmd.imageSize )
			, getBufferOffset( cmd.bufferOffset ) );
	}

	void apply( ContextLock const & context
		, CmdCompressedTexSubImage3D const & cmd )
	{
		glLogCall( context
			, glCompressedTexSubImage3D
			, cmd.copyTarget
			, GLint( cmd.mipLevel )
			, cmd.x
			, cmd.y
			, cmd.z
			, GLsizei( cmd.width )
			, GLsizei( cmd.height )
			, GLsizei( cmd.depth )
			, cmd.format
			, GLsizei( cmd.imageSize )
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
			, GLint( cmd.copy.srcSubresource.mipLevel )
			, cmd.copy.srcOffset.x
			, cmd.copy.srcOffset.y
			, ( cmd.copy.srcSubresource.baseArrayLayer
				? GLint( cmd.copy.srcSubresource.baseArrayLayer )
				: GLint( cmd.copy.srcOffset.z ) )
			, cmd.dstName
			, cmd.dstTarget
			, GLint( cmd.copy.dstSubresource.mipLevel )
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
			, GLboolean( cmd.value ) );
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
		, CmdDisable const & cmd )
	{
		glLogCall( context
			, glDisable
			, cmd.value );
	}

	void apply( ContextLock const & context
		, CmdDispatch const & cmd )
	{
		glLogCall( context
			, glDispatchCompute
			, cmd.groupCountX
			, cmd.groupCountY
			, cmd.groupCountZ );
	}

	void apply( ContextLock const & context
		, CmdDispatchIndirect const & cmd )
	{
		glLogCall( context
			, glDispatchComputeIndirect
			, GLintptr( getBufferOffset( intptr_t( cmd.offset ) ) ) );
	}

	void apply( ContextLock const & context
		, CmdDownloadMemory const & cmd )
	{
		get( cmd.memory )->download( context
			, cmd.offset
			, cmd.size );
	}

	void apply( ContextLock const & context
		, CmdDraw const & cmd )
	{
		glLogCall( context
			, glDrawArraysInstanced
			, cmd.mode
			, GLint( cmd.firstVertex )
			, GLsizei( cmd.vtxCount )
			, GLsizei( cmd.instCount ) );
	}

	void apply( ContextLock const & context
		, CmdDrawBaseInstance const & cmd )
	{
		glLogCall( context
			, glDrawArraysInstancedBaseInstance
			, cmd.mode
			, GLint( cmd.firstVertex )
			, GLsizei( cmd.vtxCount )
			, GLsizei( cmd.instCount )
			, GLuint( cmd.firstInstance ) );
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
		, CmdDrawIndexed const & cmd )
	{
		glLogCall( context
			, glDrawElementsInstancedBaseVertex
			, cmd.mode
			, GLsizei( cmd.indexCount )
			, cmd.type
			, getBufferOffset( intptr_t( cmd.indexOffset ) )
			, GLsizei( cmd.instCount )
			, GLint( cmd.vertexOffset ) );
	}

	void apply( ContextLock const & context
		, CmdDrawIndexedBaseInstance const & cmd )
	{
		glLogCall( context
			, glDrawElementsInstancedBaseVertexBaseInstance
			, cmd.mode
			, GLsizei( cmd.indexCount )
			, cmd.type
			, getBufferOffset( intptr_t( cmd.indexOffset ) )
			, GLsizei( cmd.instCount )
			, GLint( cmd.vertexOffset )
			, GLuint( cmd.firstInstance ) );
	}

	void apply( ContextLock const & context
		, CmdDrawIndexedIndirect const & cmd )
	{
		glLogCall( context
			, glMultiDrawElementsIndirect
			, cmd.mode
			, cmd.type
			, getBufferOffset( intptr_t( cmd.offset ) )
			, GLsizei( cmd.drawCount )
			, GLsizei( cmd.stride ) );
	}

	void apply( ContextLock const & context
		, CmdDrawIndirect const & cmd )
	{
		glLogCall( context
			, glMultiDrawArraysIndirect
			, cmd.mode
			, getBufferOffset( intptr_t( cmd.offset ) )
			, GLsizei( cmd.drawCount )
			, GLsizei( cmd.stride ) );
	}

	void apply( ContextLock const & context
		, CmdEnable const & cmd )
	{
		glLogCall( context
			, glEnable
			, cmd.value );
	}

	void apply( ContextLock const & context
		, CmdEndQuery const & cmd )
	{
		glLogCall( context
			, glEndQuery
			, cmd.target );
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
		, CmdFramebufferTexture const & cmd )
	{
		glLogCall( context
			, glFramebufferTexture
			, cmd.target
			, cmd.point
			, cmd.object
			, GLint( cmd.mipLevel ) );
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
			, GLint( cmd.mipLevel ) );
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
			, GLint( cmd.mipLevel ) );
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
			, GLint( cmd.mipLevel )
			, GLint( cmd.slice ) );
	}

	void apply( ContextLock const & context
		, CmdFramebufferTextureLayer const & cmd )
	{
		glLogCall( context
			, glFramebufferTextureLayer
			, cmd.target
			, cmd.point
			, cmd.object
			, GLint( cmd.mipLevel )
			, GLint( cmd.arrayLayer ) );
	}

	void apply( ContextLock const & context
		, CmdFrontFace const & cmd )
	{
		glLogCall( context
			, glFrontFace
			, cmd.value );
	}

	void apply( ContextLock const & context
		, CmdGenerateMipmaps const & cmd )
	{
		glLogCall( context
			, glGenerateMipmap
			, cmd.target );
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
			, getBufferOffset( intptr_t( cmd.bufferOffset ) ) );
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
		, CmdMemoryBarrier const & cmd )
	{
		glLogCall( context
			, glMemoryBarrier
			, cmd.flags );
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
		, CmdPixelStore const & cmd )
	{
		glLogCall( context
			, glPixelStorei
			, cmd.name
			, cmd.param );
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
		, CmdProgramUniform1fv const & cmd )
	{
		glLogCall( context
			, glProgramUniform1fv
			, cmd.program
			, GLint( cmd.location )
			, 1u
			, &cmd.buffer );
	}

	void apply( ContextLock const & context
		, CmdProgramUniform2fv const & cmd )
	{
		glLogCall( context
			, glProgramUniform2fv
			, cmd.program
			, GLint( cmd.location )
			, 1u
			, cmd.buffer );
	}

	void apply( ContextLock const & context
		, CmdProgramUniform3fv const & cmd )
	{
		glLogCall( context
			, glProgramUniform3fv
			, cmd.program
			, GLint( cmd.location )
			, 1u
			, cmd.buffer );
	}

	void apply( ContextLock const & context
		, CmdProgramUniform4fv const & cmd )
	{
		glLogCall( context
			, glProgramUniform4fv
			, cmd.program
			, GLint( cmd.location )
			, 1u
			, cmd.buffer );
	}

	void apply( ContextLock const & context
		, CmdProgramUniform1iv const & cmd )
	{
		glLogCall( context
			, glProgramUniform1iv
			, cmd.program
			, GLint( cmd.location )
			, 1u
			, &cmd.buffer );
	}

	void apply( ContextLock const & context
		, CmdProgramUniform2iv const & cmd )
	{
		glLogCall( context
			, glProgramUniform2iv
			, cmd.program
			, GLint( cmd.location )
			, 1u
			, cmd.buffer );
	}

	void apply( ContextLock const & context
		, CmdProgramUniform3iv const & cmd )
	{
		glLogCall( context
			, glProgramUniform3iv
			, cmd.program
			, GLint( cmd.location )
			, 1u
			, cmd.buffer );
	}

	void apply( ContextLock const & context
		, CmdProgramUniform4iv const & cmd )
	{
		glLogCall( context
			, glProgramUniform4iv
			, cmd.program
			, GLint( cmd.location )
			, 1u
			, cmd.buffer );
	}

	void apply( ContextLock const & context
		, CmdProgramUniform1uiv const & cmd )
	{
		glLogCall( context
			, glProgramUniform1uiv
			, cmd.program
			, GLint( cmd.location )
			, 1u
			, &cmd.buffer );
	}

	void apply( ContextLock const & context
		, CmdProgramUniform2uiv const & cmd )
	{
		glLogCall( context
			, glProgramUniform2uiv
			, cmd.program
			, GLint( cmd.location )
			, 1u
			, cmd.buffer );
	}

	void apply( ContextLock const & context
		, CmdProgramUniform3uiv const & cmd )
	{
		glLogCall( context
			, glProgramUniform3uiv
			, cmd.program
			, GLint( cmd.location )
			, 1u
			, cmd.buffer );
	}

	void apply( ContextLock const & context
		, CmdProgramUniform4uiv const & cmd )
	{
		glLogCall( context
			, glProgramUniform4uiv
			, cmd.program
			, GLint( cmd.location )
			, 1u
			, cmd.buffer );
	}

	void apply( ContextLock const & context
		, CmdProgramUniformMatrix2fv const & cmd )
	{
		glLogCall( context
			, glProgramUniformMatrix2fv
			, cmd.program
			, GLint( cmd.location )
			, 1u
			, cmd.transpose ? GL_TRUE : GL_FALSE
			, cmd.buffer );
	}

	void apply( ContextLock const & context
		, CmdProgramUniformMatrix3fv const & cmd )
	{
		glLogCall( context
			, glProgramUniformMatrix3fv
			, cmd.program
			, GLint( cmd.location )
			, 1u
			, cmd.transpose ? GL_TRUE : GL_FALSE
			, cmd.buffer );
	}

	void apply( ContextLock const & context
		, CmdProgramUniformMatrix4fv const & cmd )
	{
		glLogCall( context
			, glProgramUniformMatrix4fv
			, cmd.program
			, GLint( cmd.location )
			, 1u
			, cmd.transpose ? GL_TRUE : GL_FALSE
			, cmd.buffer );
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
		, CmdReadBuffer const & cmd )
	{
		glLogCall( context
			, glReadBuffer
			, cmd.point );
	}

	void apply( ContextLock const & context
		, CmdReadPixels const & cmd )
	{
		glLogCall( context
			, glReadPixels
			, cmd.x
			, cmd.y
			, GLsizei( cmd.width )
			, GLsizei( cmd.height )
			, cmd.format
			, cmd.type
			, nullptr );
	}

	void apply( ContextLock const & context
		, CmdResetEvent const & cmd )
	{
		get( cmd.event )->reset();
	}

	void apply( ContextLock const & context
		, CmdSetEvent const & cmd )
	{
		get( cmd.event )->set();
	}

	void apply( ContextLock const & context
		, CmdSetLineWidth const & cmd )
	{
		glLogCall( context
			, glLineWidth
			, cmd.width );
	}

	void apply( ContextLock const & context
		, CmdStencilFunc const & cmd )
	{
		glLogCall( context
			, glStencilFuncSeparate
			, cmd.face
			, cmd.op
			, GLint( cmd.ref )
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
		, CmdTexParameteri const & cmd )
	{
		glLogCall( context
			, glTexParameteri
			, cmd.target
			, cmd.name
			, cmd.param );
	}

	void apply( ContextLock const & context
		, CmdTexParameterf const & cmd )
	{
		glLogCall( context
			, glTexParameterf
			, cmd.target
			, cmd.name
			, cmd.param );
	}

	void apply( ContextLock const & context
		, CmdTexSubImage1D const & cmd )
	{
		glLogCall( context
			, glTexSubImage1D
			, cmd.copyTarget
			, GLint( cmd.mipLevel )
			, cmd.x
			, GLsizei( cmd.width )
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
			, GLint( cmd.mipLevel )
			, cmd.x
			, cmd.y
			, GLsizei( cmd.width )
			, GLsizei( cmd.height )
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
			, GLint( cmd.mipLevel )
			, cmd.x
			, cmd.y
			, cmd.z
			, GLsizei( cmd.width )
			, GLsizei( cmd.height )
			, GLsizei( cmd.depth )
			, cmd.format
			, cmd.type
			, getBufferOffset( cmd.bufferOffset ) );
	}

	void apply( ContextLock const & context
		, CmdUniform1fv const & cmd )
	{
		glLogCall( context
			, glUniform1fv
			, GLint( cmd.location )
			, 1u
			, &cmd.buffer );
	}

	void apply( ContextLock const & context
		, CmdUniform2fv const & cmd )
	{
		glLogCall( context
			, glUniform2fv
			, GLint( cmd.location )
			, 1u
			, cmd.buffer );
	}

	void apply( ContextLock const & context
		, CmdUniform3fv const & cmd )
	{
		glLogCall( context
			, glUniform3fv
			, GLint( cmd.location )
			, 1u
			, cmd.buffer );
	}

	void apply( ContextLock const & context
		, CmdUniform4fv const & cmd )
	{
		glLogCall( context
			, glUniform4fv
			, GLint( cmd.location )
			, 1u
			, cmd.buffer );
	}

	void apply( ContextLock const & context
		, CmdUniform1iv const & cmd )
	{
		glLogCall( context
			, glUniform1iv
			, GLint( cmd.location )
			, 1u
			, &cmd.buffer );
	}

	void apply( ContextLock const & context
		, CmdUniform2iv const & cmd )
	{
		glLogCall( context
			, glUniform2iv
			, GLint( cmd.location )
			, 1u
			, cmd.buffer );
	}

	void apply( ContextLock const & context
		, CmdUniform3iv const & cmd )
	{
		glLogCall( context
			, glUniform3iv
			, GLint( cmd.location )
			, 1u
			, cmd.buffer );
	}

	void apply( ContextLock const & context
		, CmdUniform4iv const & cmd )
	{
		glLogCall( context
			, glUniform4iv
			, GLint( cmd.location )
			, 1u
			, cmd.buffer );
	}

	void apply( ContextLock const & context
		, CmdUniform1uiv const & cmd )
	{
		glLogCall( context
			, glUniform1uiv
			, GLint( cmd.location )
			, 1u
			, &cmd.buffer );
	}

	void apply( ContextLock const & context
		, CmdUniform2uiv const & cmd )
	{
		glLogCall( context
			, glUniform2uiv
			, GLint( cmd.location )
			, 1u
			, cmd.buffer );
	}

	void apply( ContextLock const & context
		, CmdUniform3uiv const & cmd )
	{
		glLogCall( context
			, glUniform3uiv
			, GLint( cmd.location )
			, 1u
			, cmd.buffer );
	}

	void apply( ContextLock const & context
		, CmdUniform4uiv const & cmd )
	{
		glLogCall( context
			, glUniform4uiv
			, GLint( cmd.location )
			, 1u
			, cmd.buffer );
	}

	void apply( ContextLock const & context
		, CmdUniformMatrix2fv const & cmd )
	{
		glLogCall( context
			, glUniformMatrix2fv
			, GLint( cmd.location )
			, 1u
			, cmd.transpose ? GL_TRUE : GL_FALSE
			, cmd.buffer );
	}

	void apply( ContextLock const & context
		, CmdUniformMatrix3fv const & cmd )
	{
		glLogCall( context
			, glUniformMatrix3fv
			, GLint( cmd.location )
			, 1u
			, cmd.transpose ? GL_TRUE : GL_FALSE
			, cmd.buffer );
	}

	void apply( ContextLock const & context
		, CmdUniformMatrix4fv const & cmd )
	{
		glLogCall( context
			, glUniformMatrix4fv
			, GLint( cmd.location )
			, 1u
			, cmd.transpose ? GL_TRUE : GL_FALSE
			, cmd.buffer );
	}

	void apply( ContextLock const & context
		, CmdUpdateBuffer const & cmd )
	{
		glLogCall( context
			, glBindBuffer
			, GL_BUFFER_TARGET_COPY_WRITE
			, get( cmd.memory )->getInternal() );
		glLogCall( context
			, glBufferSubData
			, GL_BUFFER_TARGET_COPY_WRITE
			, GLintptr( cmd.memoryOffset )
			, GLsizeiptr( cmd.dataSize )
			, cmd.pData );
		glLogCall( context
			, glBindBuffer
			, GL_BUFFER_TARGET_COPY_WRITE
			, 0 );
	}

	void apply( ContextLock const & context
		, CmdUploadMemory const & cmd )
	{
		get( cmd.memory )->upload( context
			, cmd.offset
			, cmd.size );
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
		, CmdWaitEvents const & cmd )
	{
		auto count = 0u;

		do
		{
			count = uint32_t( std::count_if( cmd.events.begin()
				, cmd.events.end()
				, []( VkEvent event )
				{
					return get( event )->getStatus() == VK_EVENT_SET
						|| get( event )->getStatus() == VK_EVENT_RESET;
				} ) );
			std::this_thread::sleep_for( std::chrono::nanoseconds{ 10 } );
		}
		while ( count != cmd.events.size() );
	}

	void apply( ContextLock const & context
		, CmdWriteTimestamp const & cmd )
	{
		glLogCall( context
			, glQueryCounter
			, cmd.name
			, GL_QUERY_TYPE_TIMESTAMP );
	}
}
