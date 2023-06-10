/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include "renderer/GlRenderer/GlRendererPrerequisites.hpp"

#include <ashes/common/ArrayView.hpp>

#include <array>
#include <cstring>

#pragma warning( push )
#pragma warning( disable: 4324 )

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wnull-dereference"

namespace ashes::gl
{
	//*************************************************************************

	enum class OpType
		: uint16_t
	{
		eActiveTexture,
		eApplyDepthRanges,
		eApplyScissor,
		eApplyScissors,
		eApplyViewport,
		eApplyViewports,
		eBeginQuery,
		eBindBuffer,
		eBindBufferRange,
		eBindContextState,
		eBindFramebuffer,
		eBindSrcFramebuffer,
		eBindDstFramebuffer,
		eBindImage,
		eBindSampler,
		eBindTexture,
		eBindVextexArray,
		eBindVextexArrayObject,
		eBlendConstants,
		eBlendEquation,
		eBlendFunc,
		eBlitFramebuffer,
		eCheckFramebuffer,
		eCleanupFramebuffer,
		eClearBack,
		eClearBackColour,
		eClearBackDepth,
		eClearBackDepthStencil,
		eClearBackStencil,
		eClearColour,
		eClearDepth,
		eClearDepthStencil,
		eClearStencil,
		eClearTexColorF,
		eClearTexColorUI,
		eClearTexColorSI,
		eClearTexDepth,
		eClearTexDepthStencil,
		eClearTexStencil,
		eColorMask,
		eCompressedTexSubImage1D,
		eCompressedTexSubImage2D,
		eCompressedTexSubImage3D,
		eCopyBufferSubData,
		eCopyImageSubData,
		eCullFace,
		eDepthFunc,
		eDepthMask,
		eDepthRange,
		eDisable,
		eDispatch,
		eDispatchIndirect,
		eDownloadMemory,
		eDraw,
		eDrawBaseInstance,
		eDrawBuffer,
		eDrawBuffers,
		eDrawIndexed,
		eDrawIndexedBaseInstance,
		eDrawIndexedIndirect,
		eDrawIndirect,
		eEnable,
		eEndQuery,
		eFillBuffer,
		eFramebufferTexture,
		eFramebufferTexture1D,
		eFramebufferTexture2D,
		eFramebufferTexture3D,
		eFramebufferTextureLayer,
		eFrontFace,
		eGenerateMipmaps,
		eGetCompressedTexImage,
		eGetQueryResults,
		eGetTexImage,
		eLineWidth,
		eLogCommand,
		eLogicOp,
		eMemoryBarrier,
		eMinSampleShading,
		ePatchParameter,
		ePixelStore,
		ePolygonMode,
		ePolygonOffset,
		ePopDebugGroup,
		ePrimitiveRestartIndex,
		eProgramUniform1fv,
		eProgramUniform2fv,
		eProgramUniform3fv,
		eProgramUniform4fv,
		eProgramUniform1iv,
		eProgramUniform2iv,
		eProgramUniform3iv,
		eProgramUniform4iv,
		eProgramUniform1uiv,
		eProgramUniform2uiv,
		eProgramUniform3uiv,
		eProgramUniform4uiv,
		eProgramUniformMatrix2fv,
		eProgramUniformMatrix3fv,
		eProgramUniformMatrix4fv,
		ePushDebugGroup,
		eReadBuffer,
		eReadPixels,
		eResetEvent,
		eSetEvent,
		eSetLineWidth,
		eStencilFunc,
		eStencilMask,
		eStencilOp,
		eTexParameteri,
		eTexParameterf,
		eTexSubImage1D,
		eTexSubImage2D,
		eTexSubImage3D,
		eUniform1fv,
		eUniform2fv,
		eUniform3fv,
		eUniform4fv,
		eUniform1iv,
		eUniform2iv,
		eUniform3iv,
		eUniform4iv,
		eUniform1uiv,
		eUniform2uiv,
		eUniform3uiv,
		eUniform4uiv,
		eUniformMatrix2fv,
		eUniformMatrix3fv,
		eUniformMatrix4fv,
		eUpdateBuffer,
		eUploadMemory,
		eUseProgram,
		eUseProgramPipeline,
		eWaitEvents,
		eWriteTimestamp,
	};

	struct Op
	{
		OpType type;
		uint16_t size;
	};

	struct Command
	{
		Op op;
		uint32_t dummy{ 0u };
	};

	template< OpType OpT >
	struct CmdT;

	//*************************************************************************

	template< typename TypeT, typename IterT >
	bool hasRemaining( IterT cur
		, IterT end
		, uint32_t count = sizeof( TypeT ) / sizeof( uint32_t ) )
	{
		bool result = cur != end;

		while ( result && count )
		{
			++cur;
			--count;
			result = cur != end;
		}

		return result || !count;
	}

	//*************************************************************************

	template<>
	struct alignas( uint64_t ) CmdT< OpType::eActiveTexture >
	{
		inline CmdT( uint32_t binding )
			: cmd{ { OpType::eActiveTexture, sizeof( CmdT ) / sizeof( uint32_t ) } }
			, binding{ std::move( binding ) }
		{
		}

		Command cmd;
		uint32_t binding;
	};
	using CmdActiveTexture = CmdT< OpType::eActiveTexture >;

	void apply( ContextLock const & context
		, CmdActiveTexture const & cmd );

	//*************************************************************************

	template<>
	struct alignas( uint64_t ) CmdT< OpType::eApplyDepthRanges >
	{
		static uint32_t constexpr MaxElems = 16u;
		static uint32_t constexpr ElemCount = 2u;

		inline CmdT( uint32_t first
			, uint32_t count
			, VkViewport const * viewports
			, size_t viewportsSize )
			: cmd{ { OpType::eApplyDepthRanges, sizeof( CmdT ) / sizeof( uint32_t ) } }
			, first{ first }
			, count{ std::min( MaxElems, count ) }
		{
			auto it = this->depthRanges.begin();
			auto end = viewports + std::min( MaxElems, uint32_t( viewportsSize ) );

			for ( auto vit = viewports; vit != end; ++vit )
			{
				auto & viewport = *vit;
				*it = viewport.minDepth;
				++it;
				*it = viewport.maxDepth;
				++it;
			}
		}

		inline CmdT( uint32_t first
			, uint32_t count
			, std::vector< VkViewport > const & viewports )
			: CmdT{ first, count, viewports.data(), viewports.size() }
		{
		}

		inline CmdT( uint32_t first
			, uint32_t count
			, ArrayView< VkViewport const > const & viewports )
			: CmdT{ first, count, viewports.data(), viewports.size() }
		{
		}

		inline CmdT( std::vector< VkViewport > const & viewports )
			: CmdT{ 0u, uint32_t( viewports.size() ), viewports }
		{
		}

		Command cmd;
		uint32_t first;
		uint32_t count;
		std::array< double, MaxElems * ElemCount > depthRanges;
	};
	using CmdApplyDepthRanges = CmdT< OpType::eApplyDepthRanges >;

	void apply( ContextLock const & context
		, CmdApplyDepthRanges const & cmd );

	//*************************************************************************

	template<>
	struct alignas( uint64_t ) CmdT< OpType::eApplyScissor >
	{
		inline CmdT( VkRect2D const & scissor )
			: cmd{ { OpType::eApplyScissor, sizeof( CmdT ) / sizeof( uint32_t ) } }
			, scissor{ scissor }
		{
		}

		Command cmd;
		VkRect2D scissor;
	};
	using CmdApplyScissor = CmdT< OpType::eApplyScissor >;

	void apply( ContextLock const & context
		, CmdApplyScissor const & cmd );

	//*************************************************************************

	template<>
	struct alignas( uint64_t ) CmdT< OpType::eApplyScissors >
	{
		static uint32_t constexpr MaxElems = 16u;
		static uint32_t constexpr ElemCount = 4u;

		inline CmdT( uint32_t first
			, uint32_t count
			, VkRect2D const * scissors
			, size_t scissorsSize )
			: cmd{ { OpType::eApplyScissors, sizeof( CmdT ) / sizeof( uint32_t ) } }
			, first{ first }
			, count{ std::min( MaxElems, count ) }
		{
			auto it = this->scissors.begin();
			auto end = scissors + std::min( MaxElems, uint32_t( scissorsSize ) );

			for ( auto scit = scissors; scit != end; ++scit )
			{
				auto & scissor = *scit;
				*it = scissor.offset.x;
				++it;
				*it = scissor.offset.y;
				++it;
				*it = int( scissor.extent.width );
				++it;
				*it = int( scissor.extent.height );
				++it;
			}
		}

		inline CmdT( uint32_t first
			, uint32_t count
			, std::vector< VkRect2D > const & scissors )
			: CmdT{ first, count, scissors.data(), scissors.size() }
		{
		}

		inline CmdT( uint32_t first
			, uint32_t count
			, ArrayView< VkRect2D const > const & scissors )
			: CmdT{ first, count, scissors.data(), scissors.size() }
		{
		}

		inline CmdT( std::vector< VkRect2D > const & scissors )
			: CmdT{ 0u, uint32_t( scissors.size() ), scissors }
		{
		}

		inline CmdT( uint32_t first
			, ArrayView< VkRect2D const > const & scissors )
			: CmdT{ 0u, uint32_t( scissors.size() ), scissors }
		{
		}

		Command cmd;
		uint32_t first;
		uint32_t count;
		std::array< int, MaxElems * ElemCount > scissors;
	};
	using CmdApplyScissors = CmdT< OpType::eApplyScissors >;

	void apply( ContextLock const & context
		, CmdApplyScissors const & cmd );

	//*************************************************************************

	template<>
	struct alignas( uint64_t ) CmdT< OpType::eApplyViewport >
	{
		inline CmdT( VkViewport const & viewport )
			: cmd{ { OpType::eApplyViewport, sizeof( CmdT ) / sizeof( uint32_t ) } }
			, viewport{ viewport }
		{
		}

		Command cmd;
		uint32_t count;
		VkViewport viewport;
	};
	using CmdApplyViewport = CmdT< OpType::eApplyViewport >;

	void apply( ContextLock const & context
		, CmdApplyViewport const & cmd );

	//*************************************************************************

	template<>
	struct alignas( uint64_t ) CmdT< OpType::eApplyViewports >
	{
		static uint32_t constexpr MaxElems = 16u;
		static uint32_t constexpr ElemCount = 4u;

		inline CmdT( uint32_t first
			, uint32_t count
			, VkViewport const * viewports
			, size_t viewportsSize )
			: cmd{ { OpType::eApplyViewports, sizeof( CmdT ) / sizeof( uint32_t ) } }
			, first{ first }
			, count{ std::min( MaxElems, count ) }
		{
			auto it = this->viewports.begin();
			auto end = viewports + std::min( MaxElems, uint32_t( viewportsSize ) );

			for ( auto vit = viewports; vit != end; ++vit )
			{
				auto & viewport = *vit;
				*it = viewport.x;
				++it;
				*it = viewport.y;
				++it;
				*it = viewport.width;
				++it;
				*it = viewport.height;
				++it;
			}
		}

		inline CmdT( uint32_t first
			, uint32_t count
			, std::vector< VkViewport > const & viewports )
			: CmdT{ first, count, viewports.data(), viewports.size() }
		{
		}

		inline CmdT( uint32_t first
			, uint32_t count
			, ArrayView< VkViewport const > const & viewports )
			: CmdT{ first, count, viewports.data(), viewports.size() }
		{
		}

		inline CmdT( std::vector< VkViewport > const & viewports )
			: CmdT{ 0u, uint32_t( viewports.size() ), viewports }
		{
		}

		inline CmdT( uint32_t first
			, ArrayView< VkViewport const > const & viewports )
			: CmdT{ first, uint32_t( viewports.size() ), viewports }
		{
		}

		Command cmd;
		uint32_t first;
		uint32_t count;
		std::array< float, MaxElems * ElemCount > viewports;
	};
	using CmdApplyViewports = CmdT< OpType::eApplyViewports >;

	void apply( ContextLock const & context
		, CmdApplyViewports const & cmd );

	//*************************************************************************

	template<>
	struct alignas( uint64_t ) CmdT< OpType::eBeginQuery >
	{
		inline CmdT( uint32_t target
			, uint32_t query )
			: cmd{ { OpType::eBeginQuery, sizeof( CmdT ) / sizeof( uint32_t ) } }
			, target{ std::move( target ) }
			, query{ std::move( query ) }
		{
		}

		Command cmd;
		uint32_t target;
		uint32_t query;
	};
	using CmdBeginQuery = CmdT< OpType::eBeginQuery >;

	void apply( ContextLock const & context
		, CmdBeginQuery const & cmd );

	//*************************************************************************

	template<>
	struct alignas( uint64_t ) CmdT< OpType::eBindBuffer >
	{
		inline CmdT( uint32_t target
			, uint32_t name )
			: cmd{ { OpType::eBindBuffer, sizeof( CmdT ) / sizeof( uint32_t ) } }
			, target{ GlBufferTarget( target ) }
			, name{ std::move( name ) }
		{
		}

		Command cmd;
		GlBufferTarget target;
		uint32_t name;
	};
	using CmdBindBuffer = CmdT< OpType::eBindBuffer >;

	void apply( ContextLock const & context
		, CmdBindBuffer const & cmd );

	//*************************************************************************

	template<>
	struct alignas( uint64_t ) CmdT< OpType::eBindFramebuffer >
	{
		inline CmdT( GlFrameBufferTarget target
			, VkFramebuffer fbo )
			: cmd{ { OpType::eBindFramebuffer, sizeof( CmdT ) / sizeof( uint32_t ) } }
			, target{ std::move( target ) }
			, fbo{ fbo }
		{
		}

		Command cmd;
		GlFrameBufferTarget target;
		VkFramebuffer fbo;
	};
	using CmdBindFramebuffer = CmdT< OpType::eBindFramebuffer >;

	void apply( ContextLock const & context
		, CmdBindFramebuffer const & cmd );

	//*************************************************************************

	template<>
	struct alignas( uint64_t ) CmdT< OpType::eBindSrcFramebuffer >
	{
		inline CmdT( GlFrameBufferTarget target )
			: cmd{ { OpType::eBindSrcFramebuffer, sizeof( CmdT ) / sizeof( uint32_t ) } }
			, target{ std::move( target ) }
		{
		}

		Command cmd;
		GlFrameBufferTarget target;
	};
	using CmdBindSrcFramebuffer = CmdT< OpType::eBindSrcFramebuffer >;

