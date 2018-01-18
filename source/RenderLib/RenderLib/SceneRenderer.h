/**
*\file
*	SceneRenderer.h
*\author
*	Sylvain Doremus
*/
#ifndef ___RenderLib_SceneRenderer_HPP___
#define ___RenderLib_SceneRenderer_HPP___
#pragma once

#include "Material.h"
#include "Mesh.h"
#include <Renderer/Image/Texture.hpp>
#include "UberShader.h"

#include <Renderer/Command/CommandBuffer.hpp>
#include <Renderer/Command/CommandPool.hpp>
#include <Renderer/Descriptor/DescriptorSet.hpp>
#include <Renderer/Descriptor/DescriptorSetLayout.hpp>
#include <Renderer/Descriptor/DescriptorSetPool.hpp>
#include <Renderer/Pipeline/Pipeline.hpp>
#include <Renderer/Pipeline/PipelineLayout.hpp>
#include <Renderer/Shader/ShaderProgram.hpp>
#include <Renderer/Buffer/UniformBuffer.hpp>

#include <array>
#include <functional>

namespace render
{
	/**
	*\brief
	*	Classe stockant les noeuds de rendu utilisés pour le dessin de la
	*	scène.
	*/
	class SceneRenderer
	{
	public:
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
			renderer::UniformBufferPtr< MatrixUbo > m_mtxUbo;
			//! L'UBO contenant les informations du matériau.
			renderer::UniformBufferPtr< MaterialUbo > m_matUbo;
			//! Le layout des descriptor sets du noeud.
			renderer::DescriptorSetLayoutPtr m_descriptorLayout;
			//! Le pool de descriptor set du noeud.
			renderer::DescriptorSetPoolPtr m_descriptorPool;
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
			//! Le layout du tampon de normales.
			renderer::VertexLayoutPtr m_nmlLayout;
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
			renderer::UniformBufferPtr< BillboardUbo > m_billboardUbo;
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
		/**
		*\brief
		*	Contient les informations d'un noeud de rendu de lignes.
		*/
		struct PolyLineNode
			: RenderNode
		{
			/**
			*\brief
			*	Constructeur.
			*\param[in,out] program
			*	Le programme depuis lequel les variables sont récupérées.
			*/
			PolyLineNode( renderer::Device const & device
				, renderer::RenderPass const & renderPass
				, renderer::DescriptorSetLayoutPtr && layout
				, renderer::ShaderProgramPtr && program
				, NodeType type );
			//! L'UBO contenant les variables liées à la ligne.
			renderer::UniformBufferPtr< LineUbo > m_lineUbo;
			//! Le layout du tampon de positions.
			renderer::VertexLayoutPtr m_layout;
			//! Le layout du pipeline.
			renderer::PipelineLayoutPtr m_pipelineLayout;
			//! Le pipeline.
			renderer::PipelinePtr m_pipeline;
		};
		//! Un pointeur sur un PolyLineNode.
		using PolyLineNodePtr = std::unique_ptr< PolyLineNode >;
		//! Un tableau de PolyLineNode, un par type de noeud à créer.
		using PolyLineNodeArray = std::array< PolyLineNodePtr, size_t( NodeType::eCount ) >;

	public:
		/**
		*\brief
		*	Constructeur.
		*/
		SceneRenderer( renderer::Device const & device
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
		*\param[in] zoomScale
		*	L'échelle calculée par rapport au zoom.
		*\param[in] objects
		*	Les objets à dessiner.
		*\param[in] billboards
		*	Les billboards à dessiner.
		*\param[in] lines
		*	Les polylignes à dessiner.
		*/
		void draw( renderer::FrameBuffer const & frameBuffer
			, Camera const & camera
			, float zoomScale
			, RenderSubmeshArray const & objects
			, RenderBillboardArray const & billboards
			, RenderPolyLineArray const & lines )const;
		/**
		*\brief
		*	Dessine les objets de la scène, à travers la vue de la caméra.
		*\param[in] camera
		*	La caméra.
		*\param[in] zoomScale
		*	L'échelle calculée par rapport au zoom.
		*\param[in] objects
		*	Les objets à dessiner.
		*\param[in] billboards
		*	Les billboards à dessiner.
		*\param[in] lines
		*	Les polylignes à dessiner.
		*/
		void draw( renderer::CommandBuffer const & commandBuffer
			, Camera const & camera
			, float zoomScale
			, RenderSubmeshArray const & objects
			, RenderBillboardArray const & billboards
			, RenderPolyLineArray const & lines )const;
		/**
		*\brief
		*	Récupère le noeud d'objet pour le type donné.
		*\param[in] node
		*	Le type de noeud.
		*\return
		*	Le noeud.
		*/
		ObjectNode const & getObjectNode( NodeType node )const
		{
			return *m_objectNodes[size_t( node )];
		}
		/**
		*\brief
		*	Récupère le noeud de billboard pour le type donné.
		*\param[in] node
		*	Le type de noeud.
		*\return
		*	Le noeud.
		*/
		BillboardNode const & getBillboardNode( NodeType node )const
		{
			return *m_billboardNodes[size_t( node )];
		}
		/**
		*\brief
		*	Récupère le noeud de billboard pour le type donné.
		*\param[in] node
		*	Le type de noeud.
		*\return
		*	Le noeud.
		*/
		PolyLineNode const & getPolyLineNode()const
		{
			return *m_lineNode;
		}
		/**
		*\brief
		*	Récupère le noeud d'objet pour le type donné.
		*\param[in] node
		*	Le type de noeud.
		*\return
		*	Le noeud.
		*/
		ObjectNode & getObjectNode( NodeType node )
		{
			return *m_objectNodes[size_t( node )];
		}
		/**
		*\brief
		*	Récupère le noeud de billboard pour le type donné.
		*\param[in] node
		*	Le type de noeud.
		*\return
		*	Le noeud.
		*/
		BillboardNode & getBillboardNode( NodeType node )
		{
			return *m_billboardNodes[size_t( node )];
		}
		/**
		*\return
		*	Le noeud de polyligne.
		*/
		PolyLineNode & getPolyLineNode()
		{
			return *m_lineNode;
		}
		/**
		*\return
		*	Le tampon de commandes de rendu.
		*/
		renderer::CommandBuffer const & commandBuffer()const
		{
			return *m_commandBuffer;
		}

	private:
		void doRenderTransparent( renderer::CommandBuffer const & commandBuffer
			, Camera const & camera
			, NodeType type
			, OpacityType opacity
			, RenderSubmeshArray const & objects
			, RenderBillboardArray const & billboards
			, RenderPolyLineArray const & lines )const;
		void doRenderObjects( renderer::CommandBuffer const & commandBuffer
			, Camera const & camera
			, NodeType type
			, ObjectNode & node
			, RenderSubmeshVector const & objects )const;
		void doRenderBillboards( renderer::CommandBuffer const & commandBuffer
			, Camera const & camera
			, NodeType type
			, BillboardNode & node
			, RenderBillboardVector const & billboards )const;
		void doRenderLines( renderer::CommandBuffer const & commandBuffer
			, Camera const & camera
			, float zoomScale
			, PolyLineNode & node
			, RenderPolyLineVector const & lines )const;

	private:
		renderer::Device const & m_device;
		renderer::RenderPass const & m_renderPass;
		ObjectNodeArray m_objectNodes;
		BillboardNodeArray m_billboardNodes;
		PolyLineNodePtr m_lineNode;
		renderer::CommandPoolPtr m_commandPool;
		renderer::CommandBufferPtr m_commandBuffer;
	};
}

#endif
