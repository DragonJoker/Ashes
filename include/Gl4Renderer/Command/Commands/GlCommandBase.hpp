/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include "Gl4Renderer/GlRendererPrerequisites.hpp"

#include <array>

namespace ashes::gl4
{
	//*************************************************************************

	enum class OpType
		: uint16_t
	{
		eEnable,
		eDisable,
		eBeginQuery,
		eEndQuery,
		eClearColour,
		eClearDepth,
		eClearStencil,
		eClearDepthStencil,
		eClearBack,
		eClearBackColour,
		eClearBackDepth,
		eClearBackStencil,
		eClearBackDepthStencil,
		eApplyScissor,
		eApplyViewport,
		eBindFramebuffer,
		eDrawBuffer,
		eDrawBuffers,
		eUseProgram,
		eActiveTexture,
		eBindTexture,
		eBindSampler,
		eBindImage,
		eBindBufferRange,
		eBindVextexArray,
		eBindContextState,
		eFramebufferTexture2D,
		eBlitFramebuffer,
		eClearTexColor,
		eClearTexDepth,
		eClearTexStencil,
		eClearTexDepthStencil,
		eBindBuffer,
		eCopyBufferSubData,
		ePixelStore,
		eCompressedTexSubImage1D,
		eCompressedTexSubImage2D,
		eCompressedTexSubImage3D,
		eTexSubImage1D,
		eTexSubImage2D,
		eTexSubImage3D,
		eCopyImageSubData,
		eReadBuffer,
		eReadPixels,
		eDispatch,
		eDispatchIndirect,
		eDraw,
		eDrawIndexed,
		eDrawIndexedIndirect,
		eDrawIndirect,
		eGenerateMipmaps,
		eMemoryBarrier,
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
		eResetEvent,
		eSetEvent,
		eWaitEvents,
		eSetDepthBias,
		eSetLineWidth,
		eWriteTimestamp,
		eDownloadMemory,
		eUploadMemory,
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

	using CmdBuffer = UInt32Array;
	using CmdList = std::vector< CmdBuffer >;

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
	struct CmdConfig< OpType::eApplyScissor >
	{
		static Op constexpr value = { OpType::eApplyScissor, 5u };
	};

	template<>
	struct alignas( uint64_t ) CmdT< OpType::eApplyScissor >
	{
		inline CmdT( VkRect2D value )
			: cmd{ { OpType::eApplyScissor, sizeof( CmdT ) / sizeof( uint32_t ) } }
			, scissor{ std::move( value ) }
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
	struct CmdConfig< OpType::eApplyViewport >
	{
		static Op constexpr value = { OpType::eApplyViewport, 7u };
	};

	template<>
	struct alignas( uint64_t ) CmdT< OpType::eApplyViewport >
	{
		inline CmdT( VkViewport viewport )
			: cmd{ { OpType::eApplyViewport, sizeof( CmdT ) / sizeof( uint32_t ) } }
			, viewport{ std::move( viewport ) }
		{
		}

		Command cmd;
		VkViewport viewport;
	};
	using CmdApplyViewport = CmdT< OpType::eApplyViewport >;

	void apply( ContextLock const & context
		, CmdApplyViewport const & cmd );

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

			for ( auto & target : targets )
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
	struct CmdConfig< OpType::eBindFramebuffer >
	{
		static Op constexpr value = { OpType::eBindFramebuffer, 3u };
	};

	template<>
	struct alignas( uint64_t ) CmdT< OpType::eBindFramebuffer >
	{
		inline CmdT( uint32_t target
			, uint32_t name )
			: cmd{ { OpType::eBindFramebuffer, sizeof( CmdT ) / sizeof( uint32_t ) } }
			, target{ std::move( target ) }
			, name{ std::move( name ) }
		{
		}

		Command cmd;
		uint32_t target;
		uint32_t name;
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
			, target{ std::move( target ) }
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
	struct CmdConfig< OpType::eClearColour >
	{
		static Op constexpr value = { OpType::eClearColour, 6u };
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

	template< OpType OpT, typename ... ParamsT >
	CmdBuffer makeCmd( ParamsT && ... params )
	{
		CmdT< OpT > cmd{ std::forward< ParamsT && >( params )... };
		CmdBuffer result;
		result.resize( cmd.cmd.op.size );
		push( result.begin(), result.end(), cmd );
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
