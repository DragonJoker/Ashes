if ( os.istarget( "Windows" ) )
then
	SPIRVRootDir = path.join( "..", "..", "dependencies", "glslang" )
else
	SPIRVRootDir = VulkanSDKDir
end

SPIRVIncludeDir = path.join( SPIRVRootDir, "include" )
SPIRVEnabled = false

if ( os.isdir( SPIRVIncludeDir ) )
then
print( SPIRVRootDir )
	SPIRVLibDir = path.join( SPIRVRootDir, "lib" )
	includedirs{
		SPIRVIncludeDir,
	}
	libdirs { SPIRVLibDir }
	if ( os.istarget( "Windows" ) )
	then
		filter( "configurations:Debug" )
			links{ "SPIRVd" }
			links{ "SPVRemapperd" }
		filter( "configurations:Release" )
			links{ "SPIRV" }
			links{ "SPVRemapper" }
	else
		links{ "SPIRV" }
	end
	SPIRVEnabled = true
end
