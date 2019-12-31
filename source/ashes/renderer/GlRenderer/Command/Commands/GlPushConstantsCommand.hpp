/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include "renderer/GlRenderer/Command/Commands/GlCommandBase.hpp"

#include "renderer/GlRenderer/Shader/GlShaderDesc.hpp"

namespace ashes::gl
{
	//*************************************************************************

	template<>
	struct CmdConfig< OpType::eProgramUniform1fv >
	{
		static Op constexpr value = { OpType::eProgramUniform1fv, 4u };
	};

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
	struct CmdConfig< OpType::eProgramUniform2fv >
	{
		static Op constexpr value = { OpType::eProgramUniform2fv, 5u };
	};

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
	struct CmdConfig< OpType::eProgramUniform3fv >
	{
		static Op constexpr value = { OpType::eProgramUniform3fv, 6u };
	};

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
	struct CmdConfig< OpType::eProgramUniform4fv >
	{
		static Op constexpr value = { OpType::eProgramUniform4fv, 7u };
	};

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
	struct CmdConfig< OpType::eProgramUniformMatrix2fv >
	{
		static Op constexpr value = { OpType::eProgramUniformMatrix2fv, 8u };
	};

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
	struct CmdConfig< OpType::eProgramUniformMatrix3fv >
	{
		static Op constexpr value = { OpType::eProgramUniformMatrix3fv, 13u };
	};

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
	struct CmdConfig< OpType::eProgramUniformMatrix4fv >
	{
		static Op constexpr value = { OpType::eProgramUniformMatrix4fv, 20u };
	};

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
	struct CmdConfig< OpType::eProgramUniform1iv >
	{
		static Op constexpr value = { OpType::eProgramUniform1iv, 4u };
	};

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
	struct CmdConfig< OpType::eProgramUniform2iv >
	{
		static Op constexpr value = { OpType::eProgramUniform2iv, 5u };
	};

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
	struct CmdConfig< OpType::eProgramUniform3iv >
	{
		static Op constexpr value = { OpType::eProgramUniform3iv, 6u };
	};

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
	struct CmdConfig< OpType::eProgramUniform4iv >
	{
		static Op constexpr value = { OpType::eProgramUniform4iv, 7u };
	};

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
	struct CmdConfig< OpType::eProgramUniform1uiv >
	{
		static Op constexpr value = { OpType::eProgramUniform1uiv, 4u };
	};

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
	struct CmdConfig< OpType::eProgramUniform2uiv >
	{
		static Op constexpr value = { OpType::eProgramUniform2uiv, 5u };
	};

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
	struct CmdConfig< OpType::eProgramUniform3uiv >
	{
		static Op constexpr value = { OpType::eProgramUniform3uiv, 6u };
	};

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
	struct CmdConfig< OpType::eProgramUniform4uiv >
	{
		static Op constexpr value = { OpType::eProgramUniform4uiv, 7u };
	};

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
	struct CmdConfig< OpType::eUniform1fv >
	{
		static Op constexpr value = { OpType::eUniform1fv, 4u };
	};

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
	struct CmdConfig< OpType::eUniform2fv >
	{
		static Op constexpr value = { OpType::eUniform2fv, 5u };
	};

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
	struct CmdConfig< OpType::eUniform3fv >
	{
		static Op constexpr value = { OpType::eUniform3fv, 6u };
	};

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
	struct CmdConfig< OpType::eUniform4fv >
	{
		static Op constexpr value = { OpType::eUniform4fv, 7u };
	};

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
	struct CmdConfig< OpType::eUniformMatrix2fv >
	{
		static Op constexpr value = { OpType::eUniformMatrix2fv, 8u };
	};

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
	struct CmdConfig< OpType::eUniformMatrix3fv >
	{
		static Op constexpr value = { OpType::eUniformMatrix3fv, 13u };
	};

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
	struct CmdConfig< OpType::eUniformMatrix4fv >
	{
		static Op constexpr value = { OpType::eUniformMatrix4fv, 20u };
	};

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
	struct CmdConfig< OpType::eUniform1iv >
	{
		static Op constexpr value = { OpType::eUniform1iv, 4u };
	};

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
	struct CmdConfig< OpType::eUniform2iv >
	{
		static Op constexpr value = { OpType::eUniform2iv, 5u };
	};

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
	struct CmdConfig< OpType::eUniform3iv >
	{
		static Op constexpr value = { OpType::eUniform3iv, 6u };
	};

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
	struct CmdConfig< OpType::eUniform4iv >
	{
		static Op constexpr value = { OpType::eUniform4iv, 7u };
	};

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
	struct CmdConfig< OpType::eUniform1uiv >
	{
		static Op constexpr value = { OpType::eUniform1uiv, 4u };
	};

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
	struct CmdConfig< OpType::eUniform2uiv >
	{
		static Op constexpr value = { OpType::eUniform2uiv, 5u };
	};

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
	struct CmdConfig< OpType::eUniform3uiv >
	{
		static Op constexpr value = { OpType::eUniform3uiv, 6u };
	};

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
	struct CmdConfig< OpType::eUniform4uiv >
	{
		static Op constexpr value = { OpType::eUniform4uiv, 7u };
	};

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

	void buildPushConstantsCommand( VkDevice device
		, VkShaderStageFlags stageFlags
		, PushConstantsDesc const & pcb
		, CmdList & list );

	//*************************************************************************
}
