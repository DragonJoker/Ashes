#include "VkRendererPrerequisites.hpp"

namespace vk_renderer
{
	VkPhysicalDeviceType convert( ashes::PhysicalDeviceType const & value )
	{
		switch ( value )
		{
		case ashes::PhysicalDeviceType::eOther:
			return VK_PHYSICAL_DEVICE_TYPE_OTHER;

		case ashes::PhysicalDeviceType::eIntegratedGpu:
			return VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU;

		case ashes::PhysicalDeviceType::eDiscreteGpu:
			return VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU;

		case ashes::PhysicalDeviceType::eVirtualGpu:
			return VK_PHYSICAL_DEVICE_TYPE_VIRTUAL_GPU;

		case ashes::PhysicalDeviceType::eCpu:
			return VK_PHYSICAL_DEVICE_TYPE_CPU;

		default:
			assert( false );
			return VK_PHYSICAL_DEVICE_TYPE_OTHER;
		}
	}

	ashes::PhysicalDeviceType convertPhysicalDeviceType( VkPhysicalDeviceType const & value )
	{
		switch ( value )
		{
		case VK_PHYSICAL_DEVICE_TYPE_OTHER:
			return ashes::PhysicalDeviceType::eOther;

		case VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU:
			return ashes::PhysicalDeviceType::eIntegratedGpu;

		case VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU:
			return ashes::PhysicalDeviceType::eDiscreteGpu;

		case VK_PHYSICAL_DEVICE_TYPE_VIRTUAL_GPU:
			return ashes::PhysicalDeviceType::eVirtualGpu;

		case VK_PHYSICAL_DEVICE_TYPE_CPU:
			return ashes::PhysicalDeviceType::eCpu;

		default:
			assert( false );
			return ashes::PhysicalDeviceType::eOther;
		}
	}
}
