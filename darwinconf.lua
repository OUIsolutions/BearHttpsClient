DEPS = {
    {url = "https://github.com/OUIsolutions/BearSslSingle-Unit/releases/download/0.0.3/BearSSLSingleUnit.h", dest = "dependencies/BearSSLSingleUnit.h"},
    {url = "https://github.com/OUIsolutions/BearSslSingle-Unit/releases/download/0.0.3/BearSSLSingleUnit.c", dest = "dependencies/BearSSLSingleUnit.c"},
    {url = "https://raw.githubusercontent.com/DaveGamble/cJSON/refs/tags/v1.7.18/cJSON.c", dest = "dependencies/cJSON.c"},
    {url = "https://raw.githubusercontent.com/DaveGamble/cJSON/refs/tags/v1.7.18/cJSON.h", dest = "dependencies/cJSON.h"},
    {url = "https://github.com/OUIsolutions/C2Wasm/releases/download/0.10.0/c2wasm.c", dest = "dependencies/c2wasm.c"},
    {url = "https://github.com/SamuelHenriqueDeMoraisVitrio/UniversalSocket/releases/download/v0.2.8/UniversalSocket.c", dest = "dependencies/UniversalSocket.c"},
    {url = "https://github.com/SamuelHenriqueDeMoraisVitrio/UniversalSocket/releases/download/v0.2.8/UniversalSocket.h", dest = "dependencies/UniversalSocket.h"},
    {url = "https://github.com/OUIsolutions/BearSSLTrustedAnchors/releases/download/2025-8-6/BearSSLTrustAnchors.c", dest = "dependencies/BearSSLTrustAnchors.c"},
}

-- Downloads once, then reuses the result!
local cache_curl = darwin.dtw.create_cache_function({
    expiration = darwin.dtw.INFINITY,  -- 5 minutes
    cache_dir = "./web_cache/",
    cache_name = "libs",  -- Added cache_name
    callback = function(url)
        os.execute("curl -L " .. url .. " -o temp")
        local content =  darwin.dtw.load_file("temp")
        darwin.dtw.remove_any("temp")
        return content
    end
})



function install_dependencies()
 

    darwin.dtw.remove_any("dependencies")

    for i=1 ,#DEPS do
        local dep = DEPS[i]
        local dest = dep.dest
        local content = cache_curl(dep.url)
        darwin.dtw.write_file(dest, content)
    end

  

end

function main()
    
    if not darwin.argv.one_of_args_exist("no_dep_install") then
      install_dependencies()
    end

    local public_declare = darwin.mdeclare.transform_dir({
        dir="src",
        startswith="public_fdefine",
        endswith=".c",
    })

    darwin.dtw.write_file("src/public_declare.all.h", public_declare)
    darwin.silverchain.remove("src/public_declare.all.h")


    local private_declare = darwin.mdeclare.transform_dir({
        dir="src",
        startswith="private_fdefine",
        endswith=".c",
    })

    darwin.dtw.write_file("src/private_declare.all.h", private_declare)
    darwin.silverchain.remove("src/private_declare.all.h")

    
    darwin.silverchain.generate({
        src = "src",
        tags = { 
            ------------------- Public Headders --------------------------------------------
            "tests",
            "public_dep_declare",
            "public_types",
            "public_macros",
            "public_declare",
            ------------------- Private Headders -------------------------------------
            "private_macros",
            "private_dep_declare",
            "private_types",
            "private_declare",
            --------------------- Sources --------------------------------------------
            "private_globals",
            "public_fdefine",
            "private_fdefine",
            "private_dep_define"
    },
        implement_main = false,
        project_short_cut="bear_https_client",
    })

    if darwin.argv.one_of_args_exist("only_silverchain") then
        return 
    end 

    local MAX_CONNTENT = darwin.camalgamator.ONE_MB * 20
    local MAX_RECURSION = 100

    local lincense = "/* " .. darwin.dtw.load_file("LICENSE") .. " */ \n"

    local onefile = darwin.camalgamator.generate_amalgamation("src/one.c", MAX_CONNTENT, MAX_RECURSION)
    onefile = lincense .. onefile

    darwin.dtw.write_file("release/BearHttpsClientOne.c", onefile)


    local only_declare = darwin.camalgamator.generate_amalgamation("src/imports/imports.public_declare.h", MAX_CONNTENT,
    MAX_RECURSION)
    only_declare = lincense .. only_declare
    darwin.dtw.write_file("release/BearHttpsClient.h", only_declare)
   -- darwin.silverchain.remove("release/BearHttpsClient.h")



     if not darwin.argv.one_of_args_exist("no_zip") then
           os.execute("zip -r release/BearHttpsClient.zip dependencies src build")
     end


end
main()