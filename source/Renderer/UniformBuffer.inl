namespace renderer
{
	template< typename T >
	UniformBuffer< T >::UniformBuffer( RenderingResources const & resources
		, uint32_t count
		, BufferTarget target
		, MemoryPropertyFlags flags )
		: m_resources{ resources }
		, m_buffer{ resources.getDevice().createUniformBuffer( size
			, convert( target )
			, convert( flags ) ) }
		, m_data( count, T{} )
	{
	}
}
