# Using UTK C++ library

## Using UTK Library with externals Point formats

Points and pointsets have many different formats. UTK uses it's own, but have been designed to be integrated with many other.
Internally, the `utk::Poinset` is a `std::shared_ptr`. 
The class can be constructed in the following ways : 

* `utk::Poinset<Type>(uint n, uint d)` : Construct a pointset. The memory will be owned by the object. Copies will share the same memory.
* `utk::Poinset<Type>::View(T* data, uint n, uint d)` : Creates a wrapper around existing memory. The memory will not be deleted automatically, it is to the user to delete it properly. 
* `utk::Poinset<Type>::Copy(const T* data, uint n, uint d)` : Copies the data. The memory will be owned by the returning object. 

Only C-style linear buffers are supported.

**Important note** : Points might be stored in particular structures such as `struct Point{ float x, y; };` and arrays are built
upon such as : `std::vector<Point>`. *While technically undefined behavior (but so many codes depends on it, so it may be considered safe)*, it is possible to obtain an `utk::Pointset<float>` with the line `utk::Pointset<float>::View(static_cast<float*>(vector.data()), N, D)`; but be sure that your data are properly packed and aligned. In particular, make sure that there is no other attributes. For instance : `struct Point{ float* point; unsigned int N};` and `struct { std::vector<float> point; };` are not valid and copies will be necessary !


## Linking and Building

Most of UTK is header only. Hence, setting include path to `include/` is sufficient. However, some code provided by authors
are not header only and should be compiled along your project. Only the files required for the utk library to work have '.cpp'. 
extension. If building with CMake, you may use the following lines to get all files required: 

```cmake
FILE(GLOB_RECURSE ADDITIONNAL_CODE
        "PATH_TO_UTK/externals/AUTHOR_CODE_DIRECTORY/*.cpp"
)

target_include_directories(YOUR_PROJECT PRIVATE PATH_TO_UTK/externals/)
target_sources(YOUR_PROJECT PRIVATE ${ADDITIONNAL_CODE})
```
