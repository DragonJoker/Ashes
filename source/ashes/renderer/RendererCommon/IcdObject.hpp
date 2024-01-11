/**
*\file
*	InlineUniformBlocks.h
*\author
*	Sylvain Doremus
*/
#pragma once

#ifndef VK_NO_PROTOTYPES
#	define VK_NO_PROTOTYPES
#endif
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wold-style-cast"
#include <vulkan/vk_icd.h>
#pragma GCC diagnostic pop

namespace ashes
{
	class IcdObject
	{
	protected:
		IcdObject()
		{
			set_loader_magic_value( &m_loaderData );
		}

		~IcdObject()noexcept = default;

	private:
		VK_LOADER_DATA m_loaderData;
	};
}
