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

#include <Renderer/Pipeline.hpp>
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
			renderer::Mat4 projection;
			//! La variable uniforme contenant la matrice de vue.
			renderer::Mat4 view;
			//! La variable uniforme contenant la matrice du modèle.
			renderer::Mat4 model;
		};
		//! Les données de l'UBO contenant les informations du matériau.
		struct MaterialUbo
		{
			//! La variable uniforme contenant la couleur ambiante.
			renderer::Vec3 ambient;
			//! La variable uniforme contenant la couleur diffuse.
			renderer::Vec3 diffuse;
			//! La variable uniforme contenant la couleur spéculaire.
			renderer::Vec3 specular;
			//! La variable uniforme contenant la couleur émissive.
			renderer::Vec3 emissive;
			//! La variable uniforme contenant l'exposant spéculaire.
			float exponent;
			//! La variable uniforme contenant l'opacité
			float opacity;
		};
		//! Les données de l'UBO contenant les variables liées au billboard.
		struct BillboardUbo
		{
			//! La variable uniforme contenant les dimensions du billboard.
			renderer::Vec2 dimensions;
			//! La variable uniforme contenant la position de la caméra.
			renderer::Vec3 camera;
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
			renderer::Vec3 camera;
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
			RenderNode( renderer::RenderingResources const & resources
				, renderer::ShaderProgramPtr && program );
			//! Le programme shader.
			renderer::ShaderProgramPtr m_program;
			//! L'UBO contenant les matrices.
			renderer::UniformBuffer< MatrixUbo > m_mtxUbo;
			//! L'UBO contenant les informations du matériau.
			renderer::UniformBuffer< MaterialUbo > m_matUbo;
			//! La variable uniforme contenant la texture de diffuse.
			renderer::IntUniformPtr m_mapDiffuse;
			//! La variable uniforme contenant la texture d'opacité.
			renderer::IntUniformPtr m_mapOpacity;
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
			ObjectNode( renderer::RenderingResources const & resources
				, renderer::ShaderProgramPtr && program );
			//! L'attribut de position.
			renderer::Vec3AttributePtr m_position;
			//! L'attribut de normale.
			renderer::Vec3AttributePtr m_normal;
			//! L'attribut de coordonnées de texture.
			renderer::Vec2AttributePtr m_texture;
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
			BillboardNode( renderer::RenderingResources const & resources
				, renderer::ShaderProgramPtr && program );
			//! L'UBO contenant les variables liées au billboard.
			renderer::UniformBuffer< BillboardUbo > m_billboardUbo;
			//! Attribut de position.
			renderer::Vec3AttributePtr m_position;
			//! Attribut d'échelle.
			renderer::Vec2AttributePtr m_scale;
			//! Attribut de coordonnées de texture.
			renderer::Vec2AttributePtr m_texture;
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
			PolyLineNode( renderer::RenderingResources const & resources
				, renderer::ShaderProgramPtr && program );
			//! L'UBO contenant les variables liées à la ligne.
			renderer::UniformBuffer< LineUbo > m_lineUbo;
			//! L'attribut de position
			renderer::Vec3AttributePtr m_position;
			//! L'attribut de normale
			renderer::Vec3AttributePtr m_normal;
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
		SceneRenderer( renderer::RenderingResources const & resources );
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
		void draw( Camera const & camera
			, float zoomScale
			, RenderSubmeshArray const & objects
			, RenderBillboardArray const & billboards
			, PolyLineArray const & lines )const;

	private:
		void doRenderTransparent( Camera const & camera
			, NodeType type
			, OpacityType opacity
			, RenderSubmeshArray const & objects
			, RenderBillboardArray const & billboards
			, PolyLineArray const & lines )const;
		void doRenderObjects( Camera const & camera
			, NodeType type
			, ObjectNode const & node
			, RenderSubmeshVector const & objects )const;
		void doRenderBillboards( Camera const & camera
			, NodeType type
			, BillboardNode const & node
			, BillboardArray const & billboards )const;
		void doRenderLines( Camera const & camera
			, float zoomScale
			, PolyLineNode const & node
			, PolyLineArray const & lines )const;

	private:
		renderer::RenderingResources const & m_resources;
		ObjectNodeArray m_objectNodes;
		BillboardNodeArray m_billboardNodes;
		PolyLineNodePtr m_lineNode;
		renderer::Pipeline m_pipelineOpaque;
		renderer::Pipeline m_pipelineAlphaBlend;
	};
}

#endif
