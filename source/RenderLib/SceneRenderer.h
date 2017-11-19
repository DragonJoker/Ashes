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
#include "Texture.h"
#include "UberShader.h"

#include <GlLib/GlPipeline.h>
#include <GlLib/GlShaderProgram.h>
#include <GlLib/GlUniformBuffer.h>

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
			RenderNode( gl::ShaderProgramPtr && program );
			//! Le programme shader.
			gl::ShaderProgramPtr m_program;
			//! L'UBO contenant les matrices.
			gl::UniformBuffer m_mtxUbo;
			//! La variable uniforme contenant la matrice de projection.
			gl::Mat4Uniform * m_mtxProjection;
			//! La variable uniforme contenant la matrice de vue.
			gl::Mat4Uniform * m_mtxView;
			//! La variable uniforme contenant la matrice du modèle.
			gl::Mat4Uniform * m_mtxModel;
			//! L'UBO contenant les informations du matériau.
			gl::UniformBuffer m_matUbo;
			//! La variable uniforme contenant la couleur ambiante.
			gl::Vec3Uniform * m_matAmbient;
			//! La variable uniforme contenant la couleur diffuse.
			gl::Vec3Uniform * m_matDiffuse;
			//! La variable uniforme contenant la couleur spéculaire.
			gl::Vec3Uniform * m_matSpecular;
			//! La variable uniforme contenant la couleur émissive.
			gl::Vec3Uniform * m_matEmissive;
			//! La variable uniforme contenant l'exposant spéculaire.
			gl::FloatUniform * m_matExponent;
			//! La variable uniforme contenant l'opacité
			gl::FloatUniform * m_matOpacity;
			//! La variable uniforme contenant la texture de diffuse.
			gl::IntUniformPtr m_mapDiffuse;
			//! La variable uniforme contenant la texture d'opacité.
			gl::IntUniformPtr m_mapOpacity;
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
			ObjectNode( gl::ShaderProgramPtr && program );
			//! L'attribut de position.
			gl::Vec3AttributePtr m_position;
			//! L'attribut de normale.
			gl::Vec3AttributePtr m_normal;
			//! L'attribut de coordonnées de texture.
			gl::Vec2AttributePtr m_texture;
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
			BillboardNode( gl::ShaderProgramPtr && program );
			//! L'UBO contenant les variables liées au billboard.
			gl::UniformBuffer m_billboardUbo;
			//! La variable uniforme contenant les dimensions du billboard.
			gl::Vec2Uniform * m_dimensions;
			//! La variable uniforme contenant la position de la caméra.
			gl::Vec3Uniform * m_camera;
			//! Attribut de position.
			gl::Vec3AttributePtr m_position;
			//! Attribut d'échelle.
			gl::Vec2AttributePtr m_scale;
			//! Attribut de coordonnées de texture.
			gl::Vec2AttributePtr m_texture;
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
			PolyLineNode( gl::ShaderProgramPtr && program );
			//! L'UBO contenant les variables liées à la ligne.
			gl::UniformBuffer m_lineUbo;
			//! La variable uniforme contenant la largeur de la ligne.
			gl::FloatUniform * m_lineWidth;
			//! La variable uniforme contenant la plume.
			gl::FloatUniform * m_lineFeather;
			//! La variable uniforme contenant l'échelle.
			gl::FloatUniform * m_lineScale;
			//! La variable uniforme contenant la position de la caméra.
			gl::Vec3Uniform * m_camera;
			//! L'attribut de position
			gl::Vec3AttributePtr m_position;
			//! L'attribut de normale
			gl::Vec3AttributePtr m_normal;
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
		SceneRenderer();
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
		/**
		*\brief
		*	Dessine les objets transparents de la scène, à travers la vue
		*	de la caméra.
		*\param[in] camera
		*	La caméra.
		*\param[in] type
		*	Le type de noeud à dessiner.
		*\param[in] opacity
		*	Le type d'opacité.
		*\param[in] objects
		*	Les objets à dessiner.
		*\param[in] billboards
		*	Les billboards à dessiner.
		*\param[in] lines
		*	Les polylignes à dessiner.
		*/
		void doRenderTransparent( Camera const & camera
			, NodeType type
			, OpacityType opacity
			, RenderSubmeshArray const & objects
			, RenderBillboardArray const & billboards
			, PolyLineArray const & lines )const;
		/**
		*\brief
		*	Dessine une liste d'objets complexes du type donné.
		*\param[in] camera
		*	La caméra.
		*\param[in] type
		*	Le type de noeud à dessiner.
		*\param[in] node
		*	Le noeud de rendu.
		*\param[in] objects
		*	Les objets à dessiner.
		*/
		void doRenderObjects( Camera const & camera
			, NodeType type
			, ObjectNode const & node
			, RenderSubmeshVector const & objects )const;
		/**
		*\brief
		*	Dessine une liste de billboards du type donné.
		*\param[in] camera
		*	La caméra.
		*\param[in] type
		*	Le type de noeud à dessiner.
		*\param[in] node
		*	Le noeud de rendu.
		*\param[in] billboards
		*	Les billboards à dessiner.
		*/
		void doRenderBillboards( Camera const & camera
			, NodeType type
			, BillboardNode const & node
			, BillboardArray const & billboards )const;
		/**
		*\brief
		*	Dessine une liste de lignes.
		*\param[in] camera
		*	La caméra.
		*\param[in] zoomScale
		*	L'échelle calculée par rapport au zoom.
		*\param[in] node
		*	Le noeud de rendu.
		*\param[in] lines
		*	Les billboards à dessiner.
		*/
		void doRenderLines( Camera const & camera
			, float zoomScale
			, PolyLineNode const & node
			, PolyLineArray const & lines )const;

	private:
		//! Les noeuds de rendu d'objets complexes.
		ObjectNodeArray m_objectNodes;
		//! Les noeuds de rendu de billboards.
		BillboardNodeArray m_billboardNodes;
		//! Les noeuds de rendu de lignes.
		PolyLineNodePtr m_lineNode;
		//! Le pipeline de rendu des objets opaques (ainsi que les objets avec alpha testing).
		gl::Pipeline m_pipelineOpaque;
		//! Le pipeline de rendu des objets avec alpha blending.
		gl::Pipeline m_pipelineAlphaBlend;
	};
}

#endif
