#include "ashes/ashes.h"

#include <AshesCommon/DynamicLibrary.hpp>
#include <AshesCommon/FileUtils.hpp>

#include <cassert>
#include <iostream>
#include <memory>
#include <vector>

struct Plugin
{
	std::unique_ptr< ashes::DynamicLibrary > library;
	AshPluginDescription description;

	Plugin( std::unique_ptr< ashes::DynamicLibrary > lib )
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

PluginArray listPlugins()
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

	}

	return result;
}

struct PluginLibrary
{
	VkResult init()
	{
		VkResult result = VK_SUCCESS;

		if ( !selectedPugin )
		{
			plugins = listPlugins();

			if ( plugins.empty() )
			{
				result = VK_ERROR_INITIALIZATION_FAILED;
			}
			else
			{
				selectedPugin = &plugins.front();
				std::cout << "Using " << selectedPugin->description.description << std::endl;
				result = VK_SUCCESS;
			}
		}

		return result;
	}

	PluginArray plugins;
	Plugin * selectedPugin{ nullptr };
};

thread_local PluginLibrary g_library;

extern "C"
{
	Ashes_API PFN_vkVoidFunction VKAPI_PTR vkGetInstanceProcAddr( VkInstance instance
		, const char * name )
	{
		if ( g_library.init() == VK_SUCCESS )
		{
			return g_library.selectedPugin->description.getInstanceProcAddr( instance, name );
		}

		return nullptr;
	}

	Ashes_API void VKAPI_PTR ashGetPluginsDescriptions( uint32_t * count
		, AshPluginDescription * pDescriptions )
	{
		if ( g_library.init() == VK_SUCCESS )
		{
			*count = uint32_t( g_library.plugins.size() );

			if ( pDescriptions )
			{
				for ( auto & plugin : g_library.plugins )
				{
					*pDescriptions = plugin.description;
					++pDescriptions;
				}
			}
		}
	}

	Ashes_API VkResult VKAPI_PTR ashGetCurrentPluginDescription( AshPluginDescription * description )
	{
		auto result = g_library.init();

		if ( result == VK_SUCCESS )
		{
			*description = g_library.selectedPugin->description;
		}

		return result;
	}
}
