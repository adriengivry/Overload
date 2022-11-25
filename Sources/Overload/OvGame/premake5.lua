project "OvGame"
	language "C++"
	cppdialect "C++17"
	files { "**.h", "**.inl","**.cpp", "**.rc" }
	includedirs { "include", dependdir .. "glfw/include", dependdir .. "stb_image/include", dependdir .. "lua/include", dependdir .. "bullet3/include", dependdir .. "glew/include", dependdir .. "irrklang/include",
	"%{wks.location}/OvAnalytics/include", "%{wks.location}/OvAudio/include", "%{wks.location}/OvCore/include",
	"%{wks.location}/OvDebug/include", "%{wks.location}/OvMaths/include", "%{wks.location}/OvPhysics/include",
	"%{wks.location}/OvRendering/include", "%{wks.location}/OvTools/include", "%{wks.location}/OvUI/include", "%{wks.location}/OvWindowing/include" }

	libdirs { dependdir .. "glfw/lib", dependdir .. "bullet3/lib/%{cfg.buildcfg}", dependdir .. "lua/lib", dependdir .. "glew/lib", dependdir .. "irrklang/lib", dependdir .. "assimp/lib" }
	links { "assimp-vc142-mt", "zlibstatic", "Bullet3Collision", "Bullet3Common", "Bullet3Dynamics", "Bullet3Geometry", "BulletCollision", "BulletDynamics", "BulletSoftBody", "LinearMath", "glew32", "glfw3dll", "irrKlang", "liblua53", 
	"opengl32", "OvAnalytics", "OvAudio", "OvCore", "OvDebug", "OvMaths", "OvPhysics", "OvRendering", "OvTools", "OvUI", "OvWindowing" }

	targetdir (outputdir .. "%{cfg.buildcfg}/%{prj.name}")
	objdir (objoutdir .. "%{cfg.buildcfg}/%{prj.name}")
	characterset ("MBCS")

	postbuildcommands {
		"for /f \"delims=|\" %%i in ('dir /B /S \"%{wks.location}..\\..\\Dependencies\\*.dll\"') do xcopy /Q /Y \"%%i\" \"%{wks.location}..\\..\\Bin\\%{cfg.buildcfg}\\%{prj.name}\"",
		
		"xcopy /Y /I /Q /D \"%{wks.location}\\..\\..\\Bin\\Debug\\%{prj.name}\\*.exe\" \"%{wks.location}\\..\\..\\Build\\%{cfg.buildcfg}\\Builder\\Development\"",
		"xcopy /Y /I /Q /D \"%{wks.location}\\..\\..\\Bin\\Debug\\%{prj.name}\\*.dll\" \"%{wks.location}\\..\\..\\Build\\%{cfg.buildcfg}\\Builder\\Development\"",
		"xcopy /Y /I /Q /D \"%{wks.location}\\..\\..\\Bin\\Release\\%{prj.name}\\*.exe\" \"%{wks.location}\\..\\..\\Build\\%{cfg.buildcfg}\\Builder\\Shipping\"",
		"xcopy /Y /I /Q /D \"%{wks.location}\\..\\..\\Bin\\Release\\%{prj.name}\\*.dll\" \"%{wks.location}\\..\\..\\Build\\%{cfg.buildcfg}\\Builder\\Shipping\"",

		"EXIT /B 0"
	}

	filter { "configurations:Debug" }
		defines { "DEBUG" }
		symbols "On"
		kind "ConsoleApp"

	filter { "configurations:Release" }
		defines { "NDEBUG" }
		optimize "On"
		kind "WindowedApp"