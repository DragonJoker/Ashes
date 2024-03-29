/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "Command/Commands/TestBindDescriptorSetCommand.hpp"

#include "Buffer/TestBuffer.hpp"
#include "Buffer/TestBufferView.hpp"
#include "Descriptor/TestDescriptorSet.hpp"
#include "Pipeline/TestPipelineLayout.hpp"
#include "Image/TestSampler.hpp"
#include "Image/TestImage.hpp"
#include "Image/TestImageView.hpp"

#include "ashestest_api.hpp"

namespace ashes::test
{
	BindDescriptorSetCommand::BindDescriptorSetCommand( VkDevice context
		, VkDescriptorSet descriptorSet
		, VkPipelineLayout
		, UInt32Array const & dynamicOffsets
		, VkPipelineBindPoint )
		: CommandBase{ context }
		, m_dynamicOffsets{ dynamicOffsets }
	{
		assert( get( descriptorSet )->getDynamicBuffers().size() == m_dynamicOffsets.size()
			&& "Dynamic descriptors and dynamic offsets sizes must match." );
	}

	void BindDescriptorSetCommand::apply()const
	{
	}

	CommandPtr BindDescriptorSetCommand::clone()const
	{
		return std::make_unique< BindDescriptorSetCommand >( *this );
	}
}
