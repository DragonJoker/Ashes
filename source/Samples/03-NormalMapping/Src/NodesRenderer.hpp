#pragma once

#include "Prerequisites.hpp"

#include <NodesRenderer.hpp>

namespace vkapp
{
	class NodesRenderer
		: public common::NodesRenderer
	{
	public:
		NodesRenderer( renderer::Device const & device
			, std::string const & fragmentShaderFile
			, std::vector< renderer::Format > const & formats
			, bool clearViews
			, bool opaqueNodes
			, renderer::UniformBuffer< common::SceneData > const & sceneUbo
			, renderer::UniformBuffer< common::ObjectData > const & objectUbo
			, renderer::UniformBuffer< common::LightsData > const & lightsUbo );

	private:
		void doFillObjectDescriptorLayoutBindings( renderer::DescriptorSetLayoutBindingArray & bindings )override;
		void doFillObjectDescriptorSet( renderer::DescriptorSetLayout & descriptorLayout
			, renderer::DescriptorSet & descriptorSet )override;

	private:
		renderer::UniformBuffer< common::SceneData > const & m_sceneUbo;
		renderer::UniformBuffer< common::ObjectData > const & m_objectUbo;
		renderer::UniformBuffer< common::LightsData > const & m_lightsUbo;
	};
}
