function header_build()
    
    local lincense = "/* " .. darwin.dtw.load_file("LICENSE") .. " */ \n"

    local only_declare = darwin.camalgamator.generate_amalgamation("src/imports/imports.fdeclare.h", MAX_CONNTENT,
    MAX_RECURSION)
    only_declare = lincense .. only_declare
    darwin.dtw.write_file("release/BearHttpsClient.h", only_declare)

end

darwin.add_recipe({
    name="header_build",
    required={"amalgamation_build"},
    description="make a BearHttpsClient.h header file",
    outs={"release/BearHttpsClient.h"},
    callback=header_build
})