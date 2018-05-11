if ( os.istarget( "Windows" ) )
then
	OSDependentRootDir = path.join( "..", "..", "dependencies", "glslang" )
else
	OSDependentRootDir = VulkanSDKDir
end

OSDependentIncludeDir = path.join( OSDependentRootDir, "include" )
OSDependentEnabled = false

if ( os.isdir( OSDependentIncludeDir ) )
then
	OSDependentLibDir = path.join( OSDependentRootDir, "lib" )
	includedirs{
		OSDependentIncludeDir,
	}
	libdirs { OSDependentLibDir }
	if ( os.istarget( "Windows" ) )
	then
		filter( "configurations:Debug" )
			links{ "OSDependentd" }
		filter( "configurations:Release" )
			links{ "OSDependent" }
	else
		links{ "OSDependent" }
	end
	OSDependentEnabled = true
end
