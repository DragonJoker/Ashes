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
#include "RangedValue.h"
#include "Texture.h"
#include "UberShader.h"

#include <GlLib/GlPipeline.h>
#include <GlLib/GlShaderProgram.h>
#include <GlLib/GlUniformBuffer.h>

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
			//! La variable uniforme contenant la texture d'opacité.
			gl::IntUniformPtr m_mapOpacity;
			//! L'UBO contenant les informations de picking.
			gl::UniformBuffer m_pickUbo;
			//! La variable uniforme contenant l'indice du draw call.
			gl::IntUniform * m_drawIndex;
			//! La variable uniforme contenant l'indice de l'objet dans le draw call.
			gl::IntUniform * m_nodeIndex;
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
			//! La variable uniforme contenant la mise à l'échelle.
			gl::FloatUniformPtr m_scale;
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
			//! Attribut d'identifiant.
			gl::FloatAttributePtr m_id;
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
		PickingRenderer();
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
		void draw( Camera const & camera
			, float zoomPercent
			, RenderSubmeshArray const & objects
			, RenderBillboardArray const & billboards )const;

	private:
		/**
		*\brief
		*	Dessine les objets transparents de la scène, à travers la vue
		*	de la caméra.
		*\param[in] camera
		*	La caméra.
		*\param[in] zoomPercent
		Le pourcentage du zoom actuel.
		*\param[in] type
		*	Le type de noeud à dessiner.
		*\param[in] opacity
		*	Le type d'opacité.
		*\param[in] objects
		*	Les objets à dessiner.
		*\param[in] billboards
		*	Les billboards à dessiner.
		*/
		void doRenderTransparent( Camera const & camera
			, float zoomPercent
			, NodeType type
			, OpacityType opacity
			, RenderSubmeshArray const & objects
			, RenderBillboardArray const & billboards )const;
		/**
		*\brief
		*	Dessine une liste d'objets complexes du type donné.
		*\param[in] camera
		*	La caméra.
		*\param[in] zoomPercent
		Le pourcentage du zoom actuel.
		*\param[in] type
		*	Le type de noeud à dessiner.
		*\param[in] node
		*	Le noeud de rendu.
		*\param[in] objects
		*	Les objets à dessiner.
		*/
		void doRenderObjects( Camera const & camera
			, float zoomPercent
			, NodeType type
			, ObjectNode const & node
			, RenderSubmeshVector const & objects )const;
		/**
		*\brief
		*	Dessine une liste de billboards du type donné.
		*\param[in] camera
		*	La caméra.
		*\param[in] zoomPercent
		Le pourcentage du zoom actuel.
		*\param[in] type
		*	Le type de noeud à dessiner.
		*\param[in] node
		*	Le noeud de rendu.
		*\param[in] billboards
		*	Les billboards à dessiner.
		*/
		void doRenderBillboards( Camera const & camera
			, float zoomPercent
			, NodeType type
			, BillboardNode const & node
			, BillboardArray const & billboards )const;

	private:
		//! Les noeuds de rendu d'objets complexes.
		ObjectNodeArray m_objectNodes;
		//! Les noeuds de rendu de billboards.
		BillboardNodeArray m_billboardNodes;
		//! Le pipeline de rendu des objets opaques (ainsi que les objets avec alpha testing).
		gl::Pipeline m_pipelineOpaque;
		//! L'intervalle d'échelle pour les billboards.
		Range< float > m_billboardScale{ makeRange( 1.0f, 20.0f ) };
		//! L'intervalle d'échelle pour les objets complexes.
		Range< float > m_objectScale{ makeRange( 1.0f, 5.0f ) };
	};
}

#endif
