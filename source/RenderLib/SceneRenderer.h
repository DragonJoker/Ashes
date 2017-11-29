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
#include <Renderer/Texture.hpp>
#include "UberShader.h"

#include <Renderer/CommandBuffer.hpp>
#include <Renderer/CommandPool.hpp>
#include <Renderer/DescriptorSet.hpp>
#include <Renderer/DescriptorSetLayout.hpp>
#include <Renderer/DescriptorSetPool.hpp>
#include <Renderer/Pipeline.hpp>
#include <Renderer/PipelineLayout.hpp>
#include <Renderer/ShaderProgram.hpp>
#include <Renderer/UniformBuffer.hpp>

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
				, renderer::DescriptorSetLayout && layout
				, renderer::ShaderProgramPtr && program );
			//! Le programme shader.
			renderer::ShaderProgramPtr m_program;
			//! L'UBO contenant les matrices.
			renderer::UniformBuffer< MatrixUbo > m_mtxUbo;
			//! L'UBO contenant les informations du matériau.
			renderer::UniformBuffer< MaterialUbo > m_matUbo;
			//! Le layout des descriptor sets du noeud.
			renderer::DescriptorSetLayout m_descriptorLayout;
			//! Le pool de descriptor set du noeud.
			renderer::DescriptorSetPool m_descriptorPool;
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
				, renderer::DescriptorSetLayout && layout
				, renderer::ShaderProgramPtr && program
				, NodeType type );
			//! Le layout du tampon de positions.
			renderer::VertexLayout m_posLayout;
			//! Le layout du tampon de normales.
			renderer::VertexLayout m_nmlLayout;
			//! Le layout du tampon de coordonnées de texture.
			renderer::VertexLayout m_texLayout;
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
				, renderer::DescriptorSetLayout && layout
				, renderer::ShaderProgramPtr && program
				, NodeType type );
			//! L'UBO contenant les variables liées au billboard.
			renderer::UniformBuffer< BillboardUbo > m_billboardUbo;
			//! Le layout du tampon de positions.
			renderer::VertexLayout m_layout;
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
				, renderer::DescriptorSetLayout && layout
				, renderer::ShaderProgramPtr && program
				, NodeType type );
			//! L'UBO contenant les variables liées à la ligne.
			renderer::UniformBuffer< LineUbo > m_lineUbo;
			//! Le layout du tampon de positions.
			renderer::VertexLayout m_layout;
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
		*\brief
		*	Récupère le noeud de polyligne.
		*\return
		*	Le noeud.
		*/
		PolyLineNode & getPolyLineNode()
		{
			return *m_lineNode;
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
	};
}

#endif
