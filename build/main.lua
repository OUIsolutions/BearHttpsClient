function main()
    install_dependencies()

    darwin.silverchain.generate({
        src = "src",
        tags = { "types", "fdeclare" },
        implement_main = false
    })
end