	void apply( ContextLock const & context
		, CmdBindSrcFramebuffer const & cmd );

	//*************************************************************************

	template<>
	struct alignas( uint64_t ) CmdT< OpType::eBindDstFramebuffer >
	{
		inline CmdT( GlFrameBufferTarget target )
			: cmd{ { OpType::eBindDstFramebuffer, sizeof( CmdT ) / sizeof( uint32_t ) } }
			, target{ std::move( target ) }
		{
		}

		Command cmd;
		GlFrameBufferTarget target;
	};
	using CmdBindDstFramebuffer = CmdT< OpType::eBindDstFramebuffer >;

	void apply( ContextLock const & context
		, CmdBindDstFramebuffer const & cmd );

	//*************************************************************************

	template<>
	struct alignas( uint64_t ) CmdT< OpType::eBindBufferRange >
	{
		inline CmdT( uint32_t binding
			, uint32_t target
			, uint32_t name
			, int64_t offset
			, int64_t range )
			: cmd{ { OpType::eBindBufferRange, sizeof( CmdT ) / sizeof( uint32_t ) } }
			, binding{ std::move( binding ) }
			, target{ GlBufferTarget( target ) }
			, name{ std::move( name ) }
			, offset{ std::move( offset ) }
			, range{ std::move( range ) }
		{
		}

		Command cmd;
		uint32_t binding;
		GlBufferTarget target;
		uint32_t name;
		int64_t offset;
		int64_t range;
	};
	using CmdBindBufferRange = CmdT< OpType::eBindBufferRange >;

	void apply( ContextLock const & context
		, CmdBindBufferRange const & cmd );

	//*************************************************************************

	template<>
	struct alignas( uint64_t ) CmdT< OpType::eBindContextState >
	{
		inline CmdT( ContextStateStack * stack
			, ContextState * state )
			: cmd{ { OpType::eBindContextState, sizeof( CmdT ) / sizeof( uint32_t ) } }
			, stack{ stack }
			, state{ state }
		{
		}

		Command cmd;
		ContextStateStack * stack;
		ContextState * state;
	};
	using CmdBindContextState = CmdT< OpType::eBindContextState >;

	void apply( ContextLock const & context
		, CmdBindContextState const & cmd );

	//*************************************************************************

	template<>
	struct alignas( uint64_t ) CmdT< OpType::eBindImage >
	{
		inline CmdT( uint32_t binding
			, uint32_t name
			, uint32_t baseMipLevel
			, uint32_t layerCount
			, uint32_t baseArrayLayer
			, GlInternal internal )
			: cmd{ { OpType::eBindImage, sizeof( CmdT ) / sizeof( uint32_t ) } }
			, binding{ std::move( binding ) }
			, name{ std::move( name ) }
			, baseMipLevel{ std::move( baseMipLevel ) }
			, layerCount{ std::move( layerCount ) }
			, baseArrayLayer{ std::move( baseArrayLayer ) }
			, internal{ std::move( internal ) }
		{
		}

		Command cmd;
		uint32_t binding;
		uint32_t name;
		uint32_t baseMipLevel;
		uint32_t layerCount;
		uint32_t baseArrayLayer;
		GlInternal internal;
	};
	using CmdBindImage = CmdT< OpType::eBindImage >;

	void apply( ContextLock const & context
		, CmdBindImage const & cmd );

	//*************************************************************************

	template<>
	struct alignas( uint64_t ) CmdT< OpType::eBindSampler >
	{
		inline CmdT( uint32_t binding
			, uint32_t name )
			: cmd{ { OpType::eBindSampler, sizeof( CmdT ) / sizeof( uint32_t ) } }
			, binding{ std::move( binding ) }
			, name{ std::move( name ) }
		{
		}

		Command cmd;
		uint32_t binding;
		uint32_t name;
	};
	using CmdBindSampler = CmdT< OpType::eBindSampler >;

	void apply( ContextLock const & context
		, CmdBindSampler const & cmd );

	//*************************************************************************

	template<>
	struct alignas( uint64_t ) CmdT< OpType::eBindTexture >
	{
		inline CmdT( GlTextureType type
			, uint32_t name )
			: cmd{ { OpType::eBindTexture, sizeof( CmdT ) / sizeof( uint32_t ) } }
			, type{ std::move( type ) }
			, name{ std::move( name ) }
		{
		}

		Command cmd;
		GlTextureType type;
		uint32_t name;
	};
	using CmdBindTexture = CmdT< OpType::eBindTexture >;

	void apply( ContextLock const & context
		, CmdBindTexture const & cmd );

	//*************************************************************************

	template<>
	struct alignas( uint64_t ) CmdT< OpType::eBindVextexArray >
	{
		inline CmdT( GeometryBuffers const * vao )
			: cmd{ { OpType::eBindVextexArray, sizeof( CmdT ) / sizeof( uint32_t ) } }
			, vao{ std::move( vao ) }
		{
		}

		Command cmd;
		GeometryBuffers const * vao;
	};
	using CmdBindVextexArray = CmdT< OpType::eBindVextexArray >;

	void apply( ContextLock const & context
		, CmdBindVextexArray const & cmd );

	//*************************************************************************

	template<>
	struct alignas( uint64_t ) CmdT< OpType::eBindVextexArrayObject >
	{
		inline CmdT( GLuint vao )
			: cmd{ { OpType::eBindVextexArrayObject, sizeof( CmdT ) / sizeof( uint32_t ) } }
			, vao{ std::move( vao ) }
		{
		}

		Command cmd;
		GLuint vao;
	};
	using CmdBindVextexArrayObject = CmdT< OpType::eBindVextexArrayObject >;

	void apply( ContextLock const & context
		, CmdBindVextexArrayObject const & cmd );

	//*************************************************************************

	template<>
	struct alignas( uint64_t ) CmdT< OpType::eBlendConstants >
	{
		inline CmdT( float c0
			, float c1
			, float c2
			, float c3 )
			: cmd{ { OpType::eBlendConstants, sizeof( CmdT ) / sizeof( uint32_t ) } }
			, blendConstant0{ std::move( c0 ) }
			, blendConstant1{ std::move( c1 ) }
			, blendConstant2{ std::move( c2 ) }
			, blendConstant3{ std::move( c3 ) }
		{
		}
		
		inline CmdT( float const c[4] )
			: cmd{ { OpType::eBlendConstants, sizeof( CmdT ) / sizeof( uint32_t ) } }
			, blendConstant0{ c[0] }
			, blendConstant1{ c[1] }
			, blendConstant2{ c[2] }
			, blendConstant3{ c[3] }
		{
		}

		Command cmd;
		float blendConstant0;
		float blendConstant1;
		float blendConstant2;
		float blendConstant3;
	};
	using CmdBlendConstants = CmdT< OpType::eBlendConstants >;

	void apply( ContextLock const & context
		, CmdBlendConstants const & cmd );

	//*************************************************************************

	template<>
	struct alignas( uint64_t ) CmdT< OpType::eBlendEquation >
	{
		inline CmdT( uint32_t index
			, GlBlendOp color
			, GlBlendOp alpha )
			: cmd{ { OpType::eBlendEquation, sizeof( CmdT ) / sizeof( uint32_t ) } }
			, index{ std::move( index ) }
			, color{ std::move( color ) }
			, alpha{ std::move( alpha ) }
		{
		}

		Command cmd;
		uint32_t index;
		GlBlendOp color;
		GlBlendOp alpha;
	};
	using CmdBlendEquation = CmdT< OpType::eBlendEquation >;

	void apply( ContextLock const & context
		, CmdBlendEquation const & cmd );

	//*************************************************************************

	template<>
	struct alignas( uint64_t ) CmdT< OpType::eBlendFunc >
	{
		inline CmdT( uint32_t index
			, GlBlendFactor colorSrc
			, GlBlendFactor colorDst
			, GlBlendFactor alphaSrc
			, GlBlendFactor alphaDst )
			: cmd{ { OpType::eBlendFunc, sizeof( CmdT ) / sizeof( uint32_t ) } }
			, index{ std::move( index ) }
			, colorSrc{ std::move( colorSrc ) }
			, colorDst{ std::move( colorDst ) }
			, alphaSrc{ std::move( alphaSrc ) }
			, alphaDst{ std::move( alphaDst ) }
		{
		}

		Command cmd;
		uint32_t index;
		GlBlendFactor colorSrc;
		GlBlendFactor colorDst;
		GlBlendFactor alphaSrc;
		GlBlendFactor alphaDst;
	};
	using CmdBlendFunc = CmdT< OpType::eBlendFunc >;

	void apply( ContextLock const & context
		, CmdBlendFunc const & cmd );

	//*************************************************************************

	template<>
	struct alignas( uint64_t ) CmdT< OpType::eBlitFramebuffer >
	{
		inline CmdT( int32_t srcL
			, int32_t srcT
			, int32_t srcR
			, int32_t srcB
			, int32_t dstL
			, int32_t dstT
			, int32_t dstR
			, int32_t dstB
			, GlImageAspectFlags mask
			, GlFilter filter )
			: cmd{ { OpType::eBlitFramebuffer, sizeof( CmdT ) / sizeof( uint32_t ) } }
			, srcL{ srcL }
			, srcT{ srcT }
			, srcR{ srcR }
			, srcB{ srcB }
			, dstL{ dstL }
			, dstT{ dstT }
			, dstR{ dstR }
			, dstB{ dstB }
			, mask{ mask }
			, filter{ filter }
		{
		}

		Command cmd;
		int32_t srcL;
		int32_t srcT;
		int32_t srcR;
		int32_t srcB;
		int32_t dstL;
		int32_t dstT;
		int32_t dstR;
		int32_t dstB;
		GlImageAspectFlags mask;
		GlFilter filter;
	};
	using CmdBlitFramebuffer = CmdT< OpType::eBlitFramebuffer >;

	void apply( ContextLock const & context
		, CmdBlitFramebuffer const & cmd );

	//*************************************************************************

	template<>
	struct alignas( uint64_t ) CmdT< OpType::eCheckFramebuffer >
	{
		inline CmdT()
			: cmd{ { OpType::eCheckFramebuffer, sizeof( CmdT ) / sizeof( uint32_t ) } }
		{
		}

		Command cmd;
	};
	using CmdCheckFramebuffer = CmdT< OpType::eCheckFramebuffer >;

	void apply( ContextLock const & context
		, CmdCheckFramebuffer const & cmd );

	//*************************************************************************

	template<>
	struct alignas( uint64_t ) CmdT< OpType::eCleanupFramebuffer >
	{
		inline CmdT( GLuint * fbo )
			: cmd{ { OpType::eCleanupFramebuffer, sizeof( CmdT ) / sizeof( uint32_t ) } }
			, fbo{ fbo }
		{
		}

		Command cmd;
		GLuint * fbo;
	};
	using CmdCleanupFramebuffer = CmdT< OpType::eCleanupFramebuffer >;

	void apply( ContextLock const & context
		, CmdCleanupFramebuffer const & cmd );

	//*************************************************************************

	template<>
	struct alignas( uint64_t ) CmdT< OpType::eClearBack >
	{
		inline CmdT( uint32_t mask )
			: cmd{ { OpType::eClearBack, sizeof( CmdT ) / sizeof( uint32_t ) } }
			, mask{ std::move( mask ) }
		{
		}

		Command cmd;
		uint32_t mask;
	};
	using CmdClearBack = CmdT< OpType::eClearBack >;

	void apply( ContextLock const & context
		, CmdClearBack const & cmd );

	//*************************************************************************

	template<>
	struct alignas( uint64_t ) CmdT< OpType::eClearBackColour >
	{
		inline CmdT( VkClearColorValue color
			, uint32_t colourIndex )
			: cmd{ { OpType::eClearBackColour, sizeof( CmdT ) / sizeof( uint32_t ) } }
			, color{ std::move( color ) }
			, colourIndex{ std::move( colourIndex ) }
		{
		}

		Command cmd;
		VkClearColorValue color;
		uint32_t colourIndex;
	};
	using CmdClearBackColour = CmdT< OpType::eClearBackColour >;

	void apply( ContextLock const & context
		, CmdClearBackColour const & cmd );

	//*************************************************************************

	template<>
	struct alignas( uint64_t ) CmdT< OpType::eClearBackDepth >
	{
		inline CmdT( float depth )
			: cmd{ { OpType::eClearBackDepth, sizeof( CmdT ) / sizeof( uint32_t ) } }
			, depth{ std::move( depth ) }
		{
		}

		Command cmd;
		float depth;
	};
	using CmdClearBackDepth = CmdT< OpType::eClearBackDepth >;

	void apply( ContextLock const & context
		, CmdClearBackDepth const & cmd );

	//*************************************************************************

	template<>
	struct alignas( uint64_t ) CmdT< OpType::eClearBackDepthStencil >
	{
		inline CmdT( float depth
			, int32_t stencil )
			: cmd{ { OpType::eClearBackDepthStencil, sizeof( CmdT ) / sizeof( uint32_t ) } }
			, depth{ std::move( depth ) }
			, stencil{ std::move( stencil ) }
		{
		}

		Command cmd;
		float depth;
		int32_t stencil;
	};
	using CmdClearBackDepthStencil = CmdT< OpType::eClearBackDepthStencil >;

	void apply( ContextLock const & context
		, CmdClearBackDepthStencil const & cmd );

	//*************************************************************************

	template<>
	struct alignas( uint64_t ) CmdT< OpType::eClearBackStencil >
	{
		inline CmdT( int32_t stencil )
			: cmd{ { OpType::eClearBackStencil, sizeof( CmdT ) / sizeof( uint32_t ) } }
			, stencil{ std::move( stencil ) }
		{
		}

		Command cmd;
		int32_t stencil;
	};
	using CmdClearBackStencil = CmdT< OpType::eClearBackStencil >;

	void apply( ContextLock const & context
		, CmdClearBackStencil const & cmd );

	//*************************************************************************

	template<>
	struct alignas( uint64_t ) CmdT< OpType::eClearColour >
	{
		inline CmdT( VkClearColorValue color
			, uint32_t colourIndex )
			: cmd{ { OpType::eClearColour, sizeof( CmdT ) / sizeof( uint32_t ) } }
			, color{ std::move( color ) }
			, colourIndex{ std::move( colourIndex ) }
		{
		}

		Command cmd;
		VkClearColorValue color;
		uint32_t colourIndex;
	};
	using CmdClearColour = CmdT< OpType::eClearColour >;

