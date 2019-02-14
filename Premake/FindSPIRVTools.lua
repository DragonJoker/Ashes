if ( os.istarget( "Windows" ) )
then
	SPIRVToolsRootDir = path.join( "..", "..", "dependencies", "glslang" )
else
	SPIRVToolsRootDir = VulkanSDKDir
end

SPIRVToolsIncludeDir = path.join( SPIRVToolsRootDir, "include" )
SPIRVToolsEnabled = false

if ( os.isdir( SPIRVToolsIncludeDir ) )
then
	SPIRVToolsLibDir = path.join( SPIRVToolsRootDir, "lib" )
	includedirs{
		SPIRVToolsIncludeDir,
	}
	libdirs { SPIRVToolsLibDir }
	if ( os.istarget( "Windows" ) )
	then
		filter( "configurations:Debug" )
			links{ "SPIRV-Toolsd" }
			links{ "SPIRV-Tools-optd" }
		filter( "configurations:Release" )
			links{ "SPIRV-Tools" }
			links{ "SPIRV-Tools-opt" }
	else
		links{ "SPIRV-Tools" }
		links{ "SPIRV-Tools-opt" }
	end
	SPIRVToolsEnabled = true
end
