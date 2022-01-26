/*
See LICENSE file in root folder
*/
#ifndef ___AshesPP_AccelerationStructure_H___
#define ___AshesPP_AccelerationStructure_H___

#include "ashespp/AshesPPPrerequisites.hpp"

namespace ashes
{
	/**
	*\brief
	*	GPU query pool implementation.
	*/
	class AccelerationStructure
		: public VkObject
	{
	public:
		/**
		*\brief
		*	Constructor.
		*\param[in] device
		*	The logical device.
		*\param[in] info
		*	The creation info.
		*/
		AccelerationStructure( Device const & device
			, VkAccelerationStructureCreateInfoKHR info );
		/**
		*\brief
		*	Constructor.
		*\param[in] device
		*	The logical device.
		*\param[in] debugName
		*	The object debug name.
		*\param[in] info
		*	The creation info.
		*/
		AccelerationStructure( Device const & device
			, std::string debugName
			, VkAccelerationStructureCreateInfoKHR infos );
		/**
		*\brief
		*	Destructor.
		*/
		~AccelerationStructure();
		/**
		*\brief
		*	Copy an acceleration structure on the host.
		*/
		void copyAccelerationStructure( VkDeferredOperationKHR deferredOperation
			, AccelerationStructure & dst
			, VkCopyAccelerationStructureModeKHR mode )const;
		/**
		*\brief
		*	Serialize an acceleration structure on the host.
		*/
		void copyToMemory( VkDeferredOperationKHR deferredOperation
			, VkDeviceOrHostAddressKHR dst
			, VkCopyAccelerationStructureModeKHR mode )const;
		/**
		*\brief
		*	Serialize an acceleration structure on the host.
		*/
		void copyFromMemory( VkDeferredOperationKHR deferredOperation
			, VkDeviceOrHostAddressConstKHR src
			, VkCopyAccelerationStructureModeKHR mode )const;
		/**
		*\brief
		*	Query an address of a acceleration structure.
		*/
		VkDeviceAddress getDeviceAddress()const;
		/**
		*\brief
		*	VkAccelerationStructureKHR implicit cast operator.
		*/
		inline operator VkAccelerationStructureKHR const & ()const
		{
			return m_internal;
		}

	protected:
		Device const & m_device;
		VkAccelerationStructureCreateInfoKHR m_info;
		VkAccelerationStructureKHR m_internal{};
	};
}

#endif
