/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include "renderer/Gl4Renderer/Command/Commands/GlCommandBase.hpp"

#include "renderer/Gl4Renderer/Shader/GlShaderDesc.hpp"

namespace ashes::gl4
{
	//*************************************************************************

	template<>
	struct CmdConfig< OpType::eUniform1fv >
	{
		static Op constexpr value = { OpType::eUniform1fv, 4u };
	};

	template<>
	struct alignas( uint64_t ) CmdT< OpType::eUniform1fv >
	{
		inline CmdT( uint32_t program
			, uint32_t location
			, float const * buffer )
			: cmd{ { OpType::eUniform1fv, sizeof( CmdT ) / sizeof( uint32_t ) } }
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
	using CmdUniform1fv = CmdT< OpType::eUniform1fv >;

	void apply( ContextLock const & context
		, CmdUniform1fv const & cmd );

	//*************************************************************************

	template<>
	struct CmdConfig< OpType::eUniform2fv >
	{
		static Op constexpr value = { OpType::eUniform2fv, 5u };
	};

	template<>
	struct alignas( uint64_t ) CmdT< OpType::eUniform2fv >
	{
		inline CmdT( uint32_t program
			, uint32_t location
			, float const * buffer )
			: cmd{ { OpType::eUniform2fv, sizeof( CmdT ) / sizeof( uint32_t ) } }
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
	using CmdUniform2fv = CmdT< OpType::eUniform2fv >;

	void apply( ContextLock const & context
		, CmdUniform2fv const & cmd );

	//*************************************************************************

	template<>
	struct CmdConfig< OpType::eUniform3fv >
	{
		static Op constexpr value = { OpType::eUniform3fv, 6u };
	};

	template<>
	struct alignas( uint64_t ) CmdT< OpType::eUniform3fv >
	{
		inline CmdT( uint32_t program
			, uint32_t location
			, float const * buffer )
			: cmd{ { OpType::eUniform3fv, sizeof( CmdT ) / sizeof( uint32_t ) } }
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
	using CmdUniform3fv = CmdT< OpType::eUniform3fv >;

	void apply( ContextLock const & context
		, CmdUniform3fv const & cmd );

	//*************************************************************************

	template<>
	struct CmdConfig< OpType::eUniform4fv >
	{
		static Op constexpr value = { OpType::eUniform4fv, 7u };
	};

	template<>
	struct alignas( uint64_t ) CmdT< OpType::eUniform4fv >
	{
		inline CmdT( uint32_t program
			, uint32_t location
			, float const * buffer )
			: cmd{ { OpType::eUniform4fv, sizeof( CmdT ) / sizeof( uint32_t ) } }
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
	using CmdUniform4fv = CmdT< OpType::eUniform4fv >;

	void apply( ContextLock const & context
		, CmdUniform4fv const & cmd );
	
	//*************************************************************************

	template<>
	struct CmdConfig< OpType::eUniformMatrix2fv >
	{
		static Op constexpr value = { OpType::eUniformMatrix2fv, 8u };
	};

	template<>
	struct alignas( uint64_t ) CmdT< OpType::eUniformMatrix2fv >
	{
		inline CmdT( uint32_t program
			, uint32_t location
			, uint32_t transpose
			, float const * buffer )
			: cmd{ { OpType::eUniformMatrix2fv, sizeof( CmdT ) / sizeof( uint32_t ) } }
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
	using CmdUniformMatrix2fv = CmdT< OpType::eUniformMatrix2fv >;

	void apply( ContextLock const & context
		, CmdUniformMatrix2fv const & cmd );

	//*************************************************************************

	template<>
	struct CmdConfig< OpType::eUniformMatrix3fv >
	{
		static Op constexpr value = { OpType::eUniformMatrix3fv, 13u };
	};

	template<>
	struct alignas( uint64_t ) CmdT< OpType::eUniformMatrix3fv >
	{
		inline CmdT( uint32_t program
			, uint32_t location
			, uint32_t transpose
			, float const * buffer )
			: cmd{ { OpType::eUniformMatrix3fv, sizeof( CmdT ) / sizeof( uint32_t ) } }
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
	using CmdUniformMatrix3fv = CmdT< OpType::eUniformMatrix3fv >;

	void apply( ContextLock const & context
		, CmdUniformMatrix3fv const & cmd );

	//*************************************************************************

	template<>
	struct CmdConfig< OpType::eUniformMatrix4fv >
	{
		static Op constexpr value = { OpType::eUniformMatrix4fv, 20u };
	};

	template<>
	struct alignas( uint64_t ) CmdT< OpType::eUniformMatrix4fv >
	{
		inline CmdT( uint32_t program
			, uint32_t location
			, uint32_t transpose
			, float const * buffer )
			: cmd{ { OpType::eUniformMatrix4fv, sizeof( CmdT ) / sizeof( uint32_t ) } }
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
	using CmdUniformMatrix4fv = CmdT< OpType::eUniformMatrix4fv >;

	void apply( ContextLock const & context
		, CmdUniformMatrix4fv const & cmd );

	//*************************************************************************

	template<>
	struct CmdConfig< OpType::eUniform1iv >
	{
		static Op constexpr value = { OpType::eUniform1iv, 4u };
	};

