#define VK_NO_PROTOTYPES
#include "ashes/ashes.h"

#include <ashes/common/DynamicLibrary.hpp>
#include <ashes/common/FileUtils.hpp>

#include <algorithm>
#include <cassert>
#include <cstring>
#include <functional>
#include <iostream>
#include <memory>
#include <regex>
#include <set>
#include <vector>

struct Plugin
{
	std::unique_ptr< ashes::DynamicLibrary > library;
	AshPluginDescription description;

	inline Plugin( std::unique_ptr< ashes::DynamicLibrary > lib )
		: library{ std::move( lib ) }
	{
		PFN_ashGetPluginDescription getPluginDescription;

		if ( !library->getFunction( "ashGetPluginDescription", getPluginDescription ) )
		{
			throw std::runtime_error{ "[" + ashes::getFileName( library->getPath() ) + "] is not a renderer plugin" };
		}

		getPluginDescription( &description );
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
	PluginArray listPlugins();
}

struct PluginLibrary
{
	inline PluginLibrary()
	{
		getSelectedDesc = [this]() -> AshPluginDescription &
		{
			auto res = init();
			assert( res == VK_SUCCESS );
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
				selectedPlugin = details::findFirstSupportedPlugin( plugins );
				result = VK_SUCCESS;
			}

			getSelectedDesc = [this]() -> AshPluginDescription &
			{
				return selectedPlugin->description;
			};
		}

		return result;
	}

	inline VkResult selectDesc( AshPluginDescription const & description )
	{
		auto res = init();
		assert( res == VK_SUCCESS );
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
	Plugin * selectedPlugin{ nullptr };
	SelectedDescGetter getSelectedDesc;
};
