#include "Core/VkInstance.hpp"

#include "Core/VkDebugReportCallback.hpp"
#include "Core/VkDevice.hpp"
#include "Core/VkPhysicalDevice.hpp"
#include "Core/VkSurface.hpp"
#include "Core/VkSwapChain.hpp"

#include <cmath>
#include <iomanip>
#include <locale>

namespace vk_renderer
{
	Instance::Instance( std::unique_ptr< ashes::DynamicLibrary > library
		, PFN_vkGetInstanceProcAddr getInstanceProcAddr
		, ashes::InstanceCreateInfo createInfo )
		: ashes::Instance{ ashes::ClipDirection::eTopDown, "vk", std::move( createInfo ) }
		, m_library{ std::move( library ) }
		, m_getInstanceProcAddr{ getInstanceProcAddr }
	{
		m_features.hasTexBufferRange = true;
		m_features.hasImageTexture = true;
		m_features.hasBaseInstance = true;
		m_features.hasClearTexImage = true;
		m_features.hasComputeShaders = true;
		m_features.hasStorageBuffers = true;
		m_features.supportsPersistentMapping = true;

#define VK_LIB_GLOBAL_FUNCTION( fun ) fun = reinterpret_cast< PFN_##fun >( m_getInstanceProcAddr( nullptr, #fun ) );
#include "Miscellaneous/VulkanFunctionsList.inl"

		doInitInstance();
		enumeratePhysicalDevices();
	}

	Instance::~Instance()
	{
		if ( m_instance != VK_NULL_HANDLE )
		{
			vkDestroyInstance( m_instance, nullptr );
		}

		DEBUG_WRITE( "VkInstance.log" );
	}

	ashes::PhysicalDevicePtrArray Instance::enumeratePhysicalDevices()const
	{
		// On récupère les GPU physiques.
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

		// Et on les stocke dans des PhysicalDevice.
		ashes::PhysicalDevicePtrArray result( gpuCount );

		for ( auto gpu : gpus )
		{
			result.push_back( std::make_unique< PhysicalDevice >( *this, gpu ) );
		}

		return result;
	}

	ashes::DevicePtr Instance::createDevice( ashes::SurfacePtr surface
		, ashes::DeviceCreateInfo createInfos )const
	{
		ashes::DevicePtr result;

		try
		{
			result = std::make_shared< Device >( *this
				, std::move( surface )
				, std::move( createInfos ) );
		}
		catch ( std::exception & exc )
		{
			ashes::Logger::logError( std::string{ "Could not initialise logical device:\n" } + exc.what() );
		}

		return result;
	}

	ashes::SurfacePtr Instance::createSurface( ashes::PhysicalDevice const & gpu
		, ashes::WindowHandle handle )const
	{
		return std::make_unique< Surface >( *this
			, gpu
			, std::move( handle ) );
	}

	ashes::DebugReportCallbackPtr Instance::createDebugReportCallback( ashes::DebugReportCallbackCreateInfo createInfo )const
	{
		return std::make_unique< DebugReportCallback >( *this
			, std::move( createInfo ) );
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

	void Instance::doInitInstance()
	{
		VkApplicationInfo appInfo;
		std::vector< char const * > vkLayers;
		std::vector< char const * > vkExtensions;
		auto instInfo = convert( m_createInfo
			, appInfo
			, vkLayers
			, vkExtensions );
		DEBUG_DUMP( instInfo );

		auto res = vkCreateInstance( &instInfo, nullptr, &m_instance );
		checkError( res, "Instance creation" );

#define VK_LIB_INSTANCE_FUNCTION( fun ) fun = reinterpret_cast< PFN_##fun >( getInstanceProcAddr( #fun ) );
#include "Miscellaneous/VulkanFunctionsList.inl"
	}

	PFN_vkVoidFunction Instance::getInstanceProcAddr( char const * const name )
	{
		auto result = m_getInstanceProcAddr( m_instance, name );

		if ( !result )
		{
			ashes::Logger::logError( std::string{ "Couldn't load function " } + name );
		}

		return result;
	}
}
