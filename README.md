平行化Black-Scholes Model
===
`gcc final.c -o final -lm`
`time ./final 100 1000000`  
第一個參數是分幾期 第二個參數是蒙地卡羅模擬次數
`lm`是link libm，有用到`<math.h>`的話要用
平行化後的檔案是`ppfinal_openmp.c`
slide : [連結](https://docs.google.com/presentation/d/1Mk6xiOcPyFfEpcK95i6YivoAR7tO4QDzGo8LA8WZXPo/edit?usp=sharing)  
## 20191208
- **add** copied `ppfinal.c` to `ppfinal_openmp.c`
- **add** `Makefile` for `ppfinal_openmp`
- **change** changed all `float` type variables into type of `double`
- **change** fixed some bugs in `ppfinal_openmp.c`

---
## 20191209
- **add** use omp to parallelize `ppfinal_openmp.c`
- **bug** generated random numbers are not welled distributed thus causing huge error

---
## 20191210
- **add** `ppfinal_openmp.cpp` which uses random number generator in c++ std library
- **change** `Makefile` now compiles `ppfinal_openmp.cpp` instead of `ppfinal_openmp.c`
- **change** ~~the RNG array now has a size of 4, implying that number of threads is bounded by 4~~
- **solved** solved a **HUGE** bug causing wrong result: `ppfinal_openmp.c:99`(966d2be): local variable `SA_P` should be declared as type `double` instead of `int` (while this does not solve the distribution problem of RNGs under multithreading environment)
- **change** minor updates to `ppfinal_openmp.cpp`

---
