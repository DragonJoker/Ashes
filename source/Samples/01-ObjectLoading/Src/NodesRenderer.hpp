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
			, std::vector< renderer::PixelFormat > const & formats
			, bool clearViews
			, bool opaqueNodes
			, renderer::UniformBuffer< common::SceneData > const & sceneUbo
			, renderer::UniformBuffer< common::ObjectData > const & objectUbo );

	private:
		void doFillDescriptorLayoutBindings( renderer::DescriptorSetLayoutBindingArray & bindings )override;
		void doFillDescriptorSet( renderer::DescriptorSetLayout & descriptorLayout
			, renderer::DescriptorSet & descriptorSet )override;

	private:
		renderer::UniformBuffer< common::SceneData > const & m_sceneUbo;
		renderer::UniformBuffer< common::ObjectData > const & m_objectUbo;
	};
}
