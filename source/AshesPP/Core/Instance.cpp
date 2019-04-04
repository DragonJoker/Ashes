/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "AshesPP/Core/Instance.hpp"

#include "AshesPP/Core/Device.hpp"
#include "AshesPP/Core/Surface.hpp"
#include "AshesPP/Utils/CallStack.hpp"

#include <cmath>

namespace ashes
{
	Instance::Instance( Renderer const & renderer
		, VkInstanceCreateInfo createInfo )
		: m_renderer{ renderer }
		, m_createInfo{ std::move( createInfo ) }
	{
#ifndef NDEBUG

		callstack::initialise();

#endif

		m_features.hasTexBufferRange = true;
		m_features.hasImageTexture = true;
		m_features.hasBaseInstance = true;
		m_features.hasClearTexImage = true;
		m_features.hasComputeShaders = true;
		m_features.hasStorageBuffers = true;
		m_features.supportsPersistentMapping = true;

#define VK_LIB_GLOBAL_FUNCTION( fun ) vk##fun = reinterpret_cast< PFN_vk##fun >( m_renderer.getInstanceProcAddr( nullptr, "vk"#fun ) );
#include <AshesCommon/VulkanFunctionsList.inl>

		doInitInstance();
	}

	Instance::~Instance()
	{
		if ( m_instance != VK_NULL_HANDLE )
		{
			vkDestroyInstance( m_instance, nullptr );
		}

		DEBUG_WRITE( "VkInstance.log" );

#ifndef NDEBUG

		callstack::cleanup();

#endif
	}

	std::array< float, 16 > Instance::frustum( float left
		, float right
		, float bottom
		, float top
		, float zNear
		, float zFar )const
	{
		std::array< float, 16 > result{ 0.0f };
		result[0] = ( float( 2 ) * zNear ) / ( right - left );
		result[5] = ( float( 2 ) * zNear ) / ( top - bottom );
		result[8] = ( right + left ) / ( right - left );
		result[9] = ( top + bottom ) / ( top - bottom );
		result[11] = float( -1 );
		result[10] = zFar / ( zNear - zFar );
		result[14] = -( zFar * zNear ) / ( zFar - zNear );

		return result;
	}

	std::array< float, 16 > Instance::perspective( float radiansFovY
		, float aspect
		, float zNear
		, float zFar )const
	{
		float const tanHalfFovy = tan( radiansFovY / float( 2 ) );

		std::array< float, 16 > result{ 0.0f };
		result[0] = float( 1 ) / ( aspect * tanHalfFovy );
		result[5] = float( 1 ) / ( tanHalfFovy );
		result[11] = -float( 1 );
		result[10] = zFar / ( zNear - zFar );
		result[14] = -( zFar * zNear ) / ( zFar - zNear );

		return result;
	}

	std::array< float, 16 > Instance::ortho( float left
		, float right
		, float bottom
		, float top
		, float zNear
		, float zFar )const
	{
		std::array< float, 16 > result{ 0.0f };
		result[0] = float( 2 ) / ( right - left );
		result[5] = float( 2 ) / ( top - bottom );
		result[12] = -( right + left ) / ( right - left );
		result[13] = -( top + bottom ) / ( top - bottom );
		result[10] = -float( 1 ) / ( zFar - zNear );
		result[14] = -zNear / ( zFar - zNear );
		result[15] = 1.0f;

		return result;
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

	PhysicalDevicePtrArray Instance::enumeratePhysicalDevices()const
	{
		uint32_t gpuCount{ 0u };
		auto res = vkEnumeratePhysicalDevices( m_instance
			, &gpuCount
			, nullptr );
		checkError( res, "GPU enumeration" );

		if ( !gpuCount )
		{
			checkError( VK_ERROR_FEATURE_NOT_PRESENT, "Vulkan supporting GPU" );
		}

		std::vector< VkPhysicalDevice > gpus( gpuCount, VK_NULL_HANDLE );
		res = vkEnumeratePhysicalDevices( m_instance
			, &gpuCount
			, gpus.data() );
		checkError( res, "GPU enumeration" );

		PhysicalDevicePtrArray result;

		for ( auto gpu : gpus )
		{
			result.push_back( std::make_unique< PhysicalDevice >( *this, gpu ) );
		}

		return result;
	}

	DevicePtr Instance::createDevice( SurfacePtr surface
		, VkDeviceCreateInfo createInfos )const
	{
		DevicePtr result;

		try
		{
			auto & gpu = surface->getGpu();
			result = std::make_shared< Device >( *this
				, gpu
				, std::move( surface )
				, std::move( createInfos ) );
		}
		catch ( std::exception & exc )
		{
			Logger::logError( std::string{ "Could not initialise logical device:\n" } +exc.what() );
		}

		return result;
	}

	SurfacePtr Instance::createSurface( PhysicalDevice const & gpu
		, WindowHandle handle )const
	{
		return std::make_unique< Surface >( *this
			, gpu
			, std::move( handle ) );
	}

	VkDebugReportCallbackEXT Instance::createDebugReportCallback( VkDebugReportCallbackCreateInfoEXT & createInfo )const
	{
		VkDebugReportCallbackEXT result;
		auto res = vkCreateDebugReportCallbackEXT( m_instance
			, &createInfo
			, nullptr
			, &result );
		checkError( res, "Debug report callback creation" );
		return result;
	}

	void Instance::doInitInstance()
	{
		DEBUG_DUMP( m_createInfo );
		auto res = vkCreateInstance( &m_createInfo, nullptr, &m_instance );
		checkError( res, "Instance creation" );

#define VK_LIB_INSTANCE_FUNCTION( fun ) vk##fun = reinterpret_cast< PFN_vk##fun >( getInstanceProcAddr( "vk"#fun ) );
#include <AshesCommon/VulkanFunctionsList.inl>
	}

	PFN_vkVoidFunction Instance::getInstanceProcAddr( char const * const name )
	{
		auto result = m_renderer.getInstanceProcAddr( m_instance, name );

		if ( !result )
		{
			Logger::logError( std::string{ "Couldn't load function " } +name );
		}

		return result;
	}
}