	template<>
	struct alignas( uint64_t ) CmdT< OpType::eUniform1iv >
	{
		inline CmdT( uint32_t program
			, uint32_t location
			, int32_t const * buffer )
			: cmd{ { OpType::eUniform1iv, sizeof( CmdT ) / sizeof( uint32_t ) } }
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
	using CmdUniform1iv = CmdT< OpType::eUniform1iv >;

	void apply( ContextLock const & context
		, CmdUniform1iv const & cmd );

	//*************************************************************************

	template<>
	struct CmdConfig< OpType::eUniform2iv >
	{
		static Op constexpr value = { OpType::eUniform2iv, 5u };
	};

	template<>
	struct alignas( uint64_t ) CmdT< OpType::eUniform2iv >
	{
		inline CmdT( uint32_t program
			, uint32_t location
			, int32_t const * buffer )
			: cmd{ { OpType::eUniform2iv, sizeof( CmdT ) / sizeof( uint32_t ) } }
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
	using CmdUniform2iv = CmdT< OpType::eUniform2iv >;

	void apply( ContextLock const & context
		, CmdUniform2iv const & cmd );

	//*************************************************************************

	template<>
	struct CmdConfig< OpType::eUniform3iv >
	{
		static Op constexpr value = { OpType::eUniform3iv, 6u };
	};

	template<>
	struct alignas( uint64_t ) CmdT< OpType::eUniform3iv >
	{
		inline CmdT( uint32_t program
			, uint32_t location
			, int32_t const * buffer )
			: cmd{ { OpType::eUniform3iv, sizeof( CmdT ) / sizeof( uint32_t ) } }
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
	using CmdUniform3iv = CmdT< OpType::eUniform3iv >;

	void apply( ContextLock const & context
		, CmdUniform3iv const & cmd );

	//*************************************************************************

	template<>
	struct CmdConfig< OpType::eUniform4iv >
	{
		static Op constexpr value = { OpType::eUniform4iv, 7u };
	};

	template<>
	struct alignas( uint64_t ) CmdT< OpType::eUniform4iv >
	{
		inline CmdT( uint32_t program
			, uint32_t location
			, int32_t const * buffer )
			: cmd{ { OpType::eUniform4iv, sizeof( CmdT ) / sizeof( uint32_t ) } }
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
	using CmdUniform4iv = CmdT< OpType::eUniform4iv >;

	void apply( ContextLock const & context
		, CmdUniform4iv const & cmd );

	//*************************************************************************

	template<>
	struct CmdConfig< OpType::eUniform1uiv >
	{
		static Op constexpr value = { OpType::eUniform1uiv, 4u };
	};

	template<>
	struct alignas( uint64_t ) CmdT< OpType::eUniform1uiv >
	{
		inline CmdT( uint32_t program
			, uint32_t location
			, uint32_t const * buffer )
			: cmd{ { OpType::eUniform1uiv, sizeof( CmdT ) / sizeof( uint32_t ) } }
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
	using CmdUniform1uiv = CmdT< OpType::eUniform1uiv >;

	void apply( ContextLock const & context
		, CmdUniform1uiv const & cmd );

	//*************************************************************************

	template<>
	struct CmdConfig< OpType::eUniform2uiv >
	{
		static Op constexpr value = { OpType::eUniform2uiv, 5u };
	};

	template<>
	struct alignas( uint64_t ) CmdT< OpType::eUniform2uiv >
	{
		inline CmdT( uint32_t program
			, uint32_t location
			, uint32_t const * buffer )
			: cmd{ { OpType::eUniform2uiv, sizeof( CmdT ) / sizeof( uint32_t ) } }
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
	using CmdUniform2uiv = CmdT< OpType::eUniform2uiv >;

	void apply( ContextLock const & context
		, CmdUniform2uiv const & cmd );

	//*************************************************************************

	template<>
	struct CmdConfig< OpType::eUniform3uiv >
	{
		static Op constexpr value = { OpType::eUniform3uiv, 6u };
	};

	template<>
	struct alignas( uint64_t ) CmdT< OpType::eUniform3uiv >
	{
		inline CmdT( uint32_t program
			, uint32_t location
			, uint32_t const * buffer )
			: cmd{ { OpType::eUniform3uiv, sizeof( CmdT ) / sizeof( uint32_t ) } }
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
	using CmdUniform3uiv = CmdT< OpType::eUniform3uiv >;

	void apply( ContextLock const & context
		, CmdUniform3uiv const & cmd );

	//*************************************************************************

	template<>
	struct CmdConfig< OpType::eUniform4uiv >
	{
		static Op constexpr value = { OpType::eUniform4uiv, 7u };
	};

	template<>
	struct alignas( uint64_t ) CmdT< OpType::eUniform4uiv >
	{
		inline CmdT( uint32_t program
			, uint32_t location
			, uint32_t const * buffer )
			: cmd{ { OpType::eUniform4uiv, sizeof( CmdT ) / sizeof( uint32_t ) } }
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
	using CmdUniform4uiv = CmdT< OpType::eUniform4uiv >;

	void apply( ContextLock const & context
		, CmdUniform4uiv const & cmd );

	//*************************************************************************

	void buildPushConstantsCommand( VkShaderStageFlags stageFlags
		, PushConstantsDesc const & pcb
		, CmdList & list );

	//*************************************************************************
}
