local run_env_path = {
    "assimp",
    "glew",
    "glfw",
    "irrklang",
    "lua"
}

local function envs()
    local env_path = os.getenv("PATH")
    for _,s in pairs(run_env_path) do
        local ss = path.join(os.projectdir(),"Dependencies",s,"bin") .. ";"
        env_path = ss .. env_path
    end
    return {envs={PATH=env_path}}
end

target("editor")
    set_kind("phony")
    add_deps("OvEditor")
    on_run(function(target)
        local OvEditor = target:dep("OvEditor")
        local data_dst = path.join(os.projectdir(),OvEditor:targetdir(),"Data")
        local data_src  = path.join(os.projectdir(),"Resources")
        if not os.exists(data_dst) then
            os.cp(data_src,data_dst)
        end
        os.execv(OvEditor:targetfile(),{},envs())
    end)


target("game")
    set_kind("phony")
    add_deps("OvGame")
    on_run(function(target)
        local OvGame = target:dep("OvGame")
        local data_dst = path.join(os.projectdir(),OvGame:targetdir(),"Data")
        local data_src  = path.join(os.projectdir(),"Resources")
        if not os.exists(data_dst) then
            os.cp(data_src,data_dst)
        end
        os.execv(OvGame:targetfile(),{},envs())
    end)