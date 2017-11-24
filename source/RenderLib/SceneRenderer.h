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
	private:
		//! Les données de l'UBO contenant les matrices.
		struct MatrixUbo
		{
			//! La variable uniforme contenant la matrice de projection.
			utils::Mat4 projection;
			//! La variable uniforme contenant la matrice de vue.
			utils::Mat4 view;
			//! La variable uniforme contenant la matrice du modèle.
			utils::Mat4 model;
		};
		//! Les données de l'UBO contenant les informations du matériau.
		struct MaterialUbo
		{
			//! La variable uniforme contenant la couleur ambiante.
			utils::Vec3 ambient;
			//! La variable uniforme contenant la couleur diffuse.
			utils::Vec3 diffuse;
			//! La variable uniforme contenant la couleur spéculaire.
			utils::Vec3 specular;
			//! La variable uniforme contenant la couleur émissive.
			utils::Vec3 emissive;
			//! La variable uniforme contenant l'exposant spéculaire.
			float exponent;
			//! La variable uniforme contenant l'opacité
			float opacity;
		};
		//! Les données de l'UBO contenant les variables liées au billboard.
		struct BillboardUbo
		{
			//! La variable uniforme contenant les dimensions du billboard.
			utils::Vec2 dimensions;
			//! La variable uniforme contenant la position de la caméra.
			utils::Vec3 camera;
		};
		//! Les données de l'UBO contenant les variables liées à la ligne.
		struct LineUbo
		{
			//! La variable uniforme contenant la largeur de la ligne.
			float lineWidth;
			//! La variable uniforme contenant la plume.
			float lineFeather;
			//! La variable uniforme contenant l'échelle.
			float lineScale;
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
				, renderer::DescriptorSetLayout && layout
				, renderer::ShaderProgramPtr && program
				, NodeType type );
			//! Le programme shader.
			renderer::ShaderProgramPtr m_program;
			//! L'UBO contenant les matrices.
			renderer::UniformBuffer< MatrixUbo > m_mtxUbo;
			//! L'UBO contenant les informations du matériau.
			renderer::UniformBuffer< MaterialUbo > m_matUbo;
			//! Le layout des descriptor sets du noeud, pour les UBO.
			renderer::DescriptorSetLayout m_uboDescriptorLayout;
			//! Le pool de descriptor set du noeud, pour les UBO.
			renderer::DescriptorSetPool m_uboDescriptorPool;
			//! Le descriptor set de ce noeud, pour les UBO.
			renderer::DescriptorSet m_uboDescriptor;
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
				, renderer::DescriptorSetLayout && layout
				, renderer::ShaderProgramPtr && program
				, NodeType type );
			//! Le layout du pipeline.
			renderer::PipelineLayout m_pipelineLayout;
			//! Le pipeline.
			renderer::PipelinePtr m_pipeline;
			//! Le layout du tampon de positions.
			renderer::VertexLayout m_posLayout;
			//! Le layout du tampon de normales.
			renderer::VertexLayout m_nmlLayout;
			//! Le layout du tampon de coordonnées de texture.
			renderer::VertexLayout m_texLayout;
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
				, renderer::DescriptorSetLayout && layout
				, renderer::ShaderProgramPtr && program
				, NodeType type );
			//! L'UBO contenant les variables liées au billboard.
			renderer::UniformBuffer< BillboardUbo > m_billboardUbo;
			//! Le layout du tampon de positions.
			renderer::VertexLayout m_layout;
			//! Le layout du pipeline.
			renderer::PipelineLayout m_pipelineLayout;
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
				, renderer::DescriptorSetLayout && layout
				, renderer::ShaderProgramPtr && program
				, NodeType type );
			//! L'UBO contenant les variables liées à la ligne.
			renderer::UniformBuffer< LineUbo > m_lineUbo;
			//! Le layout du tampon de positions.
			renderer::VertexLayout m_layout;
			//! Le layout du pipeline.
			renderer::PipelineLayout m_pipelineLayout;
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
		SceneRenderer( renderer::Device const & device );
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
		void draw( renderer::StagingBuffer const & stagingBuffer
			, renderer::CommandBuffer const & commandBuffer
			, Camera const & camera
			, float zoomScale
			, RenderSubmeshArray const & objects
			, RenderBillboardArray const & billboards
			, RenderPolyLineArray const & lines )const;

	private:
		void doRenderTransparent( renderer::StagingBuffer const & stagingBuffer
			, renderer::CommandBuffer const & commandBuffer
			, Camera const & camera
			, NodeType type
			, OpacityType opacity
			, RenderSubmeshArray const & objects
			, RenderBillboardArray const & billboards
			, RenderPolyLineArray const & lines )const;
		void doRenderObjects( renderer::StagingBuffer const & stagingBuffer
			, renderer::CommandBuffer const & commandBuffer
			, Camera const & camera
			, NodeType type
			, ObjectNode & node
			, RenderSubmeshVector const & objects )const;
		void doRenderBillboards( renderer::StagingBuffer const & stagingBuffer
			, renderer::CommandBuffer const & commandBuffer
			, Camera const & camera
			, NodeType type
			, BillboardNode & node
			, RenderBillboardVector const & billboards )const;
		void doRenderLines( renderer::StagingBuffer const & stagingBuffer
			, renderer::CommandBuffer const & commandBuffer
			, Camera const & camera
			, float zoomScale
			, PolyLineNode & node
			, RenderPolyLineVector const & lines )const;

	private:
		renderer::Device const & m_device;
		ObjectNodeArray m_objectNodes;
		BillboardNodeArray m_billboardNodes;
		PolyLineNodePtr m_lineNode;
	};
}

#endif
