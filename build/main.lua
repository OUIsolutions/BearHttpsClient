function main()
    install_dependencies()

    darwin.silverchain.generate({
        src = "src",
        tags = { "dep_declare", "macros", "consts", "types", "fdeclare", "fdefine" },
        implement_main = false
    })
    local MAX_CONNTENT = darwin.camalgamator.ONE_MB * 10
    local MAX_RECURSION = 100
    local onefile = darwin.camalgamator.generate_amalgamation("src/one.c", max_content, max_recursion)
    darwin.dtw.write_file("release/BearHttpsClientOne.c", onefile)




end
