/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "Command/Commands/D3D11CopyBufferCommand.hpp"

#include "Buffer/D3D11Buffer.hpp"

#include "ashesd3d11_api.hpp"

namespace ashes::D3D11_NAMESPACE
{
	CopyBufferCommand::CopyBufferCommand( VkDevice device
		, VkBufferCopy const & copyInfo
		, VkBuffer src
		, VkBuffer dst )
		: CommandBase{ device }
		, m_src{ src }
		, m_dst{ dst }
		, m_dstOffset{ UINT( copyInfo.dstOffset ) }
		, m_srcBox{ UINT( copyInfo.srcOffset ), 0u, 0u, UINT( copyInfo.srcOffset + copyInfo.size ), 1u, 1u }
	{
	}

	void CopyBufferCommand::apply( Context const & context )const
	{
		get( m_dst )->copyFrom( *context.context
			, m_src
			, m_srcBox
			, m_dstOffset );
	}

	CommandPtr CopyBufferCommand::clone()const
	{
		return std::make_unique< CopyBufferCommand >( *this );
	}
}
