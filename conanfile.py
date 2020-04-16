from conans import ConanFile, CMake


class ConanConfig(ConanFile):
    requires = ["doctest/2.3.7"]

    generators = "cmake"

    settings = "build_type"

    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()
        self.run('mv compile_commands.json ..')
        self.run('./bin/test_lox')
