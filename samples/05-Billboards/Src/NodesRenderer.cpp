#include "NodesRenderer.hpp"

namespace vkapp
{
	NodesRenderer::NodesRenderer( utils::Device const & device
		, ashes::CommandPool const & commandPool
		, ashes::Queue const & transferQueue
		, std::string const & fragmentShaderFile
		, std::vector< VkFormat > const & formats
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

	void NodesRenderer::doFillBillboardDescriptorLayoutBindings( ashes::VkDescriptorSetLayoutBindingArray & bindings )
	{
		bindings.push_back( { 1u, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1u, VK_SHADER_STAGE_VERTEX_BIT, nullptr } );
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
