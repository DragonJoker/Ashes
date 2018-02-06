SPIRVIncludeDir = path.join( VulkanSDKDir, "include" )
SPIRVEnabled = false

if ( os.isdir( SPIRVIncludeDir ) )
then
	if ( os.istarget( "Windows" ) )
	then
		SPIRVLibDirDebug = path.join( VulkanSDKDir, "lib", "%{cfg.architecture:gsub('x86_64', 'x64')}", "Debug" )
		SPIRVLibDirRelease = path.join( VulkanSDKDir, "lib", "%{cfg.architecture:gsub('x86_64', 'x64')}", "Release" )
		if ( os.isdir( SPIRVLibDirDebug )
			and os.isdir( SPIRVLibDirRelease )
			and os.isfile( path.join( SPIRVLibDirDebug, "SPIRV.lib" ) )
			and os.isfile( path.join( SPIRVLibDirRelease, "SPIRV.lib" ) ) )
		then
			includedirs{
				SPIRVIncludeDir,
			}
			filter( "configurations:Debug" )
				libdirs { SPIRVLibDirDebug }
				links{ "SPIRV" }
			filter( "configurations:Release" )
				libdirs { SPIRVLibDirRelease }
				links{ "SPIRV" }
			SPIRVEnabled = true
		end
	else
		SPIRVLibDir = path.join( VulkanSDKDir, "lib" )

		if ( os.isdir( SPIRVLibDir )
			and os.isfile( path.join( SPIRVLibDir, "libSPIRV.so" ) ) )
		then
			includedirs{
				SPIRVIncludeDir,
			}
			libdirs { SPIRVLibDir }
			links{ "SPIRV" }
			SPIRVEnabled = true
		end
	end
end
