import os
import re
import subprocess
import sys
from pathlib import Path

from setuptools import Extension, setup, find_packages
from setuptools.command.build_ext import build_ext

setup(
    name="pyutk",
    version="0.0.0",
    author="UTK Team",
    author_email="david.coeurjolly@cnrs.fr",
    description="Quasi Monte Carlo samplers and utilities",
    long_description="Quasi Monte Carlo samplers and utilities",
    long_description_content_type="text/markdown",
    package_data={'': ['build/src/pyutk']},
    url="https://utk-team.github.io/utk/",
    zip_safe=False,
    python_requires=">=3.5",
    classifiers=[
        "Programming Language :: Python :: 3",
        "License :: OSI Approved :: BSD License",
        "Operating System :: OS Independent",
    ],
)