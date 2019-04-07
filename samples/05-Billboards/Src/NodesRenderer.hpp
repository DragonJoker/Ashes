#pragma once

#include "Prerequisites.hpp"

#include <NodesRenderer.hpp>

namespace vkapp
{
	class NodesRenderer
		: public common::NodesRenderer
	{
	public:
		NodesRenderer( utils::Device const & device
			, ashes::CommandPool const & commandPool
			, ashes::Queue const & transferQueue
			, std::string const & fragmentShaderFile
			, std::vector< VkFormat > const & formats
			, bool clearViews
			, bool opaqueNodes
			, ashes::UniformBuffer< common::SceneData > const & sceneUbo );

	private:
		void doFillBillboardDescriptorLayoutBindings( ashes::VkDescriptorSetLayoutBindingArray & bindings )override;
		void doFillBillboardDescriptorSet( ashes::DescriptorSetLayout & descriptorLayout
			, ashes::DescriptorSet & descriptorSet )override;

	private:
		ashes::UniformBuffer< common::SceneData > const & m_sceneUbo;
	};
}