	void apply( ContextLock const & context
		, CmdClearColour const & cmd );

	//*************************************************************************

	template<>
	struct alignas( uint64_t ) CmdT< OpType::eClearDepth >
	{
		inline CmdT( float depth )
			: cmd{ { OpType::eClearDepth, sizeof( CmdT ) / sizeof( uint32_t ) } }
			, depth{ std::move( depth ) }
		{
		}

		Command cmd;
		float depth;
	};
	using CmdClearDepth = CmdT< OpType::eClearDepth >;

	void apply( ContextLock const & context
		, CmdClearDepth const & cmd );

	//*************************************************************************

	template<>
	struct alignas( uint64_t ) CmdT< OpType::eClearDepthStencil >
	{
		inline CmdT( VkClearDepthStencilValue depthStencil )
			: cmd{ { OpType::eClearDepthStencil, sizeof( CmdT ) / sizeof( uint32_t ) } }
			, depthStencil{ std::move( depthStencil ) }
		{
		}

		Command cmd;
		VkClearDepthStencilValue depthStencil;
	};
	using CmdClearDepthStencil = CmdT< OpType::eClearDepthStencil >;

	void apply( ContextLock const & context
		, CmdClearDepthStencil const & cmd );

	//*************************************************************************

	template<>
	struct alignas( uint64_t ) CmdT< OpType::eClearStencil >
	{
		inline CmdT( int32_t stencil )
			: cmd{ { OpType::eClearStencil, sizeof( CmdT ) / sizeof( uint32_t ) } }
			, stencil{ std::move( stencil ) }
		{
		}

		Command cmd;
		int32_t stencil;
	};
	using CmdClearStencil = CmdT< OpType::eClearStencil >;

	void apply( ContextLock const & context
		, CmdClearStencil const & cmd );

	//*************************************************************************

	template<>
	struct alignas( uint64_t ) CmdT< OpType::eClearTexColorF >
	{
		inline CmdT( uint32_t name
			, uint32_t mipLevel
			, GlFormat format
			, std::array< float, 4u > color )
			: cmd{ { OpType::eClearTexColorF, sizeof( CmdT ) / sizeof( uint32_t ) } }
			, name{ std::move( name ) }
			, mipLevel{ std::move( mipLevel ) }
			, format{ std::move( format ) }
			, color{ std::move( color ) }
		{
		}

		inline CmdT( uint32_t name
			, uint32_t mipLevel
			, GlFormat format
			, float const ( &color )[4] )
			: cmd{ { OpType::eClearTexColorF, sizeof( CmdT ) / sizeof( uint32_t ) } }
			, name{ std::move( name ) }
			, mipLevel{ std::move( mipLevel ) }
			, format{ std::move( format ) }
			, color{ color[0], color[1], color[2], color[3] }
		{
		}

		Command cmd;
		uint32_t name;
		uint32_t mipLevel;
		GlFormat format;
		std::array< float, 4u > color;
	};
	using CmdClearTexColorF = CmdT< OpType::eClearTexColorF >;

	void apply( ContextLock const & context
		, CmdClearTexColorF const & cmd );

	//*************************************************************************

	template<>
	struct alignas( uint64_t ) CmdT< OpType::eClearTexColorUI >
	{
		inline CmdT( uint32_t name
			, uint32_t mipLevel
			, GlFormat format
			, std::array< uint32_t, 4u > color )
			: cmd{ { OpType::eClearTexColorUI, sizeof( CmdT ) / sizeof( uint32_t ) } }
			, name{ std::move( name ) }
			, mipLevel{ std::move( mipLevel ) }
			, format{ std::move( format ) }
			, color{ std::move( color ) }
		{
		}

		inline CmdT( uint32_t name
			, uint32_t mipLevel
			, GlFormat format
			, uint32_t const ( &color )[4] )
			: cmd{ { OpType::eClearTexColorUI, sizeof( CmdT ) / sizeof( uint32_t ) } }
			, name{ std::move( name ) }
			, mipLevel{ std::move( mipLevel ) }
			, format{ std::move( format ) }
			, color{ color[0], color[1], color[2], color[3] }
		{
		}

		Command cmd;
		uint32_t name;
		uint32_t mipLevel;
		GlFormat format;
		std::array< uint32_t, 4u > color;
	};
	using CmdClearTexColorUI = CmdT< OpType::eClearTexColorUI >;

	void apply( ContextLock const & context
		, CmdClearTexColorUI const & cmd );

	//*************************************************************************

	template<>
	struct alignas( uint64_t ) CmdT< OpType::eClearTexColorSI >
	{
		inline CmdT( uint32_t name
			, uint32_t mipLevel
			, GlFormat format
			, std::array< int32_t, 4u > color )
			: cmd{ { OpType::eClearTexColorSI, sizeof( CmdT ) / sizeof( uint32_t ) } }
			, name{ std::move( name ) }
			, mipLevel{ std::move( mipLevel ) }
			, format{ std::move( format ) }
			, color{ std::move( color ) }
		{
		}

		inline CmdT( uint32_t name
			, uint32_t mipLevel
			, GlFormat format
			, int32_t const ( &color )[4] )
			: cmd{ { OpType::eClearTexColorSI, sizeof( CmdT ) / sizeof( uint32_t ) } }
			, name{ std::move( name ) }
			, mipLevel{ std::move( mipLevel ) }
			, format{ std::move( format ) }
			, color{ color[0], color[1], color[2], color[3] }
		{
		}

		Command cmd;
		uint32_t name;
		uint32_t mipLevel;
		GlFormat format;
		std::array< int32_t, 4u > color;
	};
	using CmdClearTexColorSI = CmdT< OpType::eClearTexColorSI >;

	void apply( ContextLock const & context
		, CmdClearTexColorSI const & cmd );

	//*************************************************************************

	template<>
	struct alignas( uint64_t ) CmdT< OpType::eClearTexDepth >
	{
		inline CmdT( uint32_t name
			, uint32_t mipLevel
			, uint32_t format
			, uint32_t type
			, float depth )
			: cmd{ { OpType::eClearTexDepth, sizeof( CmdT ) / sizeof( uint32_t ) } }
			, name{ std::move( name ) }
			, mipLevel{ std::move( mipLevel ) }
			, format{ std::move( format ) }
			, type{ std::move( type ) }
			, depth{ std::move( depth ) }
		{
		}

		Command cmd;
		uint32_t name;
		uint32_t mipLevel;
		uint32_t format;
		uint32_t type;
		float depth;
	};
	using CmdClearTexDepth = CmdT< OpType::eClearTexDepth >;

	void apply( ContextLock const & context
		, CmdClearTexDepth const & cmd );

	//*************************************************************************

	template<>
	struct alignas( uint64_t ) CmdT< OpType::eClearTexDepthStencil >
	{
		inline CmdT( uint32_t name
			, uint32_t mipLevel
			, uint32_t format
			, uint32_t type
			, VkClearDepthStencilValue depthStencil )
			: cmd{ { OpType::eClearTexDepthStencil, sizeof( CmdT ) / sizeof( uint32_t ) } }
			, name{ std::move( name ) }
			, mipLevel{ std::move( mipLevel ) }
			, format{ std::move( format ) }
			, type{ std::move( type ) }
			, depthStencil{ std::move( depthStencil ) }
		{
		}

		Command cmd;
		uint32_t name;
		uint32_t mipLevel;
		uint32_t format;
		uint32_t type;
		VkClearDepthStencilValue depthStencil;
	};
	using CmdClearTexDepthStencil = CmdT< OpType::eClearTexDepthStencil >;

	void apply( ContextLock const & context
		, CmdClearTexDepthStencil const & cmd );

	//*************************************************************************

	template<>
	struct alignas( uint64_t ) CmdT< OpType::eClearTexStencil >
	{
		inline CmdT( uint32_t name
			, uint32_t mipLevel
			, uint32_t format
			, uint32_t type
			, int32_t stencil )
			: cmd{ { OpType::eClearTexStencil, sizeof( CmdT ) / sizeof( uint32_t ) } }
			, name{ std::move( name ) }
			, mipLevel{ std::move( mipLevel ) }
			, format{ std::move( format ) }
			, type{ std::move( type ) }
			, stencil{ std::move( stencil ) }
		{
		}

		Command cmd;
		uint32_t name;
		uint32_t mipLevel;
		uint32_t format;
		uint32_t type;
		int32_t stencil;
	};
	using CmdClearTexStencil = CmdT< OpType::eClearTexStencil >;

	void apply( ContextLock const & context
		, CmdClearTexStencil const & cmd );

	//*************************************************************************

	template<>
	struct alignas( uint64_t ) CmdT< OpType::eColorMask >
	{
		inline CmdT( uint32_t index
			, GLboolean r
			, GLboolean g
			, GLboolean b
			, GLboolean a )
			: cmd{ { OpType::eColorMask, sizeof( CmdT ) / sizeof( uint32_t ) } }
			, index{ index }
			, r{ r }
			, g{ g }
			, b{ b }
			, a{ a }
		{
		}

		Command cmd;
		uint32_t index;
		GLboolean r;
		GLboolean g;
		GLboolean b;
		GLboolean a;
	};
	using CmdColorMask = CmdT< OpType::eColorMask >;

	void apply( ContextLock const & context
		, CmdColorMask const & cmd );

	//*************************************************************************

	template<>
	struct alignas( uint64_t ) CmdT< OpType::eCompressedTexSubImage1D >
	{
		inline CmdT( GlTextureType copyTarget
			, uint32_t mipLevel
			, int32_t x
			, uint32_t width
			, GlInternal format
			, int32_t imageSize
			, int32_t bufferOffset )
			: cmd{ { OpType::eCompressedTexSubImage1D, sizeof( CmdT ) / sizeof( uint32_t ) } }
			, copyTarget{ std::move( copyTarget ) }
			, mipLevel{ std::move( mipLevel ) }
			, x{ std::move( x ) }
			, width{ std::move( width ) }
			, format{ std::move( format ) }
			, imageSize{ std::move( imageSize ) }
			, bufferOffset{ std::move( bufferOffset ) }
		{
		}

		Command cmd;
		GlTextureType copyTarget;
		uint32_t mipLevel;
		int32_t x;
		uint32_t width;
		GlInternal format;
		int32_t imageSize;
		int32_t bufferOffset;
	};
	using CmdCompressedTexSubImage1D = CmdT< OpType::eCompressedTexSubImage1D >;

	void apply( ContextLock const & context
		, CmdCompressedTexSubImage1D const & cmd );

	//*************************************************************************

	template<>
	struct alignas( uint64_t ) CmdT< OpType::eCompressedTexSubImage2D >
	{
		inline CmdT( GlTextureType copyTarget
			, uint32_t mipLevel
			, int32_t x
			, int32_t y
			, uint32_t width
			, uint32_t height
			, GlInternal format
			, int32_t imageSize
			, int32_t bufferOffset )
			: cmd{ { OpType::eCompressedTexSubImage2D, sizeof( CmdT ) / sizeof( uint32_t ) } }
			, copyTarget{ std::move( copyTarget ) }
			, mipLevel{ std::move( mipLevel ) }
			, x{ std::move( x ) }
			, y{ std::move( y ) }
			, width{ std::move( width ) }
			, height{ std::move( height ) }
			, format{ std::move( format ) }
			, imageSize{ std::move( imageSize ) }
			, bufferOffset{ std::move( bufferOffset ) }
		{
		}

		Command cmd;
		GlTextureType copyTarget;
		uint32_t mipLevel;
		int32_t x;
		int32_t y;
		uint32_t width;
		uint32_t height;
		GlInternal format;
		int32_t imageSize;
		int32_t bufferOffset;
	};
	using CmdCompressedTexSubImage2D = CmdT< OpType::eCompressedTexSubImage2D >;

	void apply( ContextLock const & context
		, CmdCompressedTexSubImage2D const & cmd );

	//*************************************************************************

	template<>
	struct alignas( uint64_t ) CmdT< OpType::eCompressedTexSubImage3D >
	{
		inline CmdT( GlTextureType copyTarget
			, uint32_t mipLevel
			, int32_t x
			, int32_t y
			, int32_t z
			, uint32_t width
			, uint32_t height
			, uint32_t depth
			, GlInternal format
			, int32_t imageSize
			, int32_t bufferOffset )
			: cmd{ { OpType::eCompressedTexSubImage3D, sizeof( CmdT ) / sizeof( uint32_t ) } }
			, copyTarget{ std::move( copyTarget ) }
			, mipLevel{ std::move( mipLevel ) }
			, x{ std::move( x ) }
			, y{ std::move( y ) }
			, z{ std::move( z ) }
			, width{ std::move( width ) }
			, height{ std::move( height ) }
			, depth{ std::move( depth ) }
			, format{ std::move( format ) }
			, imageSize{ std::move( imageSize ) }
			, bufferOffset{ std::move( bufferOffset ) }
		{
		}

		Command cmd;
		GlTextureType copyTarget;
		uint32_t mipLevel;
		int32_t x;
		int32_t y;
		int32_t z;
		uint32_t width;
		uint32_t height;
		uint32_t depth;
		GlInternal format;
		int32_t imageSize;
		int32_t bufferOffset;
	};
	using CmdCompressedTexSubImage3D = CmdT< OpType::eCompressedTexSubImage3D >;

	void apply( ContextLock const & context
		, CmdCompressedTexSubImage3D const & cmd );

	//*************************************************************************

	template<>
	struct alignas( uint64_t ) CmdT< OpType::eCopyBufferSubData >
	{
		inline CmdT( uint32_t srcTarget
			, uint32_t dstTarget
			, VkBufferCopy copy )
			: cmd{ { OpType::eCopyBufferSubData, sizeof( CmdT ) / sizeof( uint32_t ) } }
			, srcTarget{ GlBufferTarget( srcTarget ) }
			, dstTarget{ GlBufferTarget( dstTarget ) }
			, copy{ std::move( copy ) }
		{
		}

		Command cmd;
		GlBufferTarget srcTarget;
		GlBufferTarget dstTarget;
		VkBufferCopy copy;
	};
	using CmdCopyBufferSubData = CmdT< OpType::eCopyBufferSubData >;

	void apply( ContextLock const & context
		, CmdCopyBufferSubData const & cmd );

	//*************************************************************************

