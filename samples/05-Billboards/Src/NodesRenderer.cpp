#include "NodesRenderer.hpp"

namespace vkapp
{
	NodesRenderer::NodesRenderer( utils::Device const & device
		, ashes::CommandPool const & commandPool
		, ashes::Queue const & transferQueue
		, std::string const & fragmentShaderFile
		, std::vector< ashes::Format > const & formats
		, bool clearViews
		, bool opaqueNodes
		, ashes::UniformBuffer< common::SceneData > const & sceneUbo )
		: common::NodesRenderer{ device
			, commandPool
			, transferQueue
			, fragmentShaderFile
			, formats
			, clearViews
			, opaqueNodes }
		, m_sceneUbo{ sceneUbo }
	{
	}

	void NodesRenderer::doFillBillboardDescriptorLayoutBindings( ashes::DescriptorSetLayoutBindingArray & bindings )
	{
		bindings.emplace_back( 1u, ashes::DescriptorType::eUniformBuffer, ashes::ShaderStageFlag::eVertex );
	}

	void NodesRenderer::doFillBillboardDescriptorSet( ashes::DescriptorSetLayout & descriptorLayout
		, ashes::DescriptorSet & descriptorSet )
	{
		descriptorSet.createBinding( descriptorLayout.getBinding( 1u )
			, m_sceneUbo
			, 0u
			, 1u );
	}
}
