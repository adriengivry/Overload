
local over_dir = path.join(os.projectdir(),"Sources","Overload")
local proj_dir =  path.join(over_dir,"OvCore")
local dep_dir = path.join(os.projectdir(),"Dependencies")

local function add_src(...)
    for _, fn in ipairs({...}) do
        add_files(path.join(proj_dir,"src",fn))
    end
end

local function add_inc(...)
    for _, fn in ipairs({...}) do
        add_headerfiles(path.join(proj_dir,"include",fn))
    end
end

local function dep_libdirs(dirs)
    for _, fn in ipairs(dirs) do
        add_linkdirs(path.join(dep_dir,fn))
    end
end


local cfg_mode = is_mode("debug")  and "debug" or "release"

local dep_lib = {
    "assimp-vc142-mt",
    "zlibstatic",
    "Bullet3Collision",
    "Bullet3Common",
    "Bullet3Dynamics",
    "Bullet3Geometry",
    "BulletCollision",
    "BulletDynamics",
    "BulletSoftBody",
    "LinearMath",
    "glew32",
    "glfw3dll",
    "irrKlang",
    "liblua53",
    "opengl32"
}

local dep_dirs = {
    "glfw/lib",
    "bullet3/lib/" .. cfg_mode,
    "lua/lib",
    "glew/lib",
    "glew/lib",
    "irrklang/lib",
    "assimp/lib"
}


target("OvCore")
    set_languages("c++17")
    set_kind("static")
    add_src("**.cpp")
    add_inc("**.h","**.inl")
    dep_libdirs(dep_dirs)
    add_links(unpack(dep_lib))
    add_cxflags("/bigobj")
    add_cxxflags("/wd4099",{force = true,public=true})
    add_syslinks("shell32","comdlg32",{public=true})
    add_deps("OvUI","OvAudio","OvPhysics","OvAnalytics","OvRendering","OvWindowing")
    add_includedirs(path.join(proj_dir,"include"),{public=true})
    add_includedirs(path.join(dep_dir,"lua/include"),{public=true})
