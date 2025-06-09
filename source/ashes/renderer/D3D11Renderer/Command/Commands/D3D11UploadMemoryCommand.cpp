/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "Command/Commands/D3D11UploadMemoryCommand.hpp"

#include "Miscellaneous/D3D11DeviceMemory.hpp"

#include "ashesd3d11_api.hpp"

namespace ashes::D3D11_NAMESPACE
{
	UploadMemoryCommand::UploadMemoryCommand( VkDevice device
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

	void UploadMemoryCommand::apply( Context const & context )const
	{
		get( m_memory->deviceMemory )->updateUpload( *m_memory
			, m_range.first
			, m_range.second
			, m_subresource );
	}

	CommandPtr UploadMemoryCommand::clone()const
	{
		return std::make_unique< UploadMemoryCommand >( *this );
	}
}
