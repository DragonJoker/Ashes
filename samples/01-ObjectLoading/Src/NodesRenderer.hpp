#pragma once

#include "Prerequisites.hpp"

#include <NodesRenderer.hpp>

namespace vkapp
{
	class NodesRenderer
		: public common::NodesRenderer
	{
	public:
		NodesRenderer( ashes::Device const & device
			, ashes::CommandPool const & commandPool
			, ashes::Queue const & transferQueue
			, std::string const & fragmentShaderFile
			, std::vector< ashes::Format > const & formats
			, bool clearViews
			, bool opaqueNodes
			, ashes::UniformBuffer< common::SceneData > const & sceneUbo
			, ashes::UniformBuffer< common::ObjectData > const & objectUbo );

	private:
		void doFillObjectDescriptorLayoutBindings( ashes::DescriptorSetLayoutBindingArray & bindings )override;
		void doFillObjectDescriptorSet( ashes::DescriptorSetLayout & descriptorLayout
			, ashes::DescriptorSet & descriptorSet )override;

	private:
		ashes::UniformBuffer< common::SceneData > const & m_sceneUbo;
		ashes::UniformBuffer< common::ObjectData > const & m_objectUbo;
	};
}
