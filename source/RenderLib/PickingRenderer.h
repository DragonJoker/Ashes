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

#include <Renderer/RangedValue.hpp>
#include <Renderer/UniformBuffer.hpp>
#include <Renderer/Pipeline.hpp>
#include <Renderer/ShaderProgram.hpp>

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
			renderer::Mat4 projection;
			//! La variable uniforme contenant la matrice de vue.
			renderer::Mat4 view;
			//! La variable uniforme contenant la matrice du modèle.
			renderer::Mat4 model;
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
			renderer::Vec2 dimensions;
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
			//! L'UBO contenant les informations de picking.
			renderer::UniformBuffer< PickingUbo > m_pickUbo;
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
			//! La variable uniforme contenant la mise à l'échelle.
			renderer::FloatUniformPtr m_scale;
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
			//! Attribut d'identifiant.
			renderer::FloatAttributePtr m_id;
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
		PickingRenderer( renderer::RenderingResources const & resources );
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
		void doRenderTransparent( Camera const & camera
			, float zoomPercent
			, NodeType type
			, OpacityType opacity
			, RenderSubmeshArray const & objects
			, RenderBillboardArray const & billboards )const;
		void doRenderObjects( Camera const & camera
			, float zoomPercent
			, NodeType type
			, ObjectNode const & node
			, RenderSubmeshVector const & objects )const;
		void doRenderBillboards( Camera const & camera
			, float zoomPercent
			, NodeType type
			, BillboardNode const & node
			, BillboardArray const & billboards )const;

	private:
		renderer::RenderingResources const & m_resources;
		ObjectNodeArray m_objectNodes;
		BillboardNodeArray m_billboardNodes;
		renderer::Pipeline m_pipelineOpaque;
		renderer::Range< float > m_billboardScale{ renderer::makeRange( 1.0f, 20.0f ) };
		renderer::Range< float > m_objectScale{ renderer::makeRange( 1.0f, 5.0f ) };
	};
}

#endif
