#pragma once

#include "Prerequisites.hpp"

#include <NodesRenderer.hpp>

namespace vkapp
{
	class GeometryPass
		: public common::NodesRenderer
	{
	public:
		GeometryPass( renderer::Device const & device
			, std::string const & fragmentShaderFile
			, GeometryPassResult const & gbuffer
			, renderer::PixelFormat depthFormat
			, renderer::UniformBuffer< common::SceneData > const & sceneUbo
			, renderer::UniformBuffer< common::ObjectData > const & objectUbo );
		void update( common::RenderTarget const & target )override;

	private:
		void doFillDescriptorLayoutBindings( renderer::DescriptorSetLayoutBindingArray & bindings )override;
		void doFillDescriptorSet( renderer::DescriptorSetLayout & descriptorLayout
			, renderer::DescriptorSet & descriptorSet )override;

	private:
		renderer::UniformBuffer< common::SceneData > const & m_sceneUbo;
		renderer::UniformBuffer< common::ObjectData > const & m_objectUbo;
	};
}
