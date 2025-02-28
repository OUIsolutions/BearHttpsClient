function install_dependencies()
    local hasher = darwin.dtw.newHasher()
    hasher.digest_folder_by_content("dependencies")
    local EXPECTED_SHA = '2cb9d0ad7cedd30dd10b19a88b9f9c4bacdeccad916e3f3c2609ae349d8f605b'
    if hasher.get_value() == EXPECTED_SHA then
        return
    end

    darwin.dtw.remove_any("dependencies")

    os.execute("mkdir -p dependencies")
    os.execute(
        "curl -L https://github.com/OUIsolutions/BearSslSingle-Unit/releases/download/0.0.3/BearSSLSingleUnit.h -o dependencies/BearSSLSingleUnit.h ")
    os.execute(
        "curl -L https://github.com/OUIsolutions/BearSslSingle-Unit/releases/download/0.0.3/BearSSLSingleUnit.c -o dependencies/BearSSLSingleUnit.c")

    os.execute("curl -L https://raw.githubusercontent.com/DaveGamble/cJSON/refs/tags/v1.7.18/cJSON.c -o dependencies/cJSON.c")
    os.execute("curl -L https://raw.githubusercontent.com/DaveGamble/cJSON/refs/tags/v1.7.18/cJSON.h -o dependencies/cJSON.h")
    ---- trusted anchors
    darwin.dtw.remove_any("BearSSL")
    os.execute("git clone https://www.bearssl.org/git/BearSSL")
    os.execute("curl -L https://curl.se/ca/cacert.pem -o BearSSL/cacert.pem")
    os.execute("cd BearSSL && make")
    os.execute("chmod +x BearSSL/build/brssl")
    os.execute("./BearSSL/build/brssl ta BearSSL/cacert.pem > dependencies/BearSSLTrustAnchors.h")

    darwin.dtw.remove_any("BearSSL")
    os.execute(
        "curl -L https://github.com/SamuelHenriqueDeMoraisVitrio/UniversalSocket/releases/download/v0.2.4/UniversalSocket.c -o dependencies/UniversalSocket.c")
    os.execute(
        "curl -L https://github.com/SamuelHenriqueDeMoraisVitrio/UniversalSocket/releases/download/v0.2.4/UniversalSocket.h -o dependencies/UniversalSocket.h")



    local new_hasher = darwin.dtw.newHasher()
    new_hasher.digest_folder_by_content("dependencies")
    print("new hasher is: " .. new_hasher.get_value())
end
