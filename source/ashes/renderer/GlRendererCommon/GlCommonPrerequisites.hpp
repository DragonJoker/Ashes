/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include <renderer/RendererCommon/AshesRendererPrerequisites.hpp>
#include <renderer/RendererCommon/Helper/ConstantFormat.hpp>

#include <common/Format.hpp>

#include <cassert>
#include <functional>
#include <map>
#include <memory>
#include <string>
#include <vector>

namespace ashes::gl
{
	PFN_vkVoidFunction getFunction( char const * const name );

	template< typename FuncT >
	inline bool getFunction( char const * const name, FuncT & function )
	{
		function = FuncT( getFunction( name ) );
		return function != nullptr;
	}

	template< typename FuncT >
	inline bool getFunction( std::string const & name, FuncT & function )
	{
		function = FuncT( getFunction( name.c_str() ) );
		return function != nullptr;
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
