#include "ashes/ashes.h"

#include <common/DynamicLibrary.hpp>
#include <common/FileUtils.hpp>

#include <algorithm>
#include <cassert>
#include <cstring>
#include <functional>
#include <iostream>
#include <memory>
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

using PluginArray = std::vector< Plugin >;

inline PluginArray listPlugins()
{
	PluginArray result;
	ashes::StringArray files;

	if ( ashes::listDirectoryFiles( ashes::getExecutableDirectory(), files, false ) )
	{
		for ( auto & file : files )
		{
			if ( file.find( ".dll" ) != std::string::npos
				|| file.find( ".so" ) != std::string::npos )
			try
			{
				result.emplace_back( std::make_unique< ashes::DynamicLibrary >( file ) );
			}
			catch ( std::exception & exc )
			{
				std::clog << exc.what() << std::endl;
			}
		}

		std::sort( result.begin()
			, result.end()
			, []( Plugin const & lhs, Plugin const & rhs )
			{
				return lhs.description.support.priority > rhs.description.support.priority;
			} );
	}

	return result;
}

bool operator==( AshPluginFeatures const & lhs, AshPluginFeatures const & rhs )
{
	return memcmp( &lhs, &rhs, sizeof( AshPluginFeatures ) ) == 0;
}

bool operator==( AshPluginSupport const & lhs, AshPluginSupport const & rhs )
{
	return memcmp( &lhs, &rhs, sizeof( AshPluginSupport ) ) == 0;
}

bool operator==( AshPluginDescription const & lhs, AshPluginDescription const & rhs )
{
	return strncmp( lhs.description, rhs.description, 63 ) == 0
		&& strncmp( lhs.name, rhs.name, 15 ) == 0
		&& lhs.features == rhs.features
		&& lhs.support == rhs.support;
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
			plugins = listPlugins();

			if ( plugins.empty() )
			{
				result = VK_ERROR_INITIALIZATION_FAILED;
			}
			else
			{
				selectedPlugin = &plugins.front();
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
