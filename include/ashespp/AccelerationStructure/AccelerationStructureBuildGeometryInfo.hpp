/*
See LICENSE file in root folder
*/
#ifndef ___AshesPP_AccelerationStructureBuildGeometryInfo_H___
#define ___AshesPP_AccelerationStructureBuildGeometryInfo_H___

#include "ashespp/AshesPPPrerequisites.hpp"

namespace ashes
{
	using AccelerationStructureGeometryArray = std::vector< VkAccelerationStructureGeometryKHR >;
	using AccelerationStructureGeometryPtrArray = std::vector< VkAccelerationStructureGeometryKHR const * >;
	using AccelerationStructureBuildRangeInfoPtrArray = std::vector< VkAccelerationStructureBuildRangeInfoKHR const * >;

	class AccelerationStructureBuildGeometryInfo
	{
	private:
		AccelerationStructureBuildGeometryInfo( VkAccelerationStructureTypeKHR type
			, VkBuildAccelerationStructureFlagsKHR flags
			, VkBuildAccelerationStructureModeKHR mode
			, VkAccelerationStructureKHR srcAccelerationStructure
			, VkAccelerationStructureKHR dstAccelerationStructure
			, AccelerationStructureGeometryArray ppgeometries
			, AccelerationStructureGeometryPtrArray pppgeometries
			, VkDeviceOrHostAddressKHR scratchData )
			: geometries{ std::move( ppgeometries ) }
			, pgeometries{ std::move( pppgeometries ) }
			, vk{ VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_BUILD_GEOMETRY_INFO_KHR
				, nullptr
				, type
				, flags
				, mode
				, srcAccelerationStructure
				, dstAccelerationStructure
				, uint32_t( std::max( geometries.size(), pgeometries.size() ) )
				, geometries.empty() ? nullptr : geometries.data()
				, pgeometries.empty() ? nullptr : pgeometries.data()
				, scratchData }
		{
		}

	public:
		~AccelerationStructureBuildGeometryInfo()noexcept = default;

		AccelerationStructureBuildGeometryInfo( VkAccelerationStructureTypeKHR type
			, VkBuildAccelerationStructureFlagsKHR flags
			, VkBuildAccelerationStructureModeKHR mode
			, VkAccelerationStructureKHR srcAccelerationStructure
			, VkAccelerationStructureKHR dstAccelerationStructure
			, AccelerationStructureGeometryArray ppgeometries
			, VkDeviceOrHostAddressKHR scratchData )
			: AccelerationStructureBuildGeometryInfo{ type
				, flags
				, mode
				, srcAccelerationStructure
				, dstAccelerationStructure
			, std::move( ppgeometries )
				, {}
				, scratchData }
		{
		}

		AccelerationStructureBuildGeometryInfo( VkAccelerationStructureTypeKHR type
			, VkBuildAccelerationStructureFlagsKHR flags
			, VkBuildAccelerationStructureModeKHR mode
			, VkAccelerationStructureKHR srcAccelerationStructure
			, VkAccelerationStructureKHR dstAccelerationStructure
			, AccelerationStructureGeometryPtrArray ppgeometries
			, VkDeviceOrHostAddressKHR scratchData )
			: AccelerationStructureBuildGeometryInfo{ type
				, flags
				, mode
				, srcAccelerationStructure
				, dstAccelerationStructure
				, {}
				, std::move( ppgeometries )
				, scratchData }
		{
		}

		AccelerationStructureBuildGeometryInfo( AccelerationStructureBuildGeometryInfo const & rhs )
			: AccelerationStructureBuildGeometryInfo{ rhs.vk.type
			, rhs.vk.flags
			, rhs.vk.mode
			, rhs.vk.srcAccelerationStructure
			, rhs.vk.dstAccelerationStructure
			, rhs.geometries
			, rhs.pgeometries
			, rhs.vk.scratchData }
		{
		}

		AccelerationStructureBuildGeometryInfo( AccelerationStructureBuildGeometryInfo && rhs )noexcept
			: AccelerationStructureBuildGeometryInfo{ rhs.vk.type
			, rhs.vk.flags
			, rhs.vk.mode
			, rhs.vk.srcAccelerationStructure
			, rhs.vk.dstAccelerationStructure
			, std::move( rhs.geometries )
			, std::move( rhs.pgeometries )
			, rhs.vk.scratchData }
		{
		}

		explicit AccelerationStructureBuildGeometryInfo( VkAccelerationStructureBuildGeometryInfoKHR const & rhs )noexcept
			: AccelerationStructureBuildGeometryInfo{ rhs.type
			, rhs.flags
			, rhs.mode
			, rhs.srcAccelerationStructure
			, rhs.dstAccelerationStructure
			, rhs.pGeometries ? makeArray( rhs.pGeometries, rhs.geometryCount ) : AccelerationStructureGeometryArray{}
			, rhs.ppGeometries ? makeArray( rhs.ppGeometries, rhs.geometryCount ) : AccelerationStructureGeometryPtrArray{}
			, rhs.scratchData }
		{
		}

		operator VkAccelerationStructureBuildGeometryInfoKHR const & ()const noexcept
		{
			return vk;
		}

	private:
		AccelerationStructureGeometryArray geometries;
		AccelerationStructureGeometryPtrArray pgeometries;
		VkAccelerationStructureBuildGeometryInfoKHR vk;
	};

	using AccelerationStructureBuildGeometryInfoArray = std::vector< AccelerationStructureBuildGeometryInfo >;
}

#endif
