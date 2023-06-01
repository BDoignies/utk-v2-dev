from setuptools import setup

setup(
    name="pyutk",
    version="0.0.0",
    author="UTK Team",
    author_email="david.coeurjolly@cnrs.fr",
    description="Quasi Monte Carlo samplers and utilities",
    long_description="Quasi Monte Carlo samplers and utilities",
    long_description_content_type="text/markdown",
    package_dir={'':'build/src/pyutk/'},
    url="https://utk-team.github.io/utk/",
    zip_safe=False,
    python_requires=">=3.6",
    install_requires=[
        'numpy'
        # TODO : install GCC ?
    ],
    classifiers=[
        "Programming Language :: Python :: 3",
        "License :: OSI Approved :: BSD License",
        "Operating System :: OS Independent",
    ],
)