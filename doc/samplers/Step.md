# Sampler Single Peak [[HSD13]](http://kops.uni-konstanz.de/bitstream/handle/123456789/24358/Heck_243584.pdf%3Bjsessionid%3D6A7E6EFBFF35CDC5C5F8EBB5928C44F8?sequence%3D2)

## Description

The sampler from [[HSD13]](http://kops.uni-konstanz.de/bitstream/handle/123456789/24358/Heck_243584.pdf%3Bjsessionid%3D6A7E6EFBFF35CDC5C5F8EBB5928C44F8?sequence%3D2) with a step profile.  

For a more precise description of this sampler and its performances in terms of aliasing and discrepancy, please refer to the following web bundle [https://liris.cnrs.fr/ldbn/HTML_bundle/index.html](https://liris.cnrs.fr/ldbn/HTML_bundle/index.html).

## Files

```
src/samplers/Step.cpp  
include/utk/samplers/SamplerStep.hpp
externals/Step/*
```

## Usage

<button class="tablink exebutton" onclick="openCode('exe', this)" markdown="1">Exe</button> 
<button class="tablink cppbutton" onclick="openCode('cpp', this)" markdown="1">C++</button> 
<button class="tablink pybutton" onclick="openCode('py', this)" markdown="1">Python</button> 
<br/>
  

<div class="exe tabcontent">

```bash
Step sampler
Usage: ./build/src/samplers/Step [OPTIONS]

Options:
  -h,--help                   Print this help message and exit
  -n UINT REQUIRED            Number of points
  -s,--seed UINT              Seed (unspecified means 'random')
  -m UINT [1]                 Number of pointsets
  -o,--out TEXT [out.dat]     Output file
  --criticalFreq FLOAT [0.606]
                              Critical frequency
  --smoothing FLOAT [8]       Smoothing
```

</div>

<div class="cpp tabcontent">

```  cpp
#include <utk/utils/PointsetIO.hpp>
#include <utk/utils/Pointset.hpp>
#include <utk/samplers/SamplerStep.hpp>

int main()
{
    utk::Pointset<double> pts;

    utk::SamplerStep step(
      0.606, /* critical frequency */
      8,    /* smoothing */
    );
    step.setRandomSeed(args->seed);
    // Check for no errors
    if (step.generateSamples(pts, 1024 /* Number of points */))
    {
        write_text_pointset("step.dat", pts);
    }
}
```  

</div>

<div class="py tabcontent">

``` python
import pyutk

sp = pyutk.Step(
  0.606, # Critical frequency
  8      # Smoothing
)
samples = sp.sample(1024) # This is a numpy array !
```  

</div>

## Results

<div class="results"></div>
<script>
  window.addEventListener('DOMContentLoaded', function() { show_results(); }); 
</script>

## License

```
//
// Source code for the paper, edited to fit the utk framework
//
// D. Heck and T. Schloemer and O. Deussen, "Blue Noise Sampling with
// Controlled Aliasing", ACM Trans. Graph., 2013, in press
//
// Copyright (C) 2012,2013 Daniel Heck and Thomas Schloemer
//
``` 