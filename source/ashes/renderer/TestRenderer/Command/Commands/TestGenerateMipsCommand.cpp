/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "Command/Commands/TestGenerateMipsCommand.hpp"

#include "Image/TestImage.hpp"

#include "ashestest_api.hpp"

namespace ashes::test
{
	GenerateMipsCommand::GenerateMipsCommand( VkDevice device
		, VkImage texture )
		: CommandBase{ device }
		, m_texture{ texture }
	{
	}

	void GenerateMipsCommand::apply()const
	{
	}

	CommandPtr GenerateMipsCommand::clone()const
	{
		return std::make_unique< GenerateMipsCommand >( *this );
	}
}
