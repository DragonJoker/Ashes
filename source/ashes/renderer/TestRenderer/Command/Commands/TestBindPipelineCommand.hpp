/*
This file belongs to Ashes.
See LICENSE file in root folder
*/
#pragma once

#include "renderer/TestRenderer/Command/Commands/TestCommandBase.hpp"

namespace ashes::test
{
	class BindPipelineCommand
		: public CommandBase
	{
	public:
		BindPipelineCommand( VkDevice device
			, VkPipeline pipeline
			, VkPipelineBindPoint bindingPoint );

		void apply()const override;
		CommandPtr clone()const override;

	private:
		VkPipeline m_pipeline;
		VkPipelineLayout m_layout;
		bool m_dynamicLineWidth;
		bool m_dynamicDepthBias;
		bool m_dynamicScissor;
		bool m_dynamicViewport;
	};
}
