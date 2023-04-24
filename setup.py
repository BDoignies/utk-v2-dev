import os
import re
import subprocess
import sys
from pathlib import Path

from setuptools import Extension, setup
from setuptools.command.build_ext import build_ext

class CMakeExtension(Extension):
    def __init__(self, name: str, sourcedir: str = "") -> None:
        super().__init__(name, sources=[])
        self.sourcedir = os.fspath(Path(sourcedir).resolve())

class CMakeBuild(build_ext):
    def build_extension(self, ext: CMakeExtension) -> None:
        build_temp = ext.sourcedir
        subprocess.run(
            ["cmake", "--build", "."], cwd=build_temp, check=True
        )

setup(
    name="pyutk",
    version="0.0.0",
    author="UTK Team",
    author_email="david.coeurjolly@cnrs.fr",
    description="Quasi Monte Carlo samplers and utilities",
    long_description="Quasi Monte Carlo samplers and utilities",
    long_description_content_type="text/markdown",
    url="https://utk-team.github.io/utk/",
    ext_modules=[CMakeExtension("pyutk", "build/src/pyutk")],
    cmdclass={"build_ext": CMakeBuild},
    zip_safe=False,
    python_requires=">=3.5",
    classifiers=[
        "Programming Language :: Python :: 3",
        "License :: OSI Approved :: BSD License",
        "Operating System :: OS Independent",
    ],
)