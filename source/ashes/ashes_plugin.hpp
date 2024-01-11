#ifndef VK_NO_PROTOTYPES
#	define VK_NO_PROTOTYPES
#endif
#include "ashes/ashes.h"

#include <ashes/common/DynamicLibrary.hpp>
#include <ashes/common/Exception.hpp>
#include <ashes/common/FileUtils.hpp>

#pragma warning( push )
#pragma warning( disable: 5262 )
#include <algorithm>
#include <cassert>
#include <cstring>
#include <functional>
#include <iostream>
#include <memory>
#include <regex>
#include <set>
#include <stdexcept>
#include <vector>
#pragma warning( pop )

struct Plugin
{
	using PFN_ashUpdatePluginSupport = VkResult( VKAPI_PTR * )();

	std::unique_ptr< ashes::DynamicLibrary > library;
	PFN_ashGetPluginDescription fnGetPluginDescription;
	AshPluginDescription description;

	Plugin( std::unique_ptr< ashes::DynamicLibrary > lib )
		: library{ std::move( lib ) }
	{

		if ( !library->getFunction( "ashGetPluginDescription", fnGetPluginDescription ) )
		{
			throw ashes::BaseException{ "[" + ashes::getFileName( library->getPath() ) + "] is not a renderer plugin" };
		}

		fnGetPluginDescription( &description );
	}
};

inline bool operator==( AshPluginFeatures const & lhs, AshPluginFeatures const & rhs )
{
	return memcmp( &lhs, &rhs, sizeof( AshPluginFeatures ) ) == 0;
}

inline bool operator==( AshPluginSupport const & lhs, AshPluginSupport const & rhs )
{
	return memcmp( &lhs, &rhs, sizeof( AshPluginSupport ) ) == 0;
}

inline bool operator==( AshPluginDescription const & lhs, AshPluginDescription const & rhs )
{
	return strncmp( lhs.description, rhs.description, 63 ) == 0
		&& strncmp( lhs.name, rhs.name, 15 ) == 0
		&& lhs.features == rhs.features
		&& lhs.support == rhs.support;
}

using PluginArray = std::vector< Plugin >;

namespace details
{
	Plugin * findFirstSupportedPlugin( PluginArray & plugins );
	Plugin * findDefaultPlugin( PluginArray & plugins );
	PluginArray listPlugins();
}

struct PluginLibrary
{
	inline PluginLibrary()
	{
		getSelectedDesc = [this]() -> AshPluginDescription &
		{
			( void )init();
			return getSelectedDesc();
		};
	}

	inline VkResult init()
	{
		VkResult result = VK_SUCCESS;

		if ( !selectedPlugin )
		{
			plugins = details::listPlugins();

			if ( plugins.empty() )
			{
				result = VK_ERROR_INITIALIZATION_FAILED;
			}
			else
			{
				selectedPlugin = details::findDefaultPlugin( plugins );

				if ( !selectedPlugin )
				{
					selectedPlugin = details::findFirstSupportedPlugin( plugins );
				}

				if ( selectedPlugin )
				{
					result = VK_SUCCESS;
				}
				else
				{
					std::cerr << "Couldn't find a suitable plugin" << std::endl;
					result = VK_ERROR_INITIALIZATION_FAILED;
				}
			}

			if ( result == VK_SUCCESS )
			{
				getSelectedDesc = [this]()noexcept -> AshPluginDescription &
				{
					return selectedPlugin->description;
				};
			}
		}

		assert( result == VK_SUCCESS );
		return result;
	}

	inline VkResult selectDesc( AshPluginDescription const & description )
	{
		( void )init();
		auto it = std::find_if( plugins.begin()
			, plugins.end()
			, [&description]( Plugin const & lookup )
			{
				return description == lookup.description;
			} );

		if ( it == plugins.end() )
		{
			return VK_ERROR_INITIALIZATION_FAILED;
		}

		selectedPlugin = &( *it );
		return VK_SUCCESS;
	}

	using SelectedDescGetter = std::function< AshPluginDescription & () >;
	PluginArray plugins;
	bool isUsingICD{ false };
	Plugin * selectedPlugin{ nullptr };
	SelectedDescGetter getSelectedDesc;
};
