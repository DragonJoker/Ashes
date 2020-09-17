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
#include "renderer/GlRenderer/Enum/GlFenceWaitFlag.hpp"
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

#include <iostream>
#include <iomanip>
#include <locale>
#include <sstream>

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

	inline std::string toString( GlAccessType value )
	{
		return getName( value );
	}

	inline std::string toString( GlAttachmentPoint value )
	{
		return getName( value );
	}

	inline std::string toString( GlAttachmentType value )
	{
		return getName( value );
	}

	inline std::string toString( GlBaseType value )
	{
		return getName( value );
	}

	inline std::string toString( GlBlendFactor value )
	{
		return getName( value );
	}

	inline std::string toString( GlBlendOp value )
	{
		return getName( value );
	}

	inline std::string toString( GlBufferParameter value )
	{
		return getName( value );
	}

	inline std::string toString( GlBufferTarget value )
	{
		return getName( value );
	}

	inline std::string toString( GlClearTarget value )
	{
		return getName( value );
	}

	inline std::string toString( GlClipDepth value )
	{
		return getName( value );
	}

	inline std::string toString( GlClipOrigin value )
	{
		return getName( value );
	}

	inline std::string toString( GlCompareOp value )
	{
		return getName( value );
	}

	inline std::string toString( GlComponentSwizzle value )
	{
		return getName( value );
	}

	inline std::string toString( GlCullMode value )
	{
		return getName( value );
	}

	inline std::string toString( GlFenceWaitFlag value )
	{
		return getName( value );
	}

	inline std::string toString( GlFilter value )
	{
		return getName( value );
	}

	inline std::string toString( GlFrontFace value )
	{
		return getName( value );
	}

	inline std::string toString( GlFormat value )
	{
		return getName( value );
	}

	inline std::string toString( GlFormatProperty value )
	{
		return getName( value );
	}

	inline std::string toString( GlFrameBufferTarget value )
	{
		return getName( value );
	}

	inline std::string toString( GlGetParameter value )
	{
		return getName( value );
	}

	inline std::string toString( GlImageAspectFlags value )
	{
		return getName( value );
	}

	inline std::string toString( GlImageLayout value )
	{
		return getName( value );
	}

	inline std::string toString( GlImageTiling value )
	{
		return getName( value );
	}

	inline std::string toString( GlIndexType value )
	{
		return getName( value );
	}

	inline std::string toString( GlInternal value )
	{
		return getName( value );
	}

	inline std::string toString( GlLogicOp value )
	{
		return getName( value );
	}

	inline std::string toString( GlMemoryMapFlags value )
	{
		return getName( value );
	}

	inline std::string toString( gl3::GlBufferMemoryPropertyFlag value )
	{
		return getName( value );
	}

	inline std::string toString( gl4::GlMemoryPropertyFlags value )
	{
		return getName( value );
	}

	inline std::string toString( GlPolygonMode value )
	{
		return getName( value );
	}

	inline std::string toString( GlPrimitiveTopology value )
	{
		return getName( value );
	}

	inline std::string toString( GlQueryResultFlags value )
	{
		return getName( value );
	}

	inline std::string toString( GlQueryType value )
	{
		return getName( value );
	}

	inline std::string toString( GlSamplerParameter value )
	{
		return getName( value );
	}

	inline std::string toString( GlShaderInfo value )
	{
		return getName( value );
	}

	inline std::string toString( GlShaderStage value )
	{
		return getShaderStageName( value );
	}

	inline std::string toString( GlShaderStageFlags value )
	{
		return getShaderStageFlagsName( value );
	}

	inline std::string toString( GlStencilOp value )
	{
		return getName( value );
	}

	inline std::string toString( GlSwizzle value )
	{
		return getName( value );
	}

	inline std::string toString( GlTexLevelParameter value )
	{
		return getName( value );
	}

	inline std::string toString( GlTexParameter value )
	{
		return getName( value );
	}

	inline std::string toString( GlTextureType value )
	{
		return getName( value );
	}

	inline std::string toString( gl4::GlTextureViewType value )
	{
		return getName( value );
	}

	inline std::string toString( GlTextureUnit value )
	{
		return getName( value );
	}

	inline std::string toString( GlTweak value )
	{
		return getName( value );
	}

	inline std::string toString( GlType value )
	{
		return getName( value );
	}

	inline std::string toString( GlWrapMode value )
	{
		return getName( value );
	}

	inline std::string toString( GlValueName value )
	{
		return getName( value );
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

	template< typename ParamT, typename ... ParamsT >
	struct GlParamLoggerRec< ParamT, ParamsT... >
	{
		static inline void log( std::stringstream & stream
			, ParamT const & param
			, ParamsT ... params )
		{
			stream << toString( param ) + ", ";
			GlParamLoggerRec< ParamsT... >::log( stream, std::forward< ParamsT >( params )... );
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
		return glCheckError( context, name );
	}

	template< typename FuncT, typename ... ParamsT >
	inline auto executeNonVoidFunction( ContextLock const & context
		, FuncT function
		, char const * const name
		, ParamsT ... params )
	{
		std::stringstream stream;
		auto result = GlFuncCaller< FuncT, ParamsT... >::call( stream
			, function
			, name
			, std::forward< ParamsT >( params )... );
		glCheckError( context, name );
		return result;
	}

	template< typename ... ParamsT >
	inline auto glCallCheckError( ContextLock const & context
		, char const * const name
		, ParamsT ... params )
	{
		std::stringstream stream;
		stream << name;
		logParams( stream, std::forward< ParamsT >( params )... );
		return glCheckError( context, stream.str() );
	}

	inline auto glCallCheckError( ContextLock const & context
		, char const * const name )
	{
		std::stringstream stream;
		stream << name << "()";
		return glCheckError( context, stream.str() );
	}

#if AshesGL_LogCalls
#	define glLogEmptyCall( lock, name )\
	executeFunction( lock, ashes::gl::getContext( lock ).m_##name, #name )
#	define glLogCall( lock, name, ... )\
	executeFunction( lock, ashes::gl::getContext( lock ).m_##name, #name, __VA_ARGS__ )
#	define glLogNonVoidCall( lock, name, ... )\
	executeNonVoidFunction( lock, ashes::gl::getContext( lock ).m_##name, #name, __VA_ARGS__ )
#	define glLogNonVoidEmptyCall( lock, name, ... )\
	executeNonVoidFunction( lock, ashes::gl::getContext( lock ).m_##name, #name )
#	define glLogCommand( list, name )\
	list.push_back( makeCmd< OpType::eLogCommand >( name ) );
#elif defined( NDEBUG )
#	define glLogEmptyCall( lock, name )\
	( ( lock->m_##name() ), true )
#	define glLogCall( lock, name, ... )\
	( ( lock->m_##name( __VA_ARGS__ ) ), true )
#	define glLogNonVoidCall( lock, name, ... )\
	( lock->m_##name( __VA_ARGS__ ) )
#	define glLogNonVoidEmptyCall( lock, name )\
	( lock->m_##name() );
#	define glLogCommand( list, name )
#else
#	define glLogEmptyCall( lock, name )\
	( ( lock->m_##name() ), glCallCheckError( lock, #name ) )
#	define glLogCall( lock, name, ... )\
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
