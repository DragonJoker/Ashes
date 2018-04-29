project "Utils"

currentSourceDir = path.join( sourceDir, "Utils", "Utils" )
currentBinaryDir = path.join( binaryDir, "Utils", "Utils" )

kind( "StaticLib" )
targetdir( path.join( outputDir, "%{cfg.architecture}", "%{cfg.buildcfg}", staticLibDir ) )
location( currentBinaryDir )

includedirs{
	path.join( sourceDir, "Renderer", "Renderer", "Src" ),
	path.join( binaryDir, "Renderer", "Renderer", "Src" ),
	path.join( currentBinaryDir, "Src" ),
	path.join( currentSourceDir, "Src" )
}

links{
	"Renderer"
}
files{ "./Src/**.hpp",
	"./Src/**.inl",
	"./Src/**.cpp"
}

vpaths{ ["Header Files"] = "**.hpp" }
vpaths{ ["Header Files"] = "**.inl" }
vpaths{ ["Source Files"] = "**.cpp" }
