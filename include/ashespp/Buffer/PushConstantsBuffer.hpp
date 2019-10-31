/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#ifndef ___AshesPP_PushConstantsBuffer_HPP___
#define ___AshesPP_PushConstantsBuffer_HPP___
#pragma once

#include "ashespp/Core/Device.hpp"

namespace ashes
{
	/**
	*\brief
	*	A variable for a push constants buffer.
	*/
	struct PushConstant
	{
		/**
		*\brief
		*	The offset in the buffer.
		*/
		uint32_t offset;
		/**
		*\brief
		*	The variable format, the size of the variable is deduced from that.
		*/
		VkFormat format;
		/**
		*\brief
		*	The dimensions of the array, if the constant is an array.
		*/
		uint32_t arraySize{ 1u };
	};
	/**
	*\brief
	*	Wraps the push constants concept.
	*/
	class PushConstantsBufferBase
	{
	public:
		/**
		*\brief
		*	Constructor.
		*\param[in] stageFlags
		*	Specifies the shader stages that will use the push constants in the updated range.
		*\param[in] variables
		*	The constants contained in the buffer.
		*/
		PushConstantsBufferBase( VkShaderStageFlags stageFlags
			, PushConstantArray const & variables );
		/**
		*\brief
		*	Destructor.
		*/
		~PushConstantsBufferBase()
		{
		}
		/**
		*\return
		*	The base offset.
		*/
		inline uint32_t getOffset()const
		{
			return m_offset;
		}
		/**
		*\return
		*	The data size.
		*/
		inline uint32_t getSize()const
		{
			return uint32_t( m_data.size() );
		}
		/**
		*\return
		*	The shader stages that will use the push constants in the updated range.
		*/
		inline VkShaderStageFlags getStageFlags()const
		{
			return m_stageFlags;
		}
		/**
		*\brief
		*	A pointer to the buffer data.
		*/
		inline uint8_t const * getData()const
		{
			return m_data.data();
		}
		/**
		*\brief
		*	A pointer to the buffer data.
		*/
		inline uint8_t * getData()
		{
			return m_data.data();
		}
		/**
		*\return
		*	The beginning of the constants array.
		*/
		inline PushConstantArray::const_iterator begin()const
		{
			return m_variables.begin();
		}
		/**
		*\return
		*	The end of the constants array.
		*/
		inline PushConstantArray::const_iterator end()const
		{
			return m_variables.end();
		}

	protected:
		VkShaderStageFlags m_stageFlags;
		PushConstantArray m_variables;
		uint32_t m_offset;
		ByteArray m_data;
	};
	/**
	*\brief
	*	Template class wrapping a PushConstantsBufferBase.
	*/
	template< typename T >
	class PushConstantsBuffer
	{
	public:
		/**
		*\brief
		*	Constructor.
		*\param[in] stageFlags
		*	Specifies the shader stages that will use the push constants in the updated range.
		*\param[in] variables
		*	The constants contained in the buffer.
		*/
		PushConstantsBuffer( VkShaderStageFlags stageFlags
			, PushConstantArray const & variables )
			: m_pcb{ stageFlags, variables }
		{
		}
		/**
		*\return
		*	The base offset.
		*/
		inline uint32_t getOffset()const
		{
			return getBuffer().getOffset();
		}
		/**
		*\return
		*	The data size.
		*/
		inline uint32_t getSize()const
		{
			return getBuffer().getSize();
		}
		/**
		*\return
		*	The shader stages that will use the push constants in the updated range.
		*/
		inline VkShaderStageFlags getStageFlags()const
		{
			return getBuffer().getStageFlags();
		}
		/**
		*\brief
		*	A pointer to the buffer data.
		*/
		inline T const * getData()const
		{
			return reinterpret_cast< T const * >( getBuffer().getData() );
		}
		/**
		*\brief
		*	A pointer to the buffer data.
		*/
		inline T * getData()
		{
			return reinterpret_cast< T * >( getBuffer().getData() );
		}
		/**
		*\return
		*	The beginning of the constants array.
		*/
		inline PushConstantArray::const_iterator begin()const
		{
			return getBuffer().begin();
		}
		/**
		*\return
		*	The end of the constants array.
		*/
		inline PushConstantArray::const_iterator end()const
		{
			return getBuffer().end();
		}
		/**
		*\brief
		*	The internal PCB.
		*/
		inline PushConstantsBufferBase const & getBuffer()const
		{
			return m_pcb;
		}
		/**
		*\brief
		*	The internal PCB.
		*/
		inline PushConstantsBufferBase & getBuffer()
		{
			return m_pcb;
		}

	private:
		PushConstantsBufferBase m_pcb;
	};
}

#endif
