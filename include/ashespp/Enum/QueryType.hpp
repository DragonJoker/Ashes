/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#ifndef ___AshesPP_QueryType_HPP___
#define ___AshesPP_QueryType_HPP___
#pragma once

namespace ashes
{
	/**
	*\brief
	*	Gets the name of the given element type.
	*\param[in] value
	*	The element type.
	*\return
	*	The name.
	*/
	inline std::string getName( VkQueryType value )
	{
		switch ( value )
		{
		case VK_QUERY_TYPE_OCCLUSION:
			return "occlusion";
		case VK_QUERY_TYPE_PIPELINE_STATISTICS:
			return "statistics";
		case VK_QUERY_TYPE_TIMESTAMP:
			return "timestamp";
#ifdef VK_EXT_transform_feedback
		case VK_QUERY_TYPE_TRANSFORM_FEEDBACK_STREAM_EXT:
			return "transform_feedback_stream";
#endif
#ifdef VK_NV_ray_tracing
		case VK_QUERY_TYPE_ACCELERATION_STRUCTURE_COMPACTED_SIZE_NV:
			return "acceleration_structure_compacted_size";
#endif
		default:
			assert( false && "Unsupported VkQueryType." );
			return "Unsupported VkQueryType";
		}
	}
}

#endif
