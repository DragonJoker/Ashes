#pragma once

#include "Prerequisites.hpp"

#include <NodesRenderer.hpp>

namespace vkapp
{
	class TransparentRendering
		: public common::NodesRenderer
	{
	public:
		TransparentRendering( renderer::Device const & device
			, std::string const & fragmentShaderFile
			, std::vector< renderer::PixelFormat > const & formats
			, renderer::UniformBuffer< common::SceneData > const & sceneUbo
			, renderer::UniformBuffer< common::ObjectData > const & objectUbo
			, renderer::UniformBuffer< common::LightsData > const & lightsUbo );

	private:
		void doFillDescriptorLayoutBindings( renderer::DescriptorSetLayoutBindingArray & bindings )override;
		void doFillDescriptorSet( renderer::DescriptorSetLayout & descriptorLayout
			, renderer::DescriptorSet & descriptorSet )override;

	private:
		renderer::UniformBuffer< common::SceneData > const & m_sceneUbo;
		renderer::UniformBuffer< common::ObjectData > const & m_objectUbo;
		renderer::UniformBuffer< common::LightsData > const & m_lightsUbo;
	};
}
