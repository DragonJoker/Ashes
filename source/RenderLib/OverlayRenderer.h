/**
*\file
*	OverlayRenderer.h
*\author
*	Sylvain Doremus
*/
#ifndef ___RenderLib_OverlayRenderer_H___
#define ___RenderLib_OverlayRenderer_H___
#pragma once

#include "FontTexture.h"
#include "OverlayCategory.h"
#include "Viewport.h"
#include "UberShader.h"

#include <Renderer/DescriptorSet.hpp>
#include <Renderer/DescriptorSetLayout.hpp>
#include <Renderer/DescriptorSetPool.hpp>
#include <Renderer/Pipeline.hpp>
#include <Renderer/PipelineLayout.hpp>
#include <Renderer/ShaderProgram.hpp>
#include <Renderer/UniformBuffer.hpp>

#include <unordered_map>

namespace render
{
	/**
	*\brief
	*	Description du contenu du tampon d'uniformes pour les incrustations.
	*/
	struct OverlayUbo
	{
		utils::Mat4 modelProj;
		utils::Vec4 colour;
	};
	/**
	*\brief
	*	Noeud de rendu utilisé pour dessiner des incrustations.
	*/
	struct OverlayNode
	{
		/**
		*\brief
		*	Constructeur.
		*\param[in] text
		*	Dit si c'est un programme pour les incrustations texte.
		*/
		OverlayNode( renderer::Device const & device
			, renderer::RenderPass const & renderPass
			, bool text
			, OpacityType opacity
			, TextureFlags textures );

		//! Le programme sshader utilisé pour dessiner les incrustations.
		renderer::ShaderProgramPtr m_program;
		//! Le layout du pipeline.
		renderer::PipelineLayout m_pipelineLayout;
		//! Le pipeline.
		renderer::PipelinePtr m_pipeline;
		//! Le layout du tampon de sommets.
		renderer::VertexLayout m_layout;
		//! Le tampon de variables uniformes pour les incrustations.
		renderer::UniformBuffer< OverlayUbo > m_overlayUbo;
		//! Le layout des descriptor sets du noeud, pour le matériau.
		renderer::DescriptorSetLayout m_materialDescriptorLayout;
		//! Le pool de descriptor set du noeud, pour le matériau.
		renderer::DescriptorSetPool m_materialDescriptorPool;
		//! Le layout des descriptor sets du noeud, pour les UBO.
		renderer::DescriptorSetLayout m_uboDescriptorLayout;
		//! Le pool de descriptor set du noeud, pour les UBO.
		renderer::DescriptorSetPool m_uboDescriptorPool;
		//! Le descriptor set de ce noeud, pour les UBO.
		renderer::DescriptorSet m_uboDescriptor;
	};
	//! Tableau de noeuds de rendu d'incrustations.
	using OverlayNodeArray = std::array< OverlayNode, size_t( NodeType::eCount ) >;
	/**
	*\brief
	*	Le renderer d'incrustations.
	*/
	class OverlayRenderer
	{
	public:
		/**
		*\brief
		*	Constructeur.
		*\param[in] maxCharsPerBuffer
		*	Le nombre maximal de caractères par tampon de sommets de texte.
		*/
		explicit OverlayRenderer( renderer::Device const & device
			, renderer::RenderPass const & renderPass
			, uint32_t maxCharsPerBuffer = 600 );
		/**
		*brief
		*	Destructeur.
		*/
		~OverlayRenderer();
		/**
		*brief
		*	Enregistre un PanelOverlay.
		*\param[in] overlay
		*	L'incrustation à enregistrer.
		*/
		void registerOverlay( PanelOverlay const & overlay );
		/**
		*brief
		*	Enregistre un BorderPanelOverlay.
		*\param[in] overlay
		*	L'incrustation à enregistrer.
		*/
		void registerOverlay( BorderPanelOverlay const & overlay );
		/**
		*brief
		*	Enregistre un TextOverlay.
		*\param[in] overlay
		*	L'incrustation à enregistrer.
		*/
		void registerOverlay( TextOverlay const & overlay );
		/**
		*brief
		*	Désenregistre un PanelOverlay.
		*\param[in] overlay
		*	L'incrustation à désenregistrer.
		*/
		void unregisterOverlay( PanelOverlay const & overlay );
		/**
		*brief
		*	Désenregistre un BorderPanelOverlay.
		*\param[in] overlay
		*	L'incrustation à désenregistrer.
		*/
		void unregisterOverlay( BorderPanelOverlay const & overlay );
		/**
		*brief
		*	Désenregistre un TextOverlay.
		*\param[in] overlay
		*	L'incrustation à désenregistrer.
		*/
		void unregisterOverlay( TextOverlay const & overlay );
		/**
		*brief
		*	Commence le rendu des incrustations.
		*param[in] size
		*	Les dimensions de la fenêtre de rendu.
		*/
		void beginRender( utils::IVec2 const & size );
		/**
		*brief
		*	Termine le rendu des incrustations.
		*/
		void endRender();
		/**
		*brief
		*	Fonction de dessin des TextOverlays.
		*\param[in] overlays
		*	Les incrustations texte à dessiner.
		*/
		void draw( renderer::StagingBuffer const & stagingBuffer
			, renderer::CommandBuffer const & commandBuffer
			, OverlayList const & overlays );
		/**
		*brief
		*	Dessine un PanelOverlay.
		*\param[in] overlay
		*	L'incrustation à dessiner.
		*/
		void drawPanel( renderer::StagingBuffer const & stagingBuffer
			, renderer::CommandBuffer const & commandBuffer
			, PanelOverlay const & overlay );
		/**
		*brief
		*	Dessine un BorderPanelOverlay.
		*\param[in] overlay
		*	L'incrustation à dessiner.
		*/
		void drawBorderPanel( renderer::StagingBuffer const & stagingBuffer
			, renderer::CommandBuffer const & commandBuffer
			, BorderPanelOverlay const & overlay );
		/**
		*brief
		*	Dessine un TextOverlay.
		*\param[in] overlay
		*	L'incrustation à dessiner.
		*/
		void drawText( renderer::StagingBuffer const & stagingBuffer
			, renderer::CommandBuffer const & commandBuffer
			, TextOverlay const & overlay );

