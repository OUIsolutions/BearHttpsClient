function one_build()
        local content = darwin.mdeclare.transform_dir({
        dir="src",
        startswith="fdefine",
        endswith=".c",
    })

    darwin.dtw.write_file("src/fdeclare.all.h", content)
    darwin.silverchain.remove("src/fdeclare.all.h")

    
    
    darwin.silverchain.generate({
        src = "src",
        tags = { "tests","dep_declare", "macros", "types", "fdeclare","globals", "dep_define","fdefine" },
        implement_main = false,
        project_short_cut="bearhttps"
    })



    local MAX_CONNTENT = darwin.camalgamator.ONE_MB * 20
    local MAX_RECURSION = 100

    local lincense = "/* " .. darwin.dtw.load_file("LICENSE") .. " */ \n"

    local onefile = darwin.camalgamator.generate_amalgamation("src/one.c", MAX_CONNTENT, MAX_RECURSION)
    onefile = lincense .. onefile

    darwin.dtw.write_file("release/BearHttpsClientOne.c", onefile)
end

darwin.add_recipe({
    name="one_build",
    description="make a one file",
    outs={"release/BearHttpsClientOne.c"},
    callback=one_build
})