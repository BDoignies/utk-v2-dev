# Sampler Fattal [[ANH*17]](https://www.cs.huji.ac.il/w~raananf/projects/kdm/)

## Description

The sampler from [[FAT*11]](https://www.cs.huji.ac.il/w~raananf/projects/kdm/)  
For a more precise description of this sampler and its performances in terms of aliasing and discrepancy, please refer to the following web bundle [https://liris.cnrs.fr/ldbn/HTML_bundle/index.html](https://liris.cnrs.fr/ldbn/HTML_bundle/index.html).

## Files

```
src/samplers/Fattal.cpp  
include/utk/samplers/SamplerFattal.hpp
externals/Fattal/*
```

## Usage

<button class="tablink exebutton" onclick="openCode('exe', this)" markdown="1">Exe</button> 
<button class="tablink cppbutton" onclick="openCode('cpp', this)" markdown="1">C++</button> 
<button class="tablink pybutton" onclick="openCode('py', this)" markdown="1">Python</button> 
<br/>
  

<div class="exe tabcontent">

```bash
ART sampler
Usage: ./samplers/Fattal [OPTIONS]

Options:
  -h,--help                   Print this help message and exit
  -n UINT REQUIRED            Number of points
  -m UINT [1]                 Number of pointsets
  -o,--out TEXT [out.dat]     Output file
  -t,--temperature FLOAT [0.5]
                              Temperature
  -l,--level UINT [2]         Levels
```

</div>

<div class="cpp tabcontent">

```  cpp
#include <utk/utils/PointsetIO.hpp>
#include <utk/utils/Pointset.hpp>
#include <utk/samplers/SamplerFattal.hpp>

int main()
{
    utk::Pointset<double> pts;
    utk::SamplerFattal fattal(0.5 /* temperature */, 2 /* levels */);    
    // Check for no errors
    if (fattal.generateSamples(pts, 1024 /* Number of points */))
    {
        write_text_pointset("fattal.dat", pts);
    }
}
```  

</div>

<div class="py tabcontent">

``` python
import pyutk

fattal = pyutk.Fattal(
  0.5, # temperature
  2,   # levels
)
samples = fattal.sample(1024) # This is a numpy array !
```  

</div>

## Results

<div class="results"></div>
<script>
  window.addEventListener('DOMContentLoaded', function() { show_results(); }); 
</script>
## License

See author's page.