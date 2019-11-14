#pragma once

#include "Prerequisites.hpp"

#include <NodesRenderer.hpp>

namespace vkapp
{
	class TransparentRendering
		: public common::NodesRenderer
	{
	public:
		TransparentRendering( utils::Device const & device
			, ashes::CommandPool const & commandPool
			, ashes::Queue const & transferQueue
			, std::string const & fragmentShaderFile
			, std::vector< VkFormat > const & formats
			, ashes::UniformBuffer const & sceneUbo
			, ashes::UniformBuffer const & objectUbo
			, ashes::UniformBuffer const & lightsUbo );

	private:
		void doFillObjectDescriptorLayoutBindings( ashes::VkDescriptorSetLayoutBindingArray & bindings )override;
		void doFillObjectDescriptorSet( ashes::DescriptorSetLayout & descriptorLayout
			, ashes::DescriptorSet & descriptorSet )override;

	private:
		ashes::UniformBuffer const & m_sceneUbo;
		ashes::UniformBuffer const & m_objectUbo;
		ashes::UniformBuffer const & m_lightsUbo;
	};
}
