DEPS = {
    {url = "https://github.com/OUIsolutions/BearSslSingle-Unit/releases/download/0.0.3/BearSSLSingleUnit.h", dest = "dependencies/BearSSLSingleUnit.h"},
    {url = "https://github.com/OUIsolutions/BearSslSingle-Unit/releases/download/0.0.3/BearSSLSingleUnit.c", dest = "dependencies/BearSSLSingleUnit.c"},
    {url = "https://raw.githubusercontent.com/DaveGamble/cJSON/refs/tags/v1.7.18/cJSON.c", dest = "dependencies/cJSON.c"},
    {url = "https://raw.githubusercontent.com/DaveGamble/cJSON/refs/tags/v1.7.18/cJSON.h", dest = "dependencies/cJSON.h"},
    {url = "https://github.com/OUIsolutions/C2Wasm/releases/download/0.7.0/c2wasm.c", dest = "dependencies/c2wasm.c"},
    {url = "https://github.com/SamuelHenriqueDeMoraisVitrio/UniversalSocket/releases/download/v0.2.8/UniversalSocket.c", dest = "dependencies/UniversalSocket.c"},
    {url = "https://github.com/SamuelHenriqueDeMoraisVitrio/UniversalSocket/releases/download/v0.2.8/UniversalSocket.h", dest = "dependencies/UniversalSocket.h"},
    {url = "https://github.com/OUIsolutions/BearSSLTrustedAnchors/releases/download/2025-8-6/BearSSLTrustAnchors.c", dest = "dependencies/BearSSLTrustAnchors.c"},
}

-- Downloads once, then reuses the result!
local cache_curl = darwin.dtw.create_cache_function({
    expiration = 300,  -- 5 minutes
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
