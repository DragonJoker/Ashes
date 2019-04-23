/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#ifndef ___AshesPP_CommandBuffer_HPP___
#define ___AshesPP_CommandBuffer_HPP___
#pragma once

#include "AshesPP/Buffer/PushConstantsBuffer.hpp"

namespace ashes
{
	/**
	*\~english
	*\brief
	*	A command buffer.
	*\~french
	*\brief
	*	Un tampon de commandes.
	*/
	class CommandBuffer
	{
	protected:
		CommandBuffer( CommandBuffer const & ) = delete;
		CommandBuffer & operator=( CommandBuffer const & ) = delete;
		CommandBuffer() = default;

	public:
		/**
		*\~english
		*\brief
		*	Constructor.
		*\param[in] device
		*	The logical device.
		*\param[in] pool
		*	The parent command buffer pool.
		*\param[in] primary
		*	Tells if the command buffer is primary (\p true) or not (\p false).
		*\~french
		*\brief
		*	Constructeur.
		*\param[in] device
		*	Le périphérique logique.
		*\param[in] pool
		*	Le pool de tampons de commandes.
		*\param[in] primary
		*	Dit si le tampon est un tampon de commandes primaire (\p true) ou secondaire (\p false).
		*/
		CommandBuffer( Device const & device
			, CommandPool const & pool
			, bool primary = true );
		/**
		*\~english
		*\brief
		*	Destructor.
		*\~french
		*\brief
		*	Destructeur.
		*/
		~CommandBuffer();
		/**
		*\~english
		*\brief
		*	Starts recording the command buffer.
		*\~french
		*\brief
		*	Démarre l'enregistrement du tampon de commandes.
		*/
		void begin( VkCommandBufferBeginInfo const & info )const;
		/**
		*\~english
		*\brief
		*	Starts recording the command buffer.
		*\param[in] flags
		*	The usage flags for the command buffer.
		*\return
		*	\p false on any problem.
		*\~french
		*\brief
		*	Démarre l'enregistrement du tampon de commandes.
		*\param[in] flags
		*	Les indicateurs de type de charge qui sera affectée au tampon.
		*/
		inline void begin( VkCommandBufferUsageFlags flags = 0u )const
		{
			begin( VkCommandBufferBeginInfo{ VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO, nullptr, flags, nullptr } );
		}
		/**
		*\~english
		*\brief
		*	Starts recording the command buffer as a secondary command buffer.
		*\param[in] flags
		*	The usage flags for the command buffer.
		*\param[in] inheritanceInfo
		*	The inheritance informations.
		*\return
		*	\p false on any error.
		*\~french
		*\brief
		*	Démarre l'enregistrement du tampon de commandes en tant que tampon secondaire.
		*\param[in] flags
		*	Les indicateurs de type de charge qui sera affectée au tampon.
		*\param[in] inheritanceInfo
		*	Les informations d'héritage.
		*/
		inline void begin( VkCommandBufferUsageFlags flags
			, VkCommandBufferInheritanceInfo & inheritanceInfo )const
		{
			begin( VkCommandBufferBeginInfo{ VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO, nullptr, flags, &inheritanceInfo } );
		}
		/**
		*\~english
		*\brief
		*	Ends the command buffer recording.
		*\return
		*	\p false on any error.
		*\~french
		*\brief
		*	Termine l'enregistrement du tampon de commandes.
		*/
		void end()const;
		/**
		*\~english
		*\brief
		*	Reset a command buffer to the initial state.
		*\param[in] flags
		*	The flags controlling the reset operation.
		*\return
		*	\p false on any error.
		*\~french
		*\brief
		*	Réinitialise le tampon de commandes et le met dans un état où il peut à nouveau enregistrer des commandes.
		*\param[in] flags
		*	Les indicateurs contrôlant le comportement de la réinitialisation du tampon.
		*/
		void reset( VkCommandBufferResetFlags flags = 0u )const;
		/**
		*\~english
		*\brief
		*	Begins a new render pass.
		*\~french
		*\brief
		*	Démarre une passe de rendu.
		*/
		void beginRenderPass( VkRenderPassBeginInfo const & beginInfo
			, VkSubpassContents contents )const;
		/**
		*\~english
		*\brief
		*	Begins a new render pass.
		*\param[in] renderPass
		*	The render pass to begin.
		*\param[in] frameBuffer
		*	The framebuffer containing the attachments that are used with the render pass.
		*\param[in] clearValues
		*	The clear values for each attachment that needs to be cleared.
		*\param[in] contents
		*	Specifies how the commands in the first subpass will be provided.
		*\~french
		*\brief
		*	Démarre une passe de rendu.
		*\param[in] renderPass
		*	La passe de rendu.
		*\param[in] frameBuffer
		*	Le tampon d'image affecté par le rendu.
		*\param[in] clearValues
		*	Les valeurs de vidage, une par attache de la passe de rendu.
		*\param[in] contents
		*	Indique la manière dont les commandes de la première sous-passe sont fournies.
		*/
		void beginRenderPass( RenderPass const & renderPass
			, FrameBuffer const & frameBuffer
			, VkClearValueArray const & clearValues
			, VkSubpassContents contents )const;
		/**
		*\~english
		*\brief
		*	Transition to the next subpass of a render pass.
		*\param[in] contents
		*	Specifies how the commands in the next subpass will be provided.
		*\~french
		*\brief
		*	Passe à la sous-passe suivante.
		*\param[in] contents
		*	Indique la manière dont les commandes de la sous-passe suivante sont fournies.
		*/
		void nextSubpass( VkSubpassContents contents )const;
		/**
		*\~english
		*\brief
		*	End the current render pass.
		*\~french
		*\brief
		*	Termine la passe de rendu courante.
		*/
		void endRenderPass()const;
		/**
		*\~english
		*\brief
		*	Execute a secondary command buffer.
		*\param[in] commands
		*	The secondary command buffers, which are recorded to execute in the order they are listed in the array.
		*\~french
		*\brief
		*	Execute des tampons de commande secondaires, qui sont enregistrés pour être exécutés dans l'ordre du tableau.
		*\param[in] commands
		*	Les tampons de commandes.
		*/
		void executeCommands( CommandBufferCRefArray const & commands )const;
		/**
		*\~english
		*\brief
		*	Clear a color image.
		*\param[in] image
		*	The image to clear.
		*\param[in] colour
		*	The clear colour.
		*\~french
		*\brief
		*	Vide une image couleur.
		*\param[in] image
		*	L'image à vider.
		*\param[in] colour
		*	La couleur de vidage.
		*/
		void clear( ImageView const & image
			, VkClearColorValue const & colour )const;
		/**
		*\~english
		*\brief
		*	Clear a depth and/or stencil image.
		*\param[in] image
		*	The image to clear.
		*\param[in] value
		*	The clear value.
		*\~french
		*\brief
		*	Vide une image profondeur et/ou stencil.
		*\param[in] image
		*	L'image à vider.
		*\param[in] value
		*	La valeur de vidage.
		*/
		void clear( ImageView const & image
			, VkClearDepthStencilValue const & value )const;
		/**
		*\~english
		*\brief
		*	Clear regions within currently bound framebuffer attachments.
		*\param[in] clearAttachments
		*	The attachments to clear and the clear values to use.
		*\param[in] clearRects
		*	The regions within each selected attachment to clear.
		*\~french
		*\brief
		*	Vide une région des attaches du tampon d'images actuellement attaché.
		*\param[in] clearAttachments
		*	Les attaches à vider et les valeurs de nettoyage à utiliser.
		*\param[in] clearRects
		*	Les régions à nettoyer pour chaque attache sélectionnée.
		*/
		void clearAttachments( VkClearAttachmentArray const & clearAttachments
			, VkClearRectArray const & clearRects );
		/**
		*\~english
		*\brief
		*	Defines a memory dependency between commands that were submitted before it, and those submitted after it.
		*\param[in] after
		*	Specifies the pipeline stages that must be ended before the barrier.
		*\param[in] before
		*	Specifies the pipeline stages that can be started after the barrier.
		*\param[in] transitionBarrier
		*	Describes the transition.
		*\~french
		*\brief
		*	Met en place une barrière de transition d'état de tampon.
		*\param[in] after
		*	Les étapes devant être terminées avant l'exécution de la barrière.
		*\param[in] before
		*	Les étapes pouvant être commencées après l'exécution de la barrière.
		*\param[in] transitionBarrier
		*	La description de la transition.
		*/
		void pipelineBarrier( VkPipelineStageFlags after
			, VkPipelineStageFlags before
			, VkDependencyFlags dependencyFlags
			, VkMemoryBarrierArray const & memoryBarriers
			, VkBufferMemoryBarrierArray const & bufferMemoryBarriers
			, VkImageMemoryBarrierArray const & imaegMemoryBarriers )const;
		/**
		*\~english
		*\brief
		*	Defines a memory dependency between commands that were submitted before it, and those submitted after it.
		*\param[in] after
		*	Specifies the pipeline stages that must be ended before the barrier.
		*\param[in] before
		*	Specifies the pipeline stages that can be started after the barrier.
		*\param[in] transitionBarrier
		*	Describes the transition.
		*\~french
		*\brief
		*	Met en place une barrière de transition d'état de tampon.
		*\param[in] after
		*	Les étapes devant être terminées avant l'exécution de la barrière.
		*\param[in] before
		*	Les étapes pouvant être commencées après l'exécution de la barrière.
		*\param[in] transitionBarrier
		*	La description de la transition.
		*/
		void memoryBarrier( VkPipelineStageFlags after
			, VkPipelineStageFlags before
			, VkBufferMemoryBarrier const & transitionBarrier )const;
		/**
		*\~english
		*\brief
		*	Defines a memory dependency between commands that were submitted before it, and those submitted after it.
		*\param[in] after
		*	Specifies the pipeline stages that must be ended before the barrier.
		*\param[in] before
		*	Specifies the pipeline stages that can be started after the barrier.
		*\param[in] transitionBarrier
		*	Describes the transition.
		*\~french
		*\brief
		*	Met en place une barrière de transition de layout d'image.
		*\param[in] after
		*	Les étapes devant être terminées avant l'exécution de la barrière.
		*\param[in] before
		*	Les étapes pouvant être commencées après l'exécution de la barrière.
		*\param[in] transitionBarrier
		*	La description de la transition.
		*/
		void memoryBarrier( VkPipelineStageFlags after
			, VkPipelineStageFlags before
			, VkImageMemoryBarrier const & transitionBarrier )const;
		/**
		*\~english
		*\brief
		*	Binds a pipeline to the command buffer.
		*\param[in] pipeline
		*	The pipeline to be bound.
		*\param[in] bindingPoint
		*	Specifies whether to bind to the compute or graphics bind point.
		*\~french
		*\brief
		*	Active un pipeline: shaders, tests, états, ...
		*\param[in] pipeline
		*	Le pipeline à activer.
		*\param[in] bindingPoint
		*	Le point d'attache du pipeline.
		*/
		void bindPipeline( GraphicsPipeline const & pipeline
			, VkPipelineBindPoint bindingPoint = VkPipelineBindPoint::VK_PIPELINE_BIND_POINT_GRAPHICS )const;
		/**
		*\~english
		*\brief
		*	Binds a compute pipeline to the command buffer.
		*\param[in] pipeline
		*	The pipeline to be bound.
		*\param[in] bindingPoint
		*	Specifies whether to bind to the compute or graphics bind point.
		*\~french
		*\brief
		*	Active un pipeline de calcul.
		*\param[in] pipeline
		*	Le pipeline à activer.
		*\param[in] bindingPoint
		*	Le point d'attache du pipeline.
		*/
		void bindPipeline( ComputePipeline const & pipeline
			, VkPipelineBindPoint bindingPoint = VkPipelineBindPoint::VK_PIPELINE_BIND_POINT_COMPUTE )const;
		/**
		*\~english
		*\brief
		*	Binds vertex buffers to the command buffer.
		*\param[in] firstBinding
		*	The index of the first vertex input binding whose state is updated by the command.
		*\param[in] buffers
		*	The array of buffer handles.
		*\param[in] offsets
		*	The array of buffer offsets.
		*\~french
		*\brief
		*	Active des tampons de sommets sur le tampon de commandes.
		*\param[in] firstBinding
		*	L'indice de la première attache d'entrée de sommets dont l'état est mis à jour par la commande.
		*\param[in] buffers
		*	Un tableau de tampons de sommets.
		*\param[in] offsets
		*	Un tableau d'offsets dans les tampons.
		*/
		void bindVertexBuffers( uint32_t firstBinding
			, BufferCRefArray const & buffers
			, UInt64Array offsets )const;
		/**
		*\~english
		*\brief
		*	Binds an index buffer to the command buffer.
		*\param[in] buffer
		*	The buffer being bound.
		*\param[in] offset
		*	The starting offset in bytes within buffer used in index buffer address calculations.
		*\param[in] indexType
		*	Tells whether the indices are treated as 16 bits or 32 bits.
		*\~french
		*\brief
		*	Active un tampon d'indices sur le tampon de commandes.
		*\param[in] buffer
		*	Le tampon à activer.
		*\param[in] offset
		*	L'offset de départ en octets dans le tampon, utilisé pour le calcul des indices.
		*\param[in] indexType
		*	Dit si les indices sont traités en 16 bits ou 32 bits.
		*/
		void bindIndexBuffer( BufferBase const & buffer
			, uint64_t offset
			, VkIndexType indexType )const;
		/**
		*\~english
		*\brief
		*	Binds descriptor sets to the command buffer.
		*\param[in] descriptorSet
		*	The descriptor sets.
		*\param[in] layout
		*	The pipeline layout used to program the binding.
		*\param[in] dynamicOffsets
		*	The dynamic offsets for dynamic buffers.
		*\param[in] bindingPoint
		*	Indicates whether the descriptor wil be used by graphics or compute pipeline.
		*\~french
		*\brief
		*	Active des descriptor sets.
		*\param[in] descriptorSet
		*	Les descriptor sets.
		*\param[in] layout
		*	Le layout de pipeline.
		*\param[in] dynamicOffsets
		*	Les décalages dynamiques des tampons dynamiques.
		*\param[in] bindingPoint
		*	Le point d'attache du set.
		*/
		void bindDescriptorSets( DescriptorSetCRefArray const & descriptorSet
			, PipelineLayout const & layout
			, UInt32Array const & dynamicOffsets
			, VkPipelineBindPoint bindingPoint = VkPipelineBindPoint::VK_PIPELINE_BIND_POINT_GRAPHICS )const;
		/**
		*\~english
		*\brief
		*	Defines the currently bound pipeline viewport.
		*\remarks
		*	This action is possible only if the viewport is dynamic, in the pipeline.
		*\param[in] viewport
		*	The viewport.
		*\~french
		*\brief
		*	Définit le viewport du pipeline.
		*\remarks
		*	Cette action n'est faisable que si le viewport est configuré comme dynamique.
		*\param[in] viewport
		*	Le viewport.
		*/
		void setViewport( uint32_t firstViewport
			, VkViewportArray const & viewports )const;
		/**
		*\~english
		*\brief
		*	Defines the currently bound pipeline viewport.
		*\remarks
		*	This action is possible only if the viewport is dynamic, in the pipeline.
		*\param[in] viewport
		*	The viewport.
		*\~french
		*\brief
		*	Définit le viewport du pipeline.
		*\remarks
		*	Cette action n'est faisable que si le viewport est configuré comme dynamique.
		*\param[in] viewport
		*	Le viewport.
		*/
		inline void setViewport( VkViewport const & viewport )const
		{
			setViewport( 0u, VkViewportArray{ viewport } );
		}
		/**
		*\~english
		*\brief
		*	Defines the currently bound pipeline scissor.
		*\remarks
		*	This action is possible only if the scissor is dynamic, in the pipeline.
		*\param[in] scissor
		*	The scissor.
		*\~french
		*\brief
		*	Définit le scissor du pipeline.
		*\remarks
		*	Cette action n'est faisable que si le scissor est configuré comme dynamique.
		*\param[in] scissor
		*	Le scissor.
		*/
		void setScissor( uint32_t firstScissor
			, VkScissorArray const & scissors )const;
		/**
		*\~english
		*\brief
		*	Defines the currently bound pipeline scissor.
		*\remarks
		*	This action is possible only if the scissor is dynamic, in the pipeline.
		*\param[in] scissor
		*	The scissor.
		*\~french
		*\brief
		*	Définit le scissor du pipeline.
		*\remarks
		*	Cette action n'est faisable que si le scissor est configuré comme dynamique.
		*\param[in] scissor
		*	Le scissor.
		*/
		inline void setScissor( VkRect2D const & scissor )const
		{
			setScissor( 0u, VkScissorArray{ scissor } );
		}
		/**
		*\~english
		*\brief
		*	Draws some vertices.
		*\param[in] vtxCount
		*	The vertex count.
		*\param[in] instCount
		*	The instances count.
		*\param[in] firstVertex
		*	The first vertex index.
		*\param[in] firstInstance
		*	The first instance index.
		*\~french
		*\brief
		*	Dessine des sommets.
		*\param[in] vtxCount
		*	Nombre de sommets.
		*\param[in] instCount
		*	Nombre d'instances.
		*\param[in] firstVertex
		*	Index du premier sommet.
		*\param[in] firstInstance
		*	Index de la première instance.
		*/
		void draw( uint32_t vtxCount
			, uint32_t instCount = 1u
			, uint32_t firstVertex = 0u
			, uint32_t firstInstance = 0u )const;
		/**
		*\~english
		*\brief
		*	Draws some indexed vertices.
		*\param[in] indexCount
		*	The index count.
		*\param[in] instCount
		*	The instances count.
		*\param[in] firstIndex
		*	The first index index.
		*\param[in] vertexOffset
		*	The offset added to the vertex index, before indexing the vertex buffer.
		*\param[in] firstInstance
		*	The first instance index.
		*\~french
		*\brief
		*	Dessine des sommets.
		*\param[in] indexCount
		*	Nombre d'indices.
		*\param[in] instCount
		*	Nombre d'instances.
		*\param[in] firstIndex
		*	Index du premier indice.
		*\param[in] vertexOffset
		*	La valeur ajoutée à l'indice du sommet avant d'indexer le tampon de sommets.
		*\param[in] firstInstance
		*	Index de la première instance.
		*/
		void drawIndexed( uint32_t indexCount
			, uint32_t instCount = 1u
			, uint32_t firstIndex = 0u
			, uint32_t vertexOffset = 0u
			, uint32_t firstInstance = 0u )const;
		/**
		*\~english
		*\brief
		*	Perform an indirect draw.
		*\param[in] buffer
		*	The buffer containing draw parameters.
		*\param[in] offset
		*	The byte offset into \p buffer where parameters begin.
		*\param[in] drawCount
		*	The number of draws to execute, and can be zero.
		*\param[in] stride
		*	The byte stride between successive sets of draw parameters.
		*\~french
		*\brief
		*	Effectue un dessin indirect.
		*\param[in] buffer
		*	Le tampon contenant les paramètres de dessin.
		*\param[in] offset
		*	L'offset en octets dans \p buffer, où les paramètres commencent.
		*\param[in] drawCount
		*	Le nombre de dessins à effectuer, peut être zéro.
		*\param[in] stride
		*	Le stride en octets entre deux ensembles successifs de paramètres de dessin.
		*/
		void drawIndirect( BufferBase const & buffer
			, uint32_t offset
			, uint32_t drawCount
			, uint32_t stride )const;
		/**
		*\~english
		*\brief
		*	Perform an indexed indirect draw.
		*\param[in] buffer
		*	The buffer containing draw parameters.
		*\param[in] offset
		*	The byte offset into \p buffer where parameters begin.
		*\param[in] drawCount
		*	The number of draws to execute, and can be zero.
		*\param[in] stride
		*	The byte stride between successive sets of draw parameters.
		*\~french
		*\brief
		*	Effectue un dessin indirect indexé.
		*\param[in] buffer
		*	Le tampon contenant les paramètres de dessin.
		*\param[in] offset
		*	L'offset en octets dans \p buffer, où les paramètres commencent.
		*\param[in] drawCount
		*	Le nombre de dessins à effectuer, peut être zéro.
		*\param[in] stride
		*	Le stride en octets entre deux ensembles successifs de paramètres de dessin.
		*/
		void drawIndexedIndirect( BufferBase const & buffer
			, uint32_t offset
			, uint32_t drawCount
			, uint32_t stride )const;
		/**
		*\~french
		*\brief
		*	Copie les données d'un tampon vers une image.
		*\param[in] copyInfo
		*	Les informations de la copie.
		*\param[in] src
		*	Le tampon source.
		*\param[in] dst
		*	L'image destination.
		*\~english
		*\brief
		*	Copies data from a buffer to an image.
		*\param[in] copyInfo
		*	The copy informations.
		*\param[in] src
		*	The source buffer.
		*\param[in] dst
		*	The destination image.
		*/
		void copyToImage( VkBufferImageCopyArray const & copyInfo
			, BufferBase const & src
			, Image const & dst )const;
		/**
		*\~french
		*\brief
		*	Copie les données d'un tampon vers une image.
		*\param[in] copyInfo
		*	Les informations de la copie.
		*\param[in] src
		*	Le tampon source.
		*\param[in] dst
		*	L'image destination.
		*\~english
		*\brief
		*	Copies data from a buffer to an image.
		*\param[in] copyInfo
		*	The copy informations.
		*\param[in] src
		*	The source buffer.
		*\param[in] dst
		*	The destination image.
		*/
		void copyToBuffer( VkBufferImageCopyArray const & copyInfo
			, Image const & src
			, BufferBase const & dst )const;
		/**
		*\~french
		*\brief
		*	Copie les données d'un tampon vers un autre.
		*\param[in] copyInfo
		*	Les informations de la copie.
		*\param[in] src
		*	Le tampon source.
		*\param[in] dst
		*	Le tampon destination.
		*\~english
		*\brief
		*	Copies data from a buffer to another one.
		*\param[in] copyInfo
		*	The copy informations.
		*\param[in] src
		*	The source buffer.
		*\param[in] dst
		*	The destination buffer.
		*/
		void copyBuffer( VkBufferCopy const & copyInfo
			, BufferBase const & src
			, BufferBase const & dst )const;
		/**
		*\~french
		*\brief
		*	Copie les données d'une image vers une autre.
		*\param[in] copyInfo
		*	Les informations de la copie.
		*\param[in] src
		*	L'image source.
		*\param[in] srcLayout
		*	Le layout d'image voulu pour l'image source.
		*\param[in] dst
		*	L'image destination.
		*\param[in] dstLayout
		*	Le layout d'image voulu pour l'image destination.
		*\~english
		*\brief
		*	Copies data from an image to another one.
		*\param[in] copyInfo
		*	The copy informations.
		*\param[in] src
		*	The source image.
		*\param[in] srcLayout
		*	The image layout wanted for the source image.
		*\param[in] dst
		*	The destination image.
		*\param[in] dstLayout
		*	The image layout wanted for the destination image.
		*/
		void copyImage( VkImageCopy const & copyInfo
			, Image const & src
			, VkImageLayout srcLayout
			, Image const & dst
			, VkImageLayout dstLayout )const;
		/**
		*\~french
		*\brief
		*	Copie des régions d'une image vers une autre.
		*\param[in] regions
		*	Les régions à blitter.
		*\param[in] srcImage
		*	L'image source.
		*\param[in] srcLayout
		*	Le layout de l'image source.
		*\param[in] dstImage
		*	L'image destination.
		*\param[in] dstLayout
		*	Le layout de l'image destination.
		*\param[in] filter
		*	Le filtre appliqué si la copie nécessite une mise à l'échelle.
		*\~english
		*\brief
		*	Copy regions of an image to another one.
		*\param[in] regions
		*	The regions to blit.
		*\param[in] srcImage
		*	The source image.
		*\param[in] srcLayout
		*	The source image layout.
		*\param[in] dstImage
		*	The destination image.
		*\param[in] dstLayout
		*	The source image layout.
		*\param[in] filter
		*	The filter applied if the blit requires scaling.
		*/
		void blitImage( Image const & srcImage
			, VkImageLayout srcLayout
			, Image const & dstImage
			, VkImageLayout dstLayout
			, std::vector< VkImageBlit > const & regions
			, VkFilter filter )const;
		/**
		*\~english
		*\brief
		*	Resets a range of queries in a query pool.
		*\param[in] pool
		*	The query pool.
		*\param[in] firstQuery
		*	The first query index.
		*\param[in] queryCount
		*	The number of queries (starting at \p firstQuery).
		*\~french
		*\brief
		*	Réinitialise un intervalle de requêtes d'un pool.
		*\param[in] pool
		*	Le pool de requêtes.
		*\param[in] firstQuery
		*	L'index de la première requête.
		*\param[in] queryCount
		*	Le nombre de requêtes (à partir de \p firstQuery).
		*/
		void resetQueryPool( QueryPool const & pool
			, uint32_t firstQuery
			, uint32_t queryCount )const;
		/**
		*\~english
		*\brief
		*	Begins a query.
		*\param[in] pool
		*	The query pool.
		*\param[in] query
		*	The query index.
		*\param[in] flags
		*	The constraints on the types of queries that can be performed.
		*\~french
		*\brief
		*	Démarre un requête.
		*\param[in] pool
		*	Le pool de requêtes.
		*\param[in] query
		*	L'index de la requête.
		*\param[in] flags
		*	Les contraintes sur les types de requête qui peuvent être effectuées.
		*/
		void beginQuery( QueryPool const & pool
			, uint32_t query
			, VkQueryControlFlags flags )const;
		/**
		*\~english
		*\brief
		*	Ends a query.
		*\param[in] pool
		*	The query pool.
		*\param[in] query
		*	The query index.
		*\~french
		*\brief
		*	Termine une requête.
		*\param[in] pool
		*	Le pool de requêtes.
		*\param[in] query
		*	L'index de la requête.
		*/
		void endQuery( QueryPool const & pool
			, uint32_t query )const;
		/**
		*\~english
		*\brief
		*	Writes a timestamp to given query result.
		*\param[in] pipelineStage
		*	Specifies the stage of the pipeline.
		*\param[in] pool
		*	The query pool.
		*\param[in] query
		*	The query index.
		*\~french
		*\brief
		*	Ecrit un timestamp dans le résultat d'une requête.
		*\param[in] pipelineStage
		*	Définit l'étape du pipeline.
		*\param[in] pool
		*	Le pool de requêtes.
		*\param[in] query
		*	L'index de la requête.
		*/
		void writeTimestamp( VkPipelineStageFlagBits pipelineStage
			, QueryPool const & pool
			, uint32_t query )const;
		/**
		*\~french
		*\brief
		*	Met à jour les valeurs de push constants.
		*\param[in] layout
		*	Le layout de pipeline utilisé pour programmer la mise à jour des push constants.
		*\param[in] stageFlags
		*	Les niveaux de shader impaactés.
		*\param[in] offset
		*	Le début de l'intervalle (en octets).
		*\param[in] size
		*	La taille de l'intervalle (en octets).
		*\param[in] data
		*	Les données de push constants.
		*\~english
		*\brief
		*	Updates the values of push constants.
		*\param[in] layout
		*	The pipeline layout used to program the push constants updates.
		*\param[in] stageFlags
		*	The impacted shader stages.
		*\param[in] offset
		*	The range offset (in bytes).
		*\param[in] size
		*	The range size (in bytes).
		*\param[in] data
		*	The push constants data.
		*/
		void pushConstants( PipelineLayout const & layout
			, VkShaderStageFlags stageFlags
			, uint32_t offset
			, uint32_t size
			, void const * data )const;
		/**
		*\~french
		*\brief
		*	Distribue des éléments de calcul.
		*\param[in] groupCountX, groupCountY, groupCountZ
		*	Le nombre de groupes de travail locaux à distribuer dans les dimensions X, Y, et Z.
		*\~english
		*\brief
		*	Dispatch compute work items.
		*\param[in] groupCountX, groupCountY, groupCountZ
		*	The number of local workgroups to dispatch to the X, Y, and Z dimensions.
		*/
		void dispatch( uint32_t groupCountX
			, uint32_t groupCountY
			, uint32_t groupCountZ )const;
		/**
		*\~english
		*\brief
		*	Dispatch compute work items using indirect parameters.
		*\param[in] buffer
		*	The buffer containing dispatch parameters.
		*\param[in] offset
		*	The byte offset into \p buffer where parameters begin.
		*\~french
		*\brief
		*	Distribue des éléments de calcul en utilisant des paramètres indirects.
		*\param[in] buffer
		*	Le tampon contenant les paramètres de distribution.
		*\param[in] offset
		*	L'offset en octets dans \p buffer, où les paramètres commencent.
		*/
		void dispatchIndirect( BufferBase const & buffer
			, uint32_t offset )const;
		/**
		*\~french
		*\brief
		*	Définit la largeur des lignes, si celle-ci est un état dynamique du pipeline.
		*\param[in] width
		*	La largeur voulue.
		*\~english
		*\brief
		*	Specifies the lines width, if this is a dynamic state in the pipeline.
		*\param[in] width
		*	The wanted width.
		*/
		void setLineWidth( float width )const;
		/**
		*\~french
		*\brief
		*	Définit la largeur des lignes, si celle-ci est un état dynamique du pipeline.
		*\param[in] constantFactor
		*	Le facteur contrôlant la valeur constante de profondeur ajoutée à chaque fragment.
		*\param[in] clamp
		*	Le décalage maximum (ou minimum) de profondeur d'un fragment.
		*\param[in] slopeFactor
		*	Le facteur appliqué à la pente d'un fragment lors du calcul de décalage de profondeur.
		*\~english
		*\brief
		*	Specifies the lines width, if this is a dynamic state in the pipeline.
		*\param[in] constantFactor
		*	The scalar factor controlling the constant depth value added to each fragment.
		*\param[in] clamp
		*	The maximum (or minimum) depth bias of a fragment.
		*\param[in] slopeFactor
		*	The scalar factor applied to a fragment’s slope in depth bias calculations.
		*/
		void setDepthBias( float constantFactor
			, float clamp
			, float slopeFactor )const;
		/**
		*\~french
		*\brief
		*	Définit un évènement à l'état signalé.
		*\param[in] event
		*	L'évènement à signaler.
		*\param[in] stageMask
		*	Le masque de niveaux utilisé pour déterminer quand l'évènement est signalé.
		*\~english
		*\brief
		*	Sets an event to signaled state.
		*\param[in] event
		*	The event to be signaled.
		*\param[in] stageMask
		*	The source stage mask used to determine when the event is signaled.
		*/
		void setEvent( Event const & event
			, VkPipelineStageFlags stageMask )const;
		/**
		*\~french
		*\brief
		*	Définit un évènement à l'état signalé.
		*\param[in] event
		*	L'évènement à signaler.
		*\param[in] stageMask
		*	Le masque de niveaux utilisé pour déterminer quand l'évènement est signalé.
		*\~english
		*\brief
		*	Sets an event to signaled state.
		*\param[in] event
		*	The event to be signaled.
		*\param[in] stageMask
		*	The source stage mask used to determine when the event is signaled.
		*/
		void resetEvent( Event const & event
			, VkPipelineStageFlags stageMask )const;
		/**
		*\~french
		*\brief
		*	Attend qu'un ou plusieurs évènements soient signalés sur le périphérique.
		*\param[in] events
		*	Les évènements à attendre.
		*\param[in] srcStageMask
		*	Le masque de niveaux source.
		*\param[in] dstStageMask
		*	Le masque de niveaus destination.
		*\param[in] bufferMemoryBarriers
		*	Un tableau de barrières mémoire de tampons.
		*\param[in] imageMemoryBarriers
		*	Un tableau de barrières mémoire d'images.
		*\~english
		*\brief
		*	Wait for one or more events to be signaled on a device.
		*\param[in] events
		*	The events to wait on.
		*\param[in] srcStageMask
		*	The source stage mask.
		*\param[in] dstStageMask
		*	destination stage mask.
		*\param[in] bufferMemoryBarriers
		*	An array of buffer memory barriers.
		*\param[in] imageMemoryBarriers
		*	An array of image memory barriers.
		*/
		void waitEvents( EventCRefArray const & events
			, VkPipelineStageFlags srcStageMask
			, VkPipelineStageFlags dstStageMask
			, VkBufferMemoryBarrierArray const & bufferMemoryBarriers
			, VkImageMemoryBarrierArray const & imageMemoryBarriers )const;
		/**
		*\~english
		*\brief
		*	Binds a vertex buffer to the command buffer.
		*\param[in] binding
		*	The index of the vertex input binding whose state is updated by the command.
		*\param[in] buffer
		*	The buffer handle.
		*\param[in] offset
		*	The array of buffer offset.
		*\~french
		*\brief
		*	Active un tampon de sommets sur le tampon de commandes.
		*\param[in] binding
		*	L'indice de l'attache d'entrée de sommets dont l'état est mis à jour par la commande.
		*\param[in] buffer
		*	Le tampon de sommets.
		*\param[in] offset
		*	L'offset dans le tampon.
		*/
		void bindVertexBuffer( uint32_t binding
			, BufferBase const & buffer
			, uint64_t offset )const;
		/**
		*\~french
		*\brief
		*	Copie les données d'un tampon vers une image.
		*\param[in] copyInfo
		*	Les informations de la copie.
		*\param[in] src
		*	Le tampon source.
		*\param[in] dst
		*	L'image destination.
		*\~english
		*\brief
		*	Copies data from a buffer to an image.
		*\param[in] copyInfo
		*	The copy informations.
		*\param[in] src
		*	The source buffer.
		*\param[in] dst
		*	The destination image.
		*/
		void copyToImage( VkBufferImageCopy const & copyInfo
			, BufferBase const & src
			, Image const & dst )const;
		/**
		*\~french
		*\brief
		*	Copie les données d'un tampon vers une image.
		*\param[in] copyInfo
		*	Les informations de la copie.
		*\param[in] src
		*	Le tampon source.
		*\param[in] dst
		*	L'image destination.
		*\~english
		*\brief
		*	Copies data from a buffer to an image.
		*\param[in] copyInfo
		*	The copy informations.
		*\param[in] src
		*	The source buffer.
		*\param[in] dst
		*	The destination image.
		*/
		void copyToBuffer( VkBufferImageCopy const & copyInfo
			, Image const & src
			, BufferBase const & dst )const;
		/**
		*\~french
		*\brief
		*	Copie les données d'un tampon vers un autre tampon.
		*\param[in] src
		*	Le tampon source.
		*\param[in] dst
		*	Le tampon destination.
		*\param[in] size
		*	La taille des données à copier.
		*\param[in] offset
		*	Le décalage dans le tampon source.
		*\~english
		*\brief
		*	Copies data from a buffer to another one.
		*\param[in] src
		*	The source buffer.
		*\param[in] dst
		*	The destination buffer.
		*\param[in] size
		*	The byte size of the data to copy.
		*\param[in] offset
		*	The offset in the source buffer.
		*/
		void copyBuffer( BufferBase const & src
			, BufferBase const & dst
			, uint32_t size
			, uint32_t offset = 0 )const;
		/**
		*\~english
		*\brief
		*	Binds a descriptor set to the command buffer.
		*\param[in] descriptorSet
		*	The descriptor set.
		*\param[in] layout
		*	The pipeline layout used to program the binding.
		*\param[in] dynamicOffsets
		*	The dynamic offsets for dynamic buffers.
		*\param[in] bindingPoint
		*	Indicates whether the descriptor wil be used by graphics or compute pipeline.
		*\~french
		*\brief
		*	Active un descriptor set.
		*\param[in] descriptorSet
		*	Le descriptor set.
		*\param[in] layout
		*	Le layout de pipeline.
		*\param[in] dynamicOffsets
		*	Les décalages dynamiques des tampons dynamiques.
		*\param[in] bindingPoint
		*	Le point d'attache du set.
		*/
		inline void bindDescriptorSet( DescriptorSet const & descriptorSet
			, PipelineLayout const & layout
			, UInt32Array const & dynamicOffsets
			, VkPipelineBindPoint bindingPoint = VkPipelineBindPoint::VK_PIPELINE_BIND_POINT_GRAPHICS )const
		{
			bindDescriptorSets( DescriptorSetCRefArray{ descriptorSet }
				, layout
				, dynamicOffsets
				, bindingPoint );
		}
		/**
		*\~english
		*\brief
		*	Binds a descriptor set to the command buffer.
		*\param[in] descriptorSet
		*	The descriptor set.
		*\param[in] layout
		*	The pipeline layout used to program the binding.
		*\param[in] bindingPoint
		*	Indicates whether the descriptor wil be used by graphics or compute pipeline.
		*\~french
		*\brief
		*	Active un descriptor set.
		*\param[in] descriptorSet
		*	Le descriptor set.
		*\param[in] layout
		*	Le layout de pipeline.
		*\param[in] bindingPoint
		*	Le point d'attache du set.
		*/
		inline void bindDescriptorSet( DescriptorSet const & descriptorSet
			, PipelineLayout const & layout
			, VkPipelineBindPoint bindingPoint = VkPipelineBindPoint::VK_PIPELINE_BIND_POINT_GRAPHICS )const
		{
			bindDescriptorSets( DescriptorSetCRefArray{ descriptorSet }
				, layout
				, UInt32Array{}
				, bindingPoint );
		}
		/**
		*\~english
		*\brief
		*	Binds descriptor sets to the command buffer.
		*\param[in] descriptorSet
		*	The descriptor sets.
		*\param[in] layout
		*	The pipeline layout used to program the binding.
		*\param[in] bindingPoint
		*	Indicates whether the descriptor wil be used by graphics or compute pipeline.
		*\~french
		*\brief
		*	Active des descriptor sets.
		*\param[in] descriptorSet
		*	Les descriptor sets.
		*\param[in] layout
		*	Le layout de pipeline.
		*\param[in] bindingPoint
		*	Le point d'attache du set.
		*/
		inline void bindDescriptorSets( DescriptorSetCRefArray const & descriptorSet
			, PipelineLayout const & layout
			, VkPipelineBindPoint bindingPoint = VkPipelineBindPoint::VK_PIPELINE_BIND_POINT_GRAPHICS )const
		{
			bindDescriptorSets( descriptorSet
				, layout
				, UInt32Array{}
				, bindingPoint );
		}
		/**
		*\brief
		*	Copie les données d'un tampon vers un autre tampon.
		*\param[in] src
		*	Le tampon source.
		*\param[in] dst
		*	Le tampon destination.
		*\param[in] size
		*	La taille des données à copier.
		*\param[in] offset
		*	Le décalage dans le tampon source.
		*\~english
		*\brief
		*	Copies data from a buffer to another one.
		*\param[in] src
		*	The source buffer.
		*\param[in] dst
		*	The destination buffer.
		*\param[in] size
		*	The byte size of the data to copy.
		*\param[in] offset
		*	The offset in the source buffer.
		*/
		void copyBuffer( BufferBase const & src
			, VertexBufferBase const & dst
			, uint32_t size
			, uint32_t offset = 0 )const;
		/**
		*\brief
		*	Copie les données d'un tampon vers un autre tampon.
		*\param[in] src
		*	Le tampon source.
		*\param[in] dst
		*	Le tampon destination.
		*\param[in] size
		*	La taille des données à copier.
		*\param[in] offset
		*	Le décalage dans le tampon source.
		*\~english
		*\brief
		*	Copies data from a buffer to another one.
		*\param[in] src
		*	The source buffer.
		*\param[in] dst
		*	The destination buffer.
		*\param[in] size
		*	The byte size of the data to copy.
		*\param[in] offset
		*	The offset in the source buffer.
		*/
		void copyBuffer( VertexBufferBase const & src
			, BufferBase const & dst
			, uint32_t size
			, uint32_t offset = 0 )const;
		/**
		*\brief
		*	Copie les données d'un tampon vers un autre tampon.
		*\param[in] src
		*	Le tampon source.
		*\param[in] dst
		*	Le tampon destination.
		*\param[in] size
		*	La taille des données à copier.
		*\param[in] offset
		*	Le décalage dans le tampon source.
		*\~english
		*\brief
		*	Copies data from a buffer to another one.
		*\param[in] src
		*	The source buffer.
		*\param[in] dst
		*	The destination buffer.
		*\param[in] size
		*	The byte size of the data to copy.
		*\param[in] offset
		*	The offset in the source buffer.
		*/
		void copyBuffer( VertexBufferBase const & src
			, VertexBufferBase const & dst
			, uint32_t size
			, uint32_t offset = 0 )const;
		/**
		*\brief
		*	Copie les données d'un tampon vers un autre tampon.
		*\param[in] src
		*	Le tampon source.
		*\param[in] dst
		*	Le tampon destination.
		*\param[in] size
		*	La taille des données à copier.
		*\param[in] offset
		*	Le décalage dans le tampon source.
		*\~english
		*\brief
		*	Copies data from a buffer to another one.
		*\param[in] src
		*	The source buffer.
		*\param[in] dst
		*	The destination buffer.
		*\param[in] size
		*	The byte size of the data to copy.
		*\param[in] offset
		*	The offset in the source buffer.
		*/
		void copyBuffer( BufferBase const & src
			, UniformBufferBase const & dst
			, uint32_t size
			, uint32_t offset = 0 )const;
		/**
		*\brief
		*	Copie les données d'un tampon vers un autre tampon.
		*\param[in] src
		*	Le tampon source.
		*\param[in] dst
		*	Le tampon destination.
		*\param[in] size
		*	La taille des données à copier.
		*\param[in] offset
		*	Le décalage dans le tampon source.
		*\~english
		*\brief
		*	Copies data from a buffer to another one.
		*\param[in] src
		*	The source buffer.
		*\param[in] dst
		*	The destination buffer.
		*\param[in] size
		*	The byte size of the data to copy.
		*\param[in] offset
		*	The offset in the source buffer.
		*/
		void copyBuffer( UniformBufferBase const & src
			, BufferBase const & dst
			, uint32_t size
			, uint32_t offset = 0 )const;
		/**
		*\brief
		*	Copie les données d'un tampon vers un autre tampon.
		*\param[in] src
		*	Le tampon source.
		*\param[in] dst
		*	Le tampon destination.
		*\param[in] size
		*	La taille des données à copier.
		*\param[in] offset
		*	Le décalage dans le tampon source.
		*\~english
		*\brief
		*	Copies data from a buffer to another one.
		*\param[in] src
		*	The source buffer.
		*\param[in] dst
		*	The destination buffer.
		*\param[in] size
		*	The byte size of the data to copy.
		*\param[in] offset
		*	The offset in the source buffer.
		*/
		void copyBuffer( UniformBufferBase const & src
			, UniformBufferBase const & dst
			, uint32_t size
			, uint32_t offset = 0 )const;
		/**
		*\~french
		*\brief
		*	Copie les données d'une image vers une autre.
		*\param[in] src
		*	L'image source.
		*\param[in] dst
		*	L'image destination.
		*\~english
		*\brief
		*	Copies data from an image to another one.
		*\param[in] src
		*	The source image.
		*\param[in] dst
		*	The destination image.
		*/
		void copyImage( ImageView const & src
			, ImageView const & dst )const;
		/**
		*\~french
		*\brief
		*	Met à jour les valeurs de push constants.
		*\param[in] layout
		*	Le layout de pipeline utilisé pour programmer la mise à jour des push constants.
		*\param[in] pcb
		*	Le tampon de push constants.
		*\~english
		*\brief
		*	Updates the values of push constants.
		*\param[in] layout
		*	The pipeline layout used to program the push constants updates.
		*\param[in] pcb
		*	The push constants buffer.
		*/
		void pushConstants( PipelineLayout const & layout
			, PushConstantsBufferBase const & pcb )const;
		/**
		*\~french
		*\brief
		*	Met à jour les valeurs de push constants.
		*\param[in] layout
		*	Le layout de pipeline utilisé pour programmer la mise à jour des push constants.
		*\param[in] pcb
		*	Le tampon de push constants.
		*\~english
		*\brief
		*	Updates the values of push constants.
		*\param[in] layout
		*	The pipeline layout used to program the push constants updates.
		*\param[in] pcb
		*	The push constants buffer.
		*/
		template< typename T >
		inline void pushConstants( PipelineLayout const & layout
			, PushConstantsBuffer< T > const & pcb )const
		{
			pushConstants( layout, pcb.getBuffer() );
		}
		/**
		*\~french
		*\brief
		*	Conversion implicite vers VkCommandBuffer.
		*\~english
		*\brief
		*	VkCommandBuffer implicit cast operator.
		*/
		inline operator VkCommandBuffer const & ()const
		{
			return m_internal;
		}

	private:
		Device const & m_device;
		CommandPool const & m_pool;
		VkCommandBuffer m_internal{ VK_NULL_HANDLE };
	};
}

#endif
