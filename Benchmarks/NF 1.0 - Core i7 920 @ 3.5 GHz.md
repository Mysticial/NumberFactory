 - Processor: Core i7 920 @ 3.5 GHz
 - Memory: 12 GB DDR3 @ 1333 MHz
 - Benchmark Size: 100 million digits

All times in seconds. All benchmarks are multi-threaded for 8 logical cores (if applicable).<br>
Timings include the base conversion (if applicable), but not the time needed to write the digits to disk.

|                           |NF 1.0 (SSE4.1)|PiFast 4.3|QuickPi 4.5|TachusPi 0.9|y-cruncher v0.6.9|
|---------------------------|--------------:|---------:|----------:|-----------:|----------------:|
|sqrt(2)                    |          3.701|      9.94|       2.92|            |            3.583|
|exp(1)                     |         11.425|     78.94|      29.19|            |            9.252|
|Pi - Machin (Original)     |        141.735|    925.79|     440.44|            |                 |
|Pi - Machin (Takano)       |        150.168|          |           |            |                 |
|Pi - Machin (Stormer)      |        136.303|    977.05|     436.79|            |                 |
|Pi - Machin (Hwang)        |        149.565|          |           |            |                 |
|Pi - Chudnovsky            |         36.102|    255.43|     116.72|      35.977|           29.895|
|Pi - Ramanujan             |         57.310|    366.67|     181.73|      50.568|           44.415|
|Pi - AGM                   |        128.976|          |     180.90|            |                 |
|Lemniscate - AGM           |        130.315|          |           |            |                 |
|Gamma(1/4) - AGM           |        136.353|          |           |            |                 |
|log(2) - Machin (Euler)    |        193.854|          |           |            |                 |
|log(2) - 3 terms           |        104.584|    723.37|     337.32|            |           81.376|
|log(2) - 4 terms           |        118.524|    840.84|     383.56|            |           92.229|
|Zeta(3) - AZ1              |        196.879|   1718.00|     627.22|            |          146.187|
|Zeta(3) - AZ2              |        190.730|   1865.83|     650.80|            |          139.155|
|Zeta(5) - Broadhurst       |       4857.954|  40260.09|           |            |                 |
|Zeta(5) - Huvent           |       3778.961|  35996.35|           |            |                 |
|Catalan - Lupas            |        905.851|   8024.95|    4478.26|            |          635.725|
|Catalan - Huvent           |       1378.310|          |           |            |                 |
|Catalan - Huvent (modified)|       1053.099|          |           |            |          715.129|
