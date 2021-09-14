/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include "renderer/GlRenderer/Enum/GlAccessFlag.hpp"
#include "renderer/GlRenderer/Enum/GlAttachmentPoint.hpp"
#include "renderer/GlRenderer/Enum/GlAttachmentType.hpp"
#include "renderer/GlRenderer/Enum/GlBaseType.hpp"
#include "renderer/GlRenderer/Enum/GlBlendFactor.hpp"
#include "renderer/GlRenderer/Enum/GlBlendOp.hpp"
#include "renderer/GlRenderer/Enum/GlBufferDataUsageFlag.hpp"
#include "renderer/GlRenderer/Enum/GlBufferParameter.hpp"
#include "renderer/GlRenderer/Enum/GlBufferTarget.hpp"
#include "renderer/GlRenderer/Enum/GlClearTarget.hpp"
#include "renderer/GlRenderer/Enum/GlClipInfo.hpp"
#include "renderer/GlRenderer/Enum/GlCompareOp.hpp"
#include "renderer/GlRenderer/Enum/GlComponentSwizzle.hpp"
#include "renderer/GlRenderer/Enum/GlConstantFormat.hpp"
#include "renderer/GlRenderer/Enum/GlCullModeFlag.hpp"
#include "renderer/GlRenderer/Enum/GlDebugReportObjectType.hpp"
#include "renderer/GlRenderer/Enum/GlError.hpp"
#include "renderer/GlRenderer/Enum/GlFenceWaitFlag.hpp"
#include "renderer/GlRenderer/Enum/GlFenceWaitResult.hpp"
#include "renderer/GlRenderer/Enum/GlFilter.hpp"
#include "renderer/GlRenderer/Enum/GlFormat.hpp"
#include "renderer/GlRenderer/Enum/GlFormatProperty.hpp"
#include "renderer/GlRenderer/Enum/GlFrameBufferTarget.hpp"
#include "renderer/GlRenderer/Enum/GlFrontFace.hpp"
#include "renderer/GlRenderer/Enum/GlGetParameter.hpp"
#include "renderer/GlRenderer/Enum/GlImageAspectFlag.hpp"
#include "renderer/GlRenderer/Enum/GlImageLayout.hpp"
#include "renderer/GlRenderer/Enum/GlImageTiling.hpp"
#include "renderer/GlRenderer/Enum/GlIndexType.hpp"
#include "renderer/GlRenderer/Enum/GlLogicOp.hpp"
#include "renderer/GlRenderer/Enum/GlMemoryBarrierFlag.hpp"
#include "renderer/GlRenderer/Enum/GlMemoryMapFlag.hpp"
#include "renderer/GlRenderer/Enum/GlMemoryPropertyFlag.hpp"
#include "renderer/GlRenderer/Enum/GlMipmapMode.hpp"
#include "renderer/GlRenderer/Enum/GlPackAlignment.hpp"
#include "renderer/GlRenderer/Enum/GlPolygonMode.hpp"
#include "renderer/GlRenderer/Enum/GlPrimitiveTopology.hpp"
#include "renderer/GlRenderer/Enum/GlQueryResultFlag.hpp"
#include "renderer/GlRenderer/Enum/GlQueryType.hpp"
#include "renderer/GlRenderer/Enum/GlSampleCountFlag.hpp"
#include "renderer/GlRenderer/Enum/GlSamplerParameter.hpp"
#include "renderer/GlRenderer/Enum/GlShaderBinaryFormat.hpp"
#include "renderer/GlRenderer/Enum/GlShaderInfo.hpp"
#include "renderer/GlRenderer/Enum/GlShaderStageFlag.hpp"
#include "renderer/GlRenderer/Enum/GlStencilOp.hpp"
#include "renderer/GlRenderer/Enum/GlTexLevelParameter.hpp"
#include "renderer/GlRenderer/Enum/GlTexParameter.hpp"
#include "renderer/GlRenderer/Enum/GlTextureType.hpp"
#include "renderer/GlRenderer/Enum/GlTextureUnit.hpp"
#include "renderer/GlRenderer/Enum/GlTextureViewType.hpp"
#include "renderer/GlRenderer/Enum/GlTweak.hpp"
#include "renderer/GlRenderer/Enum/GlValueName.hpp"
#include "renderer/GlRenderer/Enum/GlWrapMode.hpp"

