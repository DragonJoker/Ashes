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
			, ashes::UniformBuffer< common::SceneData > const & sceneUbo
			, ashes::UniformBuffer< common::ObjectData > const & objectUbo
			, ashes::UniformBuffer< common::LightsData > const & lightsUbo );

	private:
		void doFillObjectDescriptorLayoutBindings( ashes::VkDescriptorSetLayoutBindingArray & bindings )override;
		void doFillObjectDescriptorSet( ashes::DescriptorSetLayout & descriptorLayout
			, ashes::DescriptorSet & descriptorSet )override;

	private:
		ashes::UniformBuffer< common::SceneData > const & m_sceneUbo;
		ashes::UniformBuffer< common::ObjectData > const & m_objectUbo;
		ashes::UniformBuffer< common::LightsData > const & m_lightsUbo;
	};
}
