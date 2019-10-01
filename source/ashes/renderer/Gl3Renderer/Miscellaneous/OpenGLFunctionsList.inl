/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/

// Functions from GL/gl.h
#ifndef GL_LIB_BASE_FUNCTION
#	define GL_LIB_BASE_FUNCTION( x )
#endif

GL_LIB_BASE_FUNCTION( BindTexture )
GL_LIB_BASE_FUNCTION( Clear )
GL_LIB_BASE_FUNCTION( ClearColor )
GL_LIB_BASE_FUNCTION( ClearDepth )
GL_LIB_BASE_FUNCTION( ClearStencil )
GL_LIB_BASE_FUNCTION( CullFace )
GL_LIB_BASE_FUNCTION( DeleteTextures )
GL_LIB_BASE_FUNCTION( DepthFunc )
GL_LIB_BASE_FUNCTION( DepthMask )
GL_LIB_BASE_FUNCTION( DepthRange )
GL_LIB_BASE_FUNCTION( Disable )
GL_LIB_BASE_FUNCTION( Enable )
GL_LIB_BASE_FUNCTION( Finish )
GL_LIB_BASE_FUNCTION( FrontFace )
GL_LIB_BASE_FUNCTION( GenTextures )
GL_LIB_BASE_FUNCTION( GetError )
GL_LIB_BASE_FUNCTION( GetFloatv )
GL_LIB_BASE_FUNCTION( GetIntegerv )
GL_LIB_BASE_FUNCTION( GetString )
GL_LIB_BASE_FUNCTION( GetTexImage )
GL_LIB_BASE_FUNCTION( GetTexLevelParameterfv )
GL_LIB_BASE_FUNCTION( GetTexLevelParameteriv )
GL_LIB_BASE_FUNCTION( GetTexParameterfv )
GL_LIB_BASE_FUNCTION( GetTexParameteriv )
GL_LIB_BASE_FUNCTION( LineWidth )
GL_LIB_BASE_FUNCTION( LogicOp )
GL_LIB_BASE_FUNCTION( PolygonMode )
GL_LIB_BASE_FUNCTION( PixelStorei )
GL_LIB_BASE_FUNCTION( ReadBuffer )
GL_LIB_BASE_FUNCTION( ReadPixels )
GL_LIB_BASE_FUNCTION( Scissor )
GL_LIB_BASE_FUNCTION( TexEnvfv )
GL_LIB_BASE_FUNCTION( TexImage1D )
GL_LIB_BASE_FUNCTION( TexImage2D )
GL_LIB_BASE_FUNCTION( TexParameteri )
GL_LIB_BASE_FUNCTION( TexParameterf )
GL_LIB_BASE_FUNCTION( TexSubImage1D )
GL_LIB_BASE_FUNCTION( TexSubImage2D )
GL_LIB_BASE_FUNCTION( Viewport )

#undef GL_LIB_BASE_FUNCTION

#ifndef GL_LIB_FUNCTION
#	define GL_LIB_FUNCTION( x )
#endif

