/*
This file belongs to GlInstance.
See LICENSE file in root folder.
*/
#include "Command/Commands/D3D11DownloadMemoryCommand.hpp"

#include "Miscellaneous/D3D11DeviceMemory.hpp"

#include "ashesd3d11_api.hpp"

namespace ashes::d3d11
{
	DownloadMemoryCommand::DownloadMemoryCommand( VkDevice device
		, VkDeviceMemory deviceMemory )
		: CommandBase{ device }
		, m_deviceMemory{ deviceMemory }
	{
	}

	void DownloadMemoryCommand::apply( Context const & context )const
	{
		get( m_deviceMemory )->download();
	}

	CommandPtr DownloadMemoryCommand::clone()const
	{
		return std::make_unique< DownloadMemoryCommand >( *this );
	}
}
