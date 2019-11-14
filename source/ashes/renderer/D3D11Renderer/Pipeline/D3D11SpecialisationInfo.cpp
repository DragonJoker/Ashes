/*
This file belongs to Ashes.
See LICENSE file in root folder.
*/
#include "Core/D3D11Device.hpp"

#include <algorithm>

namespace ashes::d3d11
{
	//ashes::PushConstantArray convert( VkShaderStageFlagBits stage
	//	, ashes::SpecialisationMapEntryArray::const_iterator const & begin
	//	, ashes::SpecialisationMapEntryArray::const_iterator const & end )
	//{
	//	ashes::PushConstantArray result;

	//	//for ( auto it = begin; it != end; ++it )
	//	//{
	//	//	result.push_back( {
	//	//		it->constantID,
	//	//		it->offset,
	//	//		it->format,
	//	//		it->arraySize
	//	//	} );
	//	//}

	//	return result;
	//}

	//ashes::PushConstantsBufferPtr convert( ashes::VkDevice device
	//	, uint32_t location
	//	, VkShaderStageFlagBits stage
	//	, ashes::SpecialisationInfoBase const & specialisationInfo )
	//{
	//	return ashes::PushConstantsBufferPtr{};
	//	//auto result = device.createPushConstantsBuffer( location
	//	//	, stage
	//	//	, convert( stage, specialisationInfo.begin(), specialisationInfo.end() ) );
	//	//assert( result->getSize() == specialisationInfo.getSize() );
	//	//std::memcpy( result->getData()
	//	//	, specialisationInfo.getData()
	//	//	, specialisationInfo.getSize() );
	//	//return std::move( result );
	//}
}
