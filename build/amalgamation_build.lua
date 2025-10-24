function amalgamation_build()
    
    local lincense = "/* " .. darwin.dtw.load_file("LICENSE") .. " */ \n"

    local only_definition = darwin.camalgamator.generate_amalgamation_with_callback("src/imports/imports.fdefine.h",
        function(import, path)
            if import == "src/imports/imports.fdeclare.h" then
                      return "dont-include"
            end

            return "include-once"
        end,
        
        MAX_CONNTENT,
        MAX_RECURSION
    )

    only_definition = '#include "BearHttpsClient.h"\n' .. only_definition

    only_definition = lincense .. only_definition

    darwin.dtw.write_file("release/BearHttpsClient.c", only_definition)

    os.execute("zip -r release/BearHttpsClient.zip dependencies src build")
end

darwin.add_recipe({
    name="amalgamation_build",
    requires={"header_build"},
    description="make a amalgamation file and a zip",
    outs={"release/BearHttpsClient.c", "release/BearHttpsClient.zip"},
    callback=amalgamation_build
})