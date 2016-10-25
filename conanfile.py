from conans import ConanFile, tools, CMake
import os
import shutil

class CppmetricsConan(ConanFile):
    name = "cppmetrics"
    version = "1.0"
    settings = "os", "compiler", "build_type", "arch"
    exports = "*"

    def config(self):
        self.requires.add("glog")
        self.requires.add("gtest")
        self.requires.add("Poco")

    def build(self):
        cmake = CMake(self.settings)
        self.run('cmake %s %s' % (self.conanfile_directory, cmake.command_line))
        self.run("cmake --build . %s" % cmake.build_config)
        
    def package(self):
        self.copy("*.h", dst="include")
        self.copy("*.lib", dst="lib", src="lib")
        self.copy("*.a", dst="lib", src="lib")

    def package_info(self):
        self.cpp_info.libs = ["cppmetrics"]