	template<>
	struct alignas( uint64_t ) CmdT< OpType::eCopyImageSubData >
	{
		inline CmdT( uint32_t srcName
			, GlTextureType srcTarget
			, uint32_t dstName
			, GlTextureType dstTarget
			, VkImageCopy copy )
			: cmd{ { OpType::eCopyImageSubData, sizeof( CmdT ) / sizeof( uint32_t ) } }
			, srcName{ std::move( srcName ) }
			, srcTarget{ std::move( srcTarget ) }
			, dstName{ std::move( dstName ) }
			, dstTarget{ std::move( dstTarget ) }
			, copy{ std::move( copy ) }
		{
		}

		Command cmd;
		uint32_t srcName;
		GlTextureType srcTarget;
		uint32_t dstName;
		GlTextureType dstTarget;
		VkImageCopy copy;
	};
	using CmdCopyImageSubData = CmdT< OpType::eCopyImageSubData >;

	void apply( ContextLock const & context
		, CmdCopyImageSubData const & cmd );

	//*************************************************************************

	template<>
	struct alignas( uint64_t ) CmdT< OpType::eCullFace >
	{
		inline CmdT( GlCullMode value )
			: cmd{ { OpType::eCullFace, sizeof( CmdT ) / sizeof( uint32_t ) } }
			, value{ std::move( value ) }
		{
		}

		Command cmd;
		GlCullMode value;
	};
	using CmdCullFace = CmdT< OpType::eCullFace >;

	void apply( ContextLock const & context
		, CmdCullFace const & cmd );

	//*************************************************************************

	template<>
	struct alignas( uint64_t ) CmdT< OpType::eDepthFunc >
	{
		inline CmdT( GlCompareOp value )
			: cmd{ { OpType::eDepthFunc, sizeof( CmdT ) / sizeof( uint32_t ) } }
			, value{ std::move( value ) }
		{
		}

		Command cmd;
		GlCompareOp value;
	};
	using CmdDepthFunc = CmdT< OpType::eDepthFunc >;

	void apply( ContextLock const & context
		, CmdDepthFunc const & cmd );

	//*************************************************************************

	template<>
	struct alignas( uint64_t ) CmdT< OpType::eDepthMask >
	{
		inline CmdT( uint32_t value )
			: cmd{ { OpType::eDepthMask, sizeof( CmdT ) / sizeof( uint32_t ) } }
			, value{ std::move( value ) }
		{
		}

		Command cmd;
		uint32_t value;
	};
	using CmdDepthMask = CmdT< OpType::eDepthMask >;

	void apply( ContextLock const & context
		, CmdDepthMask const & cmd );

	//*************************************************************************

	template<>
	struct alignas( uint64_t ) CmdT< OpType::eDepthRange >
	{
		inline CmdT( float min
			, float max )
			: cmd{ { OpType::eDepthRange, sizeof( CmdT ) / sizeof( uint32_t ) } }
			, min{ std::move( min ) }
			, max{ std::move( max ) }
		{
		}

		Command cmd;
		float min;
		float max;
	};
	using CmdDepthRange = CmdT< OpType::eDepthRange >;

	void apply( ContextLock const & context
		, CmdDepthRange const & cmd );

	//*************************************************************************

	template<>
	struct alignas( uint64_t ) CmdT< OpType::eDisable >
	{
		inline CmdT( GlTweak value )
			: cmd{ { OpType::eDisable, sizeof( CmdT ) / sizeof( uint32_t ) } }
			, value{ std::move( value ) }
		{
		}

		Command cmd;
		GlTweak value;
	};
	using CmdDisable = CmdT< OpType::eDisable >;

	void apply( ContextLock const & context
		, CmdDisable const & cmd );

	//*************************************************************************

	template<>
	struct alignas( uint64_t ) CmdT< OpType::eDispatch >
	{
		inline CmdT( uint32_t groupCountX
			, uint32_t groupCountY
			, uint32_t groupCountZ )
			: cmd{ { OpType::eDispatch, sizeof( CmdT ) / sizeof( uint32_t ) } }
			, groupCountX{ std::move( groupCountX ) }
			, groupCountY{ std::move( groupCountY ) }
			, groupCountZ{ std::move( groupCountZ ) }
		{
		}

		Command cmd;
		uint32_t groupCountX;
		uint32_t groupCountY;
		uint32_t groupCountZ;
	};
	using CmdDispatch = CmdT< OpType::eDispatch >;

	void apply( ContextLock const & context
		, CmdDispatch const & cmd );

	//*************************************************************************

	template<>
	struct alignas( uint64_t ) CmdT< OpType::eDispatchIndirect >
	{
		inline CmdT( uint64_t offset )
			: cmd{ { OpType::eDispatchIndirect, sizeof( CmdT ) / sizeof( uint32_t ) } }
			, offset{ std::move( offset ) }
		{
		}

		Command cmd;
		uint64_t offset;
	};
	using CmdDispatchIndirect = CmdT< OpType::eDispatchIndirect >;

	void apply( ContextLock const & context
		, CmdDispatchIndirect const & cmd );

	//*************************************************************************

	template<>
	struct alignas( uint64_t ) CmdT< OpType::eDownloadMemory >
	{
		inline CmdT( VkDeviceMemory memory
			, VkDeviceSize offset
			, VkDeviceSize size )
			: cmd{ { OpType::eDownloadMemory, sizeof( CmdT ) / sizeof( uint32_t ) } }
			, memory{ memory }
			, offset{ offset }
			, size{ size }
		{
		}

		Command cmd;
		VkDeviceMemory memory;
		VkDeviceSize offset;
		VkDeviceSize size;
	};
	using CmdDownloadMemory = CmdT< OpType::eDownloadMemory >;

	void apply( ContextLock const & context
		, CmdDownloadMemory const & cmd );

	//*************************************************************************

	template<>
	struct alignas( uint64_t ) CmdT< OpType::eDraw >
	{
		inline CmdT( uint32_t vtxCount
			, uint32_t instCount
			, uint32_t firstVertex
			, GlPrimitiveTopology mode )
			: cmd{ { OpType::eDraw, sizeof( CmdT ) / sizeof( uint32_t ) } }
			, vtxCount{ std::move( vtxCount ) }
			, instCount{ std::move( instCount ) }
			, firstVertex{ std::move( firstVertex ) }
			, mode{ std::move( mode ) }
		{
		}

		Command cmd;
		uint32_t vtxCount;
		uint32_t instCount;
		uint32_t firstVertex;
		GlPrimitiveTopology mode;
	};
	using CmdDraw = CmdT< OpType::eDraw >;

	void apply( ContextLock const & context
		, CmdDraw const & cmd );

	//*************************************************************************

	template<>
	struct alignas( uint64_t ) CmdT< OpType::eDrawBaseInstance >
	{
		inline CmdT( uint32_t vtxCount
			, uint32_t instCount
			, uint32_t firstVertex
			, uint32_t firstInstance
			, GlPrimitiveTopology mode )
			: cmd{ { OpType::eDrawBaseInstance, sizeof( CmdT ) / sizeof( uint32_t ) } }
			, vtxCount{ std::move( vtxCount ) }
			, instCount{ std::move( instCount ) }
			, firstVertex{ std::move( firstVertex ) }
			, firstInstance{ std::move( firstInstance ) }
			, mode{ std::move( mode ) }
		{
		}

		Command cmd;
		uint32_t vtxCount;
		uint32_t instCount;
		uint32_t firstVertex;
		uint32_t firstInstance;
		GlPrimitiveTopology mode;
	};
	using CmdDrawBaseInstance = CmdT< OpType::eDrawBaseInstance >;

	void apply( ContextLock const & context
		, CmdDrawBaseInstance const & cmd );

	//*************************************************************************

	template<>
	struct alignas( uint64_t ) CmdT< OpType::eDrawBuffer >
	{
		inline CmdT( GlAttachmentPoint value )
			: cmd{ { OpType::eDrawBuffer, sizeof( CmdT ) / sizeof( uint32_t ) } }
			, target{ std::move( value ) }
		{
		}

		Command cmd;
		GlAttachmentPoint target;
	};
	using CmdDrawBuffer = CmdT< OpType::eDrawBuffer >;

	void apply( ContextLock const & context
		, CmdDrawBuffer const & cmd );

	//*************************************************************************

	template<>
	struct alignas( uint64_t ) CmdT< OpType::eDrawBuffers >
	{
		inline CmdT( GlAttachmentPoint target )
			: cmd{ { OpType::eDrawBuffers, sizeof( CmdT ) / sizeof( uint32_t ) } }
			, count{ 1u }
		{
			assert( target != GL_ATTACHMENT_POINT_DEPTH_STENCIL
				&& target != GL_ATTACHMENT_POINT_DEPTH
				&& target != GL_ATTACHMENT_POINT_STENCIL );
			targets[0] = target;
		}

		template< size_t N >
		inline CmdT( GlAttachmentPoint targets[N] )
			: cmd{ { OpType::eDrawBuffers, sizeof( CmdT ) / sizeof( uint32_t ) } }
			, count{ uint32_t( N ) }
		{
			static_assert( N <= 16u, "No more that 16 draw buffers" );
			uint32_t i = 0u;

			for ( auto & target : makeArrayView( targets, targets + N ) )
			{
				assert( target != GL_ATTACHMENT_POINT_DEPTH_STENCIL
					&& target != GL_ATTACHMENT_POINT_DEPTH
					&& target != GL_ATTACHMENT_POINT_STENCIL );
				this->targets[i++] = target;
			}
		}

		template< size_t N >
		inline CmdT( std::array< GlAttachmentPoint, N > targets )
			: cmd{ { OpType::eDrawBuffers, sizeof( CmdT ) / sizeof( uint32_t ) } }
			, count{ uint32_t( N ) }
		{
			static_assert( N <= 16u, "No more that 16 draw buffers" );
			uint32_t i = 0u;

			for ( auto & target : targets )
			{
				assert( target != GL_ATTACHMENT_POINT_DEPTH_STENCIL
					&& target != GL_ATTACHMENT_POINT_DEPTH
					&& target != GL_ATTACHMENT_POINT_STENCIL );
				this->targets[i++] = target;
			}
		}

		inline CmdT( std::vector< GlAttachmentPoint > targets )
			: cmd{ { OpType::eDrawBuffers, sizeof( CmdT ) / sizeof( uint32_t ) } }
			, count{ std::min( 16u, uint32_t( targets.size() ) ) }
		{
			uint32_t i = 0u;

			for ( auto & target : targets )
			{
				if ( i < this->targets.size() )
				{
					assert( target != GL_ATTACHMENT_POINT_DEPTH_STENCIL
						&& target != GL_ATTACHMENT_POINT_DEPTH
						&& target != GL_ATTACHMENT_POINT_STENCIL );
					this->targets[i++] = target;
				}
			}
		}

		Command cmd;
		uint32_t count;
		std::array< GlAttachmentPoint, 16u > targets{ GlAttachmentPoint( 0u ) };
	};
	using CmdDrawBuffers = CmdT< OpType::eDrawBuffers >;

	void apply( ContextLock const & context
		, CmdDrawBuffers const & cmd );

	//*************************************************************************

	template<>
	struct alignas( uint64_t ) CmdT< OpType::eDrawIndexed >
	{
		inline CmdT( uint32_t indexCount
			, uint32_t instCount
			, uint32_t indexOffset
			, uint32_t vertexOffset
			, GlPrimitiveTopology mode
			, GlIndexType type )
			: cmd{ { OpType::eDrawIndexed, sizeof( CmdT ) / sizeof( uint32_t ) } }
			, indexCount{ std::move( indexCount ) }
			, instCount{ std::move( instCount ) }
			, indexOffset{ std::move( indexOffset ) }
			, vertexOffset{ std::move( vertexOffset ) }
			, mode{ std::move( mode ) }
			, type{ std::move( type ) }
		{
		}

		Command cmd;
		uint32_t indexCount;
		uint32_t instCount;
		uint32_t indexOffset;
		uint32_t vertexOffset;
		GlPrimitiveTopology mode;
		GlIndexType type;
	};
	using CmdDrawIndexed = CmdT< OpType::eDrawIndexed >;

	void apply( ContextLock const & context
		, CmdDrawIndexed const & cmd );

	//*************************************************************************

	template<>
	struct alignas( uint64_t ) CmdT< OpType::eDrawIndexedBaseInstance >
	{
		inline CmdT( uint32_t indexCount
			, uint32_t instCount
			, uint32_t indexOffset
			, uint32_t vertexOffset
			, uint32_t firstInstance
			, GlPrimitiveTopology mode
			, GlIndexType type )
			: cmd{ { OpType::eDrawIndexedBaseInstance, sizeof( CmdT ) / sizeof( uint32_t ) } }
			, indexCount{ std::move( indexCount ) }
			, instCount{ std::move( instCount ) }
			, indexOffset{ std::move( indexOffset ) }
			, vertexOffset{ std::move( vertexOffset ) }
			, firstInstance{ std::move( firstInstance ) }
			, mode{ std::move( mode ) }
			, type{ std::move( type ) }
		{
		}

		Command cmd;
		uint32_t indexCount;
		uint32_t instCount;
		uint32_t indexOffset;
		uint32_t vertexOffset;
		uint32_t firstInstance;
		GlPrimitiveTopology mode;
		GlIndexType type;
	};
	using CmdDrawIndexedBaseInstance = CmdT< OpType::eDrawIndexedBaseInstance >;

	void apply( ContextLock const & context
		, CmdDrawIndexedBaseInstance const & cmd );

	//*************************************************************************

	template<>
	struct alignas( uint64_t ) CmdT< OpType::eDrawIndexedIndirect >
	{
		inline CmdT( uint64_t offset
			, uint32_t drawCount
			, uint32_t stride
			, GlPrimitiveTopology mode
			, GlIndexType type )
			: cmd{ { OpType::eDrawIndexedIndirect, sizeof( CmdT ) / sizeof( uint32_t ) } }
			, offset{ std::move( offset ) }
			, drawCount{ std::move( drawCount ) }
			, stride{ std::move( stride ) }
			, mode{ std::move( mode ) }
			, type{ std::move( type ) }
		{
		}

		Command cmd;
		uint64_t offset;
		uint32_t drawCount;
		uint32_t stride;
		GlPrimitiveTopology mode;
		GlIndexType type;
	};
	using CmdDrawIndexedIndirect = CmdT< OpType::eDrawIndexedIndirect >;

	void apply( ContextLock const & context
		, CmdDrawIndexedIndirect const & cmd );

	//*************************************************************************

