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
			, renderer::UniformBuffer< common::SceneData > const & sceneUbo );

	private:
		void doFillBillboardDescriptorLayoutBindings( renderer::DescriptorSetLayoutBindingArray & bindings )override;
		void doFillBillboardDescriptorSet( renderer::DescriptorSetLayout & descriptorLayout
			, renderer::DescriptorSet & descriptorSet )override;

	private:
		renderer::UniformBuffer< common::SceneData > const & m_sceneUbo;
	};
}
