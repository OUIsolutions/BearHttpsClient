function main()
    install_dependencies()

    darwin.silverchain.generate({
        src = "src",
        tags = { "dep_declare", "macros", "types", "fdeclare", "fdefine" },
        implement_main = false
    })
end
