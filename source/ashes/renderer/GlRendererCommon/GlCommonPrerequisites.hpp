/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include <renderer/RendererCommon/AshesRendererPrerequisites.hpp>
#include <renderer/RendererCommon/Helper/ConstantFormat.hpp>

#include <ashes/common/Format.hpp>

#include <cassert>
#include <functional>
#include <map>
#include <memory>
#include <string>
#include <vector>

namespace ashes::gl
{
	PFN_vkVoidFunction getFunction( char const * const name );

	namespace details
	{
		template< typename FuncT >
		inline bool getFunctionRec( std::string const & name
			, FuncT & function )
		{
			function = FuncT( ashes::gl::getFunction( name.c_str() ) );
			return function != nullptr;
		}

		template< typename FuncT, typename ParamT >
		inline bool getFunctionRec( std::string const & name
			, FuncT & function
			, char const * const last )
		{
			function = FuncT( ashes::gl::getFunction( ( name + last ).c_str() ) );
			return function != nullptr;
		}

		template< typename FuncT, typename ... ParamsT >
		inline bool getFunctionRec( std::string const & name
			, FuncT & function
			, char const * const current
			, ParamsT ... params )
		{
			function = FuncT( ashes::gl::getFunction( ( name + current ).c_str() ) );

			if ( !function )
			{
				return getFunctionRec( name, function, params... );
			}

			return function != nullptr;
		}
	}

	template< typename FuncT, typename ... ParamsT >
	inline bool getFunction( char const * const name
		, FuncT & function
		, ParamsT ... params )
	{
		return details::getFunctionRec( std::string{ name ? name : "" }, function, params... );
	}

	template< typename FuncT, typename ... ParamsT >
	inline bool getFunction( std::string const & name
		, FuncT & function
		, ParamsT ... params )
	{
		return details::getFunctionRec( name, params... );
	}

	inline void * getBufferOffset( intptr_t value )
	{
		return reinterpret_cast< void * >( reinterpret_cast< uint8_t * >( 0u ) + value );
	}

	struct ContextState;
	class ContextImpl;
	class ExtensionsHandler;
	class RenderWindow;

	using ContextImplPtr = std::unique_ptr< ContextImpl >;
	using ContextStateArray = std::vector< ContextState >;

	uint32_t getScreenIndex( VkDisplayModeKHR displayMode );
	VkDisplayModeParametersKHR getDisplayModeParameters( VkDisplayModeKHR displayMode );
	VkExtent2D getDisplayResolution( VkDisplayModeKHR displayMode );
}