	template<>
	struct alignas( uint64_t ) CmdT< OpType::eDrawIndirect >
	{
		inline CmdT( uint64_t offset
			, uint32_t drawCount
			, uint32_t stride
			, GlPrimitiveTopology mode )
			: cmd{ { OpType::eDrawIndirect, sizeof( CmdT ) / sizeof( uint32_t ) } }
			, offset{ std::move( offset ) }
			, drawCount{ std::move( drawCount ) }
			, stride{ std::move( stride ) }
			, mode{ std::move( mode ) }
		{
		}

		Command cmd;
		uint64_t offset;
		uint32_t drawCount;
		uint32_t stride;
		GlPrimitiveTopology mode;
	};
	using CmdDrawIndirect = CmdT< OpType::eDrawIndirect >;

	void apply( ContextLock const & context
		, CmdDrawIndirect const & cmd );

	//*************************************************************************

	template<>
	struct alignas( uint64_t ) CmdT< OpType::eEnable >
	{
		inline CmdT( GlTweak value )
			: cmd{ { OpType::eEnable, sizeof( CmdT ) / sizeof( uint32_t ) } }
			, value{ std::move( value ) }
		{
		}

		Command cmd;
		GlTweak value;
	};
	using CmdEnable = CmdT< OpType::eEnable >;

	void apply( ContextLock const & context
		, CmdEnable const & cmd );

	//*************************************************************************

	template<>
	struct alignas( uint64_t ) CmdT< OpType::eEndQuery >
	{
		inline CmdT( uint32_t target )
			: cmd{ { OpType::eEndQuery, sizeof( CmdT ) / sizeof( uint32_t ) } }
			, target{ std::move( target ) }
		{
		}

		Command cmd;
		uint32_t target;
	};
	using CmdEndQuery = CmdT< OpType::eEndQuery >;

	void apply( ContextLock const & context
		, CmdEndQuery const & cmd );

	//*************************************************************************

	template<>
	struct alignas( uint64_t ) CmdT< OpType::eFillBuffer >
	{
		inline CmdT( VkDeviceMemory memory
			, VkDeviceSize memoryOffset
			, VkDeviceSize dataSize
			, uint32_t data )
			: cmd{ { OpType::eFillBuffer, sizeof( CmdT ) / sizeof( uint32_t ) } }
			, memory{ std::move( memory ) }
			, memoryOffset{ std::move( memoryOffset ) }
			, dataSize{ std::move( dataSize ) }
			, data{ std::move( data ) }
		{
		}

		Command cmd;
		VkDeviceMemory memory;
		VkDeviceSize memoryOffset;
		VkDeviceSize dataSize;
		uint32_t data;
	};
	using CmdFillBuffer = CmdT< OpType::eFillBuffer >;

	void apply( ContextLock const & context
		, CmdFillBuffer const & cmd );

	//*************************************************************************

	template<>
	struct alignas( uint64_t ) CmdT< OpType::eFramebufferTexture >
	{
		inline CmdT( GlFrameBufferTarget target
			, GlAttachmentPoint point
			, uint32_t object
			, uint32_t mipLevel )
			: cmd{ { OpType::eFramebufferTexture, sizeof( CmdT ) / sizeof( uint32_t ) } }
			, target{ target }
			, point{ point }
			, object{ object }
			, mipLevel{ mipLevel }
		{
		}

		Command cmd;
		GlFrameBufferTarget target;
		GlAttachmentPoint point;
		uint32_t object;
		uint32_t mipLevel;
	};
	using CmdFramebufferTexture = CmdT< OpType::eFramebufferTexture >;

	void apply( ContextLock const & context
		, CmdFramebufferTexture const & cmd );

	//*************************************************************************

	template<>
	struct alignas( uint64_t ) CmdT< OpType::eFramebufferTexture1D >
	{
		inline CmdT( GlFrameBufferTarget target
			, GlAttachmentPoint point
			, GlTextureType texTarget
			, uint32_t object
			, uint32_t mipLevel )
			: cmd{ { OpType::eFramebufferTexture1D, sizeof( CmdT ) / sizeof( uint32_t ) } }
			, target{ target }
			, point{ point }
			, texTarget{ texTarget }
			, object{ object }
			, mipLevel{ mipLevel }
		{
		}

		Command cmd;
		GlFrameBufferTarget target;
		GlAttachmentPoint point;
		GlTextureType texTarget;
		uint32_t object;
		uint32_t mipLevel;
	};
	using CmdFramebufferTexture1D = CmdT< OpType::eFramebufferTexture1D >;

	void apply( ContextLock const & context
		, CmdFramebufferTexture1D const & cmd );

	//*************************************************************************

	template<>
	struct alignas( uint64_t ) CmdT< OpType::eFramebufferTexture2D >
	{
		inline CmdT( GlFrameBufferTarget target
			, GlAttachmentPoint point
			, GlTextureType texTarget
			, uint32_t object
			, uint32_t mipLevel )
			: cmd{ { OpType::eFramebufferTexture2D, sizeof( CmdT ) / sizeof( uint32_t ) } }
			, target{ target }
			, point{ point }
			, texTarget{ texTarget }
			, object{ object }
			, mipLevel{ mipLevel }
		{
		}

		Command cmd;
		GlFrameBufferTarget target;
		GlAttachmentPoint point;
		GlTextureType texTarget;
		uint32_t object;
		uint32_t mipLevel;
	};
	using CmdFramebufferTexture2D = CmdT< OpType::eFramebufferTexture2D >;

	void apply( ContextLock const & context
		, CmdFramebufferTexture2D const & cmd );

	//*************************************************************************

	template<>
	struct alignas( uint64_t ) CmdT< OpType::eFramebufferTexture3D >
	{
		inline CmdT( GlFrameBufferTarget target
			, GlAttachmentPoint point
			, GlTextureType texTarget
			, uint32_t object
			, uint32_t mipLevel
			, uint32_t slice )
			: cmd{ { OpType::eFramebufferTexture3D, sizeof( CmdT ) / sizeof( uint32_t ) } }
			, target{ target }
			, point{ point }
			, texTarget{ texTarget }
			, object{ object }
			, mipLevel{ mipLevel }
			, slice{ slice }
		{
		}

		Command cmd;
		GlFrameBufferTarget target;
		GlAttachmentPoint point;
		GlTextureType texTarget;
		uint32_t object;
		uint32_t mipLevel;
		uint32_t slice;
	};
	using CmdFramebufferTexture3D = CmdT< OpType::eFramebufferTexture3D >;

	void apply( ContextLock const & context
		, CmdFramebufferTexture3D const & cmd );

	//*************************************************************************

	template<>
	struct alignas( uint64_t ) CmdT< OpType::eFramebufferTextureLayer >
	{
		inline CmdT( GlFrameBufferTarget target
			, GlAttachmentPoint point
			, uint32_t object
			, uint32_t mipLevel
			, uint32_t arrayLayer )
			: cmd{ { OpType::eFramebufferTextureLayer, sizeof( CmdT ) / sizeof( uint32_t ) } }
			, target{ target }
			, point{ point }
			, object{ object }
			, mipLevel{ mipLevel }
			, arrayLayer{ arrayLayer }
		{
		}

		Command cmd;
		GlFrameBufferTarget target;
		GlAttachmentPoint point;
		uint32_t object;
		uint32_t mipLevel;
		uint32_t arrayLayer;
	};
	using CmdFramebufferTextureLayer = CmdT< OpType::eFramebufferTextureLayer >;

	void apply( ContextLock const & context
		, CmdFramebufferTextureLayer const & cmd );

	//*************************************************************************

	template<>
	struct alignas( uint64_t ) CmdT< OpType::eFrontFace >
	{
		inline CmdT( GlFrontFace value )
			: cmd{ { OpType::eFrontFace, sizeof( CmdT ) / sizeof( uint32_t ) } }
			, value{ std::move( value ) }
		{
		}

		Command cmd;
		GlFrontFace value;
	};
	using CmdFrontFace = CmdT< OpType::eFrontFace >;

	void apply( ContextLock const & context
		, CmdFrontFace const & cmd );

	//*************************************************************************

	template<>
	struct alignas( uint64_t ) CmdT< OpType::eGenerateMipmaps >
	{
		inline CmdT( uint32_t target )
			: cmd{ { OpType::eGenerateMipmaps, sizeof( CmdT ) / sizeof( uint32_t ) } }
			, target{ std::move( target ) }
		{
		}

		Command cmd;
		uint32_t target;
	};
	using CmdGenerateMipmaps = CmdT< OpType::eGenerateMipmaps >;

	void apply( ContextLock const & context
		, CmdGenerateMipmaps const & cmd );

	//*************************************************************************

	template<>
	struct alignas( uint64_t ) CmdT< OpType::eGetCompressedTexImage >
	{
		inline CmdT( GlTextureType target
			, GLint level = 0
			, intptr_t offset = 0 )
			: cmd{ { OpType::eGetCompressedTexImage, sizeof( CmdT ) / sizeof( uint32_t ) } }
			, target{ std::move( target ) }
			, level{ std::move( level ) }
			, offset{ std::move( offset ) }
		{
		}

		Command cmd;
		GlTextureType target;
		GLint level;
		intptr_t offset;
	};
	using CmdGetCompressedTexImage = CmdT< OpType::eGetCompressedTexImage >;

	void apply( ContextLock const & context
		, CmdGetCompressedTexImage const & cmd );

	//*************************************************************************

	template<>
	struct alignas( uint64_t ) CmdT< OpType::eGetQueryResults >
	{
		inline CmdT( VkQueryPool queryPool
			, uint32_t firstQuery
			, uint32_t queryCount
			, VkDeviceSize stride
			, VkQueryResultFlags flags
			, VkDeviceSize bufferOffset )
			: cmd{ { OpType::eGetQueryResults, sizeof( CmdT ) / sizeof( uint32_t ) } }
			, queryPool{ queryPool }
			, firstQuery{ firstQuery }
			, queryCount{ queryCount }
			, stride{ stride }
			, flags{ flags }
			, bufferOffset{ bufferOffset }
		{
		}

		Command cmd;
		VkQueryPool queryPool;
		uint32_t firstQuery;
		uint32_t queryCount;
		VkDeviceSize stride;
		VkQueryResultFlags flags;
		VkDeviceSize bufferOffset;
	};
	using CmdGetQueryResults = CmdT< OpType::eGetQueryResults >;

	void apply( ContextLock const & context
		, CmdGetQueryResults const & cmd );

	//*************************************************************************

	template<>
	struct alignas( uint64_t ) CmdT< OpType::eGetTexImage >
	{
		inline CmdT( GlTextureType target
			, GlFormat format
			, GlType type
			, GLint level = 0
			, intptr_t offset = 0 )
			: cmd{ { OpType::eGetTexImage, sizeof( CmdT ) / sizeof( uint32_t ) } }
			, target{ std::move( target ) }
			, format{ std::move( format ) }
			, type{ std::move( type ) }
			, level{ std::move( level ) }
			, offset{ std::move( offset ) }
		{
		}

		Command cmd;
		GlTextureType target;
		GlFormat format;
		GlType type;
		GLint level;
		intptr_t offset;
	};
	using CmdGetTexImage = CmdT< OpType::eGetTexImage >;

	void apply( ContextLock const & context
		, CmdGetTexImage const & cmd );

	//*************************************************************************

	template<>
	struct alignas( uint64_t ) CmdT< OpType::eLineWidth >
	{
		inline CmdT( float value )
			: cmd{ { OpType::eLineWidth, sizeof( CmdT ) / sizeof( uint32_t ) } }
			, value{ std::move( value ) }
		{
		}

		Command cmd;
		float value;
	};
	using CmdLineWidth = CmdT< OpType::eLineWidth >;

	void apply( ContextLock const & context
		, CmdLineWidth const & cmd );

	//*************************************************************************

	template<>
	struct alignas( uint64_t ) CmdT< OpType::eLogCommand >
	{
		inline CmdT( char const * const value )
			: cmd{ { OpType::eLogCommand, sizeof( CmdT ) / sizeof( uint32_t ) } }
		{
			strncpy( this->value, value, 63 );
		}

		Command cmd;
		char value[64];
	};
	using CmdLogCommand = CmdT< OpType::eLogCommand >;

	void apply( ContextLock const & context
		, CmdLogCommand const & cmd );

	//*************************************************************************

	template<>
	struct alignas( uint64_t ) CmdT< OpType::eLogicOp >
	{
		inline CmdT( GlLogicOp value )
			: cmd{ { OpType::eLogicOp, sizeof( CmdT ) / sizeof( uint32_t ) } }
			, value{ std::move( value ) }
		{
		}

		Command cmd;
		GlLogicOp value;
	};
	using CmdLogicOp = CmdT< OpType::eLogicOp >;

	void apply( ContextLock const & context
		, CmdLogicOp const & cmd );

	//*************************************************************************

	template<>
	struct alignas( uint64_t ) CmdT< OpType::eMemoryBarrier >
	{
		inline CmdT( GlMemoryBarrierFlags flags )
			: cmd{ { OpType::eMemoryBarrier, sizeof( CmdT ) / sizeof( uint32_t ) } }
			, flags{ std::move( flags ) }
		{
		}

		Command cmd;
		GlMemoryBarrierFlags flags;
	};
	using CmdMemoryBarrier = CmdT< OpType::eMemoryBarrier >;

	void apply( ContextLock const & context
		, CmdMemoryBarrier const & cmd );

	//*************************************************************************

	template<>
	struct alignas( uint64_t ) CmdT< OpType::eMinSampleShading >
	{
		inline CmdT( float value )
			: cmd{ { OpType::eMinSampleShading, sizeof( CmdT ) / sizeof( uint32_t ) } }
			, value{ std::move( value ) }
		{
		}

		Command cmd;
		float value;
	};
	using CmdMinSampleShading = CmdT< OpType::eMinSampleShading >;

	void apply( ContextLock const & context
		, CmdMinSampleShading const & cmd );

	//*************************************************************************

	template<>
	struct alignas( uint64_t ) CmdT< OpType::ePatchParameter >
	{
		inline CmdT( PatchParameter param
			, int value )
			: cmd{ { OpType::ePatchParameter, sizeof( CmdT ) / sizeof( uint32_t ) } }
			, param{ std::move( param ) }
			, value{ std::move( value ) }
		{
		}

		Command cmd;
		PatchParameter param;
		int value;
	};
	using CmdPatchParameter = CmdT< OpType::ePatchParameter >;

	void apply( ContextLock const & context
		, CmdPatchParameter const & cmd );


	//*************************************************************************

