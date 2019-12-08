平行化Black-Scholes Model
===
`gcc final.c -o final -lm`
`time ./final 100 1000000`  
第一個參數是分幾期 第二個參數是蒙地卡羅模擬次數
`lm`是link libm，有用到`<math.h>`的話要用

平行化後的檔案是`ppfinal_openmp.c`

## 20191208
- **add** copied `ppfinal.c` to `ppfinal_openmp.c`
- **add** Makefile for `ppfinal_openmp`
- **change** changed all `float` type variables into type of `double`
- **change** fixed some bugs in `ppfinal_openmp.c`

---
