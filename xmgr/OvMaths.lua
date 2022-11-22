
local root_dir = path.join(os.projectdir(),"Sources","Overload","OvMaths")

local function add_src(...)
    for _, fn in ipairs({...}) do
        add_files(path.join(root_dir,"src",fn))
    end
end

local function add_inc(...)
    for _, fn in ipairs({...}) do
        add_headerfiles(path.join(root_dir,"include",fn))
    end
end

target("OvMaths")
    set_languages("c++17")
    set_kind("static")
    add_src("**.cpp")
    add_inc("**.h","**.inl")
    add_includedirs(path.join(root_dir,"include"),{public=true})