	template<>
	struct alignas( uint64_t ) CmdT< OpType::ePixelStore >
	{
		inline CmdT( GlPackAlignment name
			, int32_t param )
			: cmd{ { OpType::ePixelStore, sizeof( CmdT ) / sizeof( uint32_t ) } }
			, name{ std::move( name ) }
			, param{ std::move( param ) }
		{
		}

		Command cmd;
		GlPackAlignment name;
		int32_t param;
	};
	using CmdPixelStore = CmdT< OpType::ePixelStore >;

	void apply( ContextLock const & context
		, CmdPixelStore const & cmd );
	//*************************************************************************

	template<>
	struct alignas( uint64_t ) CmdT< OpType::ePolygonMode >
	{
		inline CmdT( GlPolygonMode value )
			: cmd{ { OpType::ePolygonMode, sizeof( CmdT ) / sizeof( uint32_t ) } }
			, value{ std::move( value ) }
		{
		}

		Command cmd;
		GlPolygonMode value;
	};
	using CmdPolygonMode = CmdT< OpType::ePolygonMode >;

	void apply( ContextLock const & context
		, CmdPolygonMode const & cmd );

	//*************************************************************************

	template<>
	struct alignas( uint64_t ) CmdT< OpType::ePolygonOffset >
	{
		inline CmdT( float constantFactor
			, float clamp
			, float slopeFactor )
			: cmd{ { OpType::ePolygonOffset, sizeof( CmdT ) / sizeof( uint32_t ) } }
			, constantFactor{ std::move( constantFactor ) }
			, clamp{ std::move( clamp ) }
			, slopeFactor{ std::move( slopeFactor ) }
		{
		}

		Command cmd;
		float constantFactor;
		float clamp;
		float slopeFactor;
	};
	using CmdPolygonOffset = CmdT< OpType::ePolygonOffset >;

	void apply( ContextLock const & context
		, CmdPolygonOffset const & cmd );

	//*************************************************************************

	template<>
	struct alignas( uint64_t ) CmdT< OpType::ePopDebugGroup >
	{
		inline CmdT()
			: cmd{ { OpType::ePopDebugGroup, sizeof( CmdT ) / sizeof( uint32_t ) } }
		{
		}

		Command cmd;
	};
	using CmdPopDebugGroup = CmdT< OpType::ePopDebugGroup >;

	void apply( ContextLock const & context
		, CmdPopDebugGroup const & cmd );

	//*************************************************************************

	template<>
	struct alignas( uint64_t ) CmdT< OpType::ePrimitiveRestartIndex >
	{
		inline CmdT( uint32_t index )
			: cmd{ { OpType::ePrimitiveRestartIndex, sizeof( CmdT ) / sizeof( uint32_t ) } }
			, index{ std::move( index ) }
		{
		}

		Command cmd;
		uint32_t index;
	};
	using CmdPrimitiveRestartIndex = CmdT< OpType::ePrimitiveRestartIndex >;

	void apply( ContextLock const & context
		, CmdPrimitiveRestartIndex const & cmd );

	//*************************************************************************

	template<>
	struct alignas( uint64_t ) CmdT< OpType::eProgramUniform1fv >
	{
		inline CmdT( uint32_t program
			, uint32_t location
			, float const * buffer )
			: cmd{ { OpType::eProgramUniform1fv, sizeof( CmdT ) / sizeof( uint32_t ) } }
			, program{ std::move( program ) }
			, location{ std::move( location ) }
			, buffer{ *buffer }
		{
		}

		Command cmd;
		uint32_t program;
		uint32_t location;
		float buffer;
	};
	using CmdProgramUniform1fv = CmdT< OpType::eProgramUniform1fv >;

	void apply( ContextLock const & context
		, CmdProgramUniform1fv const & cmd );

	//*************************************************************************

	template<>
	struct alignas( uint64_t ) CmdT< OpType::eProgramUniform2fv >
	{
		inline CmdT( uint32_t program
			, uint32_t location
			, float const * buffer )
			: cmd{ { OpType::eProgramUniform2fv, sizeof( CmdT ) / sizeof( uint32_t ) } }
			, program{ std::move( program ) }
			, location{ std::move( location ) }
			, buffer{ buffer[0], buffer[1] }
		{
		}

		Command cmd;
		uint32_t program;
		uint32_t location;
		float buffer[2];
	};
	using CmdProgramUniform2fv = CmdT< OpType::eProgramUniform2fv >;

	void apply( ContextLock const & context
		, CmdProgramUniform2fv const & cmd );

	//*************************************************************************

	template<>
	struct alignas( uint64_t ) CmdT< OpType::eProgramUniform3fv >
	{
		inline CmdT( uint32_t program
			, uint32_t location
			, float const * buffer )
			: cmd{ { OpType::eProgramUniform3fv, sizeof( CmdT ) / sizeof( uint32_t ) } }
			, program{ std::move( program ) }
			, location{ std::move( location ) }
			, buffer{ buffer[0], buffer[1], buffer[2] }
		{
		}

		Command cmd;
		uint32_t program;
		uint32_t location;
		float buffer[3];
	};
	using CmdProgramUniform3fv = CmdT< OpType::eProgramUniform3fv >;

	void apply( ContextLock const & context
		, CmdProgramUniform3fv const & cmd );

	//*************************************************************************

	template<>
	struct alignas( uint64_t ) CmdT< OpType::eProgramUniform4fv >
	{
		inline CmdT( uint32_t program
			, uint32_t location
			, float const * buffer )
			: cmd{ { OpType::eProgramUniform4fv, sizeof( CmdT ) / sizeof( uint32_t ) } }
			, program{ std::move( program ) }
			, location{ std::move( location ) }
			, buffer{ buffer[0], buffer[1], buffer[2], buffer[3] }
		{
		}

		Command cmd;
		uint32_t program;
		uint32_t location;
		float buffer[4];
	};
	using CmdProgramUniform4fv = CmdT< OpType::eProgramUniform4fv >;

	void apply( ContextLock const & context
		, CmdProgramUniform4fv const & cmd );

	//*************************************************************************

	template<>
	struct alignas( uint64_t ) CmdT< OpType::eProgramUniform1iv >
	{
		inline CmdT( uint32_t program
			, uint32_t location
			, int32_t const * buffer )
			: cmd{ { OpType::eProgramUniform1iv, sizeof( CmdT ) / sizeof( uint32_t ) } }
			, program{ std::move( program ) }
			, location{ std::move( location ) }
			, buffer{ *buffer }
		{
		}

		Command cmd;
		uint32_t program;
		uint32_t location;
		int32_t buffer;
	};
	using CmdProgramUniform1iv = CmdT< OpType::eProgramUniform1iv >;

	void apply( ContextLock const & context
		, CmdProgramUniform1iv const & cmd );

	//*************************************************************************

	template<>
	struct alignas( uint64_t ) CmdT< OpType::eProgramUniform2iv >
	{
		inline CmdT( uint32_t program
			, uint32_t location
			, int32_t const * buffer )
			: cmd{ { OpType::eProgramUniform2iv, sizeof( CmdT ) / sizeof( uint32_t ) } }
			, program{ std::move( program ) }
			, location{ std::move( location ) }
			, buffer{ buffer[0], buffer[1] }
		{
		}

		Command cmd;
		uint32_t program;
		uint32_t location;
		int32_t buffer[2];
	};
	using CmdProgramUniform2iv = CmdT< OpType::eProgramUniform2iv >;

	void apply( ContextLock const & context
		, CmdProgramUniform2iv const & cmd );

	//*************************************************************************

	template<>
	struct alignas( uint64_t ) CmdT< OpType::eProgramUniform3iv >
	{
		inline CmdT( uint32_t program
			, uint32_t location
			, int32_t const * buffer )
			: cmd{ { OpType::eProgramUniform3iv, sizeof( CmdT ) / sizeof( uint32_t ) } }
			, program{ std::move( program ) }
			, location{ std::move( location ) }
			, buffer{ buffer[0], buffer[1], buffer[2] }
		{
		}

		Command cmd;
		uint32_t program;
		uint32_t location;
		int32_t buffer[3];
	};
	using CmdProgramUniform3iv = CmdT< OpType::eProgramUniform3iv >;

	void apply( ContextLock const & context
		, CmdProgramUniform3iv const & cmd );

	//*************************************************************************

	template<>
	struct alignas( uint64_t ) CmdT< OpType::eProgramUniform4iv >
	{
		inline CmdT( uint32_t program
			, uint32_t location
			, int32_t const * buffer )
			: cmd{ { OpType::eProgramUniform4iv, sizeof( CmdT ) / sizeof( uint32_t ) } }
			, program{ std::move( program ) }
			, location{ std::move( location ) }
			, buffer{ buffer[0], buffer[1], buffer[2], buffer[3] }
		{
		}

		Command cmd;
		uint32_t program;
		uint32_t location;
		int32_t buffer[4];
	};
	using CmdProgramUniform4iv = CmdT< OpType::eProgramUniform4iv >;

	void apply( ContextLock const & context
		, CmdProgramUniform4iv const & cmd );

	//*************************************************************************

	template<>
	struct alignas( uint64_t ) CmdT< OpType::eProgramUniform1uiv >
	{
		inline CmdT( uint32_t program
			, uint32_t location
			, uint32_t const * buffer )
			: cmd{ { OpType::eProgramUniform1uiv, sizeof( CmdT ) / sizeof( uint32_t ) } }
			, program{ std::move( program ) }
			, location{ std::move( location ) }
			, buffer{ *buffer }
		{
		}

		Command cmd;
		uint32_t program;
		uint32_t location;
		uint32_t buffer;
	};
	using CmdProgramUniform1uiv = CmdT< OpType::eProgramUniform1uiv >;

	void apply( ContextLock const & context
		, CmdProgramUniform1uiv const & cmd );

	//*************************************************************************

	template<>
	struct alignas( uint64_t ) CmdT< OpType::eProgramUniform2uiv >
	{
		inline CmdT( uint32_t program
			, uint32_t location
			, uint32_t const * buffer )
			: cmd{ { OpType::eProgramUniform2uiv, sizeof( CmdT ) / sizeof( uint32_t ) } }
			, program{ std::move( program ) }
			, location{ std::move( location ) }
			, buffer{ buffer[0], buffer[1] }
		{
		}

		Command cmd;
		uint32_t program;
		uint32_t location;
		uint32_t buffer[2];
	};
	using CmdProgramUniform2uiv = CmdT< OpType::eProgramUniform2uiv >;

	void apply( ContextLock const & context
		, CmdProgramUniform2uiv const & cmd );

	//*************************************************************************

	template<>
	struct alignas( uint64_t ) CmdT< OpType::eProgramUniform3uiv >
	{
		inline CmdT( uint32_t program
			, uint32_t location
			, uint32_t const * buffer )
			: cmd{ { OpType::eProgramUniform3uiv, sizeof( CmdT ) / sizeof( uint32_t ) } }
			, program{ std::move( program ) }
			, location{ std::move( location ) }
			, buffer{ buffer[0], buffer[1], buffer[2] }
		{
		}

		Command cmd;
		uint32_t program;
		uint32_t location;
		uint32_t buffer[3];
	};
	using CmdProgramUniform3uiv = CmdT< OpType::eProgramUniform3uiv >;

	void apply( ContextLock const & context
		, CmdProgramUniform3uiv const & cmd );

	//*************************************************************************

	template<>
	struct alignas( uint64_t ) CmdT< OpType::eProgramUniform4uiv >
	{
		inline CmdT( uint32_t program
			, uint32_t location
			, uint32_t const * buffer )
			: cmd{ { OpType::eProgramUniform4uiv, sizeof( CmdT ) / sizeof( uint32_t ) } }
			, program{ std::move( program ) }
			, location{ std::move( location ) }
			, buffer{ buffer[0], buffer[1], buffer[2], buffer[3] }
		{
		}

		Command cmd;
		uint32_t program;
		uint32_t location;
		uint32_t buffer[4];
	};
	using CmdProgramUniform4uiv = CmdT< OpType::eProgramUniform4uiv >;

	void apply( ContextLock const & context
		, CmdProgramUniform4uiv const & cmd );

	//*************************************************************************

	template<>
	struct alignas( uint64_t ) CmdT< OpType::eProgramUniformMatrix2fv >
	{
		inline CmdT( uint32_t program
			, uint32_t location
			, uint32_t transpose
			, float const * buffer )
			: cmd{ { OpType::eProgramUniformMatrix2fv, sizeof( CmdT ) / sizeof( uint32_t ) } }
			, program{ std::move( program ) }
			, location{ std::move( location ) }
			, transpose{ std::move( transpose ) }
			, buffer
		{
			buffer[0], buffer[1],
			buffer[2], buffer[3]
		}
		{
		}

		Command cmd;
		uint32_t program;
		uint32_t location;
		uint32_t transpose;
		float buffer[4];
	};
	using CmdProgramUniformMatrix2fv = CmdT< OpType::eProgramUniformMatrix2fv >;

	void apply( ContextLock const & context
		, CmdProgramUniformMatrix2fv const & cmd );

	//*************************************************************************

	template<>
	struct alignas( uint64_t ) CmdT< OpType::eProgramUniformMatrix3fv >
	{
		inline CmdT( uint32_t program
			, uint32_t location
			, uint32_t transpose
			, float const * buffer )
			: cmd{ { OpType::eProgramUniformMatrix3fv, sizeof( CmdT ) / sizeof( uint32_t ) } }
			, program{ std::move( program ) }
			, location{ std::move( location ) }
			, transpose{ std::move( transpose ) }
			, buffer
		{
			buffer[0], buffer[1], buffer[2],
			buffer[3], buffer[4], buffer[5],
			buffer[6], buffer[7], buffer[8],
		}
		{
		}

		Command cmd;
		uint32_t program;
		uint32_t location;
		uint32_t transpose;
		float buffer[9];
	};
	using CmdProgramUniformMatrix3fv = CmdT< OpType::eProgramUniformMatrix3fv >;

	void apply( ContextLock const & context
		, CmdProgramUniformMatrix3fv const & cmd );

	//*************************************************************************

	template<>
	struct alignas( uint64_t ) CmdT< OpType::eProgramUniformMatrix4fv >
	{
		inline CmdT( uint32_t program
			, uint32_t location
			, uint32_t transpose
			, float const * buffer )
			: cmd{ { OpType::eProgramUniformMatrix4fv, sizeof( CmdT ) / sizeof( uint32_t ) } }
			, program{ std::move( program ) }
			, location{ std::move( location ) }
			, transpose{ std::move( transpose ) }
			, buffer
		{
			buffer[0], buffer[1], buffer[2], buffer[3],
			buffer[4], buffer[5], buffer[6], buffer[7],
			buffer[8], buffer[9], buffer[10], buffer[11],
			buffer[12], buffer[13], buffer[14], buffer[15],
		}
		{
		}

