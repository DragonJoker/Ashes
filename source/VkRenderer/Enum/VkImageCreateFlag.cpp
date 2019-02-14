#include "VkRendererPrerequisites.hpp"

namespace vk_renderer
{
	VkImageCreateFlags convert( ashes::ImageCreateFlags const & flags )
	{
		VkImageCreateFlags result{ 0 };

		if ( checkFlag( flags, ashes::ImageCreateFlag::eSparseBinding ) )
		{
			result |= VK_IMAGE_CREATE_SPARSE_BINDING_BIT;
		}

		if ( checkFlag( flags, ashes::ImageCreateFlag::eSparseResidency ) )
		{
			result |= VK_IMAGE_CREATE_SPARSE_RESIDENCY_BIT;
		}

		if ( checkFlag( flags, ashes::ImageCreateFlag::eSparseAliased ) )
		{
			result |= VK_IMAGE_CREATE_SPARSE_ALIASED_BIT;
		}

		if ( checkFlag( flags, ashes::ImageCreateFlag::eMutableFormat ) )
		{
			result |= VK_IMAGE_CREATE_MUTABLE_FORMAT_BIT;
		}

		if ( checkFlag( flags, ashes::ImageCreateFlag::eCubeCompatible ) )
		{
			result |= VK_IMAGE_CREATE_CUBE_COMPATIBLE_BIT;
		}

		return result;
	}

	ashes::ImageCreateFlags convertImageCreateFlags( VkImageCreateFlags const & flags )
	{
		ashes::ImageCreateFlags result;

		if ( ashes::checkFlag( flags, VK_IMAGE_CREATE_SPARSE_BINDING_BIT ) )
		{
			result |= ashes::ImageCreateFlag::eSparseBinding;
		}

		if ( ashes::checkFlag( flags, VK_IMAGE_CREATE_SPARSE_RESIDENCY_BIT ) )
		{
			result |= ashes::ImageCreateFlag::eSparseResidency;
		}

		if ( ashes::checkFlag( flags, VK_IMAGE_CREATE_SPARSE_ALIASED_BIT ) )
		{
			result |= ashes::ImageCreateFlag::eSparseAliased;
		}

		if ( ashes::checkFlag( flags, VK_IMAGE_CREATE_MUTABLE_FORMAT_BIT ) )
		{
			result |= ashes::ImageCreateFlag::eMutableFormat;
		}

		if ( ashes::checkFlag( flags, VK_IMAGE_CREATE_CUBE_COMPATIBLE_BIT ) )
		{
			result |= ashes::ImageCreateFlag::eCubeCompatible;
		}

		return result;
	}
}
