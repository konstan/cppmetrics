from conans import ConanFile, tools, CMake
import os
import shutil

class CppmetricsConan(ConanFile):
    name = "cppmetrics"
    version = "1.0"
    settings = "os", "compiler", "build_type", "arch"
    exports = "*"
    generators = "cmake"

    def config(self):
        self.requires.add("glog/0.3.4@dwerner/testing")
        self.requires.add("gtest/1.7.0@lasote/stable")
        self.requires.add("Poco/1.7.5@fizx/testing")

    def build(self):
        cmake = CMake(self.settings)
        self.run('cmake %s %s' % (self.conanfile_directory, cmake.command_line))
        self.run("cmake --build . %s" % cmake.build_config)
        
    def package(self):
        self.copy("*.h", dst="include", src="src)
        self.copy("*.lib", dst="lib", src="lib")
        self.copy("*.dylib", dst="lib", src="lib")
        self.copy("*.a", dst="lib", src="lib")

    def package_info(self):
        self.cpp_info.libs = ["cppmetrics"]
