/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include "renderer/XBoxRenderer/Command/Commands/XBoxCommandBase.hpp"
#include "renderer/XBoxRenderer/Image/XBoxImageView.hpp"

namespace ashes::xbox
{
	class GenerateMipsCommand
		: public CommandBase
	{
	public:
		GenerateMipsCommand( VkDevice device
			, VkImage texture );

		void apply( Context const & context )const override;
		CommandPtr clone()const override;

	private:
		VkImage m_texture;
		ImageView m_view;
	};
}
