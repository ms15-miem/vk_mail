import qbs.base 1.0

Product {
    condition: false

    property string clientId
    property string clientSecret
    property string clientName

    type: ["staticlibrary", "installed_content"]
    name: "vreendirectauth"
    destination: "lib"

    cpp.defines: [
        "VREEN_DIRECTAUTH_CLIENT_ID=\"" + clientId + "\"",
        "VREEN_DIRECTAUTH_CLIENT_SECRET=\"" + clientSecret + "\"",
        "VREEN_DIRECTAUTH_CLIENT_NAME=\"" + clientName + "\""
    ]
    //cpp.warningLevel: "all"

    Depends { name: "cpp" }
    Depends { name: "qt"; submodules: ['core', 'network', 'webkit'] }
    Depends { name: "vreen"; }

    files: [
        "*.cpp",
        "*.h"
    ]

    Group {
        qbs.installDir: "include/vreen/auth"
        overrideTags: false
        fileTags: ["install"]
        files: ["*.h"]
    }
}