	private:
		/**
		*\brief
		*	Crée un tampon de sommets et ses attrobite pour les incrustations
		*	texte, et l'ajoute à la liste.
		*/
		renderer::VertexBuffer< Overlay::Quad > const & doCreateTextBuffer( renderer::StagingBuffer const & stagingBuffer
			, renderer::CommandBuffer const & commandBuffer );
		/**
		*brief
		*	Fonction de dessin d'une incrustation.
		*param[in] buffer
		*	Le tampon de la géométrie de l'incrustation.
		*param[in] count
		*	Le nombre de sommets.
		*param[in] transform
		*	La matrice de transformation de l'incrustation.
		*param[in] material
		*	Le matériau de l'incrustation.
		*param[in] node
		*	Le noeud de rendu.
		*/
		void doDrawBuffer( renderer::StagingBuffer const & stagingBuffer
			, renderer::CommandBuffer const & commandBuffer
			, renderer::VertexBuffer< Overlay::Quad > const & buffer
			, uint32_t count
			, utils::Mat4 const & transform
			, Material const & material
			, OverlayNode & node
			, renderer::DescriptorSet const & descriptor );
		/**
		*brief
		*	Remplit un GeometryBuffers d'une partie d'un tableau de sommets
		*	pour texte.
		*param[in] count
		*	Le nombre de sommets à copier dans le GeometryBuffers.
		*param[in] it
		*	Le début du tableau de sommets.
		*param[in] index
		*	L'indice courant dans le tableau de GeometryBuffers de texte.
		*return
		*	Le GeometryBuffers utilisé.
		*/
		renderer::VertexBuffer< Overlay::Quad > const & doFillTextPart( renderer::StagingBuffer const & stagingBuffer
			, renderer::CommandBuffer const & commandBuffer
			, uint32_t count
			, uint32_t & offset
			, Overlay::QuadArray::const_iterator & it
			, uint32_t & index );

	private:
		renderer::Device const & m_device;
		renderer::RenderPass const & m_renderPass;
		OverlayNodeArray m_panelNodes;
		OverlayNode m_textNode;
		renderer::VertexBufferPtr< Overlay::Quad > m_panelBuffer;
		renderer::VertexBufferPtr< Overlay::Quad > m_borderBuffer;
		std::vector< renderer::VertexBufferPtr< Overlay::Quad > > m_textBuffers;
		uint32_t m_maxCharsPerBuffer;
		bool m_sizeChanged{ true };
		utils::Mat4 m_transform;
		Viewport m_viewport;
		using MateriaDescriptorMap = std::unordered_map< Overlay const *, renderer::DescriptorSet >;
		MateriaDescriptorMap m_panelOverlays;
		MateriaDescriptorMap m_borderOverlays;
		MateriaDescriptorMap m_textOverlays;
	};
}

#endif
