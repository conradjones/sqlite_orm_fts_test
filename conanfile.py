from conan import ConanFile
from conan.errors import ConanInvalidConfiguration
from conan.tools.cmake import CMake, CMakeToolchain, cmake_layout
from conan.tools.files import copy, get, replace_in_file, rmdir
from conan.tools.cmake import CMake, CMakeDeps, CMakeToolchain, cmake_layout

class TestMe(ConanFile):
    name = "test"
    version = "0.1"
    settings = "os", "compiler", "build_type", "arch"
    keep_imports = True
    short_paths = True
    scm = {
        "type": "git",  # Use "type": "svn", if local repo is managed using SVN
        "url": "auto",
        "revision": "auto"
    }

    def requirements(self):
        user = "master"
        channel = "turboit"

        self.requires("sqlite3/3.45.0")
        self.requires("sqlite_orm/1.9.1")

def generate(self):
        tc = CMakeToolchain(self)
        tc.generate()
