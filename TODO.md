# TODO-list 

## Samplers

- [X] Whitenoise
- [X] Halton
- [X] Hammersley
- [X] Regular Grid
- [X] Stratified
- [X] Hex Grid
- [X] MultiJitter (Removed: This is the same as CMJ...)
- [X] CMJ
- [X] Faure
- [X] NRooks
- [X] Niederreiter
- [X] LDBN
- [X] LutLDBN (Added, for simplicity of usage)
- [X] PMJ (With same structure as UTK (not single cpp, but actual UTK samplers). Method as parameter)
- [X] DartThrowing
- [X] FastPoisson
- [X] Projective Blue Noise (Complete refactoring: support arbitrary dimension and proper code)
- [X] Rank1
- [X] Korobov
- [X] Kronecker
- [X] AAPaterns
- [X] Penrose
- [X] ART
- [X] Sobol
- [X] SobolIndices (Removed: its Sobol with Owen Depth = 0)
- [X] Owen (Removed: by default in sobol)
- [X] Fattal
- [X] ForcedRandom
- [X] Lloyd
- [X] Step
- [X] SinglePeak

- [X] FromMatrix (Removed, code will be in Matbuilder)
- [X] GradientDescent (Removed. A differentiable discrepancy will be built instead)
- [X] Nebraska (The same is still a mystery to this day...)

## Scramblers 

- [X] Digital Shift (To be check for conversions. Is it optimal ?)
- [X] Owen
- [X] Cranley Patterson
- [X] Micro Jittering

## Remaining Samplers

In UTK : 

- [ ] BNOT   (CGAL, waiting for proper CMake)
- [ ] CapCVT (CGAL, waiting for proper CMake)
- [ ] FPO    (CGAL, waiting for proper CMake)
- [ ] BLNDS  (Messy, requires PCF, ...)

- [ ] Genetric Owen    (requires owen version where the whole permutation tree is stored...)
- [ ] Exhaustive Owen  (requires owen version where the whole permutation tree is stored...)
- [ ] GenerativeMatrix (To be removed?, matbuilder is way better !)

New samplers : 

- [ ] SOT 
- [ ] Cascaded
- [ ] GBN
- [ ] HexTiles 
- [ ] Dyadic 
- [ ] MatBuilder 
- [ ] LatNetBuilder

## Evaluation metrics 

Discrepancies 

- [ ] Star Discrepancy (With N-D version ?)
- [X] L2
- [X] GL2
- [X] Centered L2 
- [X] Unanchored L2 
- [X] Symmetric L2 
- [X] Fast Bound on Star
- [X] Diaphony ???

Note: to be validated (against utk ?)

Distance : 

- [X] Spectrum
- [X] Radial Spectrums
- [X] PCF
- [ ] MinDist

Test : 

- [ ] Integration test 
    - Currently, ground thruth are evaluated with 16k Grid points...
    - What to do for ground thruth ?
- [ ] ZonePlate test

Exes :

- [ ] Output format = ?? Keep the same as UTK ? 
- [ ] Interface to run on directories : what directory structure ? 

## Functionnalities (to be discussed)

- [ ] Error reporting
    - Global Error stack ?
    - Global logger ? 
    - Local logger : Requires an additionnal parameter for each sampler...
- [ ] Change PRNG for Whitenoise
    - MT is slow... (And is not used in Owen scrambling for this exact reasong)
    - Use PCG : simple code and very fast
- [ ] Some Author code exit on error. 
    - Make them return some error instead so that this is recoverable ?



## Checks :

There are a few thing I've noticed, here is a (probably non-exhaustive) list of things to check

- [ ] Check parameter names and documentation in exe.
- [ ] CMake : proper CMake for dependancy handling
- [ ] LDBN : What is the target ? Points are weird...
- [ ] LutLDBN : Cmake fetch content / git submodule
- [ ] Sampler sampling 'random' number of points : AAPatterns, FastPoisson, Fattal. What to do ? 
- [ ] HexGrid : Top Row (y ~= 1) is probably missing, but UTK's algorithm yield 'weird' number of points
- [ ] DartThrowing/Projective Blue Noise : What to do when D >= 8 ???
- [ ] ProjectiveBlueNoise : to be thoroughly tested
- [ ] FastPoisson : Why are most methods commented in current UTK ? 
- [ ] Lloyd3D : to be tested. Also link with openMP instead of ptheads ? Move Delaunay_psm to 'externals/' ? 
- [ ] Step/SinglePeak : Link with OpenMP ? 
- [ ] Penrose : is magfactor to be parametrized ? 
- [ ] Check discrepancies : parallel computation yield differents results each time the function is called. It looks like 
      numerical innacuracy and thread ordering being different... But still to be checked !

## Python Binding :

- [X] Build system
- [X] Samplers (allmost all) 
    - [ ] Lloyd     (requires additionnal dependancies, waiting for proper CMake)
    - [X] Kronecker (Uses a global constant to store Alphas)
    - [X] Halton    (Uses a global constant to store Basis)
- [X] Discrepancies, Spectrum, PCF, ...
- [ ] Integration test
- [X] Scramblers

## Design :

- [ ] Check current interface. Is everything ok ? 

## Misct : 

- [ ] CMake precompiled header (for CLI11 ?)
- [ ] Create an actual linkable library with <double> samplers ? 
- [ ] data install path or included by samplers (or convert data to string, and parse it at runtime) ?
- [ ] Add parametrisable seed for FastPoisson
- [ ] Projective Blue Noise : Enhance projection method (not only axis based, MC evaluation,...)
- [ ] NRooks: Integer sampling 
- [ ] Add parametrization for openmp threads (Note: env variable OMP_NUM_THREADS could do the trick too)
- [ ] Adding a capacity to Pointset ? 
- [ ] Spectrum to image 