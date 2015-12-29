 - Processor: Core i7 4770K @ 4.0 GHz (3.8 GHz uncore)
 - Memory: 32 GB DDR3 @ 1866 MHz
 - Benchmark Size: 100 million digits

All times in seconds. All benchmarks are multi-threaded for 8 logical cores (if applicable).<br>
Timings include the base conversion (if applicable), but not the time needed to write the digits to disk.

|Number Factory 1.0:        |SSE4.1  |AVX     |AVX2    |
|---------------------------|-------:|-------:|-------:|
|Sqrt(2)                    |   2.763|   2.512|   1.584|
|e                          |   8.527|   7.372|   4.972|
|Pi - Machin (Original)     | 105.295|  92.696|  62.318|
|Pi - Machin (Takano)       | 110.988|  96.691|  65.030|
|Pi - Machin (Stormer)      | 100.702|  87.532|  59.118|
|Pi - Machin (Hwang)        | 108.101|  94.250|  63.582|
|Pi - Chudnovsky            |  26.575|  23.117|  15.512|
|Pi - Ramanujan             |  42.036|  37.286|  24.694|
|Pi - AGM                   |  96.778|  88.331|  55.918|
|Lemniscate - AGM           |  98.271|  89.615|  56.247|
|Gamma(1/4) - AGM           | 102.157|  93.604|  59.237|
|log(2) - Machin (Euler)    | 143.852| 125.821|  84.716|
|log(2) - 3 terms           |  77.005|  67.188|  45.726|
|log(2) - 4 terms           |  87.560|  76.024|  51.171|
|Zeta(3) - AZ1              | 145.414| 128.755|  84.581|
|Zeta(3) - AZ2              | 139.869| 123.681|  82.431|
|Zeta(5) - Broadhurst       |3585.454|3140.959|2047.464|
|Zeta(5) - Huvent           |2789.752|2456.438|1606.481|
|Catalan - Lupas            | 670.189| 589.218| 388.814|
|Catalan - Huvent           |1018.643| 892.831| 587.147|
|Catalan - Huvent (modified)| 778.982| 676.100| 445.974|

|                           |NF 1.0 (AVX2)|PiFast 4.3|QuickPi 4.5|TachusPi 0.9|y-cruncher v0.6.8|
|---------------------------|------------:|---------:|----------:|-----------:|----------------:|
|sqrt(2)                    |        1.584|      7.05|       2.25|            |            1.265|
|exp(1)                     |        4.972|     56.23|      22.00|            |            3.208|
|Pi - Machin (Original)     |       62.318|    654.62|     330.50|            |                 |
|Pi - Machin (Takano)       |       65.030|          |           |            |                 |
|Pi - Machin (Stormer)      |       59.118|    689.16|     327.85|            |                 |
|Pi - Machin (Hwang)        |       63.582|          |           |            |                 |
|Pi - Chudnovsky            |       15.512|    177.83|      86.86|      24.324|           10.250|
|Pi - Ramanujan             |       24.694|    256.17|     136.14|      33.763|           14.931|
|Pi - AGM                   |       55.918|          |     138.59|            |                 |
|Lemniscate - AGM           |       56.247|          |           |            |                 |
|Gamma(1/4) - AGM           |       59.237|          |           |            |                 |
|log(2) - Machin (Euler)    |       84.716|          |           |            |                 |
|log(2) - 3 terms           |       45.726|    509.13|     252.83|            |           26.043|
|log(2) - 4 terms           |       51.171|    592.70|     287.73|            |           29.527|
|Zeta(3) - AZ1              |       84.581|   1227.84|     504.83|            |           47.574|
|Zeta(3) - AZ2              |       82.431|   1335.35|     479.72|            |           44.980|
|Zeta(5) - Broadhurst       |     2047.464|  28301.54|           |            |                 |
|Zeta(5) - Huvent           |     1606.481|  25246.89|           |            |                 |
|Catalan - Lupas            |      388.814|   5664.93|    3386.20|            |          205.689|
|Catalan - Huvent           |      587.147|          |           |            |                 |
|Catalan - Huvent (modified)|      445.974|          |           |            |          222.457|