GL_LIB_FUNCTION( ActiveTexture )
GL_LIB_FUNCTION( AttachShader )
GL_LIB_FUNCTION( BeginQuery )
GL_LIB_FUNCTION( BindBuffer )
GL_LIB_FUNCTION( BindBufferBase )
GL_LIB_FUNCTION( BindBufferRange )
GL_LIB_FUNCTION( BindFramebuffer )
GL_LIB_FUNCTION( BindSampler )
GL_LIB_FUNCTION( BindVertexArray )
GL_LIB_FUNCTION( BlendColor )
GL_LIB_FUNCTION( BlendEquationSeparate )
GL_LIB_FUNCTION( BlendFuncSeparate )
GL_LIB_FUNCTION( BlitFramebuffer )
GL_LIB_FUNCTION( BufferData )
GL_LIB_FUNCTION( CheckFramebufferStatus )
GL_LIB_FUNCTION( ClearBufferfi )
GL_LIB_FUNCTION( ClearBufferfv )
GL_LIB_FUNCTION( ClearBufferiv )
GL_LIB_FUNCTION( ClearBufferuiv )
GL_LIB_FUNCTION( ClientWaitSync )
GL_LIB_FUNCTION( CompileShader )
GL_LIB_FUNCTION( CompressedTexSubImage1D )
GL_LIB_FUNCTION( CompressedTexSubImage2D )
GL_LIB_FUNCTION( CompressedTexSubImage3D )
GL_LIB_FUNCTION( CopyBufferSubData )
GL_LIB_FUNCTION( CreateProgram )
GL_LIB_FUNCTION( CreateShader )
GL_LIB_FUNCTION( DeleteBuffers )
GL_LIB_FUNCTION( DeleteFramebuffers )
GL_LIB_FUNCTION( DeleteProgram )
GL_LIB_FUNCTION( DeleteQueries )
GL_LIB_FUNCTION( DeleteSamplers )
GL_LIB_FUNCTION( DeleteShader )
GL_LIB_FUNCTION( DeleteSync )
GL_LIB_FUNCTION( DeleteVertexArrays )
GL_LIB_FUNCTION( DrawArraysInstanced )
GL_LIB_FUNCTION( DrawBuffers )
GL_LIB_FUNCTION( DrawElementsBaseVertex )
GL_LIB_FUNCTION( DrawElementsInstanced )
GL_LIB_FUNCTION( DrawElementsInstancedBaseVertex )
GL_LIB_FUNCTION( EnableVertexAttribArray )
GL_LIB_FUNCTION( EndQuery )
GL_LIB_FUNCTION( FenceSync )
GL_LIB_FUNCTION( FlushMappedBufferRange )
GL_LIB_FUNCTION( FramebufferTexture1D )
GL_LIB_FUNCTION( FramebufferTexture2D )
GL_LIB_FUNCTION( FramebufferTexture3D )
GL_LIB_FUNCTION( FramebufferTextureLayer )
GL_LIB_FUNCTION( GenBuffers )
GL_LIB_FUNCTION( GenFramebuffers )
GL_LIB_FUNCTION( GenQueries )
GL_LIB_FUNCTION( GenSamplers )
GL_LIB_FUNCTION( GenerateMipmap )
GL_LIB_FUNCTION( GenVertexArrays )
GL_LIB_FUNCTION( GetProgramInfoLog )
GL_LIB_FUNCTION( GetProgramInterfaceiv )
GL_LIB_FUNCTION( GetProgramiv )
GL_LIB_FUNCTION( GetProgramResourceiv )
GL_LIB_FUNCTION( GetProgramResourceIndex )
GL_LIB_FUNCTION( GetProgramResourceName )
GL_LIB_FUNCTION( GetQueryObjectui64v )
GL_LIB_FUNCTION( GetQueryObjectuiv )
GL_LIB_FUNCTION( GetShaderInfoLog )
GL_LIB_FUNCTION( GetShaderiv )
GL_LIB_FUNCTION( GetShaderSource )
GL_LIB_FUNCTION( LinkProgram )
GL_LIB_FUNCTION( MapBufferRange )
GL_LIB_FUNCTION( PolygonOffsetClampEXT )
GL_LIB_FUNCTION( QueryCounter )
GL_LIB_FUNCTION( SamplerParameterf )
GL_LIB_FUNCTION( SamplerParameterfv )
GL_LIB_FUNCTION( SamplerParameteri )
GL_LIB_FUNCTION( SamplerParameteriv )
GL_LIB_FUNCTION( ShaderSource )
GL_LIB_FUNCTION( StencilFuncSeparate )
GL_LIB_FUNCTION( StencilMaskSeparate )
GL_LIB_FUNCTION( StencilOpSeparate )
GL_LIB_FUNCTION( TexBuffer )
GL_LIB_FUNCTION( TexImage2DMultisample )
GL_LIB_FUNCTION( TexImage3D )
GL_LIB_FUNCTION( TexImage3DMultisample )
GL_LIB_FUNCTION( TexSubImage3D )
GL_LIB_FUNCTION( Uniform1fv )
GL_LIB_FUNCTION( Uniform1iv )
GL_LIB_FUNCTION( Uniform1uiv )
GL_LIB_FUNCTION( Uniform2fv )
GL_LIB_FUNCTION( Uniform2iv )
GL_LIB_FUNCTION( Uniform2uiv )
GL_LIB_FUNCTION( Uniform3fv )
GL_LIB_FUNCTION( Uniform3iv )
GL_LIB_FUNCTION( Uniform3uiv )
GL_LIB_FUNCTION( Uniform4fv )
GL_LIB_FUNCTION( Uniform4iv )
GL_LIB_FUNCTION( Uniform4uiv )
GL_LIB_FUNCTION( UniformMatrix2fv )
GL_LIB_FUNCTION( UniformMatrix3fv )
GL_LIB_FUNCTION( UniformMatrix4fv )
GL_LIB_FUNCTION( UnmapBuffer )
GL_LIB_FUNCTION( UseProgram )
GL_LIB_FUNCTION( VertexAttribDivisor )
GL_LIB_FUNCTION( VertexAttribPointer )
GL_LIB_FUNCTION( VertexAttribIPointer )

