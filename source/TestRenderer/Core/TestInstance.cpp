#include "Core/TestInstance.hpp"

#include "Core/TestSurface.hpp"
#include "Core/TestDebugReportCallback.hpp"
#include "Core/TestDevice.hpp"
#include "Core/TestPhysicalDevice.hpp"
#include "Core/TestSwapChain.hpp"

#include <cmath>
#include <iomanip>
#include <locale>

namespace test_renderer
{
	ashes::PhysicalDeviceMemoryProperties const Instance::m_memoryProperties = []()
		{
			ashes::PhysicalDeviceMemoryProperties result;

			// Emulate one device local heap
			result.memoryHeaps.push_back(
				{
					~( 0ull ),
					0u | ashes::MemoryHeapFlag::eDeviceLocal
				} );
			// and one host visible heap
			result.memoryHeaps.push_back(
				{
					~( 0ull ),
					0u
				} );

			// Emulate all combinations of device local memory types
			result.memoryTypes.push_back(
				{
					0u | ashes::MemoryPropertyFlag::eDeviceLocal,
					0u,
				} );
			result.memoryTypes.push_back(
				{
					0u | ashes::MemoryPropertyFlag::eLazilyAllocated,
					0u,
				} );
			result.memoryTypes.push_back(
				{
					ashes::MemoryPropertyFlag::eDeviceLocal | ashes::MemoryPropertyFlag::eLazilyAllocated,
					0u,
				} );

			// and all combinations of host visible memory types
			result.memoryTypes.push_back(
				{
					0u | ashes::MemoryPropertyFlag::eHostVisible,
					1u,
				} );

			result.memoryTypes.push_back(
				{
					ashes::MemoryPropertyFlag::eHostVisible | ashes::MemoryPropertyFlag::eHostCoherent,
					1u,
				} );

			result.memoryTypes.push_back(
				{
					ashes::MemoryPropertyFlag::eHostVisible | ashes::MemoryPropertyFlag::eHostCached,
					1u,
				} );

			result.memoryTypes.push_back(
				{
					ashes::MemoryPropertyFlag::eHostVisible | ashes::MemoryPropertyFlag::eHostCoherent | ashes::MemoryPropertyFlag::eHostCached,
					1u,
				} );

			return result;
		}();

	Instance::Instance( ashes::InstanceCreateInfo createInfo )
		: ashes::Instance{ ashes::ClipDirection::eTopDown, "test", std::move( createInfo ) }
	{
		m_features.hasTexBufferRange = true;
		m_features.hasImageTexture = true;
		m_features.hasBaseInstance = true;
		m_features.hasClearTexImage = true;
		m_features.hasComputeShaders = true;
		m_features.hasStorageBuffers = true;
		m_features.supportsPersistentMapping = true;
	}

	Instance::~Instance()
	{
	}

	ashes::PhysicalDevicePtrArray Instance::enumeratePhysicalDevices()const
	{
		ashes::PhysicalDevicePtrArray result;
		result.emplace_back( std::make_unique< PhysicalDevice >( *this ) );
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
}