		Command cmd;
		uint32_t program;
		uint32_t location;
		uint32_t transpose;
		float buffer[16];
	};
	using CmdProgramUniformMatrix4fv = CmdT< OpType::eProgramUniformMatrix4fv >;

	void apply( ContextLock const & context
		, CmdProgramUniformMatrix4fv const & cmd );

	//*************************************************************************

	template<>
	struct alignas( uint64_t ) CmdT< OpType::ePushDebugGroup >
	{
		inline CmdT( GlDebugSource source
			, GLuint id
			, GLsizei length
			, const char * message )
			: cmd{ { OpType::ePushDebugGroup, sizeof( CmdT ) / sizeof( uint32_t ) } }
			, source{ source }
			, id{ id }
			, length{ length }
		{
			this->length = GLsizei( std::min( strnlen( message, size_t( length ) ), size_t( 255u ) ) );
			strncpy( this->message
				, message
				, size_t( this->length ) );
			this->message[this->length] = 0;
		}

		Command cmd;
		GlDebugSource source;
		GLuint id;
		GLsizei length;
		char message[256];
	};
	using CmdPushDebugGroup = CmdT< OpType::ePushDebugGroup >;

	void apply( ContextLock const & context
		, CmdPushDebugGroup const & cmd );

	//*************************************************************************

	template<>
	struct alignas( uint64_t ) CmdT< OpType::eReadBuffer >
	{
		inline CmdT( GlAttachmentPoint point )
			: cmd{ { OpType::eReadBuffer, sizeof( CmdT ) / sizeof( uint32_t ) } }
			, point{ std::move( point ) }
		{
		}

		Command cmd;
		GlAttachmentPoint point;
	};
	using CmdReadBuffer = CmdT< OpType::eReadBuffer >;

	void apply( ContextLock const & context
		, CmdReadBuffer const & cmd );

	//*************************************************************************

	template<>
	struct alignas( uint64_t ) CmdT< OpType::eReadPixels >
	{
		inline CmdT( int32_t x
			, int32_t y
			, uint32_t width
			, uint32_t height
			, GlFormat format
			, GlType type )
			: cmd{ { OpType::eReadPixels, sizeof( CmdT ) / sizeof( uint32_t ) } }
			, x{ std::move( x ) }
			, y{ std::move( y ) }
			, width{ std::move( width ) }
			, height{ std::move( height ) }
			, format{ std::move( format ) }
			, type{ std::move( type ) }
		{
		}

		Command cmd;
		int32_t x;
		int32_t y;
		uint32_t width;
		uint32_t height;
		GlFormat format;
		GlType type;
	};
	using CmdReadPixels = CmdT< OpType::eReadPixels >;

	void apply( ContextLock const & context
		, CmdReadPixels const & cmd );

	//*************************************************************************

	template<>
	struct alignas( uint64_t ) CmdT< OpType::eResetEvent >
	{
		inline CmdT( VkEvent event )
			: cmd{ { OpType::eResetEvent, sizeof( CmdT ) / sizeof( uint32_t ) } }
			, event{ std::move( event ) }
		{
		}

		Command cmd;
		VkEvent event;
	};
	using CmdResetEvent = CmdT< OpType::eResetEvent >;

	void apply( ContextLock const & context
		, CmdResetEvent const & cmd );

	//*************************************************************************

	template<>
	struct alignas( uint64_t ) CmdT< OpType::eSetEvent >
	{
		inline CmdT( VkEvent event )
			: cmd{ { OpType::eSetEvent, sizeof( CmdT ) / sizeof( uint32_t ) } }
			, event{ std::move( event ) }
		{
		}

		Command cmd;
		VkEvent event;
	};
	using CmdSetEvent = CmdT< OpType::eSetEvent >;

	void apply( ContextLock const & context
		, CmdSetEvent const & cmd );

	//*************************************************************************

	template<>
	struct alignas( uint64_t ) CmdT< OpType::eSetLineWidth >
	{
		inline CmdT( float width )
			: cmd{ { OpType::eSetLineWidth, sizeof( CmdT ) / sizeof( uint32_t ) } }
			, width{ std::move( width ) }
		{
		}

		Command cmd;
		float width;
	};
	using CmdSetLineWidth = CmdT< OpType::eSetLineWidth >;

	void apply( ContextLock const & context
		, CmdSetLineWidth const & cmd );

	//*************************************************************************

	template<>
	struct alignas( uint64_t ) CmdT< OpType::eStencilFunc >
	{
		inline CmdT( GlCullMode face
			, GlCompareOp op
			, uint32_t ref
			, uint32_t compMask )
			: cmd{ { OpType::eStencilFunc, sizeof( CmdT ) / sizeof( uint32_t ) } }
			, face{ std::move( face ) }
			, op{ std::move( op ) }
			, ref{ std::move( ref ) }
			, compMask{ std::move( compMask ) }
		{
		}

		Command cmd;
		GlCullMode face;
		GlCompareOp op;
		uint32_t ref;
		uint32_t compMask;
	};
	using CmdStencilFunc = CmdT< OpType::eStencilFunc >;

	void apply( ContextLock const & context
		, CmdStencilFunc const & cmd );

	//*************************************************************************

	template<>
	struct alignas( uint64_t ) CmdT< OpType::eStencilMask >
	{
		inline CmdT( GlCullMode face
			, uint32_t mask )
			: cmd{ { OpType::eStencilMask, sizeof( CmdT ) / sizeof( uint32_t ) } }
			, face{ std::move( face ) }
			, mask{ std::move( mask ) }
		{
		}

		Command cmd;
		GlCullMode face;
		uint32_t mask;
	};
	using CmdStencilMask = CmdT< OpType::eStencilMask >;

	void apply( ContextLock const & context
		, CmdStencilMask const & cmd );

	//*************************************************************************

	template<>
	struct alignas( uint64_t ) CmdT< OpType::eStencilOp >
	{
		inline CmdT( GlCullMode face
			, GlStencilOp fail
			, GlStencilOp depthFail
			, GlStencilOp pass )
			: cmd{ { OpType::eStencilOp, sizeof( CmdT ) / sizeof( uint32_t ) } }
			, face{ std::move( face ) }
			, fail{ std::move( fail ) }
			, depthFail{ std::move( depthFail ) }
			, pass{ std::move( pass ) }
		{
		}

		Command cmd;
		GlCullMode face;
		GlStencilOp fail;
		GlStencilOp depthFail;
		GlStencilOp pass;
	};
	using CmdStencilOp = CmdT< OpType::eStencilOp >;

	void apply( ContextLock const & context
		, CmdStencilOp const & cmd );

	//*************************************************************************

	template<>
	struct alignas( uint64_t ) CmdT< OpType::eTexParameteri >
	{
		inline CmdT( GlTextureType target
			, uint32_t name
			, int32_t param )
			: cmd{ { OpType::eTexParameteri, sizeof( CmdT ) / sizeof( uint32_t ) } }
			, target{ std::move( target ) }
			, name{ std::move( name ) }
			, param{ std::move( param ) }
		{
		}

		Command cmd;
		GlTextureType target;
		uint32_t name;
		int32_t param;
	};
	using CmdTexParameteri = CmdT< OpType::eTexParameteri >;

	void apply( ContextLock const & context
		, CmdTexParameteri const & cmd );

	//*************************************************************************

	template<>
	struct alignas( uint64_t ) CmdT< OpType::eTexParameterf >
	{
		inline CmdT( GlTextureType target
			, uint32_t name
			, float param )
			: cmd{ { OpType::eTexParameterf, sizeof( CmdT ) / sizeof( uint32_t ) } }
			, target{ std::move( target ) }
			, name{ std::move( name ) }
			, param{ std::move( param ) }
		{
		}

		Command cmd;
		GlTextureType target;
		uint32_t name;
		float param;
	};
	using CmdTexParameterf = CmdT< OpType::eTexParameterf >;

	void apply( ContextLock const & context
		, CmdTexParameterf const & cmd );

	//*************************************************************************

	template<>
	struct alignas( uint64_t ) CmdT< OpType::eTexSubImage1D >
	{
		inline CmdT( GlTextureType copyTarget
			, uint32_t mipLevel
			, int32_t x
			, uint32_t width
			, GlFormat format
			, GlType type
			, int32_t bufferOffset )
			: cmd{ { OpType::eTexSubImage1D, sizeof( CmdT ) / sizeof( uint32_t ) } }
			, copyTarget{ std::move( copyTarget ) }
			, mipLevel{ std::move( mipLevel ) }
			, x{ std::move( x ) }
			, width{ std::move( width ) }
			, format{ std::move( format ) }
			, type{ std::move( type ) }
			, bufferOffset{ std::move( bufferOffset ) }
		{
		}

		Command cmd;
		GlTextureType copyTarget;
		uint32_t mipLevel;
		int32_t x;
		uint32_t width;
		GlFormat format;
		GlType type;
		int32_t bufferOffset;
	};
	using CmdTexSubImage1D = CmdT< OpType::eTexSubImage1D >;

	void apply( ContextLock const & context
		, CmdTexSubImage1D const & cmd );

	//*************************************************************************

	template<>
	struct alignas( uint64_t ) CmdT< OpType::eTexSubImage2D >
	{
		inline CmdT( GlTextureType copyTarget
			, uint32_t mipLevel
			, int32_t x
			, int32_t y
			, uint32_t width
			, uint32_t height
			, GlFormat format
			, GlType type
			, int32_t bufferOffset )
			: cmd{ { OpType::eTexSubImage2D, sizeof( CmdT ) / sizeof( uint32_t ) } }
			, copyTarget{ std::move( copyTarget ) }
			, mipLevel{ std::move( mipLevel ) }
			, x{ std::move( x ) }
			, y{ std::move( y ) }
			, width{ std::move( width ) }
			, height{ std::move( height ) }
			, format{ std::move( format ) }
			, type{ std::move( type ) }
			, bufferOffset{ std::move( bufferOffset ) }
		{
		}

		Command cmd;
		GlTextureType copyTarget;
		uint32_t mipLevel;
		int32_t x;
		int32_t y;
		uint32_t width;
		uint32_t height;
		GlFormat format;
		GlType type;
		int32_t bufferOffset;
	};
	using CmdTexSubImage2D = CmdT< OpType::eTexSubImage2D >;

	void apply( ContextLock const & context
		, CmdTexSubImage2D const & cmd );

	//*************************************************************************

	template<>
	struct alignas( uint64_t ) CmdT< OpType::eTexSubImage3D >
	{
		inline CmdT( GlTextureType copyTarget
			, uint32_t mipLevel
			, int32_t x
			, int32_t y
			, int32_t z
			, uint32_t width
			, uint32_t height
			, uint32_t depth
			, GlFormat format
			, GlType type
			, int32_t bufferOffset )
			: cmd{ { OpType::eTexSubImage3D, sizeof( CmdT ) / sizeof( uint32_t ) } }
			, copyTarget{ std::move( copyTarget ) }
			, mipLevel{ std::move( mipLevel ) }
			, x{ std::move( x ) }
			, y{ std::move( y ) }
			, z{ std::move( z ) }
			, width{ std::move( width ) }
			, height{ std::move( height ) }
			, depth{ std::move( depth ) }
			, format{ std::move( format ) }
			, type{ std::move( type ) }
			, bufferOffset{ std::move( bufferOffset ) }
		{
		}

		Command cmd;
		GlTextureType copyTarget;
		uint32_t mipLevel;
		int32_t x;
		int32_t y;
		int32_t z;
		uint32_t width;
		uint32_t height;
		uint32_t depth;
		GlFormat format;
		GlType type;
		int32_t bufferOffset;
	};
	using CmdTexSubImage3D = CmdT< OpType::eTexSubImage3D >;

	void apply( ContextLock const & context
		, CmdTexSubImage3D const & cmd );

	//*************************************************************************

	template<>
	struct alignas( uint64_t ) CmdT< OpType::eUniform1fv >
	{
		inline CmdT( uint32_t location
			, float const * buffer )
			: cmd{ { OpType::eUniform1fv, sizeof( CmdT ) / sizeof( uint32_t ) } }
			, location{ std::move( location ) }
			, buffer{ *buffer }
		{
		}

		Command cmd;
		uint32_t location;
		float buffer;
	};
	using CmdUniform1fv = CmdT< OpType::eUniform1fv >;

	void apply( ContextLock const & context
		, CmdUniform1fv const & cmd );

	//*************************************************************************

	template<>
	struct alignas( uint64_t ) CmdT< OpType::eUniform2fv >
	{
		inline CmdT( uint32_t location
			, float const * buffer )
			: cmd{ { OpType::eUniform2fv, sizeof( CmdT ) / sizeof( uint32_t ) } }
			, location{ std::move( location ) }
			, buffer{ buffer[0], buffer[1] }
		{
		}

		Command cmd;
		uint32_t location;
		float buffer[2];
	};
	using CmdUniform2fv = CmdT< OpType::eUniform2fv >;

	void apply( ContextLock const & context
		, CmdUniform2fv const & cmd );

	//*************************************************************************

	template<>
	struct alignas( uint64_t ) CmdT< OpType::eUniform3fv >
	{
		inline CmdT( uint32_t location
			, float const * buffer )
			: cmd{ { OpType::eUniform3fv, sizeof( CmdT ) / sizeof( uint32_t ) } }
			, location{ std::move( location ) }
			, buffer{ buffer[0], buffer[1], buffer[2] }
		{
		}

		Command cmd;
		uint32_t location;
		float buffer[3];
	};
	using CmdUniform3fv = CmdT< OpType::eUniform3fv >;

	void apply( ContextLock const & context
		, CmdUniform3fv const & cmd );

	//*************************************************************************

	template<>
	struct alignas( uint64_t ) CmdT< OpType::eUniform4fv >
	{
		inline CmdT( uint32_t location
			, float const * buffer )
			: cmd{ { OpType::eUniform4fv, sizeof( CmdT ) / sizeof( uint32_t ) } }
			, location{ std::move( location ) }
			, buffer{ buffer[0], buffer[1], buffer[2], buffer[3] }
		{
		}

