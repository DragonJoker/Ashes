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
		, ObjectMemory const * memory
		, VkDeviceSize offset
		, VkDeviceSize size
		, UINT subresource )
		: CommandBase{ device }
		, m_memory{ memory }
		, m_range{ offset, size }
		, m_subresource{ subresource }
	{
	}

	void DownloadMemoryCommand::apply( Context const & context )const
	{
		get( m_memory->deviceMemory )->updateDownload( *m_memory
			, m_range.first
			, m_range.second
			, m_subresource );
	}

	CommandPtr DownloadMemoryCommand::clone()const
	{
		return std::make_unique< DownloadMemoryCommand >( *this );
	}
}
