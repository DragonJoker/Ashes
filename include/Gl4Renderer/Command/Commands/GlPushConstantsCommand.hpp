/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include "Gl4Renderer/Command/Commands/GlCommandBase.hpp"

#include "Gl4Renderer/Shader/GlShaderDesc.hpp"

namespace ashes::gl4
{
	//*************************************************************************

	template<>
	struct CmdConfig< OpType::eUniform1fv >
	{
		static Op constexpr value = { OpType::eUniform1fv, 5u };
	};

	template<>
	struct alignas( uint64_t ) CmdT< OpType::eUniform1fv >
	{
		inline CmdT( uint32_t location
			, uint32_t arraySize
			, float const * buffer )
			: cmd{ { OpType::eUniform1fv, sizeof( CmdT ) / sizeof( uint32_t ) } }
			, location{ std::move( location ) }
			, arraySize{ std::move( arraySize ) }
			, buffer{ std::move( buffer ) }
		{
		}

		Command cmd;
		uint32_t location;
		uint32_t arraySize;
		float const * buffer;
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
			, uint32_t arraySize
			, float const * buffer )
			: cmd{ { OpType::eUniform2fv, sizeof( CmdT ) / sizeof( uint32_t ) } }
			, location{ std::move( location ) }
			, arraySize{ std::move( arraySize ) }
			, buffer{ std::move( buffer ) }
		{
		}

