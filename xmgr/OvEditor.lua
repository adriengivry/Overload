
local over_dir = path.join(os.projectdir(),"Sources","Overload")
local proj_dir =  path.join(over_dir,"OvEditor")
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

local run_env_path = {
    "assimp",
    "glew",
    "glfw",
    "irrklang",
    "lua"
}

local function envs()
    local mt = {os.getenv("PATH")}
    for _,s in ipairs(run_env_path) do
        local ss =  path.join(os.projectdir(),"Dependencies",s,"bin")
        table.insert(mt,ss)
    end
    return  path.joinenv(mt)
end


target("OvEditor")
    set_languages("c++17")
    set_kind("binary")
    add_src("**.cpp")
    add_inc("**.h","**.inl")
    add_deps("OvCore")
    add_includedirs(path.join(proj_dir,"include"),{public=true})
    set_runenv("PATH", envs())
    -- set_runargs("arg1","arg2")


