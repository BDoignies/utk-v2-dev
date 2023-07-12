# Uni{corn|form} Tool Kit

![](figs/logo_utk2.jpg) The UTK tool kit aims at providing executables to generate and analyze point sets in unit domains $[0,1)^s$. It is originally meant to help researchers developing sampling patterns in a numerical integration using Monte Carlo estimators. More precisely, it was developed with the precise question of optimizing image synthesis via Path tracing algorithms.

UTK is a C++ library that implements a large variety of samplers and tools to analyze and compare them (discrepancy evaluation, spectral analysis, numerical integration tests...).

## License

The core of the library is available under the [BSD](license) license. For some samplers, the library is just a wrapper on some codes with specific license (see the [samplers](samplers) page for details).

## Clone and Build

UTK is hosted on a git repository. To install it, the first step is to clone the repository on your computer. This can be done using the following command line:

    git clone https://github.com/utk-team/utk.git

The code can be build on a Linux or a Mac environment. The Windows build is not yet supported. To build it, one can use the following commands, assuming the current working directory is the cloned utk repository.

```
mkdir build
cd build
cmake ..
make
```

See `Using UTK/*` documentation pages for mor information and building options. 

## External libraries

Most libraries are installed and downloaded automatically. However, some are not.

BNOT requires the following librarires to be built : 

* CGAL
* BOOST
* SuiteSparse
* LAPACK
* BLAS
* METIS
* MPFR
* GMP

CGal and Boost are downloaded automatically if not found. Other should be install 
and made available to relevent cmake find_package. 

For Ubuntu the following lines installs everything :

```
sudo apt-get install libblas-dev 
sudo apt-get install liblapack-dev
sudo apt-get install libgmp-dev
sudo apt-get install libmpfr-dev
sudo apt-get install libmetis-dev
sudo apt-get install libsuitesparse-dev 
sudo apt-get install libcgal-dev
```

## Authors

The UTK library was developed mainly by

*   David Coeurjolly [david.coeurjolly@liris.cnrs.fr](mailto:david.coeurjolly@liris.cnrs.fr)
*   Hélène Perrier [helene.perrier@liris.cnrs.fr](mailto:helene.perrier@liris.cnrs.fr)
*   Bastien Doignies [bastien.doignies@liris.cnrs.fr](mailto:bastien.doignies@liris.cnrs.fr)

But was also contributed to by

*   Victor Ostromoukhov [victor.ostromoukhov@liris.cnrs.fr](mailto:victor.ostromoukhov@liris.cnrs.fr)
*   Adrien Pilleboue
*   Jean David Genevaux

If you want to refer to UTK in your publications, please use the following bibtex entry:

```
@Misc{UTK,
title = {Uni(corn|form) tool kit},
note = {url{https://utk-team.github.io/utk/}}
}
```
## Contributing

UTK is an open-source, community driven, project. If you want to help us (typos, documentations issues, new samplers, new tools...), feel free to create a Pull-Request on the Github project.