#pragma warning( push )
#pragma warning( disable: 4365 )
#include <iostream>
#include <iomanip>
#include <sstream>
#include <locale>
#pragma warning( pop )

namespace ashes::gl
{
	class Context;
	class ContextLock;

	Context const & getContext( ContextLock const & lock );

	template< typename T >
	struct Stringifier
	{
		static inline std::string toString( T const value )
		{
			std::stringstream stream;
			stream.imbue( std::locale{ "C" } );
			stream << value;
			return stream.str();
		}
	};
	
	template<>
	struct Stringifier< uint8_t >
	{
		static inline std::string toString( uint8_t const value )
		{
			std::stringstream stream;
			stream.imbue( std::locale{ "C" } );
			stream << uint16_t( value );
			return stream.str();
		}
	};

	template< typename T >
	struct Stringifier< T * >
	{
		static inline std::string toString( T * const value )
		{
			if ( value )
			{
				std::stringstream stream;
				stream.imbue( std::locale{ "C" } );
				stream << "0x" << std::hex << std::setw( sizeof( intptr_t ) * 2u ) << std::setfill( '0' ) << intptr_t( value );
				return stream.str();
			}

			return std::string( "nullptr" );
		}
	};

	template< typename T >
	struct Stringifier< T const * >
	{
		static inline std::string toString( T const * const value )
		{
			if ( value )
			{
				std::stringstream stream;
				stream.imbue( std::locale{ "C" } );
				stream << "0x" << std::hex << std::setw( sizeof( intptr_t ) * 2u ) << std::setfill( '0' ) << intptr_t( value );
				return stream.str();
			}

			return std::string( "nullptr" );
		}
	};
	
	template< typename T >
	inline std::string toString( T const value )
	{
		return Stringifier< T >::toString( value );
	}

	inline std::string toString( std::nullptr_t )
	{
		return std::string( "nullptr" );
	}

	inline std::string toString( GLchar * txt )
	{
		if ( !txt )
		{
			return std::string( "nullstr" );
		}

		return std::string( reinterpret_cast< char const * >( txt ) );
	}

	template< typename ... ParamsT >
	struct GlParamLoggerRec;

	template<>
	struct GlParamLoggerRec<>
	{
		static inline void log( std::stringstream & stream )
		{
		}
	};

	template< typename ParamT >
	struct GlParamLoggerRec< ParamT >
	{
		static inline void log( std::stringstream & stream
			, ParamT const & last )
		{
			stream << toString( last );
		}
	};

	template< typename ParamT, typename ParamU >
	struct GlParamLoggerRec< ParamT, ParamU >
	{
		static inline void log( std::stringstream & stream
			, ParamT const & lastT
			, ParamU const & lastU )
		{
			if constexpr ( std::is_same_v< ParamU, GLuint * >
				&& ( std::is_same_v< ParamT, GLsizei >
					|| std::is_same_v< ParamT, unsigned int >
					|| std::is_same_v< ParamT, unsigned long >
					|| std::is_same_v< ParamT, unsigned long long > ) )
			{
				auto pElems = lastU;
				std::string sep = "[";

				for ( ParamT i = 0; i < lastT; ++i )
				{
					stream << sep << toString( *pElems );
					sep = ", ";
					++pElems;
				}

				stream << "]";
			}
			else if constexpr ( std::is_same_v< ParamU, const char ** >
				&& ( std::is_same_v< ParamT, GLsizei >
					|| std::is_same_v< ParamT, unsigned int >
					|| std::is_same_v< ParamT, unsigned long >
					|| std::is_same_v< ParamT, unsigned long long > ) )
			{
				auto pElems = lastU;
				std::string sep = "[";

				for ( ParamT i = 0; i < lastT; ++i )
				{
					stream << sep << "\n" << *pElems;
					sep = ",";
					++pElems;
				}

				stream << "\n]";
			}
			else
			{
				stream << toString( lastT ) << ", ";
				stream << toString( lastU );
			}
		}
	};

