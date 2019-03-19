/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "Ashes/Core/Instance.hpp"

#include "Ashes/Core/Surface.hpp"
#include "Ashes/Miscellaneous/QueueCreateInfo.hpp"
#include "Ashes/Utils/CallStack.hpp"

#include <cmath>

namespace ashes
{
	void addOptionalValidationLayer( std::string const & layer
		, std::string description
		, ashes::StringArray & names )
	{
#if LOAD_VALIDATION_LAYERS
		if ( layerName.find( "validation" ) != std::string::npos
			|| description.find( "LunarG Validation" ) != std::string::npos )
		{
			names.push_back( layerName );
		}
#endif
	}

	void addOptionalDebugReportLayer( ashes::StringArray & names )
	{
#if LOAD_VALIDATION_LAYERS
		names.push_back( VK_EXT_DEBUG_REPORT_EXTENSION_NAME );
		//names.push_back( VK_EXT_DEBUG_MARKER_EXTENSION_NAME );
#endif
	}

	Instance::Instance( ClipDirection clipDirection
		, std::string const & name
		, Configuration const & configuration )
		: m_configuration{ configuration }
		, m_clipDirection{ clipDirection }
		, m_name{ name }
	{
#ifndef NDEBUG

		callstack::initialise();

#endif
	}

	Instance::~Instance()
	{
#ifndef NDEBUG

		callstack::cleanup();

#endif
	}

	PhysicalDevice & Instance::getPhysicalDevice( uint32_t gpuIndex )const
	{
		if ( gpuIndex >= m_gpus.size() )
		{
			throw std::logic_error( "Invalid GPU index" );
		}

		return *m_gpus[gpuIndex];
	}

	std::array< float, 16 > Instance::infinitePerspective( float radiansFovY
		, float aspect
		, float zNear )const
	{
		float const range = tan( radiansFovY / float( 2 ) ) * zNear;
		float const left = -range * aspect;
		float const right = range * aspect;
		float const bottom = -range;
		float const top = range;

		std::array< float, 16 > result{ 0.0f };
		result[0] = ( float( 2 ) * zNear ) / ( right - left );
		result[5] = ( float( 2 ) * zNear ) / ( top - bottom );
		result[10] = -float( 1 );
		result[11] = -float( 1 );
		result[14] = -float( 2 ) * zNear;
		return result;
	}

	void Instance::completeLayerNames( ashes::StringArray & names )const
	{
		for ( auto const & props : m_layers )
		{
			addOptionalValidationLayer( props.layerName
				, props.description
				, names );
		}
	}
}
