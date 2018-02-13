#include "TransparentRendering.hpp"

namespace vkapp
{
	TransparentRendering::TransparentRendering( renderer::Device const & device
		, renderer::ShaderProgramPtr && program
		, std::vector< renderer::PixelFormat > const & formats
		, renderer::UniformBuffer< renderer::Mat4 > const & matrixUbo
		, renderer::UniformBuffer< renderer::Mat4 > const & objectUbo
		, renderer::UniformBuffer< common::LightsData > const & lightsUbo )
		: common::NodesRenderer{ device
			, std::move( program )
			, formats
			, false
			, false }
		, m_matrixUbo{ matrixUbo }
		, m_objectUbo{ objectUbo }
		, m_lightsUbo{ lightsUbo }
	{
	}

	void TransparentRendering::doFillDescriptorLayoutBindings( renderer::DescriptorSetLayoutBindingArray & bindings )
	{
		bindings.emplace_back( 1u, renderer::DescriptorType::eUniformBuffer, renderer::ShaderStageFlag::eVertex );
		bindings.emplace_back( 2u, renderer::DescriptorType::eUniformBuffer, renderer::ShaderStageFlag::eVertex );
		bindings.emplace_back( 3u, renderer::DescriptorType::eUniformBuffer, renderer::ShaderStageFlag::eFragment );
	}

	void TransparentRendering::doFillDescriptorSet( renderer::DescriptorSetLayout & descriptorLayout
		, renderer::DescriptorSet & descriptorSet )
	{
		descriptorSet.createBinding( descriptorLayout.getBinding( 1u )
			, m_matrixUbo
			, 0u
			, 1u );
		descriptorSet.createBinding( descriptorLayout.getBinding( 2u )
			, m_objectUbo
			, 0u
			, 1u );
		descriptorSet.createBinding( descriptorLayout.getBinding( 3u )
			, m_lightsUbo
			, 0u
			, 1u );
	}
}
