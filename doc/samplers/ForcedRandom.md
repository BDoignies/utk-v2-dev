# Sampler Forced Random [[CTS*17]](https://link.springer.com/article/10.1007/s00371-017-1392-7)

## Description

The sampler from [[FAT*11]](https://www.cs.huji.ac.il/w~raananf/projects/kdm/)  
For a more precise description of this sampler and its performances in terms of aliasing and discrepancy, please refer to the following web bundle [https://liris.cnrs.fr/ldbn/HTML_bundle/index.html](https://liris.cnrs.fr/ldbn/HTML_bundle/index.html).

## Files

```
src/samplers/ForcedRandom.cpp  
include/utk/samplers/SamplerForcedRandom.hpp
externals/Fattal/ForcedRandom
```

## Usage

<button class="tablink exebutton" onclick="openCode('exe', this)" markdown="1">Exe</button> 
<button class="tablink cppbutton" onclick="openCode('cpp', this)" markdown="1">C++</button> 
<button class="tablink pybutton" onclick="openCode('py', this)" markdown="1">Python</button> 
<br/>
  

<div class="exe tabcontent">

```bash
ForcedRandom sampler
Usage: ./samplers/ForcedRandom [OPTIONS]

Options:
  -h,--help                   Print this help message and exit
  -n UINT REQUIRED            Number of points
  -m UINT [1]                 Number of pointsets
  -o,--out TEXT [out.dat]     Output file
  --sparsity INT [8]          Sparsity coeff
  --matrixSize UINT [512]     Matrix Size (512, 1024, 2048. Other will be interpreted as 512)
```

</div>

<div class="cpp tabcontent">

```  cpp
#include <utk/utils/PointsetIO.hpp>
#include <utk/utils/Pointset.hpp>
#include <utk/samplers/SamplerForcedRandom.hpp>

int main()
{
    utk::Pointset<double> pts;
    utk::SamplerForcedRandom fr(8 /* sparsity */, 512 /* matrix size */);    
    // Check for no errors
    if (fr.generateSamples(pts, 1024 /* Number of points */))
    {
        write_text_pointset("fr.dat", pts);
    }
}
```  

</div>

<div class="py tabcontent">

``` python
import pyutk

fr = pyutk.SamplerForcedRandom(
  8  , # Sparsity
  512, # Matrix size
)
samples = fr.sample(1024) # This is a numpy array !
```  

</div>

## License

See author's page.