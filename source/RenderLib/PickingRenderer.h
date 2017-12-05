/**
*\file
*	PickingRenderer.h
*\author
*	Sylvain Doremus
*/
#ifndef ___RenderLib_PickingRenderer_HPP___
#define ___RenderLib_PickingRenderer_HPP___
#pragma once

#include "Material.h"
#include "Mesh.h"
#include <Renderer/Texture.hpp>
#include "UberShader.h"

#include <Renderer/DescriptorSet.hpp>
#include <Renderer/DescriptorSetLayout.hpp>
#include <Renderer/DescriptorSetPool.hpp>
#include <Renderer/Pipeline.hpp>
#include <Renderer/PipelineLayout.hpp>
#include <Renderer/ShaderProgram.hpp>
#include <Renderer/UniformBuffer.hpp>
#include <Renderer/VertexLayout.hpp>

#include <Utils/RangedValue.hpp>

#include <array>
#include <functional>

namespace render
{
	//! Le masque utilisé pour définir un objet.
	static uint8_t constexpr ObjectMask = 0x10;
	//! Le masque utilisé pour définir un billboard.
	static uint8_t constexpr BillboardMask = 0x20;
	/**
	*\brief
	*	Classe stockant les noeuds de rendu utilisés pour le dessin du picking.
	*\remarks
	*	Afin de pouvoir sélectionner facilement les petits objets, un
	*	multiplicateur de mise à l'échelle est appliqué lors du rendu, pour le
	*	agrandir.
	*/
	class PickingRenderer
	{
	private:
		struct MatrixUbo
		{
			//! La variable uniforme contenant la matrice de projection.
			utils::Mat4 projection;
			//! La variable uniforme contenant la matrice de vue.
			utils::Mat4 view;
			//! La variable uniforme contenant la matrice du modèle.
			utils::Mat4 model;
		};
		struct PickingUbo
		{
			//! La variable uniforme contenant l'indice du draw call.
			int drawIndex;
			//! La variable uniforme contenant l'indice de l'objet dans le draw call.
			int nodeIndex;
		};
		struct BillboardUbo
		{
			//! La variable uniforme contenant les dimensions du billboard.
			utils::Vec2 dimensions;
			//! La variable uniforme contenant la position de la caméra.
			utils::Vec3 camera;
		};
		/**
		*\brief
		*	Contient les informations communes d'un noeud de rendu.
		*/
		struct RenderNode
		{
			/**
			*\brief
			*	Constructeur.
			*\param[in,out] program
			*	Le programme depuis lequel les variables sont récupérées.
			*/
			RenderNode( renderer::Device const & device
				, renderer::DescriptorSetLayoutPtr && layout
				, renderer::ShaderProgramPtr && program );
			//! Le programme shader.
			renderer::ShaderProgramPtr m_program;
			//! L'UBO contenant les matrices.
			renderer::UniformBuffer< MatrixUbo > m_mtxUbo;
			//! L'UBO contenant les informations de picking.
			renderer::UniformBuffer< PickingUbo > m_pickUbo;
			//! Le layout des descriptor sets du noeud.
			renderer::DescriptorSetLayoutPtr m_descriptorLayout;
			//! Le pool de descriptor set du noeud.
			renderer::DescriptorSetPoolPtr m_descriptorPool;
			//! Le descriptor set de ce noeud.
			renderer::DescriptorSetPtr m_descriptor;
		};
		/**
		*\brief
		*	Contient les informations d'un noeud de rendu d'objets.
		*/
		struct ObjectNode
			: RenderNode
		{
			/**
			*\brief
			*	Constructeur.
			*\param[in,out] program
			*	Le programme depuis lequel les variables sont récupérées.
			*/
			ObjectNode( renderer::Device const & device
				, renderer::RenderPass const & renderPass
				, renderer::DescriptorSetLayoutPtr && layout
				, renderer::ShaderProgramPtr && program
				, NodeType type );
			//! Le layout du tampon de positions.
			renderer::VertexLayoutPtr m_posLayout;
			//! Le layout du tampon de coordonnées de texture.
			renderer::VertexLayoutPtr m_texLayout;
			//! Le layout du pipeline.
			renderer::PipelineLayoutPtr m_pipelineLayout;
			//! Le pipeline.
			renderer::PipelinePtr m_pipeline;
		};
		//! Un pointeur sur un ObjectNode.
		using ObjectNodePtr = std::unique_ptr< ObjectNode >;
		//! Un tableau d'ObjectNode, un par type de noeud à créer.
		using ObjectNodeArray = std::array< ObjectNodePtr, size_t( NodeType::eCount ) >;
		/**
		*\brief
		*	Contient les informations d'un noeud de rendu de billboards.
		*/
		struct BillboardNode
			: RenderNode
		{
			/**
			*\brief
			*	Constructeur.
			*\param[in,out] program
			*	Le programme depuis lequel les variables sont récupérées.
			*/
			BillboardNode( renderer::Device const & device
				, renderer::RenderPass const & renderPass
				, renderer::DescriptorSetLayoutPtr && layout
				, renderer::ShaderProgramPtr && program
				, NodeType type );
			//! L'UBO contenant les variables liées au billboard.
			renderer::UniformBuffer< BillboardUbo > m_billboardUbo;
			//! Le layout du tampon de positions.
			renderer::VertexLayoutPtr m_layout;
			//! Le layout du pipeline.
			renderer::PipelineLayoutPtr m_pipelineLayout;
			//! Le pipeline.
			renderer::PipelinePtr m_pipeline;
		};
		//! Un pointeur sur un BillboardNode.
		using BillboardNodePtr = std::unique_ptr< BillboardNode >;
		//! Un tableau de BillboardNode, un par type de noeud à créer.
		using BillboardNodeArray = std::array< BillboardNodePtr, size_t( NodeType::eCount ) >;