		Command cmd;
		uint32_t location;
		uint32_t arraySize;
		float const * buffer;
	};
	using CmdUniform2fv = CmdT< OpType::eUniform2fv >;

	void apply( ContextLock const & context
		, CmdUniform2fv const & cmd );

	//*************************************************************************

	template<>
	struct CmdConfig< OpType::eUniform3fv >
	{
		static Op constexpr value = { OpType::eUniform3fv, 5u };
	};

	template<>
	struct alignas( uint64_t ) CmdT< OpType::eUniform3fv >
	{
		inline CmdT( uint32_t location
			, uint32_t arraySize
			, float const * buffer )
			: cmd{ { OpType::eUniform3fv, sizeof( CmdT ) / sizeof( uint32_t ) } }
			, location{ std::move( location ) }
			, arraySize{ std::move( arraySize ) }
			, buffer{ std::move( buffer ) }
		{
		}

		Command cmd;
		uint32_t location;
		uint32_t arraySize;
		float const * buffer;
	};
	using CmdUniform3fv = CmdT< OpType::eUniform3fv >;

	void apply( ContextLock const & context
		, CmdUniform3fv const & cmd );

	//*************************************************************************

	template<>
	struct CmdConfig< OpType::eUniform4fv >
	{
		static Op constexpr value = { OpType::eUniform4fv, 5u };
	};

	template<>
	struct alignas( uint64_t ) CmdT< OpType::eUniform4fv >
	{
		inline CmdT( uint32_t location
			, uint32_t arraySize
			, float const * buffer )
			: cmd{ { OpType::eUniform4fv, sizeof( CmdT ) / sizeof( uint32_t ) } }
			, location{ std::move( location ) }
			, arraySize{ std::move( arraySize ) }
			, buffer{ std::move( buffer ) }
		{
		}

		Command cmd;
		uint32_t location;
		uint32_t arraySize;
		float const * buffer;
	};
	using CmdUniform4fv = CmdT< OpType::eUniform4fv >;

	void apply( ContextLock const & context
		, CmdUniform4fv const & cmd );
	
	//*************************************************************************

	template<>
	struct CmdConfig< OpType::eUniformMatrix2fv >
	{
		static Op constexpr value = { OpType::eUniformMatrix2fv, 6u };
	};

	template<>
	struct alignas( uint64_t ) CmdT< OpType::eUniformMatrix2fv >
	{
		inline CmdT( uint32_t location
			, uint32_t arraySize
			, uint32_t transpose
			, float const * buffer )
			: cmd{ { OpType::eUniformMatrix2fv, sizeof( CmdT ) / sizeof( uint32_t ) } }
			, location{ std::move( location ) }
			, arraySize{ std::move( arraySize ) }
			, transpose{ std::move( transpose ) }
			, buffer{ std::move( buffer ) }
		{
		}

		Command cmd;
		uint32_t location;
		uint32_t arraySize;
		uint32_t transpose;
		float const * buffer;
	};
	using CmdUniformMatrix2fv = CmdT< OpType::eUniformMatrix2fv >;

	void apply( ContextLock const & context
		, CmdUniformMatrix2fv const & cmd );

	//*************************************************************************

	template<>
	struct CmdConfig< OpType::eUniformMatrix3fv >
	{
		static Op constexpr value = { OpType::eUniformMatrix3fv, 6u };
	};

	template<>
	struct alignas( uint64_t ) CmdT< OpType::eUniformMatrix3fv >
	{
		inline CmdT( uint32_t location
			, uint32_t arraySize
			, uint32_t transpose
			, float const * buffer )
			: cmd{ { OpType::eUniformMatrix3fv, sizeof( CmdT ) / sizeof( uint32_t ) } }
			, location{ std::move( location ) }
			, arraySize{ std::move( arraySize ) }
			, transpose{ std::move( transpose ) }
			, buffer{ std::move( buffer ) }
		{
		}

		Command cmd;
		uint32_t location;
		uint32_t arraySize;
		uint32_t transpose;
		float const * buffer;
	};
	using CmdUniformMatrix3fv = CmdT< OpType::eUniformMatrix3fv >;

	void apply( ContextLock const & context
		, CmdUniformMatrix3fv const & cmd );

	//*************************************************************************

	template<>
	struct CmdConfig< OpType::eUniformMatrix4fv >
	{
		static Op constexpr value = { OpType::eUniformMatrix4fv, 6u };
	};

	template<>
	struct alignas( uint64_t ) CmdT< OpType::eUniformMatrix4fv >
	{
		inline CmdT( uint32_t location
			, uint32_t arraySize
			, uint32_t transpose
			, float const * buffer )
			: cmd{ { OpType::eUniformMatrix4fv, sizeof( CmdT ) / sizeof( uint32_t ) } }
			, location{ std::move( location ) }
			, arraySize{ std::move( arraySize ) }
			, transpose{ std::move( transpose ) }
			, buffer{ std::move( buffer ) }
		{
		}

		Command cmd;
		uint32_t location;
		uint32_t arraySize;
		uint32_t transpose;
		float const * buffer;
	};
	using CmdUniformMatrix4fv = CmdT< OpType::eUniformMatrix4fv >;

	void apply( ContextLock const & context
		, CmdUniformMatrix4fv const & cmd );

	//*************************************************************************

	template<>
	struct CmdConfig< OpType::eUniform1iv >
	{
		static Op constexpr value = { OpType::eUniform1iv, 5u };
	};

	template<>
	struct alignas( uint64_t ) CmdT< OpType::eUniform1iv >
	{
		inline CmdT( uint32_t location
			, uint32_t arraySize
			, int32_t const * buffer )
			: cmd{ { OpType::eUniform1iv, sizeof( CmdT ) / sizeof( uint32_t ) } }
			, location{ std::move( location ) }
			, arraySize{ std::move( arraySize ) }
			, buffer{ std::move( buffer ) }
		{
		}

		Command cmd;
		uint32_t location;
		uint32_t arraySize;
		int32_t const * buffer;
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
			, uint32_t arraySize
			, int32_t const * buffer )
			: cmd{ { OpType::eUniform2iv, sizeof( CmdT ) / sizeof( uint32_t ) } }
			, location{ std::move( location ) }
			, arraySize{ std::move( arraySize ) }
			, buffer{ std::move( buffer ) }
		{
		}

		Command cmd;
		uint32_t location;
		uint32_t arraySize;
		int32_t const * buffer;
	};
	using CmdUniform2iv = CmdT< OpType::eUniform2iv >;

	void apply( ContextLock const & context
		, CmdUniform2iv const & cmd );

	//*************************************************************************

	template<>
	struct CmdConfig< OpType::eUniform3iv >
	{
		static Op constexpr value = { OpType::eUniform3iv, 5u };
	};

	template<>
	struct alignas( uint64_t ) CmdT< OpType::eUniform3iv >
	{
		inline CmdT( uint32_t location
			, uint32_t arraySize
			, int32_t const * buffer )
			: cmd{ { OpType::eUniform3iv, sizeof( CmdT ) / sizeof( uint32_t ) } }
			, location{ std::move( location ) }
			, arraySize{ std::move( arraySize ) }
			, buffer{ std::move( buffer ) }
		{
		}

		Command cmd;
		uint32_t location;
		uint32_t arraySize;
		int32_t const * buffer;
	};
	using CmdUniform3iv = CmdT< OpType::eUniform3iv >;

	void apply( ContextLock const & context
		, CmdUniform3iv const & cmd );

	//*************************************************************************

	template<>
	struct CmdConfig< OpType::eUniform4iv >
	{
		static Op constexpr value = { OpType::eUniform4iv, 5u };
	};

	template<>
	struct alignas( uint64_t ) CmdT< OpType::eUniform4iv >
	{
		inline CmdT( uint32_t location
			, uint32_t arraySize
			, int32_t const * buffer )
			: cmd{ { OpType::eUniform4iv, sizeof( CmdT ) / sizeof( uint32_t ) } }
			, location{ std::move( location ) }
			, arraySize{ std::move( arraySize ) }
			, buffer{ std::move( buffer ) }
		{
		}

		Command cmd;
		uint32_t location;
		uint32_t arraySize;
		int32_t const * buffer;
	};
	using CmdUniform4iv = CmdT< OpType::eUniform4iv >;

	void apply( ContextLock const & context
		, CmdUniform4iv const & cmd );

	//*************************************************************************

	template<>
	struct CmdConfig< OpType::eUniform1uiv >
	{
		static Op constexpr value = { OpType::eUniform1uiv, 5u };
	};

	template<>
	struct alignas( uint64_t ) CmdT< OpType::eUniform1uiv >
	{
		inline CmdT( uint32_t location
			, uint32_t arraySize
			, uint32_t const * buffer )
			: cmd{ { OpType::eUniform1uiv, sizeof( CmdT ) / sizeof( uint32_t ) } }
			, location{ std::move( location ) }
			, arraySize{ std::move( arraySize ) }
			, buffer{ std::move( buffer ) }
		{
		}

		Command cmd;
		uint32_t location;
		uint32_t arraySize;
		uint32_t const * buffer;
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
			, uint32_t arraySize
			, uint32_t const * buffer )
			: cmd{ { OpType::eUniform2uiv, sizeof( CmdT ) / sizeof( uint32_t ) } }
			, location{ std::move( location ) }
			, arraySize{ std::move( arraySize ) }
			, buffer{ std::move( buffer ) }
		{
		}

		Command cmd;
		uint32_t location;
		uint32_t arraySize;
		uint32_t const * buffer;
	};
	using CmdUniform2uiv = CmdT< OpType::eUniform2uiv >;

	void apply( ContextLock const & context
		, CmdUniform2uiv const & cmd );

	//*************************************************************************

	template<>
	struct CmdConfig< OpType::eUniform3uiv >
	{
		static Op constexpr value = { OpType::eUniform3uiv, 5u };
	};

	template<>
	struct alignas( uint64_t ) CmdT< OpType::eUniform3uiv >
	{
		inline CmdT( uint32_t location
			, uint32_t arraySize
			, uint32_t const * buffer )
			: cmd{ { OpType::eUniform3uiv, sizeof( CmdT ) / sizeof( uint32_t ) } }
			, location{ std::move( location ) }
			, arraySize{ std::move( arraySize ) }
			, buffer{ std::move( buffer ) }
		{
		}

		Command cmd;
		uint32_t location;
		uint32_t arraySize;
		uint32_t const * buffer;
	};
	using CmdUniform3uiv = CmdT< OpType::eUniform3uiv >;

	void apply( ContextLock const & context
		, CmdUniform3uiv const & cmd );

	//*************************************************************************

	template<>
	struct CmdConfig< OpType::eUniform4uiv >
	{
		static Op constexpr value = { OpType::eUniform4uiv, 5u };
	};

	template<>
	struct alignas( uint64_t ) CmdT< OpType::eUniform4uiv >
	{
		inline CmdT( uint32_t location
			, uint32_t arraySize
			, uint32_t const * buffer )
			: cmd{ { OpType::eUniform4uiv, sizeof( CmdT ) / sizeof( uint32_t ) } }
			, location{ std::move( location ) }
			, arraySize{ std::move( arraySize ) }
			, buffer{ std::move( buffer ) }
		{
		}

		Command cmd;
		uint32_t location;
		uint32_t arraySize;
		uint32_t const * buffer;
	};
	using CmdUniform4uiv = CmdT< OpType::eUniform4uiv >;

	void apply( ContextLock const & context
		, CmdUniform4uiv const & cmd );

	//*************************************************************************

	void buildPushConstantsCommand( PushConstantsDesc const & pcb
		, CmdList & list );

	//*************************************************************************
}
