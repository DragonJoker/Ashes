/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "Command/Commands/TestBeginQueryCommand.hpp"

#include "Miscellaneous/TestQueryPool.hpp"

#include "ashestest_api.hpp"

namespace ashes::test
{
	BeginQueryCommand::BeginQueryCommand( VkDevice device
		, VkQueryPool pool
		, uint32_t query
		, VkQueryControlFlags flags )
		: CommandBase{ device }
	{
	}

	void BeginQueryCommand::apply()const
	{
	}

	CommandPtr BeginQueryCommand::clone()const
	{
		return std::make_unique< BeginQueryCommand >( *this );
	}
}
