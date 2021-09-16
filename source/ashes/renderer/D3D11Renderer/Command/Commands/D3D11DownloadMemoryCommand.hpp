/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include "renderer/D3D11Renderer/Command/Commands/D3D11CommandBase.hpp"

namespace ashes::d3d11
{
	class DownloadMemoryCommand
		: public CommandBase
	{
	public:
		DownloadMemoryCommand( VkDevice device
			, ObjectMemory const * memory
			, VkDeviceSize offset
			, VkDeviceSize size
			, UINT subresource );

		void apply( Context const & context )const override;
		CommandPtr clone()const override;

	private:
		ObjectMemory const * m_memory;
		std::pair< VkDeviceSize, VkDeviceSize > m_range;
		UINT m_subresource;
	};
}
