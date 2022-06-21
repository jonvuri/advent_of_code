from conans import ConanFile


class AdventOfCodeProject(ConanFile):
    name = "AdventOfCodeProject"
    version = "0.1"
    requires = (
        "docopt.cpp/0.6.3",
        "doctest/2.4.6",
        "spdlog/1.9.2",
    )
    generators = "cmake", "gcc", "txt", "cmake_find_package"

