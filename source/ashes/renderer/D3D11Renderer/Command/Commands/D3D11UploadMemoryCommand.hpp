/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include "renderer/D3D11Renderer/Command/Commands/D3D11CommandBase.hpp"

namespace ashes::d3d11
{
	class UploadMemoryCommand
		: public CommandBase
	{
	public:
		UploadMemoryCommand( VkDevice device
			, VkDeviceMemory deviceMemory );
		void apply( Context const & context )const;
		CommandPtr clone()const;

	private:
		VkDeviceMemory m_deviceMemory;
	};
}
