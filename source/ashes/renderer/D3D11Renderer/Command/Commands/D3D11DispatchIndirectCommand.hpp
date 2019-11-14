/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include "renderer/D3D11Renderer/Command/Commands/D3D11CommandBase.hpp"

namespace ashes::d3d11
{
	class DispatchIndirectCommand
		: public CommandBase
	{
	public:
		DispatchIndirectCommand( VkDevice device
			, VkBuffer buffer
			, VkDeviceSize offset );

		void apply( Context const & context )const;
		CommandPtr clone()const;

	private:
		VkBuffer m_buffer;
		UINT m_offset;
	};
}