	public:
		/**
		*\brief
		*	Constructeur.
		*/
		PickingRenderer( renderer::Device const & device
			, renderer::RenderPass const & renderPass );
		/**
		*\brief
		*	Crée tous les noeuds de rendu.
		*/
		void initialise();
		/**
		*\brief
		*	Supprime tous les noeuds de rendu.
		*/
		void cleanup();
		/**
		*\brief
		*	Dessine les objets de la scène, à travers la vue de la caméra.
		*\param[in] camera
		*	La caméra.
		*\param[in] zoomPercent
			Le pourcentage du zoom actuel.
		*\param[in] objects
		*	Les objets à dessiner.
		*\param[in] billboards
		*	Les billboards à dessiner.
		*/
		void draw( renderer::StagingBuffer const & stagingBuffer
			, renderer::CommandBuffer const & commandBuffer
			, Camera const & camera
			, float zoomPercent
			, RenderSubmeshArray const & objects
			, RenderBillboardArray const & billboards )const;

	private:
		void doRenderTransparent( renderer::StagingBuffer const & stagingBuffer
			, renderer::CommandBuffer const & commandBuffer
			, Camera const & camera
			, float zoomPercent
			, NodeType type
			, OpacityType opacity
			, RenderSubmeshArray const & objects
			, RenderBillboardArray const & billboards )const;
		void doRenderObjects( renderer::StagingBuffer const & stagingBuffer
			, renderer::CommandBuffer const & commandBuffer
			, Camera const & camera
			, float zoomPercent
			, NodeType type
			, ObjectNode & node
			, RenderSubmeshVector const & objects )const;
		void doRenderBillboards( renderer::StagingBuffer const & stagingBuffer
			, renderer::CommandBuffer const & commandBuffer
			, Camera const & camera
			, float zoomPercent
			, NodeType type
			, BillboardNode & node
			, RenderBillboardVector const & billboards )const;

	private:
		renderer::Device const & m_device;
		renderer::RenderPass const & m_renderPass;
		ObjectNodeArray m_objectNodes;
		BillboardNodeArray m_billboardNodes;
		utils::Range< float > m_billboardScale{ utils::makeRange( 1.0f, 20.0f ) };
		utils::Range< float > m_objectScale{ utils::makeRange( 1.0f, 5.0f ) };
	};
}

#endif