	template< typename ParamT, typename ParamU, typename ... ParamsT >
	struct GlParamLoggerRec< ParamT, ParamU, ParamsT... >
	{
		static inline void log( std::stringstream & stream
			, ParamT const & paramT
			, ParamU const & paramU
			, ParamsT ... params )
		{
			if constexpr ( ( std::is_same_v< ParamU, GLuint * >
					|| std::is_same_v< ParamU, const char ** > )
				&& ( std::is_same_v< ParamT, GLsizei >
					|| std::is_same_v< ParamT, unsigned int >
					|| std::is_same_v< ParamT, unsigned long >
					|| std::is_same_v< ParamT, unsigned long long > ) )
			{
				GlParamLoggerRec< ParamT, ParamU >::log( stream, paramT, paramU );
				stream << ", ";
				GlParamLoggerRec< ParamsT... >::log( stream, std::forward< ParamsT >( params )... );
			}
			else
			{
				GlParamLoggerRec< ParamT >::log( stream, paramT );
				stream << ", ";
				GlParamLoggerRec< ParamU, ParamsT... >::log( stream, paramU, std::forward< ParamsT >( params )... );
			}
		}
	};

	template< typename ... ParamsT >
	void logParams( std::stringstream & stream
		, ParamsT ... params )
	{
		stream << "(";
		GlParamLoggerRec< ParamsT... >::log( stream, std::forward< ParamsT >( params )... );
		stream << ")";
	}

	void logStream( std::stringstream & stream );

	template< typename FuncT, typename ... ParamsT >
	struct GlFuncCaller
	{
		static inline auto call( std::stringstream & stream
			, FuncT function
			, char const * const name
			, ParamsT ... params )
		{
			stream << name;
			logParams( stream, std::forward< ParamsT >( params )... );
			logStream( stream );
			return function( std::forward< ParamsT >( params )... );
		}

		static inline void callCreate( std::stringstream & stream
			, FuncT function
			, char const * const name
			, ParamsT ... params )
		{
			function( std::forward< ParamsT >( params )... );
			stream << name;
			logParams( stream, std::forward< ParamsT >( params )... );
			logStream( stream );
		}

		static inline auto callNonVoid( std::stringstream & stream
			, FuncT function
			, char const * const name
			, ParamsT ... params )
		{
			auto result = function( std::forward< ParamsT >( params )... );
			stream << toString( result ) << " = " << name;
			logParams( stream, std::forward< ParamsT >( params )... );
			logStream( stream );
			return result;
		}
	};

	template< typename FuncT >
	struct GlFuncCaller< FuncT, void >
	{
		static inline void call( std::stringstream & stream
			, FuncT function
			, char const * const name )
		{
			stream << name;
			logParams( stream );
			logStream( stream );
			function();
		}

		static inline void callCreate( std::stringstream & stream
			, FuncT function
			, char const * const name )
		{
			stream << name;
			logParams( stream );
			logStream( stream );
			function();
		}
	};

	template< typename FuncT, typename ... ParamsT >
	inline bool executeFunction( FuncT function
		, char const * const name
		, ParamsT ... params )
	{
		std::stringstream stream;
		GlFuncCaller< FuncT, ParamsT... >::call( stream
			, function
			, name
			, std::forward< ParamsT >( params )... );
		return true;
	}

	template< typename FuncT, typename ... ParamsT >
	inline bool executeFunction( ContextLock const & context
		, FuncT function
		, char const * const name
		, ParamsT ... params )
	{
		std::stringstream stream;
		GlFuncCaller< FuncT, ParamsT... >::call( stream
			, function
			, name
			, std::forward< ParamsT >( params )... );
		return glCheckError( context, name, false );
	}

	template< typename FuncT, typename ... ParamsT >
	inline bool executeCreateFunction( ContextLock const & context
		, FuncT function
		, char const * const name
		, ParamsT ... params )
	{
		std::stringstream stream;
		GlFuncCaller< FuncT, ParamsT... >::callCreate( stream
			, function
			, name
			, std::forward< ParamsT >( params )... );
		return glCheckError( context, name, false );
	}

