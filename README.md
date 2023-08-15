# GPTEfficiencyMeasurements
Repository to host code used to collect measurements on ChatGPT's ability to improve energy efficiency of code.
Each directory holds several files that correspond to different optimizations methods used in increasing efficiency.
Files labelled "simple" are baseline files with no optimizations.
Files with "Open" used OpenMP libraries and files with "SIMD" used Armv8 Neon SIMD libraries.
All code outside of the non-optimized code was produced by prompting ChatGpt to Optimize the original file for energy efficiency using the corresponding libraries.
