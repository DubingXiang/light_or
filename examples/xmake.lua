target("scheduling_example")
    set_kind("binary")
    add_includedirs("../include")    
    add_files("scheduling_example.cc")
    add_packages("spdlog")
    add_packages("cli11")
    add_packages("nlohmann_json")

    add_deps("light_or")
    if is_mode("release") then 
        set_targetdir("../bin/release")
    end 
    if is_mode("debug") then 
        set_targetdir("../bin/debug")
    end 

-- function all_tests()
--     local res = {}
--     for _,x in ipairs(os.files("**.cc")) 
-- do
--     local item = {}
--     local s = path.filename(x)
--     table.insert(item, s:sub(1, #s-3)) -- target
--     table.insert(item, path.relative(x, ".")) --source    
--     table.insert(res, item)
--     end 
--     return res
-- end

-- for _, test in ipairs(all_tests()) do
-- target(test[1])
--     print("########## build target: ", test[2])
--     set_kind("binary")
--     add_files(test[2])
--     add_includedirs("../include")
--     add_packages("spdlog")
--     add_packages("cli11")
--     add_packages("nlohmann_json")
--     add_deps("light_or")
--     if is_mode("release") then 
--         set_targetdir("../bin/release")
--     end 
--     if is_mode("debug") then 
--         set_targetdir("../bin/debug")
--     end 

-- end