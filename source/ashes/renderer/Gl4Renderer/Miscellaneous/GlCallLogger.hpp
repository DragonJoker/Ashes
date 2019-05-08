/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include "renderer/Gl4Renderer/Enum/GlAccessFlag.hpp"
#include "renderer/Gl4Renderer/Enum/GlAttachmentPoint.hpp"
#include "renderer/Gl4Renderer/Enum/GlAttachmentType.hpp"
#include "renderer/Gl4Renderer/Enum/GlBaseType.hpp"
#include "renderer/Gl4Renderer/Enum/GlBlendFactor.hpp"
#include "renderer/Gl4Renderer/Enum/GlBlendOp.hpp"
#include "renderer/Gl4Renderer/Enum/GlBorderColour.hpp"
#include "renderer/Gl4Renderer/Enum/GlBufferTarget.hpp"
#include "renderer/Gl4Renderer/Enum/GlClearTarget.hpp"
#include "renderer/Gl4Renderer/Enum/GlClipInfo.hpp"
#include "renderer/Gl4Renderer/Enum/GlColourComponentFlag.hpp"
#include "renderer/Gl4Renderer/Enum/GlCompareOp.hpp"
#include "renderer/Gl4Renderer/Enum/GlComponentSwizzle.hpp"
#include "renderer/Gl4Renderer/Enum/GlConstantFormat.hpp"
#include "renderer/Gl4Renderer/Enum/GlCullModeFlag.hpp"
#include "renderer/Gl4Renderer/Enum/GlDebugReportObjectType.hpp"
#include "renderer/Gl4Renderer/Enum/GlFenceWaitFlag.hpp"
#include "renderer/Gl4Renderer/Enum/GlFilter.hpp"
#include "renderer/Gl4Renderer/Enum/GlFormat.hpp"
#include "renderer/Gl4Renderer/Enum/GlFrameBufferTarget.hpp"
#include "renderer/Gl4Renderer/Enum/GlFrontFace.hpp"
#include "renderer/Gl4Renderer/Enum/GlGetParameter.hpp"
#include "renderer/Gl4Renderer/Enum/GlImageAspectFlag.hpp"
#include "renderer/Gl4Renderer/Enum/GlImageLayout.hpp"
#include "renderer/Gl4Renderer/Enum/GlImageTiling.hpp"
#include "renderer/Gl4Renderer/Enum/GlIndexType.hpp"
#include "renderer/Gl4Renderer/Enum/GlLogicOp.hpp"
#include "renderer/Gl4Renderer/Enum/GlMemoryBarrierFlag.hpp"
#include "renderer/Gl4Renderer/Enum/GlMemoryMapFlag.hpp"
#include "renderer/Gl4Renderer/Enum/GlMemoryPropertyFlag.hpp"
#include "renderer/Gl4Renderer/Enum/GlMipmapMode.hpp"
#include "renderer/Gl4Renderer/Enum/GlPolygonMode.hpp"
#include "renderer/Gl4Renderer/Enum/GlPrimitiveTopology.hpp"
#include "renderer/Gl4Renderer/Enum/GlQueryResultFlag.hpp"
#include "renderer/Gl4Renderer/Enum/GlQueryType.hpp"
#include "renderer/Gl4Renderer/Enum/GlSampleCountFlag.hpp"
#include "renderer/Gl4Renderer/Enum/GlSamplerParameter.hpp"
#include "renderer/Gl4Renderer/Enum/GlShaderBinaryFormat.hpp"
#include "renderer/Gl4Renderer/Enum/GlShaderInfo.hpp"
#include "renderer/Gl4Renderer/Enum/GlShaderStageFlag.hpp"
#include "renderer/Gl4Renderer/Enum/GlStencilOp.hpp"
#include "renderer/Gl4Renderer/Enum/GlTexLevelParameter.hpp"
#include "renderer/Gl4Renderer/Enum/GlTexParameter.hpp"
#include "renderer/Gl4Renderer/Enum/GlTextureType.hpp"
#include "renderer/Gl4Renderer/Enum/GlTextureUnit.hpp"
#include "renderer/Gl4Renderer/Enum/GlTextureViewType.hpp"
#include "renderer/Gl4Renderer/Enum/GlTweak.hpp"
#include "renderer/Gl4Renderer/Enum/GlWrapMode.hpp"

#include <iostream>
#include <iomanip>
#include <locale>
#include <sstream>

#if !defined( NDEBUG )
#	define GL_LOG_CALLS 0
#else
#	define GL_LOG_CALLS 0
#endif

namespace ashes::gl4
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

	inline std::string toString( GlMemoryPropertyFlags value )
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

	inline std::string toString( GlShaderStageFlags value )
	{
		return getShaderStageFlagName( value );
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

	inline std::string toString( GlTextureViewType value )
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
			std::cout << stream.str() << std::endl;
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
			std::cout << std::string{ name } << "()" << std::endl;
			function();
		}
	};

	template< typename FuncT, typename ... ParamsT >
	inline auto executeFunction( FuncT function
		, char const * const name
		, ParamsT ... params )
	{
		std::stringstream stream;
		return GlFuncCaller< FuncT, ParamsT... >::call( stream
			, function
			, name
			, std::forward< ParamsT >( params )... );
	}

#if GL_LOG_CALLS
#	define glLogCall( lock, name, ... )\
	executeFunction( ashes::gl4::getContext( lock ).m_##name, #name, __VA_ARGS__ )
#	define glLogCommand( name )\
	std::cout << "Command: " << name << std::endl
#elif defined( NDEBUG )
#	define glLogCall( lock, name, ... )\
	( lock->m_##name( __VA_ARGS__ ) )
#	define glLogCommand( name )
#	else
#	define glLogCall( lock, name, ... )\
	( lock->m_##name( __VA_ARGS__ ) );\
	glCheckError( #name )
#	define glLogCommand( name )
#endif
}