		Command cmd;
		uint32_t location;
		float buffer[4];
	};
	using CmdUniform4fv = CmdT< OpType::eUniform4fv >;

	void apply( ContextLock const & context
		, CmdUniform4fv const & cmd );

	//*************************************************************************

	template<>
	struct alignas( uint64_t ) CmdT< OpType::eUniform1iv >
	{
		inline CmdT( uint32_t location
			, int32_t const * buffer )
			: cmd{ { OpType::eUniform1iv, sizeof( CmdT ) / sizeof( uint32_t ) } }
			, location{ std::move( location ) }
			, buffer{ *buffer }
		{
		}

		Command cmd;
		uint32_t location;
		int32_t buffer;
	};
	using CmdUniform1iv = CmdT< OpType::eUniform1iv >;

	void apply( ContextLock const & context
		, CmdUniform1iv const & cmd );

	//*************************************************************************

	template<>
	struct alignas( uint64_t ) CmdT< OpType::eUniform2iv >
	{
		inline CmdT( uint32_t location
			, int32_t const * buffer )
			: cmd{ { OpType::eUniform2iv, sizeof( CmdT ) / sizeof( uint32_t ) } }
			, location{ std::move( location ) }
			, buffer{ buffer[0], buffer[1] }
		{
		}

		Command cmd;
		uint32_t location;
		int32_t buffer[2];
	};
	using CmdUniform2iv = CmdT< OpType::eUniform2iv >;

	void apply( ContextLock const & context
		, CmdUniform2iv const & cmd );

	//*************************************************************************

	template<>
	struct alignas( uint64_t ) CmdT< OpType::eUniform3iv >
	{
		inline CmdT( uint32_t location
			, int32_t const * buffer )
			: cmd{ { OpType::eUniform3iv, sizeof( CmdT ) / sizeof( uint32_t ) } }
			, location{ std::move( location ) }
			, buffer{ buffer[0], buffer[1], buffer[2] }
		{
		}

		Command cmd;
		uint32_t location;
		int32_t buffer[3];
	};
	using CmdUniform3iv = CmdT< OpType::eUniform3iv >;

	void apply( ContextLock const & context
		, CmdUniform3iv const & cmd );

	//*************************************************************************

	template<>
	struct alignas( uint64_t ) CmdT< OpType::eUniform4iv >
	{
		inline CmdT( uint32_t location
			, int32_t const * buffer )
			: cmd{ { OpType::eUniform4iv, sizeof( CmdT ) / sizeof( uint32_t ) } }
			, location{ std::move( location ) }
			, buffer{ buffer[0], buffer[1], buffer[2], buffer[3] }
		{
		}

		Command cmd;
		uint32_t location;
		int32_t buffer[4];
	};
	using CmdUniform4iv = CmdT< OpType::eUniform4iv >;

	void apply( ContextLock const & context
		, CmdUniform4iv const & cmd );

	//*************************************************************************

	template<>
	struct alignas( uint64_t ) CmdT< OpType::eUniform1uiv >
	{
		inline CmdT( uint32_t location
			, uint32_t const * buffer )
			: cmd{ { OpType::eUniform1uiv, sizeof( CmdT ) / sizeof( uint32_t ) } }
			, location{ std::move( location ) }
			, buffer{ *buffer }
		{
		}

		Command cmd;
		uint32_t location;
		uint32_t buffer;
	};
	using CmdUniform1uiv = CmdT< OpType::eUniform1uiv >;

	void apply( ContextLock const & context
		, CmdUniform1uiv const & cmd );

	//*************************************************************************

	template<>
	struct alignas( uint64_t ) CmdT< OpType::eUniform2uiv >
	{
		inline CmdT( uint32_t location
			, uint32_t const * buffer )
			: cmd{ { OpType::eUniform2uiv, sizeof( CmdT ) / sizeof( uint32_t ) } }
			, location{ std::move( location ) }
			, buffer{ buffer[0], buffer[1] }
		{
		}

		Command cmd;
		uint32_t location;
		uint32_t buffer[2];
	};
	using CmdUniform2uiv = CmdT< OpType::eUniform2uiv >;

	void apply( ContextLock const & context
		, CmdUniform2uiv const & cmd );

	//*************************************************************************

	template<>
	struct alignas( uint64_t ) CmdT< OpType::eUniform3uiv >
	{
		inline CmdT( uint32_t location
			, uint32_t const * buffer )
			: cmd{ { OpType::eUniform3uiv, sizeof( CmdT ) / sizeof( uint32_t ) } }
			, location{ std::move( location ) }
			, buffer{ buffer[0], buffer[1], buffer[2] }
		{
		}

		Command cmd;
		uint32_t location;
		uint32_t buffer[3];
	};
	using CmdUniform3uiv = CmdT< OpType::eUniform3uiv >;

	void apply( ContextLock const & context
		, CmdUniform3uiv const & cmd );

	//*************************************************************************

	template<>
	struct alignas( uint64_t ) CmdT< OpType::eUniform4uiv >
	{
		inline CmdT( uint32_t location
			, uint32_t const * buffer )
			: cmd{ { OpType::eUniform4uiv, sizeof( CmdT ) / sizeof( uint32_t ) } }
			, location{ std::move( location ) }
			, buffer{ buffer[0], buffer[1], buffer[2], buffer[3] }
		{
		}

		Command cmd;
		uint32_t location;
		uint32_t buffer[4];
	};
	using CmdUniform4uiv = CmdT< OpType::eUniform4uiv >;

	void apply( ContextLock const & context
		, CmdUniform4uiv const & cmd );

	//*************************************************************************

	template<>
	struct alignas( uint64_t ) CmdT< OpType::eUniformMatrix2fv >
	{
		inline CmdT( uint32_t location
			, uint32_t transpose
			, float const * buffer )
			: cmd{ { OpType::eUniformMatrix2fv, sizeof( CmdT ) / sizeof( uint32_t ) } }
			, location{ std::move( location ) }
			, transpose{ std::move( transpose ) }
			, buffer
		{
			buffer[0], buffer[1],
			buffer[2], buffer[3]
		}
		{
		}

		Command cmd;
		uint32_t location;
		uint32_t transpose;
		float buffer[4];
	};
	using CmdUniformMatrix2fv = CmdT< OpType::eUniformMatrix2fv >;

	void apply( ContextLock const & context
		, CmdUniformMatrix2fv const & cmd );

	//*************************************************************************

	template<>
	struct alignas( uint64_t ) CmdT< OpType::eUniformMatrix3fv >
	{
		inline CmdT( uint32_t location
			, uint32_t transpose
			, float const * buffer )
			: cmd{ { OpType::eUniformMatrix3fv, sizeof( CmdT ) / sizeof( uint32_t ) } }
			, location{ std::move( location ) }
			, transpose{ std::move( transpose ) }
			, buffer
		{
			buffer[0], buffer[1], buffer[2],
			buffer[3], buffer[4], buffer[5],
			buffer[6], buffer[7], buffer[8],
		}
		{
		}

		Command cmd;
		uint32_t location;
		uint32_t transpose;
		float buffer[9];
	};
	using CmdUniformMatrix3fv = CmdT< OpType::eUniformMatrix3fv >;

	void apply( ContextLock const & context
		, CmdUniformMatrix3fv const & cmd );

	//*************************************************************************

	template<>
	struct alignas( uint64_t ) CmdT< OpType::eUniformMatrix4fv >
	{
		inline CmdT( uint32_t location
			, uint32_t transpose
			, float const * buffer )
			: cmd{ { OpType::eUniformMatrix4fv, sizeof( CmdT ) / sizeof( uint32_t ) } }
			, location{ std::move( location ) }
			, transpose{ std::move( transpose ) }
			, buffer
		{
			buffer[0], buffer[1], buffer[2], buffer[3],
			buffer[4], buffer[5], buffer[6], buffer[7],
			buffer[8], buffer[9], buffer[10], buffer[11],
			buffer[12], buffer[13], buffer[14], buffer[15],
		}
		{
		}

		Command cmd;
		uint32_t location;
		uint32_t transpose;
		float buffer[16];
	};
	using CmdUniformMatrix4fv = CmdT< OpType::eUniformMatrix4fv >;

	void apply( ContextLock const & context
		, CmdUniformMatrix4fv const & cmd );

	//*************************************************************************

	template<>
	struct alignas( uint64_t ) CmdT< OpType::eUpdateBuffer >
	{
		inline CmdT( VkDeviceMemory memory
			, VkDeviceSize memoryOffset
			, VkDeviceSize dataSize
			, void * pData )
			: cmd{ { OpType::eUpdateBuffer, sizeof( CmdT ) / sizeof( uint32_t ) } }
			, memory{ std::move( memory ) }
			, memoryOffset{ std::move( memoryOffset ) }
			, dataSize{ std::move( dataSize ) }
			, pData{ std::move( pData ) }
		{
		}

		Command cmd;
		VkDeviceMemory memory;
		VkDeviceSize memoryOffset;
		VkDeviceSize dataSize;
		void * pData;
	};
	using CmdUpdateBuffer = CmdT< OpType::eUpdateBuffer >;

	void apply( ContextLock const & context
		, CmdUpdateBuffer const & cmd );

	//*************************************************************************

	template<>
	struct alignas( uint64_t ) CmdT< OpType::eUploadMemory >
	{
		inline CmdT( VkDeviceMemory memory
			, VkDeviceSize offset
			, VkDeviceSize size )
			: cmd{ { OpType::eUploadMemory, sizeof( CmdT ) / sizeof( uint32_t ) } }
			, memory{ memory }
			, offset{ offset }
			, size{ size }
		{
		}

		Command cmd;
		VkDeviceMemory memory;
		VkDeviceSize offset;
		VkDeviceSize size;
	};
	using CmdUploadMemory = CmdT< OpType::eUploadMemory >;

	void apply( ContextLock const & context
		, CmdUploadMemory const & cmd );

	//*************************************************************************

	template<>
	struct alignas( uint64_t ) CmdT< OpType::eUseProgram >
	{
		inline CmdT( uint32_t value )
			: cmd{ { OpType::eUseProgram, sizeof( CmdT ) / sizeof( uint32_t ) } }
			, program{ std::move( value ) }
		{
		}

		Command cmd;
		uint32_t program;
	};
	using CmdUseProgram = CmdT< OpType::eUseProgram >;

	void apply( ContextLock const & context
		, CmdUseProgram const & cmd );

	//*************************************************************************

	template<>
	struct alignas( uint64_t ) CmdT< OpType::eUseProgramPipeline >
	{
		inline CmdT( uint32_t value )
			: cmd{ { OpType::eUseProgramPipeline, sizeof( CmdT ) / sizeof( uint32_t ) } }
			, program{ std::move( value ) }
		{
		}

		Command cmd;
		uint32_t program;
	};
	using CmdUseProgramPipeline = CmdT< OpType::eUseProgramPipeline >;

	void apply( ContextLock const & context
		, CmdUseProgramPipeline const & cmd );

	//*************************************************************************

	template<>
	struct alignas( uint64_t ) CmdT< OpType::eWaitEvents >
	{
		inline CmdT( VkEventArray events )
			: cmd{ { OpType::eWaitEvents, uint16_t( sizeof( Command ) + events.size() * 2u ) } }
			, events{ std::move( events ) }
		{
		}

		Command cmd;
		VkEventArray events;
	};
	using CmdWaitEvents = CmdT< OpType::eWaitEvents >;

	void apply( ContextLock const & context
		, CmdWaitEvents const & cmd );

	//*************************************************************************

	template<>
	struct alignas( uint64_t ) CmdT< OpType::eWriteTimestamp >
	{
		inline CmdT( uint32_t name )
			: cmd{ { OpType::eWriteTimestamp, sizeof( CmdT ) / sizeof( uint32_t ) } }
			, name{ std::move( name ) }
		{
		}

		Command cmd;
		uint32_t name;
	};
	using CmdWriteTimestamp = CmdT< OpType::eWriteTimestamp >;

	void apply( ContextLock const & context
		, CmdWriteTimestamp const & cmd );

	//*************************************************************************

	template< OpType OpT, typename ... ParamsT >
	CmdBuffer makeCmd( ParamsT && ... params )
	{
		CmdT< OpT > cmd{ std::forward< ParamsT && >( params )... };
		CmdBuffer result;
		result.resize( cmd.cmd.op.size );
		auto it = result.begin();
		push( it, result.end(), cmd );
		return result;
	}

	template< typename IterT >
	bool map( IterT & cur, IterT end, Command const *& cmd )
	{
		bool result = hasRemaining< uint32_t >( cur, end, 1u );

		if ( result )
		{
			cmd = reinterpret_cast< Command const * >( &( *cur ) );
		}

		return result;
	}

	template< typename IterT >
	bool map( IterT & cur, IterT end, Command *& cmd )
	{
		bool result = hasRemaining< uint32_t >( cur, end, 1u );

		if ( result )
		{
			cmd = reinterpret_cast< Command * >( &( *cur ) );
		}

		return result;
	}

	template< OpType OpT >
	CmdT< OpT > const & map( Command const & cmd )
	{
		return *reinterpret_cast< CmdT< OpT > const * >( &cmd );
	}

	template< OpType OpT >
	CmdT< OpT > & map( Command & cmd )
	{
		return *reinterpret_cast< CmdT< OpT > * >( &cmd );
	}

	template< typename IterT, OpType OpT >
	bool push( IterT & cur, IterT end, CmdT< OpT > const & cmd )
	{
		auto count = cmd.cmd.op.size;
		bool result = hasRemaining< uint32_t >( cur, end, count );

		if ( result )
		{
			*reinterpret_cast< CmdT< OpT > * >( &( *cur ) ) = cmd;
			cur += count;
		}
		else
		{
			cur = end;
		}

		return result;
	}

	template< typename IterT, OpType OpT >
	bool pop( IterT & cur, IterT end, CmdT< OpT > & cmd )
	{
		bool result = hasRemaining< uint32_t >( cur, end, 1u );
		auto it = cur;

		if ( result )
		{
			Command command;
			result = pop( cur, end, command );

			if ( result )
			{
				auto count = command.op.size - 1u;
				result = hasRemaining< uint32_t >( cur, end, count );

				if ( result )
				{
					cmd = *reinterpret_cast< CmdT< OpT > * >( &( *it ) );
					cur += count;
				}
			}
		}

		if ( !result )
		{
			cur = end;
		}

		return result;
	}

	//*************************************************************************
}

#pragma GCC diagnostic pop
#pragma warning( pop )
