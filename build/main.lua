function main()
    install_dependencies()

    darwin.silverchain.generate({
        src = "src",
        tags = { "dep_declare", "macros", "consts", "types", "fdeclare", "fdefine" },
        implement_main = false
    })
    local MAX_CONNTENT = darwin.camalgamator.ONE_MB * 20
    local MAX_RECURSION = 100

    local lincense = "/* " .. darwin.dtw.load_file("LICENSE") .. " */ \n"

    local onefile = darwin.camalgamator.generate_amalgamation("src/one.c", max_content, max_recursion)
    onefile = lincense .. onefile

    darwin.dtw.write_file("release/BearHttpsClientOne.c", onefile)

    local only_declare = darwin.camalgamator.generate_amalgamation("src/imports/imports.fdeclare.h", MAX_CONTENT,
    MAX_RECURSION)
    only_declare = lincense .. only_declare
    darwin.dtw.write_file("release/BearHttpsClient.h", only_declare)



    local only_definition = darwin.camalgamator.generate_amalgamation_with_callback("src/imports/imports.fdefine.h",
        function(import, path)
            if darwin.dtw.ends_with(import, ".c") then
                return "include-once"
            end
            return "dont-include"
        end,
        MAX_CONTENT,
        MAX_RECURSION
    )
    only_definition = '#include "BearHttpsClient.h"\n' .. only_definition

    only_definition = lincense .. only_definition

    darwin.dtw.write_file("release/BearHttpsClient.c", only_definition)

    os.execute("zip -r release/BearHttpsClient.zip dependencies src build")

end
