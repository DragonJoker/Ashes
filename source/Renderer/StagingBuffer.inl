namespace renderer
{
	template< typename T >
	inline void StagingBuffer::doCopyUniformDataToStagingBuffer( T const * const datas
		, uint32_t count
		, uint32_t offset )const
	{
		auto size = count * offset;
		auto buffer = m_stagingBuffer->getBuffer().lock( 0
			, size
			, 0 );

		if ( !buffer )
		{
			throw std::runtime_error{ "Staging buffer storage memory mapping failed." };
		}

		for ( uint32_t i = 0; i < count; ++i )
		{
			std::memcpy( buffer, &datas[i], sizeof( T ) );
			buffer += offset;
		}

		m_stagingBuffer->getBuffer().unlock( size
			, true );
	}
}
