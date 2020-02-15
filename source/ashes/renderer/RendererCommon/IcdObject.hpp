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
#include <vulkan/vk_icd.h>

namespace ashes
{
	class IcdObject
	{
	protected:
		IcdObject()
		{
			set_loader_magic_value( &m_loaderData );
		}

		~IcdObject()
		{
		}

	private:
		VK_LOADER_DATA m_loaderData;
	};
}
