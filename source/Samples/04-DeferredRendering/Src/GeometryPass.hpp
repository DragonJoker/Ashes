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
			, renderer::ShaderProgramPtr && program
			, GeometryPassResult const & gbuffer
			, renderer::PixelFormat depthFormat
			, renderer::UniformBuffer< renderer::Mat4 > const & matrixUbo
			, renderer::UniformBuffer< renderer::Mat4 > const & objectUbo );
		void update( common::RenderTarget const & target )override;

	private:
		void doFillDescriptorLayoutBindings( renderer::DescriptorSetLayoutBindingArray & bindings )override;
		void doFillDescriptorSet( renderer::DescriptorSetLayout & descriptorLayout
			, renderer::DescriptorSet & descriptorSet )override;

	private:
		renderer::UniformBuffer< renderer::Mat4 > const & m_matrixUbo;
		renderer::UniformBuffer< renderer::Mat4 > const & m_objectUbo;
	};
}
