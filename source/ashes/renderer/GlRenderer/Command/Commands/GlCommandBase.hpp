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
		constexpr Command( OpType cmd, size_t size )
			: op{ cmd, uint16_t( size ) }
		{
		}

		Op op;
		uint32_t dummy{ 0u };
	};

	template< typename CommandT >
	constexpr Command makeCommand( OpType op )
	{
		return Command{ op, sizeof( CommandT ) / sizeof( uint32_t ) };
	}

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
		explicit CmdT( uint32_t binding )
			: binding{ binding }
		{
		}

		Command cmd{ makeCommand< CmdT >( OpType::eActiveTexture ) };
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

		explicit CmdT( uint32_t first
			, uint32_t count
			, VkViewport const * viewports
			, size_t viewportsSize )
			: first{ first }
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

		explicit CmdT( uint32_t first
			, uint32_t count
			, std::vector< VkViewport > const & viewports )
			: CmdT{ first, count, viewports.data(), viewports.size() }
		{
		}

		explicit CmdT( uint32_t first
			, uint32_t count
			, ArrayView< VkViewport const > const & viewports )
			: CmdT{ first, count, viewports.data(), viewports.size() }
		{
		}

		explicit CmdT( std::vector< VkViewport > const & viewports )
			: CmdT{ 0u, uint32_t( viewports.size() ), viewports }
		{
		}

		Command cmd{ makeCommand< CmdT >( OpType::eApplyDepthRanges ) };
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
		explicit CmdT( VkRect2D const & scissor )
			: scissor{ scissor }
		{
		}

		Command cmd{ makeCommand< CmdT >( OpType::eApplyScissor ) };
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

		explicit CmdT( uint32_t first
			, uint32_t count
			, VkRect2D const * scissors
			, size_t scissorsSize )
			: first{ first }
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

		explicit CmdT( uint32_t first
			, uint32_t count
			, std::vector< VkRect2D > const & scissors )
			: CmdT{ first, count, scissors.data(), scissors.size() }
		{
		}

		explicit CmdT( uint32_t first
			, uint32_t count
			, ArrayView< VkRect2D const > const & scissors )
			: CmdT{ first, count, scissors.data(), scissors.size() }
		{
		}

		explicit CmdT( std::vector< VkRect2D > const & scissors )
			: CmdT{ 0u, uint32_t( scissors.size() ), scissors }
		{
		}

		explicit CmdT( uint32_t first
			, ArrayView< VkRect2D const > const & scissors )
			: CmdT{ first, uint32_t( scissors.size() ), scissors }
		{
		}

		Command cmd{ makeCommand< CmdT >( OpType::eApplyScissors ) };
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
		explicit CmdT( VkViewport const & viewport )
			: viewport{ viewport }
		{
		}

		Command cmd{ makeCommand< CmdT >( OpType::eApplyViewport ) };
		uint32_t count{ 1u };
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

		explicit CmdT( uint32_t first
			, uint32_t count
			, VkViewport const * viewports
			, size_t viewportsSize )
			: first{ first }
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

		explicit CmdT( uint32_t first
			, uint32_t count
			, std::vector< VkViewport > const & viewports )
			: CmdT{ first, count, viewports.data(), viewports.size() }
		{
		}

		explicit CmdT( uint32_t first
			, uint32_t count
			, ArrayView< VkViewport const > const & viewports )
			: CmdT{ first, count, viewports.data(), viewports.size() }
		{
		}

		explicit CmdT( std::vector< VkViewport > const & viewports )
			: CmdT{ 0u, uint32_t( viewports.size() ), viewports }
		{
		}

		explicit CmdT( uint32_t first
			, ArrayView< VkViewport const > const & viewports )
			: CmdT{ first, uint32_t( viewports.size() ), viewports }
		{
		}

		Command cmd{ makeCommand< CmdT >( OpType::eApplyViewports ) };
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
		explicit CmdT( uint32_t target
			, uint32_t query )
			: target{ target }
			, query{ query }
		{
		}

		Command cmd{ makeCommand< CmdT >( OpType::eBeginQuery ) };
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
		explicit CmdT( uint32_t target
			, uint32_t name )
			: target{ GlBufferTarget( target ) }
			, name{ name }
		{
		}

		Command cmd{ makeCommand< CmdT >( OpType::eBindBuffer ) };
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
		explicit CmdT( GlFrameBufferTarget target
			, VkFramebuffer fbo )
			: target{ target }
			, fbo{ fbo }
		{
		}

		Command cmd{ makeCommand< CmdT >( OpType::eBindFramebuffer ) };
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
		explicit CmdT( GlFrameBufferTarget target )
			: target{ target }
		{
		}

		Command cmd{ makeCommand< CmdT >( OpType::eBindSrcFramebuffer ) };
		GlFrameBufferTarget target;
	};
	using CmdBindSrcFramebuffer = CmdT< OpType::eBindSrcFramebuffer >;

	void apply( ContextLock const & context
		, CmdBindSrcFramebuffer const & cmd );

	//*************************************************************************

	template<>
	struct alignas( uint64_t ) CmdT< OpType::eBindDstFramebuffer >
	{
		explicit CmdT( GlFrameBufferTarget target )
			: target{ target }
		{
		}

		Command cmd{ makeCommand< CmdT >( OpType::eBindDstFramebuffer ) };
		GlFrameBufferTarget target;
	};
	using CmdBindDstFramebuffer = CmdT< OpType::eBindDstFramebuffer >;

	void apply( ContextLock const & context
		, CmdBindDstFramebuffer const & cmd );

	//*************************************************************************

	template<>
	struct alignas( uint64_t ) CmdT< OpType::eBindBufferRange >
	{
		explicit CmdT( uint32_t binding
			, uint32_t target
			, uint32_t name
			, int64_t offset
			, int64_t range )
			: binding{ binding }
			, target{ GlBufferTarget( target ) }
			, name{ name }
			, offset{ offset }
			, range{ range }
		{
		}

		Command cmd{ makeCommand< CmdT >( OpType::eBindBufferRange ) };
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
		explicit CmdT( ContextStateStack * stack
			, ContextState * state )
			: stack{ stack }
			, state{ state }
		{
		}

		Command cmd{ makeCommand< CmdT >( OpType::eBindContextState ) };
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
		explicit CmdT( uint32_t binding
			, uint32_t name
			, uint32_t baseMipLevel
			, uint32_t layerCount
			, uint32_t baseArrayLayer
			, GlInternal internal )
			: binding{ binding }
			, name{ name }
			, baseMipLevel{ baseMipLevel }
			, layerCount{ layerCount }
			, baseArrayLayer{ baseArrayLayer }
			, internal{ internal }
		{
		}

		Command cmd{ makeCommand< CmdT >( OpType::eBindImage ) };
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
		explicit CmdT( uint32_t binding
			, uint32_t name )
			: binding{ binding }
			, name{ name }
		{
		}

		Command cmd{ makeCommand< CmdT >( OpType::eBindSampler ) };
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
		explicit CmdT( GlTextureType type
			, uint32_t name )
			: type{ type }
			, name{ name }
		{
		}

		Command cmd{ makeCommand< CmdT >( OpType::eBindTexture ) };
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
		explicit CmdT( GeometryBuffers const * vao )
			: vao{ vao }
		{
		}

		Command cmd{ makeCommand< CmdT >( OpType::eBindVextexArray ) };
		GeometryBuffers const * vao;
	};
	using CmdBindVextexArray = CmdT< OpType::eBindVextexArray >;

	void apply( ContextLock const & context
		, CmdBindVextexArray const & cmd );

	//*************************************************************************

	template<>
	struct alignas( uint64_t ) CmdT< OpType::eBindVextexArrayObject >
	{
		explicit CmdT( GLuint vao )
			: vao{ vao }
		{
		}

		Command cmd{ makeCommand< CmdT >( OpType::eBindVextexArrayObject ) };
		GLuint vao;
	};
	using CmdBindVextexArrayObject = CmdT< OpType::eBindVextexArrayObject >;

	void apply( ContextLock const & context
		, CmdBindVextexArrayObject const & cmd );

	//*************************************************************************

	template<>
	struct alignas( uint64_t ) CmdT< OpType::eBlendConstants >
	{
		explicit CmdT( float c0
			, float c1
			, float c2
			, float c3 )
			: blendConstant0{ c0 }
			, blendConstant1{ c1 }
			, blendConstant2{ c2 }
			, blendConstant3{ c3 }
		{
		}
		
		explicit CmdT( float const c[4] )
			: CmdT{ c[0], c[1], c[2], c[3] }
		{
		}

		Command cmd{ makeCommand< CmdT >( OpType::eBlendConstants ) };
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
		explicit CmdT( uint32_t index
			, GlBlendOp color
			, GlBlendOp alpha )
			: index{ index }
			, color{ color }
			, alpha{ alpha }
		{
		}

		Command cmd{ makeCommand< CmdT >( OpType::eBlendEquation ) };
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
		explicit CmdT( uint32_t index
			, GlBlendFactor colorSrc
			, GlBlendFactor colorDst
			, GlBlendFactor alphaSrc
			, GlBlendFactor alphaDst )
			: index{ index }
			, colorSrc{ colorSrc }
			, colorDst{ colorDst }
			, alphaSrc{ alphaSrc }
			, alphaDst{ alphaDst }
		{
		}

		Command cmd{ makeCommand< CmdT >( OpType::eBlendFunc ) };
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
		explicit CmdT( int32_t srcL
			, int32_t srcT
			, int32_t srcR
			, int32_t srcB
			, int32_t dstL
			, int32_t dstT
			, int32_t dstR
			, int32_t dstB
			, GlImageAspectFlags mask
			, GlFilter filter )
			: srcL{ srcL }
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

		Command cmd{ makeCommand< CmdT >( OpType::eBlitFramebuffer ) };
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
		explicit CmdT() = default;

		Command cmd{ makeCommand< CmdT >( OpType::eCheckFramebuffer ) };
	};
	using CmdCheckFramebuffer = CmdT< OpType::eCheckFramebuffer >;

	void apply( ContextLock const & context
		, CmdCheckFramebuffer const & cmd );

	//*************************************************************************

	template<>
	struct alignas( uint64_t ) CmdT< OpType::eCleanupFramebuffer >
	{
		explicit CmdT( GLuint * fbo )
			: fbo{ fbo }
		{
		}

		Command cmd{ makeCommand< CmdT >( OpType::eCleanupFramebuffer ) };
		GLuint * fbo;
	};
	using CmdCleanupFramebuffer = CmdT< OpType::eCleanupFramebuffer >;

	void apply( ContextLock const & context
		, CmdCleanupFramebuffer const & cmd );

	//*************************************************************************

	template<>
	struct alignas( uint64_t ) CmdT< OpType::eClearBack >
	{
		explicit CmdT( uint32_t mask )
			: mask{ mask }
		{
		}

		Command cmd{ makeCommand< CmdT >( OpType::eClearBack ) };
		uint32_t mask;
	};
	using CmdClearBack = CmdT< OpType::eClearBack >;

	void apply( ContextLock const & context
		, CmdClearBack const & cmd );

	//*************************************************************************

	template<>
	struct alignas( uint64_t ) CmdT< OpType::eClearBackColour >
	{
		explicit CmdT( VkClearColorValue color
			, uint32_t colourIndex )
			: color{ std::move( color ) }
			, colourIndex{ colourIndex }
		{
		}

		Command cmd{ makeCommand< CmdT >( OpType::eClearBackColour ) };
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
		explicit CmdT( float depth )
			: depth{ depth }
		{
		}

		Command cmd{ makeCommand< CmdT >( OpType::eClearBackDepth ) };
		float depth;
	};
	using CmdClearBackDepth = CmdT< OpType::eClearBackDepth >;

	void apply( ContextLock const & context
		, CmdClearBackDepth const & cmd );

	//*************************************************************************

	template<>
	struct alignas( uint64_t ) CmdT< OpType::eClearBackDepthStencil >
	{
		explicit CmdT( float depth
			, int32_t stencil )
			: depth{ depth }
			, stencil{ stencil }
		{
		}

		Command cmd{ makeCommand< CmdT >( OpType::eClearBackDepthStencil ) };
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
		explicit CmdT( int32_t stencil )
			: stencil{ stencil }
		{
		}

		Command cmd{ makeCommand< CmdT >( OpType::eClearBackStencil ) };
		int32_t stencil;
	};
	using CmdClearBackStencil = CmdT< OpType::eClearBackStencil >;

	void apply( ContextLock const & context
		, CmdClearBackStencil const & cmd );

	//*************************************************************************

	template<>
	struct alignas( uint64_t ) CmdT< OpType::eClearColour >
	{
		explicit CmdT( VkClearColorValue color
			, uint32_t colourIndex )
			: color{ std::move( color ) }
			, colourIndex{ colourIndex }
		{
		}

		Command cmd{ makeCommand< CmdT >( OpType::eClearColour ) };
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
		explicit CmdT( float depth )
			: depth{ depth }
		{
		}

		Command cmd{ makeCommand< CmdT >( OpType::eClearDepth ) };
		float depth;
	};
	using CmdClearDepth = CmdT< OpType::eClearDepth >;

	void apply( ContextLock const & context
		, CmdClearDepth const & cmd );

	//*************************************************************************

	template<>
	struct alignas( uint64_t ) CmdT< OpType::eClearDepthStencil >
	{
		explicit CmdT( VkClearDepthStencilValue depthStencil )
			: depthStencil{ std::move( depthStencil ) }
		{
		}

		Command cmd{ makeCommand< CmdT >( OpType::eClearDepthStencil ) };
		VkClearDepthStencilValue depthStencil;
	};
	using CmdClearDepthStencil = CmdT< OpType::eClearDepthStencil >;

	void apply( ContextLock const & context
		, CmdClearDepthStencil const & cmd );

	//*************************************************************************

	template<>
	struct alignas( uint64_t ) CmdT< OpType::eClearStencil >
	{
		explicit CmdT( int32_t stencil )
			: stencil{ stencil }
		{
		}

		Command cmd{ makeCommand< CmdT >( OpType::eClearStencil ) };
		int32_t stencil;
	};
	using CmdClearStencil = CmdT< OpType::eClearStencil >;

	void apply( ContextLock const & context
		, CmdClearStencil const & cmd );

	//*************************************************************************

	template<>
	struct alignas( uint64_t ) CmdT< OpType::eClearTexColorF >
	{
		explicit CmdT( uint32_t name
			, uint32_t mipLevel
			, GlFormat format
			, std::array< float, 4u > color )
			: name{ name }
			, mipLevel{ mipLevel }
			, format{ format }
			, color{ std::move( color ) }
		{
		}

		explicit CmdT( uint32_t name
			, uint32_t mipLevel
			, GlFormat format
			, float const ( &color )[4] )
			: name{ name }
			, mipLevel{ mipLevel }
			, format{ format }
			, color{ color[0], color[1], color[2], color[3] }
		{
		}

		Command cmd{ makeCommand< CmdT >( OpType::eClearTexColorF ) };
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
		explicit CmdT( uint32_t name
			, uint32_t mipLevel
			, GlFormat format
			, std::array< uint32_t, 4u > color )
			: name{ name }
			, mipLevel{ mipLevel }
			, format{ format }
			, color{ std::move( color ) }
		{
		}

		explicit CmdT( uint32_t name
			, uint32_t mipLevel
			, GlFormat format
			, uint32_t const ( &color )[4] )
			: name{ name }
			, mipLevel{ mipLevel }
			, format{ format }
			, color{ color[0], color[1], color[2], color[3] }
		{
		}

		Command cmd{ makeCommand< CmdT >( OpType::eClearTexColorUI ) };
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
		explicit CmdT( uint32_t name
			, uint32_t mipLevel
			, GlFormat format
			, std::array< int32_t, 4u > color )
			: name{ name }
			, mipLevel{ mipLevel }
			, format{ format }
			, color{ std::move( color ) }
		{
		}

		explicit CmdT( uint32_t name
			, uint32_t mipLevel
			, GlFormat format
			, int32_t const ( &color )[4] )
			: name{ name }
			, mipLevel{ mipLevel }
			, format{ format }
			, color{ color[0], color[1], color[2], color[3] }
		{
		}

		Command cmd{ makeCommand< CmdT >( OpType::eClearTexColorSI ) };
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
		explicit CmdT( uint32_t name
			, uint32_t mipLevel
			, uint32_t format
			, uint32_t type
			, float depth )
			: name{ name }
			, mipLevel{ mipLevel }
			, format{ format }
			, type{ type }
			, depth{ depth }
		{
		}

		Command cmd{ makeCommand< CmdT >( OpType::eClearTexDepth ) };
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
		explicit CmdT( uint32_t name
			, uint32_t mipLevel
			, uint32_t format
			, uint32_t type
			, VkClearDepthStencilValue depthStencil )
			: name{ name }
			, mipLevel{ mipLevel }
			, format{ format }
			, type{ type }
			, depthStencil{ std::move( depthStencil ) }
		{
		}

		Command cmd{ makeCommand< CmdT >( OpType::eClearTexDepthStencil ) };
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
		explicit CmdT( uint32_t name
			, uint32_t mipLevel
			, uint32_t format
			, uint32_t type
			, int32_t stencil )
			: name{ name }
			, mipLevel{ mipLevel }
			, format{ format }
			, type{ type }
			, stencil{ stencil }
		{
		}

		Command cmd{ makeCommand< CmdT >( OpType::eClearTexStencil ) };
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
		explicit CmdT( uint32_t index
			, GLboolean r
			, GLboolean g
			, GLboolean b
			, GLboolean a )
			: index{ index }
			, r{ r }
			, g{ g }
			, b{ b }
			, a{ a }
		{
		}

		Command cmd{ makeCommand< CmdT >( OpType::eColorMask ) };
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
		explicit CmdT( GlTextureType copyTarget
			, uint32_t mipLevel
			, int32_t x
			, uint32_t width
			, GlInternal format
			, int32_t imageSize
			, int32_t bufferOffset )
			: copyTarget{ copyTarget }
			, mipLevel{ mipLevel }
			, x{ x }
			, width{ width }
			, format{ format }
			, imageSize{ imageSize }
			, bufferOffset{ bufferOffset }
		{
		}

		Command cmd{ makeCommand< CmdT >( OpType::eCompressedTexSubImage1D ) };
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
		explicit CmdT( GlTextureType copyTarget
			, uint32_t mipLevel
			, int32_t x
			, int32_t y
			, uint32_t width
			, uint32_t height
			, GlInternal format
			, int32_t imageSize
			, int32_t bufferOffset )
			: copyTarget{ copyTarget }
			, mipLevel{ mipLevel }
			, x{ x }
			, y{ y }
			, width{ width }
			, height{ height }
			, format{ format }
			, imageSize{ imageSize }
			, bufferOffset{ bufferOffset }
		{
		}

		Command cmd{ makeCommand< CmdT >( OpType::eCompressedTexSubImage2D ) };
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
		explicit CmdT( GlTextureType copyTarget
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
			: copyTarget{ copyTarget }
			, mipLevel{ mipLevel }
			, x{ x }
			, y{ y }
			, z{ z }
			, width{ width }
			, height{ height }
			, depth{ depth }
			, format{ format }
			, imageSize{ imageSize }
			, bufferOffset{ bufferOffset }
		{
		}

		Command cmd{ makeCommand< CmdT >( OpType::eCompressedTexSubImage3D ) };
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
		explicit CmdT( uint32_t srcTarget
			, uint32_t dstTarget
			, VkBufferCopy copy )
			: srcTarget{ GlBufferTarget( srcTarget ) }
			, dstTarget{ GlBufferTarget( dstTarget ) }
			, copy{ std::move( copy ) }
		{
		}

		Command cmd{ makeCommand< CmdT >( OpType::eCopyBufferSubData ) };
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
		explicit CmdT( uint32_t srcName
			, GlTextureType srcTarget
			, uint32_t dstName
			, GlTextureType dstTarget
			, VkImageCopy copy )
			: srcName{ srcName }
			, srcTarget{ srcTarget }
			, dstName{ dstName }
			, dstTarget{ dstTarget }
			, copy{ std::move( copy ) }
		{
		}

		Command cmd{ makeCommand< CmdT >( OpType::eCopyImageSubData ) };
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
		explicit CmdT( GlCullMode value )
			: value{ value }
		{
		}

		Command cmd{ makeCommand< CmdT >( OpType::eCullFace ) };
		GlCullMode value;
	};
	using CmdCullFace = CmdT< OpType::eCullFace >;

	void apply( ContextLock const & context
		, CmdCullFace const & cmd );

	//*************************************************************************

	template<>
	struct alignas( uint64_t ) CmdT< OpType::eDepthFunc >
	{
		explicit CmdT( GlCompareOp value )
			: value{ value }
		{
		}

		Command cmd{ makeCommand< CmdT >( OpType::eDepthFunc ) };
		GlCompareOp value;
	};
	using CmdDepthFunc = CmdT< OpType::eDepthFunc >;

	void apply( ContextLock const & context
		, CmdDepthFunc const & cmd );

	//*************************************************************************

	template<>
	struct alignas( uint64_t ) CmdT< OpType::eDepthMask >
	{
		explicit CmdT( uint32_t value )
			: value{ value }
		{
		}

		Command cmd{ makeCommand< CmdT >( OpType::eDepthMask ) };
		uint32_t value;
	};
	using CmdDepthMask = CmdT< OpType::eDepthMask >;

	void apply( ContextLock const & context
		, CmdDepthMask const & cmd );

	//*************************************************************************

	template<>
	struct alignas( uint64_t ) CmdT< OpType::eDepthRange >
	{
		explicit CmdT( float min
			, float max )
			: min{ min }
			, max{ max }
		{
		}

		Command cmd{ makeCommand< CmdT >( OpType::eDepthRange ) };
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
		explicit CmdT( GlTweak value )
			: value{ value }
		{
		}

		Command cmd{ makeCommand< CmdT >( OpType::eDisable ) };
		GlTweak value;
	};
	using CmdDisable = CmdT< OpType::eDisable >;

	void apply( ContextLock const & context
		, CmdDisable const & cmd );

	//*************************************************************************

	template<>
	struct alignas( uint64_t ) CmdT< OpType::eDispatch >
	{
		explicit CmdT( uint32_t groupCountX
			, uint32_t groupCountY
			, uint32_t groupCountZ )
			: groupCountX{ groupCountX }
			, groupCountY{ groupCountY }
			, groupCountZ{ groupCountZ }
		{
		}

		Command cmd{ makeCommand< CmdT >( OpType::eDispatch ) };
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
		explicit CmdT( uint64_t offset )
			: offset{ offset }
		{
		}

		Command cmd{ makeCommand< CmdT >( OpType::eDispatchIndirect ) };
		uint64_t offset;
	};
	using CmdDispatchIndirect = CmdT< OpType::eDispatchIndirect >;

	void apply( ContextLock const & context
		, CmdDispatchIndirect const & cmd );

	//*************************************************************************

	template<>
	struct alignas( uint64_t ) CmdT< OpType::eDownloadMemory >
	{
		explicit CmdT( VkDeviceMemory memory
			, VkDeviceSize offset
			, VkDeviceSize size )
			: memory{ memory }
			, offset{ offset }
			, size{ size }
		{
		}

		Command cmd{ makeCommand< CmdT >( OpType::eDownloadMemory ) };
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
		explicit CmdT( uint32_t vtxCount
			, uint32_t instCount
			, uint32_t firstVertex
			, GlPrimitiveTopology mode )
			: vtxCount{ vtxCount }
			, instCount{ instCount }
			, firstVertex{ firstVertex }
			, mode{ mode }
		{
		}

		Command cmd{ makeCommand< CmdT >( OpType::eDraw ) };
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
		explicit CmdT( uint32_t vtxCount
			, uint32_t instCount
			, uint32_t firstVertex
			, uint32_t firstInstance
			, GlPrimitiveTopology mode )
			: vtxCount{ vtxCount }
			, instCount{ instCount }
			, firstVertex{ firstVertex }
			, firstInstance{ firstInstance }
			, mode{ mode }
		{
		}

		Command cmd{ makeCommand< CmdT >( OpType::eDrawBaseInstance ) };
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
		explicit CmdT( GlAttachmentPoint value )
			: target{ value }
		{
		}

		Command cmd{ makeCommand< CmdT >( OpType::eDrawBuffer ) };
		GlAttachmentPoint target;
	};
	using CmdDrawBuffer = CmdT< OpType::eDrawBuffer >;

	void apply( ContextLock const & context
		, CmdDrawBuffer const & cmd );

	//*************************************************************************

	template<>
	struct alignas( uint64_t ) CmdT< OpType::eDrawBuffers >
	{
		explicit CmdT( GlAttachmentPoint target )
			: count{ 1u }
		{
			assert( target != GL_ATTACHMENT_POINT_DEPTH_STENCIL
				&& target != GL_ATTACHMENT_POINT_DEPTH
				&& target != GL_ATTACHMENT_POINT_STENCIL );
			targets[0] = target;
		}

		template< size_t N >
		explicit CmdT( GlAttachmentPoint targets[N] )
			: count{ uint32_t( N ) }
		{
			static_assert( N <= 16u, "No more that 16 draw buffers" );
			uint32_t i = 0u;

			for ( auto & target : makeArrayView( targets, targets + N ) )
			{
				assert( target != GL_ATTACHMENT_POINT_DEPTH_STENCIL
					&& target != GL_ATTACHMENT_POINT_DEPTH
					&& target != GL_ATTACHMENT_POINT_STENCIL );
				this->targets[i] = target;
				++i;
			}
		}

		template< size_t N >
		explicit CmdT( std::array< GlAttachmentPoint, N > const & targets )
			: count{ uint32_t( N ) }
		{
			static_assert( N <= 16u, "No more that 16 draw buffers" );
			uint32_t i = 0u;

			for ( auto & target : targets )
			{
				assert( target != GL_ATTACHMENT_POINT_DEPTH_STENCIL
					&& target != GL_ATTACHMENT_POINT_DEPTH
					&& target != GL_ATTACHMENT_POINT_STENCIL );
				this->targets[i] = target;
				++i;
			}
		}

		explicit CmdT( std::vector< GlAttachmentPoint > const & targets )
			: count{ std::min( 16u, uint32_t( targets.size() ) ) }
		{
			uint32_t i = 0u;

			for ( auto & target : targets )
			{
				if ( i < this->targets.size() )
				{
					assert( target != GL_ATTACHMENT_POINT_DEPTH_STENCIL
						&& target != GL_ATTACHMENT_POINT_DEPTH
						&& target != GL_ATTACHMENT_POINT_STENCIL );
					this->targets[i] = target;
					++i;
				}
			}
		}

		Command cmd{ makeCommand< CmdT >( OpType::eDrawBuffers ) };
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
		explicit CmdT( uint32_t indexCount
			, uint32_t instCount
			, uint32_t indexOffset
			, uint32_t vertexOffset
			, GlPrimitiveTopology mode
			, GlIndexType type )
			: indexCount{ indexCount }
			, instCount{ instCount }
			, indexOffset{ indexOffset }
			, vertexOffset{ vertexOffset }
			, mode{ mode }
			, type{ type }
		{
		}

		Command cmd{ makeCommand< CmdT >( OpType::eDrawIndexed ) };
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
		explicit CmdT( uint32_t indexCount
			, uint32_t instCount
			, uint32_t indexOffset
			, uint32_t vertexOffset
			, uint32_t firstInstance
			, GlPrimitiveTopology mode
			, GlIndexType type )
			: indexCount{ indexCount }
			, instCount{ instCount }
			, indexOffset{ indexOffset }
			, vertexOffset{ vertexOffset }
			, firstInstance{ firstInstance }
			, mode{ mode }
			, type{ type }
		{
		}

		Command cmd{ makeCommand< CmdT >( OpType::eDrawIndexedBaseInstance ) };
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
		explicit CmdT( uint64_t offset
			, uint32_t drawCount
			, uint32_t stride
			, GlPrimitiveTopology mode
			, GlIndexType type )
			: offset{ offset }
			, drawCount{ drawCount }
			, stride{ stride }
			, mode{ mode }
			, type{ type }
		{
		}

		Command cmd{ makeCommand< CmdT >( OpType::eDrawIndexedIndirect ) };
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
		explicit CmdT( uint64_t offset
			, uint32_t drawCount
			, uint32_t stride
			, GlPrimitiveTopology mode )
			: offset{ offset }
			, drawCount{ drawCount }
			, stride{ stride }
			, mode{ mode }
		{
		}

		Command cmd{ makeCommand< CmdT >( OpType::eDrawIndirect ) };
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
		explicit CmdT( GlTweak value )
			: value{ value }
		{
		}

		Command cmd{ makeCommand< CmdT >( OpType::eEnable ) };
		GlTweak value;
	};
	using CmdEnable = CmdT< OpType::eEnable >;

	void apply( ContextLock const & context
		, CmdEnable const & cmd );

	//*************************************************************************

	template<>
	struct alignas( uint64_t ) CmdT< OpType::eEndQuery >
	{
		explicit CmdT( uint32_t target )
			: target{ target }
		{
		}

		Command cmd{ makeCommand< CmdT >( OpType::eEndQuery ) };
		uint32_t target;
	};
	using CmdEndQuery = CmdT< OpType::eEndQuery >;

	void apply( ContextLock const & context
		, CmdEndQuery const & cmd );

	//*************************************************************************

	template<>
	struct alignas( uint64_t ) CmdT< OpType::eFillBuffer >
	{
		explicit CmdT( VkDeviceMemory memory
			, VkDeviceSize memoryOffset
			, VkDeviceSize dataSize
			, uint32_t data )
			: memory{ memory }
			, memoryOffset{ memoryOffset }
			, dataSize{ dataSize }
			, data{ data }
		{
		}

		Command cmd{ makeCommand< CmdT >( OpType::eFillBuffer ) };
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
		explicit CmdT( GlFrameBufferTarget target
			, GlAttachmentPoint point
			, uint32_t object
			, uint32_t mipLevel )
			: target{ target }
			, point{ point }
			, object{ object }
			, mipLevel{ mipLevel }
		{
		}

		Command cmd{ makeCommand< CmdT >( OpType::eFramebufferTexture ) };
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
		explicit CmdT( GlFrameBufferTarget target
			, GlAttachmentPoint point
			, GlTextureType texTarget
			, uint32_t object
			, uint32_t mipLevel )
			: target{ target }
			, point{ point }
			, texTarget{ texTarget }
			, object{ object }
			, mipLevel{ mipLevel }
		{
		}

		Command cmd{ makeCommand< CmdT >( OpType::eFramebufferTexture1D ) };
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
		explicit CmdT( GlFrameBufferTarget target
			, GlAttachmentPoint point
			, GlTextureType texTarget
			, uint32_t object
			, uint32_t mipLevel )
			: target{ target }
			, point{ point }
			, texTarget{ texTarget }
			, object{ object }
			, mipLevel{ mipLevel }
		{
		}

		Command cmd{ makeCommand< CmdT >( OpType::eFramebufferTexture2D ) };
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
		explicit CmdT( GlFrameBufferTarget target
			, GlAttachmentPoint point
			, GlTextureType texTarget
			, uint32_t object
			, uint32_t mipLevel
			, uint32_t slice )
			: target{ target }
			, point{ point }
			, texTarget{ texTarget }
			, object{ object }
			, mipLevel{ mipLevel }
			, slice{ slice }
		{
		}

		Command cmd{ makeCommand< CmdT >( OpType::eFramebufferTexture3D ) };
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
		explicit CmdT( GlFrameBufferTarget target
			, GlAttachmentPoint point
			, uint32_t object
			, uint32_t mipLevel
			, uint32_t arrayLayer )
			: target{ target }
			, point{ point }
			, object{ object }
			, mipLevel{ mipLevel }
			, arrayLayer{ arrayLayer }
		{
		}

		Command cmd{ makeCommand< CmdT >( OpType::eFramebufferTextureLayer ) };
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
		explicit CmdT( GlFrontFace value )
			: value{ value }
		{
		}

		Command cmd{ makeCommand< CmdT >( OpType::eFrontFace ) };
		GlFrontFace value;
	};
	using CmdFrontFace = CmdT< OpType::eFrontFace >;

	void apply( ContextLock const & context
		, CmdFrontFace const & cmd );

	//*************************************************************************

	template<>
	struct alignas( uint64_t ) CmdT< OpType::eGenerateMipmaps >
	{
		explicit CmdT( uint32_t target )
			: target{ target }
		{
		}

		Command cmd{ makeCommand< CmdT >( OpType::eGenerateMipmaps ) };
		uint32_t target;
	};
	using CmdGenerateMipmaps = CmdT< OpType::eGenerateMipmaps >;

	void apply( ContextLock const & context
		, CmdGenerateMipmaps const & cmd );

	//*************************************************************************

	template<>
	struct alignas( uint64_t ) CmdT< OpType::eGetCompressedTexImage >
	{
		explicit CmdT( GlTextureType target
			, GLint level = 0
			, intptr_t offset = 0 )
			: target{ target }
			, level{ level }
			, offset{ offset }
		{
		}

		Command cmd{ makeCommand< CmdT >( OpType::eGetCompressedTexImage ) };
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
		explicit CmdT( VkQueryPool queryPool
			, uint32_t firstQuery
			, uint32_t queryCount
			, VkDeviceSize stride
			, VkQueryResultFlags flags
			, VkDeviceSize bufferOffset )
			: queryPool{ queryPool }
			, firstQuery{ firstQuery }
			, queryCount{ queryCount }
			, stride{ stride }
			, flags{ flags }
			, bufferOffset{ bufferOffset }
		{
		}

		Command cmd{ makeCommand< CmdT >( OpType::eGetQueryResults ) };
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
		explicit CmdT( GlTextureType target
			, GlFormat format
			, GlType type
			, GLint level = 0
			, intptr_t offset = 0 )
			: target{ target }
			, format{ format }
			, type{ type }
			, level{ level }
			, offset{ offset }
		{
		}

		Command cmd{ makeCommand< CmdT >( OpType::eGetTexImage ) };
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
		explicit CmdT( float value )
			: value{ value }
		{
		}

		Command cmd{ makeCommand< CmdT >( OpType::eLineWidth ) };
		float value;
	};
	using CmdLineWidth = CmdT< OpType::eLineWidth >;

	void apply( ContextLock const & context
		, CmdLineWidth const & cmd );

	//*************************************************************************

	template<>
	struct alignas( uint64_t ) CmdT< OpType::eLogCommand >
	{
		explicit CmdT( char const * const value )
		{
			strncpy( this->value, value, 63 );
		}

		Command cmd{ makeCommand< CmdT >( OpType::eLogCommand ) };
		char value[64];
	};
	using CmdLogCommand = CmdT< OpType::eLogCommand >;

	void apply( ContextLock const & context
		, CmdLogCommand const & cmd );

	//*************************************************************************

	template<>
	struct alignas( uint64_t ) CmdT< OpType::eLogicOp >
	{
		explicit CmdT( GlLogicOp value )
			: value{ value }
		{
		}

		Command cmd{ makeCommand< CmdT >( OpType::eLogicOp ) };
		GlLogicOp value;
	};
	using CmdLogicOp = CmdT< OpType::eLogicOp >;

	void apply( ContextLock const & context
		, CmdLogicOp const & cmd );

	//*************************************************************************

	template<>
	struct alignas( uint64_t ) CmdT< OpType::eMemoryBarrier >
	{
		explicit CmdT( GlMemoryBarrierFlags flags )
			: flags{ flags }
		{
		}

		Command cmd{ makeCommand< CmdT >( OpType::eMemoryBarrier ) };
		GlMemoryBarrierFlags flags;
	};
	using CmdMemoryBarrier = CmdT< OpType::eMemoryBarrier >;

	void apply( ContextLock const & context
		, CmdMemoryBarrier const & cmd );

	//*************************************************************************

	template<>
	struct alignas( uint64_t ) CmdT< OpType::eMinSampleShading >
	{
		explicit CmdT( float value )
			: value{ value }
		{
		}

		Command cmd{ makeCommand< CmdT >( OpType::eMinSampleShading ) };
		float value;
	};
	using CmdMinSampleShading = CmdT< OpType::eMinSampleShading >;

	void apply( ContextLock const & context
		, CmdMinSampleShading const & cmd );

	//*************************************************************************

	template<>
	struct alignas( uint64_t ) CmdT< OpType::ePatchParameter >
	{
		explicit CmdT( PatchParameter param
			, int value )
			: param{ param }
			, value{ value }
		{
		}

		Command cmd{ makeCommand< CmdT >( OpType::ePatchParameter ) };
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
		explicit CmdT( GlPackAlignment name
			, int32_t param )
			: name{ name }
			, param{ param }
		{
		}

		Command cmd{ makeCommand< CmdT >( OpType::ePixelStore ) };
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
		explicit CmdT( GlPolygonMode value )
			: value{ value }
		{
		}

		Command cmd{ makeCommand< CmdT >( OpType::ePolygonMode ) };
		GlPolygonMode value;
	};
	using CmdPolygonMode = CmdT< OpType::ePolygonMode >;

	void apply( ContextLock const & context
		, CmdPolygonMode const & cmd );

	//*************************************************************************

	template<>
	struct alignas( uint64_t ) CmdT< OpType::ePolygonOffset >
	{
		explicit CmdT( float constantFactor
			, float clamp
			, float slopeFactor )
			: constantFactor{ constantFactor }
			, clamp{ clamp }
			, slopeFactor{ slopeFactor }
		{
		}

		Command cmd{ makeCommand< CmdT >( OpType::ePolygonOffset ) };
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
		explicit CmdT() = default;

		Command cmd{ makeCommand< CmdT >( OpType::ePopDebugGroup ) };
	};
	using CmdPopDebugGroup = CmdT< OpType::ePopDebugGroup >;

	void apply( ContextLock const & context
		, CmdPopDebugGroup const & cmd );

	//*************************************************************************

	template<>
	struct alignas( uint64_t ) CmdT< OpType::ePrimitiveRestartIndex >
	{
		explicit CmdT( uint32_t index )
			: index{ index }
		{
		}

		Command cmd{ makeCommand< CmdT >( OpType::ePrimitiveRestartIndex ) };
		uint32_t index;
	};
	using CmdPrimitiveRestartIndex = CmdT< OpType::ePrimitiveRestartIndex >;

	void apply( ContextLock const & context
		, CmdPrimitiveRestartIndex const & cmd );

	//*************************************************************************

	template<>
	struct alignas( uint64_t ) CmdT< OpType::eProgramUniform1fv >
	{
		explicit CmdT( uint32_t program
			, uint32_t location
			, float const * buffer )
			: program{ program }
			, location{ location }
			, buffer{ *buffer }
		{
		}

		Command cmd{ makeCommand< CmdT >( OpType::eProgramUniform1fv ) };
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
		explicit CmdT( uint32_t program
			, uint32_t location
			, float const * buffer )
			: program{ program }
			, location{ location }
			, buffer{ buffer[0], buffer[1] }
		{
		}

		Command cmd{ makeCommand< CmdT >( OpType::eProgramUniform2fv ) };
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
		explicit CmdT( uint32_t program
			, uint32_t location
			, float const * buffer )
			: program{ program }
			, location{ location }
			, buffer{ buffer[0], buffer[1], buffer[2] }
		{
		}

		Command cmd{ makeCommand< CmdT >( OpType::eProgramUniform3fv ) };
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
		explicit CmdT( uint32_t program
			, uint32_t location
			, float const * buffer )
			: program{ program }
			, location{ location }
			, buffer{ buffer[0], buffer[1], buffer[2], buffer[3] }
		{
		}

		Command cmd{ makeCommand< CmdT >( OpType::eProgramUniform4fv ) };
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
		explicit CmdT( uint32_t program
			, uint32_t location
			, int32_t const * buffer )
			: program{ program }
			, location{ location }
			, buffer{ *buffer }
		{
		}

		Command cmd{ makeCommand< CmdT >( OpType::eProgramUniform1iv ) };
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
		explicit CmdT( uint32_t program
			, uint32_t location
			, int32_t const * buffer )
			: program{ program }
			, location{ location }
			, buffer{ buffer[0], buffer[1] }
		{
		}

		Command cmd{ makeCommand< CmdT >( OpType::eProgramUniform2iv ) };
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
		explicit CmdT( uint32_t program
			, uint32_t location
			, int32_t const * buffer )
			: program{ program }
			, location{ location }
			, buffer{ buffer[0], buffer[1], buffer[2] }
		{
		}

		Command cmd{ makeCommand< CmdT >( OpType::eProgramUniform3iv ) };
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
		explicit CmdT( uint32_t program
			, uint32_t location
			, int32_t const * buffer )
			: program{ program }
			, location{ location }
			, buffer{ buffer[0], buffer[1], buffer[2], buffer[3] }
		{
		}

		Command cmd{ makeCommand< CmdT >( OpType::eProgramUniform4iv ) };
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
		explicit CmdT( uint32_t program
			, uint32_t location
			, uint32_t const * buffer )
			: program{ program }
			, location{ location }
			, buffer{ *buffer }
		{
		}

		Command cmd{ makeCommand< CmdT >( OpType::eProgramUniform1uiv ) };
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
		explicit CmdT( uint32_t program
			, uint32_t location
			, uint32_t const * buffer )
			: program{ program }
			, location{ location }
			, buffer{ buffer[0], buffer[1] }
		{
		}

		Command cmd{ makeCommand< CmdT >( OpType::eProgramUniform2uiv ) };
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
		explicit CmdT( uint32_t program
			, uint32_t location
			, uint32_t const * buffer )
			: program{ program }
			, location{ location }
			, buffer{ buffer[0], buffer[1], buffer[2] }
		{
		}

		Command cmd{ makeCommand< CmdT >( OpType::eProgramUniform3uiv ) };
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
		explicit CmdT( uint32_t program
			, uint32_t location
			, uint32_t const * buffer )
			: program{ program }
			, location{ location }
			, buffer{ buffer[0], buffer[1], buffer[2], buffer[3] }
		{
		}

		Command cmd{ makeCommand< CmdT >( OpType::eProgramUniform4uiv ) };
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
		explicit CmdT( uint32_t program
			, uint32_t location
			, uint32_t transpose
			, float const * buffer )
			: program{ program }
			, location{ location }
			, transpose{ transpose }
			, buffer
		{
			buffer[0], buffer[1],
			buffer[2], buffer[3]
		}
		{
		}

		Command cmd{ makeCommand< CmdT >( OpType::eProgramUniformMatrix2fv ) };
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
		explicit CmdT( uint32_t program
			, uint32_t location
			, uint32_t transpose
			, float const * buffer )
			: program{ program }
			, location{ location }
			, transpose{ transpose }
			, buffer
		{
			buffer[0], buffer[1], buffer[2],
			buffer[3], buffer[4], buffer[5],
			buffer[6], buffer[7], buffer[8],
		}
		{
		}

		Command cmd{ makeCommand< CmdT >( OpType::eProgramUniformMatrix3fv ) };
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
		explicit CmdT( uint32_t program
			, uint32_t location
			, uint32_t transpose
			, float const * buffer )
			: program{ program }
			, location{ location }
			, transpose{ transpose }
			, buffer
		{
			buffer[0], buffer[1], buffer[2], buffer[3],
			buffer[4], buffer[5], buffer[6], buffer[7],
			buffer[8], buffer[9], buffer[10], buffer[11],
			buffer[12], buffer[13], buffer[14], buffer[15],
		}
		{
		}

		Command cmd{ makeCommand< CmdT >( OpType::eProgramUniformMatrix4fv ) };
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
		explicit CmdT( GlDebugSource source
			, GLuint id
			, GLsizei length
			, const char * message )
			: source{ source }
			, id{ id }
			, length{ length }
		{
			this->length = GLsizei( std::min( strnlen( message, size_t( length ) ), size_t( 255u ) ) );
			strncpy( this->message
				, message
				, size_t( this->length ) );
			this->message[this->length] = 0;
		}

		Command cmd{ makeCommand< CmdT >( OpType::ePushDebugGroup ) };
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
		explicit CmdT( GlAttachmentPoint point )
			: point{ point }
		{
		}

		Command cmd{ makeCommand< CmdT >( OpType::eReadBuffer ) };
		GlAttachmentPoint point;
	};
	using CmdReadBuffer = CmdT< OpType::eReadBuffer >;

	void apply( ContextLock const & context
		, CmdReadBuffer const & cmd );

	//*************************************************************************

	template<>
	struct alignas( uint64_t ) CmdT< OpType::eReadPixels >
	{
		explicit CmdT( int32_t x
			, int32_t y
			, uint32_t width
			, uint32_t height
			, GlFormat format
			, GlType type )
			: x{ x }
			, y{ y }
			, width{ width }
			, height{ height }
			, format{ format }
			, type{ type }
		{
		}

		Command cmd{ makeCommand< CmdT >( OpType::eReadPixels ) };
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
		explicit CmdT( VkEvent event )
			: event{ event }
		{
		}

		Command cmd{ makeCommand< CmdT >( OpType::eResetEvent ) };
		VkEvent event;
	};
	using CmdResetEvent = CmdT< OpType::eResetEvent >;

	void apply( ContextLock const & context
		, CmdResetEvent const & cmd );

	//*************************************************************************

	template<>
	struct alignas( uint64_t ) CmdT< OpType::eSetEvent >
	{
		explicit CmdT( VkEvent event )
			: event{ event }
		{
		}

		Command cmd{ makeCommand< CmdT >( OpType::eSetEvent ) };
		VkEvent event;
	};
	using CmdSetEvent = CmdT< OpType::eSetEvent >;

	void apply( ContextLock const & context
		, CmdSetEvent const & cmd );

	//*************************************************************************

	template<>
	struct alignas( uint64_t ) CmdT< OpType::eSetLineWidth >
	{
		explicit CmdT( float width )
			: width{ width }
		{
		}

		Command cmd{ makeCommand< CmdT >( OpType::eSetLineWidth ) };
		float width;
	};
	using CmdSetLineWidth = CmdT< OpType::eSetLineWidth >;

	void apply( ContextLock const & context
		, CmdSetLineWidth const & cmd );

	//*************************************************************************

	template<>
	struct alignas( uint64_t ) CmdT< OpType::eStencilFunc >
	{
		explicit CmdT( GlCullMode face
			, GlCompareOp op
			, uint32_t ref
			, uint32_t compMask )
			: face{ face }
			, op{ op }
			, ref{ ref }
			, compMask{ compMask }
		{
		}

		Command cmd{ makeCommand< CmdT >( OpType::eStencilFunc ) };
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
		explicit CmdT( GlCullMode face
			, uint32_t mask )
			: face{ face }
			, mask{ mask }
		{
		}

		Command cmd{ makeCommand< CmdT >( OpType::eStencilMask ) };
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
		explicit CmdT( GlCullMode face
			, GlStencilOp fail
			, GlStencilOp depthFail
			, GlStencilOp pass )
			: face{ face }
			, fail{ fail }
			, depthFail{ depthFail }
			, pass{ pass }
		{
		}

		Command cmd{ makeCommand< CmdT >( OpType::eStencilOp ) };
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
		explicit CmdT( GlTextureType target
			, uint32_t name
			, int32_t param )
			: target{ target }
			, name{ name }
			, param{ param }
		{
		}

		Command cmd{ makeCommand< CmdT >( OpType::eTexParameteri ) };
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
		explicit CmdT( GlTextureType target
			, uint32_t name
			, float param )
			: target{ target }
			, name{ name }
			, param{ param }
		{
		}

		Command cmd{ makeCommand< CmdT >( OpType::eTexParameterf ) };
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
		explicit CmdT( GlTextureType copyTarget
			, uint32_t mipLevel
			, int32_t x
			, uint32_t width
			, GlFormat format
			, GlType type
			, int32_t bufferOffset )
			: copyTarget{ copyTarget }
			, mipLevel{ mipLevel }
			, x{ x }
			, width{ width }
			, format{ format }
			, type{ type }
			, bufferOffset{ bufferOffset }
		{
		}

		Command cmd{ makeCommand< CmdT >( OpType::eTexSubImage1D ) };
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
		explicit CmdT( GlTextureType copyTarget
			, uint32_t mipLevel
			, int32_t x
			, int32_t y
			, uint32_t width
			, uint32_t height
			, GlFormat format
			, GlType type
			, int32_t bufferOffset )
			: copyTarget{ copyTarget }
			, mipLevel{ mipLevel }
			, x{ x }
			, y{ y }
			, width{ width }
			, height{ height }
			, format{ format }
			, type{ type }
			, bufferOffset{ bufferOffset }
		{
		}

		Command cmd{ makeCommand< CmdT >( OpType::eTexSubImage2D ) };
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
		explicit CmdT( GlTextureType copyTarget
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
			: copyTarget{ copyTarget }
			, mipLevel{ mipLevel }
			, x{ x }
			, y{ y }
			, z{ z }
			, width{ width }
			, height{ height }
			, depth{ depth }
			, format{ format }
			, type{ type }
			, bufferOffset{ bufferOffset }
		{
		}

		Command cmd{ makeCommand< CmdT >( OpType::eTexSubImage3D ) };
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
		explicit CmdT( uint32_t location
			, float const * buffer )
			: location{ location }
			, buffer{ *buffer }
		{
		}

		Command cmd{ makeCommand< CmdT >( OpType::eUniform1fv ) };
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
		explicit CmdT( uint32_t location
			, float const * buffer )
			: location{ location }
			, buffer{ buffer[0], buffer[1] }
		{
		}

		Command cmd{ makeCommand< CmdT >( OpType::eUniform2fv ) };
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
		explicit CmdT( uint32_t location
			, float const * buffer )
			: location{ location }
			, buffer{ buffer[0], buffer[1], buffer[2] }
		{
		}

		Command cmd{ makeCommand< CmdT >( OpType::eUniform3fv ) };
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
		explicit CmdT( uint32_t location
			, float const * buffer )
			: location{ location }
			, buffer{ buffer[0], buffer[1], buffer[2], buffer[3] }
		{
		}

		Command cmd{ makeCommand< CmdT >( OpType::eUniform4fv ) };
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
		explicit CmdT( uint32_t location
			, int32_t const * buffer )
			: location{ location }
			, buffer{ *buffer }
		{
		}

		Command cmd{ makeCommand< CmdT >( OpType::eUniform1iv ) };
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
		explicit CmdT( uint32_t location
			, int32_t const * buffer )
			: location{ location }
			, buffer{ buffer[0], buffer[1] }
		{
		}

		Command cmd{ makeCommand< CmdT >( OpType::eUniform2iv ) };
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
		explicit CmdT( uint32_t location
			, int32_t const * buffer )
			: location{ location }
			, buffer{ buffer[0], buffer[1], buffer[2] }
		{
		}

		Command cmd{ makeCommand< CmdT >( OpType::eUniform3iv ) };
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
		explicit CmdT( uint32_t location
			, int32_t const * buffer )
			: location{ location }
			, buffer{ buffer[0], buffer[1], buffer[2], buffer[3] }
		{
		}

		Command cmd{ makeCommand< CmdT >( OpType::eUniform4iv ) };
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
		explicit CmdT( uint32_t location
			, uint32_t const * buffer )
			: location{ location }
			, buffer{ *buffer }
		{
		}

		Command cmd{ makeCommand< CmdT >( OpType::eUniform1uiv ) };
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
		explicit CmdT( uint32_t location
			, uint32_t const * buffer )
			: location{ location }
			, buffer{ buffer[0], buffer[1] }
		{
		}

		Command cmd{ makeCommand< CmdT >( OpType::eUniform2uiv ) };
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
		explicit CmdT( uint32_t location
			, uint32_t const * buffer )
			: location{ location }
			, buffer{ buffer[0], buffer[1], buffer[2] }
		{
		}

		Command cmd{ makeCommand< CmdT >( OpType::eUniform3uiv ) };
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
		explicit CmdT( uint32_t location
			, uint32_t const * buffer )
			: location{ location }
			, buffer{ buffer[0], buffer[1], buffer[2], buffer[3] }
		{
		}

		Command cmd{ makeCommand< CmdT >( OpType::eUniform4uiv ) };
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
		explicit CmdT( uint32_t location
			, uint32_t transpose
			, float const * buffer )
			: location{ location }
			, transpose{ transpose }
			, buffer
		{
			buffer[0], buffer[1],
			buffer[2], buffer[3]
		}
		{
		}

		Command cmd{ makeCommand< CmdT >( OpType::eUniformMatrix2fv ) };
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
		explicit CmdT( uint32_t location
			, uint32_t transpose
			, float const * buffer )
			: location{ location }
			, transpose{ transpose }
			, buffer
		{
			buffer[0], buffer[1], buffer[2],
			buffer[3], buffer[4], buffer[5],
			buffer[6], buffer[7], buffer[8],
		}
		{
		}

		Command cmd{ makeCommand< CmdT >( OpType::eUniformMatrix3fv ) };
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
		explicit CmdT( uint32_t location
			, uint32_t transpose
			, float const * buffer )
			: location{ location }
			, transpose{ transpose }
			, buffer
		{
			buffer[0], buffer[1], buffer[2], buffer[3],
			buffer[4], buffer[5], buffer[6], buffer[7],
			buffer[8], buffer[9], buffer[10], buffer[11],
			buffer[12], buffer[13], buffer[14], buffer[15],
		}
		{
		}

		Command cmd{ makeCommand< CmdT >( OpType::eUniformMatrix4fv ) };
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
		explicit CmdT( VkDeviceMemory memory
			, VkDeviceSize memoryOffset
			, VkDeviceSize dataSize
			, void * pData )
			: memory{ memory }
			, memoryOffset{ memoryOffset }
			, dataSize{ dataSize }
			, pData{ pData }
		{
		}

		Command cmd{ makeCommand< CmdT >( OpType::eUpdateBuffer ) };
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
		explicit CmdT( VkDeviceMemory memory
			, VkDeviceSize offset
			, VkDeviceSize size )
			: memory{ memory }
			, offset{ offset }
			, size{ size }
		{
		}

		Command cmd{ makeCommand< CmdT >( OpType::eUploadMemory ) };
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
		explicit CmdT( uint32_t value )
			: program{ value }
		{
		}

		Command cmd{ makeCommand< CmdT >( OpType::eUseProgram ) };
		uint32_t program;
	};
	using CmdUseProgram = CmdT< OpType::eUseProgram >;

	void apply( ContextLock const & context
		, CmdUseProgram const & cmd );

	//*************************************************************************

	template<>
	struct alignas( uint64_t ) CmdT< OpType::eUseProgramPipeline >
	{
		explicit CmdT( uint32_t value )
			: program{ value }
		{
		}

		Command cmd{ makeCommand< CmdT >( OpType::eUseProgramPipeline ) };
		uint32_t program;
	};
	using CmdUseProgramPipeline = CmdT< OpType::eUseProgramPipeline >;

	void apply( ContextLock const & context
		, CmdUseProgramPipeline const & cmd );

	//*************************************************************************

	template<>
	struct alignas( uint64_t ) CmdT< OpType::eWaitEvents >
	{
		explicit CmdT( VkEventArray events )
			: events{ std::move( events ) }
		{
		}

		Command cmd{ makeCommand< CmdT >( OpType::eWaitEvents ) };
		VkEventArray events;
	};
	using CmdWaitEvents = CmdT< OpType::eWaitEvents >;

	void apply( ContextLock const & context
		, CmdWaitEvents const & cmd );

	//*************************************************************************

	template<>
	struct alignas( uint64_t ) CmdT< OpType::eWriteTimestamp >
	{
		explicit CmdT( uint32_t name )
			: name{ name }
		{
		}

		Command cmd{ makeCommand< CmdT >( OpType::eWriteTimestamp ) };
		uint32_t name;
	};
	using CmdWriteTimestamp = CmdT< OpType::eWriteTimestamp >;

	void apply( ContextLock const & context
		, CmdWriteTimestamp const & cmd );

	//*************************************************************************

	template< OpType OpT, typename ... ParamsT >
	CmdBuffer makeCmd( ParamsT && ... params )
	{
		CmdT< OpT > cmd{ std::forward< ParamsT >( params )... };
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
			Command command{ makeCommand< CmdT< OpT > >( OpT ) };
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
