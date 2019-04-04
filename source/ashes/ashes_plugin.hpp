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

	}

	return result;
}

struct PluginLibrary
{
	inline VkResult init()
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

	inline AshPluginDescription & getSelectedDesc()
	{
		auto res = init();
		assert( res == VK_SUCCESS );
		return selectedPugin->description;
	}

	PluginArray plugins;
	Plugin * selectedPugin{ nullptr };
};
