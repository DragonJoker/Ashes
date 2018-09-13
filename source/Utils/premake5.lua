project "Utils"

currentSourceDir = path.join( sourceDir, "Utils", "Utils" )
currentBinaryDir = path.join( binaryDir, "Utils", "Utils" )

kind( "StaticLib" )
targetdir( path.join( outputDir, "%{cfg.architecture}", "%{cfg.buildcfg}", staticLibDir ) )
location( currentBinaryDir )

includedirs{
	path.join( sourceDir, "Ashes", "Ashes", "Src" ),
	path.join( binaryDir, "Ashes", "Ashes", "Src" ),
	path.join( currentBinaryDir, "Src" ),
	path.join( currentSourceDir, "Src" )
}

links{
	"Ashes"
}
files{ "./Src/**.hpp",
	"./Src/**.inl",
	"./Src/**.cpp"
}

vpaths{ ["Header Files"] = "**.hpp" }
vpaths{ ["Header Files"] = "**.inl" }
vpaths{ ["Source Files"] = "**.cpp" }