#undef GL_LIB_FUNCTION

#ifndef GL_LIB_FUNCTION_EXT
#	define GL_LIB_FUNCTION_EXT( x, ext, name )
#endif

GL_LIB_FUNCTION_EXT( BindImageTexture, ARB, GL_ARB_shader_image_load_store )
GL_LIB_FUNCTION_EXT( ClearTexImage, ARB, GL_ARB_clear_texture )
GL_LIB_FUNCTION_EXT( CopyImageSubData, ARB, GL_ARB_copy_image )
GL_LIB_FUNCTION_EXT( DispatchCompute, ARB, GL_ARB_combute_shader )
GL_LIB_FUNCTION_EXT( DispatchComputeIndirect, ARB, GL_ARB_combute_shader )
GL_LIB_FUNCTION_EXT( DrawArraysInstancedBaseInstance, ARB, GL_ARB_base_instance )
GL_LIB_FUNCTION_EXT( DrawElementsInstancedBaseVertexBaseInstance, ARB, GL_ARB_base_instance )
GL_LIB_FUNCTION_EXT( InvalidateBufferSubData, ARB, GL_ARB_invalidate_subdata )
GL_LIB_FUNCTION_EXT( MemoryBarrier, ARB, GL_ARB_shader_image_load_store )
GL_LIB_FUNCTION_EXT( MinSampleShading, ARB, GL_ARB_sample_shading )
GL_LIB_FUNCTION_EXT( MultiDrawArraysIndirect, ARB, GL_ARB_multi_draw_indirect )
GL_LIB_FUNCTION_EXT( MultiDrawElementsIndirect, ARB, GL_ARB_multi_draw_indirect )
GL_LIB_FUNCTION_EXT( PatchParameteri, ARB, GL_ARB_tessellation_shader )
GL_LIB_FUNCTION_EXT( ShaderBinary, ARB, GL_ARB_ES2_compatibility )
GL_LIB_FUNCTION_EXT( SpecializeShader, ARB, GL_ARB_gl_spirv )
GL_LIB_FUNCTION_EXT( TexBufferRange, ARB, GL_ARB_texture_buffer_range )
GL_LIB_FUNCTION_EXT( GetInternalformativ, ARB, GL_ARB_internalformat_query )
GL_LIB_FUNCTION_EXT( GetInternalformati64v, ARB, GL_ARB_internalformat_query2 )
GL_LIB_FUNCTION_EXT( ObjectLabel, ARB, GL_ARB_debug_output )
GL_LIB_FUNCTION_EXT( ObjectPtrLabel, ARB, GL_ARB_debug_output )

#undef GL_LIB_FUNCTION_EXT

#ifndef GL_LIB_FUNCTION_VSN
#	define GL_LIB_FUNCTION_VSN( x, version )
#endif

GL_LIB_FUNCTION_VSN( BlendEquationSeparatei, 40 )
GL_LIB_FUNCTION_VSN( BlendFuncSeparatei, 40 )

#undef GL_LIB_FUNCTION_VSN

// Extended WGL functions
#ifndef WGL_LIB_FUNCTION
#	define WGL_LIB_FUNCTION( x )
#endif

WGL_LIB_FUNCTION( SwapIntervalEXT )

#undef WGL_LIB_FUNCTION

#ifndef WGL_LIB_FUNCTION_EXT
#	define WGL_LIB_FUNCTION_EXT( x, ext )
#endif

#undef WGL_LIB_FUNCTION_EXT

// Extended GLX functions
#ifndef GLX_LIB_FUNCTION
#	define GLX_LIB_FUNCTION( x )
#endif

GLX_LIB_FUNCTION( SwapIntervalEXT )

#undef GLX_LIB_FUNCTION