	template< typename FuncT, typename ... ParamsT >
	inline auto executeNonVoidFunction( ContextLock const & context
		, FuncT function
		, char const * const name
		, ParamsT ... params )
	{
		std::stringstream stream;
		auto result = GlFuncCaller< FuncT, ParamsT... >::callNonVoid( stream
			, function
			, name
			, std::forward< ParamsT >( params )... );
		glCheckError( context, name, false );
		return result;
	}

	template< typename ... ParamsT >
	inline auto glCallCheckError( ContextLock const & context
		, char const * const name
		, ParamsT ... params )
	{
		return glCheckError( context
			, [name , &params...]()
			{
				std::stringstream stream;
				stream << name;
				logParams( stream, std::forward< ParamsT >( params )... );
				return stream.str();
			}
			, true );
	}

	inline auto glCallCheckOutOfMemory( ContextLock const & context )
	{
		return glCheckOutOfMemory( context );
	}

	inline auto glCallCheckError( ContextLock const & context
		, char const * const name )
	{
		return glCheckError( context
			, [name]()
			{
				std::stringstream stream;
				stream << name << "()";
				return stream.str();
			}
			, true );
	}

#if AshesGL_LogCalls && !defined( NDEBUG )
#	define glLogEmptyCall( lock, name )\
	executeFunction( lock, ashes::gl::getContext( lock ).m_##name, #name )
#	define glLogCall( lock, name, ... )\
	executeFunction( lock, ashes::gl::getContext( lock ).m_##name, #name, __VA_ARGS__ )
#	define glLogCreateCall( lock, name, ... )\
	executeCreateFunction( lock, ashes::gl::getContext( lock ).m_##name, #name, __VA_ARGS__ )
#	define glLogNonVoidCall( lock, name, ... )\
	executeNonVoidFunction( lock, ashes::gl::getContext( lock ).m_##name, #name, __VA_ARGS__ )
#	define glLogNonVoidEmptyCall( lock, name, ... )\
	executeNonVoidFunction( lock, ashes::gl::getContext( lock ).m_##name, #name )
#	define glLogCommand( list, name )\
	list.push_back( makeCmd< OpType::eLogCommand >( name ) );
#elif defined( NDEBUG )
#	define glLogEmptyCall( lock, name )\
	( ( lock->m_##name() ), glCallCheckOutOfMemory( lock ) )
#	define glLogCall( lock, name, ... )\
	( ( lock->m_##name( __VA_ARGS__ ) ), glCallCheckOutOfMemory( lock ) )
#	define glLogCreateCall( lock, name, ... )\
	( ( lock->m_##name( __VA_ARGS__ ) ), glCallCheckOutOfMemory( lock ) )
#	define glLogNonVoidCall( lock, name, ... )\
	( lock->m_##name( __VA_ARGS__ ) );\
	glCallCheckOutOfMemory( lock )
#	define glLogNonVoidEmptyCall( lock, name )\
	( lock->m_##name() );\
	glCallCheckOutOfMemory( lock )
#	define glLogCommand( list, name )
#else
#	define glLogEmptyCall( lock, name )\
	( ( lock->m_##name() ), glCallCheckError( lock, #name ) )
#	define glLogCall( lock, name, ... )\
	( ( lock->m_##name( __VA_ARGS__ ) ), glCallCheckError( lock, #name, __VA_ARGS__ ) )
#	define glLogCreateCall( lock, name, ... )\
	( ( lock->m_##name( __VA_ARGS__ ) ), glCallCheckError( lock, #name, __VA_ARGS__ ) )
#	define glLogNonVoidCall( lock, name, ... )\
	( lock->m_##name( __VA_ARGS__ ) );\
	glCallCheckError( lock, #name, __VA_ARGS__ )
#	define glLogNonVoidEmptyCall( lock, name )\
	( lock->m_##name() );\
	glCallCheckError( lock, #name )
#	define glLogCommand( list, name )
#endif
}
