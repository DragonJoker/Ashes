/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "ashespp/Core/Instance.hpp"

#include "ashespp/Core/Device.hpp"
#include "ashespp/Core/Surface.hpp"

#include <cmath>

#pragma warning( disable: 4191 ) // warning C4191: 'reinterpret_cast': unsafe conversion from 'PFN_vkVoidFunction' to ...
#pragma clang diagnostic ignored "-Wimplicit-float-conversion"

namespace ashes
{
	Instance::Instance( AshPluginDescription const & plugin
		, DeviceAllocatorPtr allocator
		, ashes::InstanceCreateInfo createInfo )
		: m_getInstanceProcAddr{ plugin.getInstanceProcAddr }
		, m_allocator{ std::move( allocator ) }
		, m_createInfo{ std::move( createInfo ) }
		, m_features{ plugin.features }
	{
#define VK_LIB_GLOBAL_FUNCTION( ver, fun )\
		vk##fun = reinterpret_cast< PFN_vk##fun >( m_getInstanceProcAddr( nullptr, "vk"#fun ) );
#define VK_LIB_GLOBAL_FUNCTION_EXT( ver, ext, fun )\
		vk##fun = reinterpret_cast< PFN_vk##fun >( m_getInstanceProcAddr( nullptr, "vk"#fun ) );
#	include <ashes/ashes_functions_list.hpp>

		doInitInstance();
	}

	Instance::Instance( PFN_vkGetInstanceProcAddr pgetInstanceProcAddr
		, DeviceAllocatorPtr allocator
		, ashes::InstanceCreateInfo createInfo )
		: m_getInstanceProcAddr{ pgetInstanceProcAddr }
		, m_allocator{ std::move( allocator ) }
		, m_createInfo{ std::move( createInfo ) }
		, m_features{ VK_TRUE, VK_TRUE, VK_TRUE, VK_TRUE, VK_TRUE, VK_TRUE, VK_TRUE, makeVersion( 1, 0, 0 ) }
	{
#define VK_LIB_GLOBAL_FUNCTION( ver, fun )\
		vk##fun = reinterpret_cast< PFN_vk##fun >( m_getInstanceProcAddr( nullptr, "vk"#fun ) );
#define VK_LIB_GLOBAL_FUNCTION_EXT( ver, ext, fun )\
		vk##fun = reinterpret_cast< PFN_vk##fun >( m_getInstanceProcAddr( nullptr, "vk"#fun ) );
#	include <ashes/ashes_functions_list.hpp>

		doInitInstance();
	}

	Instance::~Instance()noexcept
	{
		if ( m_instance != nullptr )
		{
			vkDestroyInstance( m_instance, nullptr );
		}

		DEBUG_WRITE( "VkInstance.log" )
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
		auto const tanHalfFovy = tan( radiansFovY / float( 2 ) );

		std::array< float, 16 > result{ 0.0f };
		result[0] = float( 1.0 / ( aspect * tanHalfFovy ) );
		result[5] = float( 1.0 / tanHalfFovy );
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
		auto const tanHalfFovy = float( tan( radiansFovY / float( 2 ) ) );
		float const range = tanHalfFovy * zNear;
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

		std::vector< VkPhysicalDevice > gpus( gpuCount, nullptr );
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

	DevicePtr Instance::createDevice( PhysicalDevice const & physicalDevice
		, ashes::DeviceCreateInfo createInfos )const
	{
		DevicePtr result;

		try
		{
			result = std::make_shared< Device >( *this
				, ( m_allocator
					? m_allocator->getAllocationCallbacks()
					: nullptr )
				, physicalDevice
				, std::move( createInfos ) );
		}
		catch ( std::exception & exc )
		{
			log::error << "Could not initialise logical device:\n" << exc.what() << std::endl;
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

#if VK_EXT_debug_utils

	VkDebugUtilsMessengerEXT Instance::createDebugUtilsMessenger( VkDebugUtilsMessengerCreateInfoEXT const & createInfo )const
	{
		VkDebugUtilsMessengerEXT result{};

		if ( vkCreateDebugUtilsMessengerEXT )
		{
			auto res = vkCreateDebugUtilsMessengerEXT( m_instance
				, &createInfo
				, getAllocationCallbacks()
				, &result );
			checkError( res, "Debug utils messenger creation" );
		}

		return result;
	}

	void Instance::destroyDebugUtilsMessenger( VkDebugUtilsMessengerEXT messenger )const
	{
		if ( vkDestroyDebugUtilsMessengerEXT )
		{
			vkDestroyDebugUtilsMessengerEXT( m_instance
				, messenger
				, getAllocationCallbacks() );
		}
	}

	void Instance::submitDebugMessenger( VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity
		, VkDebugUtilsMessageTypeFlagsEXT messageTypes
		, VkDebugUtilsMessengerCallbackDataEXT const * pCallbackData )const
	{
		if ( vkSubmitDebugUtilsMessageEXT )
		{
			vkSubmitDebugUtilsMessageEXT( m_instance
				, messageSeverity
				, messageTypes
				, pCallbackData );
		}
	}

#endif
#if VK_EXT_debug_report

	VkDebugReportCallbackEXT Instance::createDebugReportCallback( VkDebugReportCallbackCreateInfoEXT const & createInfo )const
	{
		VkDebugReportCallbackEXT result{};

		if ( vkCreateDebugReportCallbackEXT )
		{
			auto res = vkCreateDebugReportCallbackEXT( m_instance
				, &createInfo
				, getAllocationCallbacks()
				, &result );
			checkError( res, "Debug report callback creation" );
		}

		return result;
	}

#endif

	void Instance::doInitInstance()
	{
		DEBUG_DUMP( m_createInfo );
		auto res = vkCreateInstance( &static_cast< VkInstanceCreateInfo const & >( m_createInfo )
			, nullptr
			, &m_instance );
		checkError( res, "Instance creation" );

#define VK_LIB_INSTANCE_FUNCTION( ver, fun )\
		vk##fun = reinterpret_cast< PFN_vk##fun >( getInstanceProcAddr( "vk"#fun ) );
#define VK_LIB_INSTANCE_FUNCTION_EXT( ver, ext, fun )\
		vk##fun = reinterpret_cast< PFN_vk##fun >( getInstanceProcAddr( "vk"#fun ) );
#include <ashes/ashes_functions_list.hpp>
	}

	PFN_vkVoidFunction Instance::getInstanceProcAddr( char const * const name )const
	{
		auto result = m_getInstanceProcAddr( m_instance, name );

		if ( !result )
		{
			log::debug << "Instance: Couldn't load function [" << name << "]" << std::endl;
		}

		return result;
	}

	bool Instance::checkExtension( std::string const & name )const
	{
		return m_createInfo.enabledExtensionNames.end() != std::find( m_createInfo.enabledExtensionNames.begin()
			, m_createInfo.enabledExtensionNames.end()
			, name );
	}

	VkAllocationCallbacks const * Instance::getAllocationCallbacks()const
	{
		return m_allocator
			? m_allocator->getAllocationCallbacks()
			: nullptr;
	}
}
