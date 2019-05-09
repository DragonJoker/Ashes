/*
This file belongs to GlInstance.
See LICENSE file in root folder.
*/
#include "Command/Commands/D3D11UploadMemoryCommand.hpp"

#include "Miscellaneous/D3D11DeviceMemory.hpp"

#include "ashesd3d11_api.hpp"

namespace ashes::d3d11
{
	UploadMemoryCommand::UploadMemoryCommand( VkDevice device
		, VkDeviceMemory deviceMemory )
		: CommandBase{ device }
		, m_deviceMemory{ deviceMemory }
	{
	}

	void UploadMemoryCommand::apply( Context const & context )const
	{
		get( m_deviceMemory )->upload();
	}

	CommandPtr UploadMemoryCommand::clone()const
	{
		return std::make_unique< UploadMemoryCommand >( *this );
	}
}
