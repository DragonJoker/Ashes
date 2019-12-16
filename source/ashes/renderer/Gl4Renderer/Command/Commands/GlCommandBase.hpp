/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include "renderer/Gl4Renderer/GlRendererPrerequisites.hpp"

#include <ashes/common/ArrayView.hpp>

#include <array>

namespace ashes::gl4
{
	//*************************************************************************

	enum class OpType
		: uint16_t
	{
		eActiveTexture,
		eApplyDepthRanges,
		eApplyScissors,
		eApplyViewports,
		eBeginQuery,
		eBindBuffer,
		eBindBufferRange,
		eBindContextState,
		eBindFramebuffer,
		eBindImage,
		eBindSampler,
		eBindTexture,
		eBindVextexArray,
		eBindVextexArrayObject,
		eBlendConstants,
		eBlendEquation,
		eBlendFunc,
		eBlitFramebuffer,
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
		eClearTexColor,
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
		eDrawBuffer,
		eDrawBuffers,
		eDrawIndexed,
		eDrawIndexedIndirect,
		eDrawIndirect,
		eEnable,
		eEndQuery,
		eFillBuffer,
		eFramebufferTexture,
		eFramebufferTexture2D,
		eFramebufferTextureLayer,
		eFrontFace,
		eGenerateMipmaps,
		eGetQueryResults,
		eGetTexImage,
		eInitFramebuffer,
		eLineWidth,
		eLogicOp,
		eMemoryBarrier,
		eMinSampleShading,
		ePatchParameter,
		ePixelStore,
		ePolygonMode,
		ePolygonOffset,
		ePrimitiveRestartIndex,
		eReadBuffer,
		eReadPixels,
		eResetEvent,
		eSetDepthBias,
		eSetLineWidth,
		eSetEvent,
		eStencilFunc,
		eStencilMask,
		eStencilOp,
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

	template< OpType OpT >
	struct CmdConfig;

	template< OpType OpT >
	static Op constexpr CmdConfigT = CmdConfig< OpT >::value;

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
	struct CmdConfig< OpType::eEnable >
	{
		static Op constexpr value = { OpType::eEnable, 2u };
	};

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
	struct CmdConfig< OpType::eDisable >
	{
		static Op constexpr value = { OpType::eDisable, 2u };
	};

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
	struct CmdConfig< OpType::eApplyScissors >
	{
		static Op constexpr value = { OpType::eApplyScissors, 5u };
	};

	template<>
	struct alignas( uint64_t ) CmdT< OpType::eApplyScissors >
	{
		static uint32_t constexpr MaxElems = 16u;
		static uint32_t constexpr ElemCount = 4u;

		inline CmdT( uint32_t first
			, uint32_t count
			, std::vector< VkRect2D > const & scissors )
			: cmd{ { OpType::eApplyScissors, sizeof( CmdT ) / sizeof( uint32_t ) } }
			, first{ first }
			, count{ std::min( MaxElems, count ) }
		{
			auto it = this->scissors.begin();
			auto end = scissors.begin() + std::min( MaxElems, uint32_t( scissors.size() ) );

			for ( auto scit = scissors.begin(); scit != end; ++scit )
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

		inline CmdT( std::vector< VkRect2D > const & scissors )
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
	struct CmdConfig< OpType::eApplyViewports >
	{
		static Op constexpr value = { OpType::eApplyViewports, 7u };
	};

	template<>
	struct alignas( uint64_t ) CmdT< OpType::eApplyViewports >
	{
		static uint32_t constexpr MaxElems = 16u;
		static uint32_t constexpr ElemCount = 4u;

		inline CmdT( uint32_t first
			, uint32_t count
			, std::vector< VkViewport > const & viewports )
			: cmd{ { OpType::eApplyViewports, sizeof( CmdT ) / sizeof( uint32_t ) } }
			, first{ first }
			, count{ std::min( MaxElems, count ) }
		{
			auto it = this->viewports.begin();
			auto end = viewports.begin() + std::min( MaxElems, uint32_t( viewports.size() ) );

			for ( auto vit = viewports.begin(); vit != end; ++vit )
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

		inline CmdT( std::vector< VkViewport > const & viewports )
			: CmdT{ 0u, uint32_t( viewports.size() ), viewports }
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
	struct CmdConfig< OpType::eApplyDepthRanges >
	{
		static Op constexpr value = { OpType::eApplyDepthRanges, 7u };
	};

	template<>
	struct alignas( uint64_t ) CmdT< OpType::eApplyDepthRanges >
	{
		static uint32_t constexpr MaxElems = 16u;
		static uint32_t constexpr ElemCount = 2u;

		inline CmdT( uint32_t first
			, uint32_t count
			, std::vector< VkViewport > const & viewports )
			: cmd{ { OpType::eApplyDepthRanges, sizeof( CmdT ) / sizeof( uint32_t ) } }
			, first{ first }
			, count{ std::min( MaxElems, count ) }
		{
			auto it = this->depthRanges.begin();
			auto end = viewports.begin() + std::min( MaxElems, uint32_t( viewports.size() ) );

			for ( auto vit = viewports.begin(); vit != end; ++vit )
			{
				auto & viewport = *vit;
				*it = viewport.minDepth;
				++it;
				*it = viewport.maxDepth;
				++it;
			}
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
	struct CmdConfig< OpType::eDrawBuffers >
	{
		static Op constexpr value = { OpType::eDrawBuffers, 18u };
	};

	template<>
	struct alignas( uint64_t ) CmdT< OpType::eDrawBuffers >
	{
		template< size_t N >
		inline CmdT( uint32_t targets[N] )
			: cmd{ { OpType::eDrawBuffers, sizeof( CmdT ) / sizeof( uint32_t ) } }
			, count{ uint32_t( N ) }
		{
			static_assert( N <= 16u, "No more that 16 draw buffers" );
			uint32_t i = 0u;

			for ( auto & target : makeArrayView( targets, targets + N ) )
			{
				this->targets[i++] = target;
			}
		}

		template< size_t N >
		inline CmdT( std::array< uint32_t, N > targets )
			: cmd{ { OpType::eDrawBuffers, sizeof( CmdT ) / sizeof( uint32_t ) } }
			, count{ uint32_t( N ) }
		{
			static_assert( N <= 16u, "No more that 16 draw buffers" );
			uint32_t i = 0u;

			for ( auto & target : targets )
			{
				this->targets[i++] = target;
			}
		}

		inline CmdT( std::vector< uint32_t > targets )
			: cmd{ { OpType::eDrawBuffers, sizeof( CmdT ) / sizeof( uint32_t ) } }
			, count{ std::min( 16u, uint32_t( targets.size() ) ) }
		{
			uint32_t i = 0u;

			for ( auto & target : targets )
			{
				if ( i < this->targets.size() )
				{
					this->targets[i++] = target;
				}
			}
		}

		Command cmd;
		uint32_t count;
		std::array< uint32_t, 16u > targets{ 0u };
	};
	using CmdDrawBuffers = CmdT< OpType::eDrawBuffers >;

	void apply( ContextLock const & context
		, CmdDrawBuffers const & cmd );

	//*************************************************************************

	template<>
	struct CmdConfig< OpType::eDrawBuffer >
	{
		static Op constexpr value = { OpType::eDrawBuffer, 2u };
	};

	template<>
	struct alignas( uint64_t ) CmdT< OpType::eDrawBuffer >
	{
		inline CmdT( uint32_t value )
			: cmd{ { OpType::eDrawBuffer, sizeof( CmdT ) / sizeof( uint32_t ) } }
			, target{ std::move( value ) }
		{
		}

		Command cmd;
		uint32_t target;
	};
	using CmdDrawBuffer = CmdT< OpType::eDrawBuffer >;

	void apply( ContextLock const & context
		, CmdDrawBuffer const & cmd );

	//*************************************************************************

	template<>
	struct CmdConfig< OpType::eUseProgram >
	{
		static Op constexpr value = { OpType::eUseProgram, 2u };
	};

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
	struct CmdConfig< OpType::eUseProgramPipeline >
	{
		static Op constexpr value = { OpType::eUseProgramPipeline, 2u };
	};

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
	struct CmdConfig< OpType::eGetQueryResults >
	{
		static Op constexpr value = { OpType::eGetQueryResults, 13u };
	};

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
	struct CmdConfig< OpType::eInitFramebuffer >
	{
		static Op constexpr value = { OpType::eInitFramebuffer, 4u };
	};

	template<>
	struct alignas( uint64_t ) CmdT< OpType::eInitFramebuffer >
	{
		inline CmdT( GLuint * fbo )
			: cmd{ { OpType::eInitFramebuffer, sizeof( CmdT ) / sizeof( uint32_t ) } }
			, fbo{ fbo }
		{
		}

		Command cmd;
		GLuint * fbo;
	};
	using CmdInitFramebuffer = CmdT< OpType::eInitFramebuffer >;

	void apply( ContextLock const & context
		, CmdInitFramebuffer const & cmd );

	//*************************************************************************

	template<>
	struct CmdConfig< OpType::eCleanupFramebuffer >
	{
		static Op constexpr value = { OpType::eCleanupFramebuffer, 4u };
	};

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
	struct CmdConfig< OpType::eBindFramebuffer >
	{
		static Op constexpr value = { OpType::eBindFramebuffer, 3u };
	};

	template<>
	struct alignas( uint64_t ) CmdT< OpType::eBindFramebuffer >
	{
		inline CmdT( uint32_t target
			, VkFramebuffer fbo )
			: cmd{ { OpType::eBindFramebuffer, sizeof( CmdT ) / sizeof( uint32_t ) } }
			, target{ std::move( target ) }
			, fbo{ fbo }
		{
		}

		Command cmd;
		uint32_t target;
		VkFramebuffer fbo;
	};
	using CmdBindFramebuffer = CmdT< OpType::eBindFramebuffer >;

	void apply( ContextLock const & context
		, CmdBindFramebuffer const & cmd );

	//*************************************************************************

	template<>
	struct CmdConfig< OpType::eBindBuffer >
	{
		static Op constexpr value = { OpType::eBindBuffer, 3u };
	};

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
	struct CmdConfig< OpType::eBindTexture >
	{
		static Op constexpr value = { OpType::eBindTexture, 3u };
	};

	template<>
	struct alignas( uint64_t ) CmdT< OpType::eBindTexture >
	{
		inline CmdT( uint32_t type
			, uint32_t name )
			: cmd{ { OpType::eBindTexture, sizeof( CmdT ) / sizeof( uint32_t ) } }
			, type{ std::move( type ) }
			, name{ std::move( name ) }
		{
		}

		Command cmd;
		uint32_t type;
		uint32_t name;
	};
	using CmdBindTexture = CmdT< OpType::eBindTexture >;

	void apply( ContextLock const & context
		, CmdBindTexture const & cmd );

	//*************************************************************************

	template<>
	struct CmdConfig< OpType::eBlendConstants >
	{
		static Op constexpr value = { OpType::eBlendConstants, 6u };
	};

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
	struct CmdConfig< OpType::eBlendEquation >
	{
		static Op constexpr value = { OpType::eBlendEquation, 5u };
	};

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
	struct CmdConfig< OpType::eBlendFunc >
	{
		static Op constexpr value = { OpType::eBlendFunc, 5u };
	};

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
	struct CmdConfig< OpType::eColorMask >
	{
		static Op constexpr value = { OpType::eColorMask, 4u };
	};

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
	struct CmdConfig< OpType::eClearColour >
	{
		static Op constexpr value = { OpType::eClearColour, 7u };
	};

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
	struct CmdConfig< OpType::eClearDepth >
	{
		static Op constexpr value = { OpType::eClearDepth, 2u };
	};

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
	struct CmdConfig< OpType::eClearStencil >
	{
		static Op constexpr value = { OpType::eClearStencil, 2u };
	};

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
	struct CmdConfig< OpType::eClearDepthStencil >
	{
		static Op constexpr value = { OpType::eClearDepthStencil, 3u };
	};

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
	struct CmdConfig< OpType::eCullFace >
	{
		static Op constexpr value = { OpType::eCullFace, 2u };
	};

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
	struct CmdConfig< OpType::eDepthFunc >
	{
		static Op constexpr value = { OpType::eDepthFunc, 2u };
	};

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
	struct CmdConfig< OpType::eDepthMask >
	{
		static Op constexpr value = { OpType::eDepthMask, 3u };
	};

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
	struct CmdConfig< OpType::eDepthRange >
	{
		static Op constexpr value = { OpType::eDepthRange, 2u };
	};

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
	struct CmdConfig< OpType::eFrontFace >
	{
		static Op constexpr value = { OpType::eFrontFace, 2u };
	};

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
	struct CmdConfig< OpType::eLineWidth >
	{
		static Op constexpr value = { OpType::eLineWidth, 3u };
	};

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
	struct CmdConfig< OpType::eLogicOp >
	{
		static Op constexpr value = { OpType::eLogicOp, 3u };
	};

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
	struct CmdConfig< OpType::eMinSampleShading >
	{
		static Op constexpr value = { OpType::eMinSampleShading, 3u };
	};

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
	struct CmdConfig< OpType::ePatchParameter >
	{
		static Op constexpr value = { OpType::ePatchParameter, 3u };
	};

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
	struct CmdConfig< OpType::ePolygonMode >
	{
		static Op constexpr value = { OpType::ePolygonMode, 3u };
	};

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
	struct CmdConfig< OpType::ePolygonOffset >
	{
		static Op constexpr value = { OpType::ePolygonOffset, 5u };
	};

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
	struct CmdConfig< OpType::ePrimitiveRestartIndex >
	{
		static Op constexpr value = { OpType::ePrimitiveRestartIndex, 3u };
	};

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
	struct CmdConfig< OpType::eStencilFunc >
	{
		static Op constexpr value = { OpType::eStencilFunc, 4u };
	};

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
	struct CmdConfig< OpType::eStencilMask >
	{
		static Op constexpr value = { OpType::eStencilMask, 4u };
	};

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
	struct CmdConfig< OpType::eStencilOp >
	{
		static Op constexpr value = { OpType::eStencilOp, 4u };
	};

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
	struct CmdConfig< OpType::eFramebufferTexture >
	{
		static Op constexpr value = { OpType::eFramebufferTexture, 5u };
	};

	template<>
	struct alignas( uint64_t ) CmdT< OpType::eFramebufferTexture >
	{
		inline CmdT( uint32_t target
			, uint32_t point
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
		uint32_t target;
		uint32_t point;
		uint32_t object;
		uint32_t mipLevel;
	};
	using CmdFramebufferTexture = CmdT< OpType::eFramebufferTexture >;

	void apply( ContextLock const & context
		, CmdFramebufferTexture const & cmd );

	//*************************************************************************

	template<>
	struct CmdConfig< OpType::eFramebufferTexture2D >
	{
		static Op constexpr value = { OpType::eFramebufferTexture2D, 6u };
	};

	template<>
	struct alignas( uint64_t ) CmdT< OpType::eFramebufferTexture2D >
	{
		inline CmdT( uint32_t target
			, uint32_t point
			, uint32_t texTarget
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
		uint32_t target;
		uint32_t point;
		uint32_t texTarget;
		uint32_t object;
		uint32_t mipLevel;
	};
	using CmdFramebufferTexture2D = CmdT< OpType::eFramebufferTexture2D >;

	void apply( ContextLock const & context
		, CmdFramebufferTexture2D const & cmd );

	//*************************************************************************

	template<>
	struct CmdConfig< OpType::eFramebufferTextureLayer >
	{
		static Op constexpr value = { OpType::eFramebufferTextureLayer, 6u };
	};

	template<>
	struct alignas( uint64_t ) CmdT< OpType::eFramebufferTextureLayer >
	{
		inline CmdT( uint32_t target
			, uint32_t point
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
		uint32_t target;
		uint32_t point;
		uint32_t object;
		uint32_t mipLevel;
		uint32_t arrayLayer;
	};
	using CmdFramebufferTextureLayer = CmdT< OpType::eFramebufferTextureLayer >;

	void apply( ContextLock const & context
		, CmdFramebufferTextureLayer const & cmd );

	//*************************************************************************

	template<>
	struct CmdConfig< OpType::eActiveTexture >
	{
		static Op constexpr value = { OpType::eActiveTexture, 2u };
	};

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
	struct CmdConfig< OpType::eUploadMemory >
	{
		static Op constexpr value = { OpType::eUploadMemory, 2u };
	};

	template<>
	struct alignas( uint64_t ) CmdT< OpType::eUploadMemory >
	{
		inline CmdT( VkDeviceMemory memory )
			: cmd{ { OpType::eUploadMemory, sizeof( CmdT ) / sizeof( uint32_t ) } }
			, memory{ memory }
		{
		}

		Command cmd;
		VkDeviceMemory memory;
	};
	using CmdUploadMemory = CmdT< OpType::eUploadMemory >;

	void apply( ContextLock const & context
		, CmdUploadMemory const & cmd );

	//*************************************************************************

	template<>
	struct CmdConfig< OpType::eDownloadMemory >
	{
		static Op constexpr value = { OpType::eDownloadMemory, 2u };
	};

	template<>
	struct alignas( uint64_t ) CmdT< OpType::eDownloadMemory >
	{
		inline CmdT( VkDeviceMemory memory )
			: cmd{ { OpType::eDownloadMemory, sizeof( CmdT ) / sizeof( uint32_t ) } }
			, memory{ memory }
		{
		}

		Command cmd;
		VkDeviceMemory memory;
	};
	using CmdDownloadMemory = CmdT< OpType::eDownloadMemory >;

	void apply( ContextLock const & context
		, CmdDownloadMemory const & cmd );

	//*************************************************************************

	template<>
	struct CmdConfig< OpType::eReadBuffer >
	{
		static Op constexpr value = { OpType::eReadBuffer, 2u };
	};

	template<>
	struct alignas( uint64_t ) CmdT< OpType::eReadBuffer >
	{
		inline CmdT( uint32_t point )
			: cmd{ { OpType::eReadBuffer, sizeof( CmdT ) / sizeof( uint32_t ) } }
			, point{ std::move( point ) }
		{
		}

		Command cmd;
		uint32_t point;
	};
	using CmdReadBuffer = CmdT< OpType::eReadBuffer >;

	void apply( ContextLock const & context
		, CmdReadBuffer const & cmd );

	//*************************************************************************

	template<>
	struct CmdConfig< OpType::eUpdateBuffer >
	{
		static Op constexpr value = { OpType::eUpdateBuffer, 5 };
	};

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
	struct CmdConfig< OpType::eFillBuffer >
	{
		static Op constexpr value = { OpType::eFillBuffer, 5 };
	};

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
		return *reinterpret_cast< CmdT< OpT > const * >( &cmd );
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
