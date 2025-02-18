function install_dependencies()
    local hasher = darwin.dtw.newHasher()
    hasher.digest_folder_by_content("dependencies")
    local expected_sha = '8e8303b3a4349f2980ac47f810af790b1170eeaea127084492040f14c1c6e074'
    if hasher.get_value() == expected_sha then
        return
    end

    darwin.dtw.remove_any("dependencies")

    os.execute("mkdir -p dependencies")
    os.execute(
        "curl -L https://github.com/OUIsolutions/BearSslSingle-Unit/releases/download/0.0.2/BearSSLSingleUnit.h -o dependencies/BearSSLSingleUnit.h ")
    os.execute(
        "curl -L https://github.com/OUIsolutions/BearSslSingle-Unit/releases/download/0.0.2/BearSSLSingleUnit.c -o dependencies/BearSSLSingleUnit.c")

    local new_hasher = darwin.dtw.newHasher()
    new_hasher.digest_folder_by_content("dependencies")
    print("new hasher is: " .. new_hasher.get_value())
end
