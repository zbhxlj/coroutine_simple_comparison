分别对 **c++20 stackless_coroutine**        **ucontext**      **boost_fcontext**  切换速度进行了简陋测试

测试环境：

1. 系统环境：Linux arch 5.14.5-arch1-1 #1 SMP PREEMPT Thu, 16 Sep 2021 11:02:40 +0000 x86_64 GNU/Linux
2. cpu型号： Intel(R) Core(TM) i5-9300H CPU @ 2.40GHz
3. 编译环境：gcc version 11.1.0 (GCC)  x86_64-pc-linux-gnu (暂时不支持clang)  -O2 -std=c++20



环境准备：boost 库、googleTest、google benchmark(后两项可通过cmake自动下载)

步骤：

```shell
git clone https://github.com/zbhxlj/coroutine_simple_comparison.git
cd coroutine_simple_comparison
mkdir build && cd build
cmake .. 
make
./c++20_stackless 
./boost_ucontext
./boost_fcontext
```



代码来源：

1. boost_fcontext: 来自boost 库，头文件里删去了冗余的名称空间，和单独编译的boost_context进行链接（或者去boost源码中 src/asm 下找到自己平台对应的头文件里面那两个函数所对应的汇编文件，而不必编译整个库）
2. boost_ucontext: 自己利用ucontext api简单封装，因为 boost 库中也是对ucontext的封装，但耦合很重，为了测试方便，自己简单封装
3. c++20_stackless: < coroutine >  是c++20 新增头文件，但是g++暂不支持 generator ,  "generator.h" 来自于 franktea https://github.com/franktea/treehouse/blob/cb1e9851f04c4be9f35b4afe25830b6226f1e6d7/cmake%E7%AE%80%E4%BB%8B%E5%8F%8A%E8%87%AA%E5%8A%A8%E4%B8%8B%E8%BD%BD%E7%AC%AC%E4%B8%89%E6%96%B9%E5%BA%93%E7%9A%84%E7%94%A8%E6%B3%95/sfml_cmake/generator.h (本仓库的测试方式也基于他的测试代码)

测试结果：

| Benchmark      | Time   | CPU    | Iterations |
| -------------- | ------ | ------ | ---------- |
| boost_fcontext | 5.57ns | 5.56ns | 107960576  |
| cpp20          | 2.41ns | 2.40ns | 295078185  |
| boost_ucontext | 825ns  | 825ns  | 711493     |

结果分析：

1. ucontext 慢很多， 因为要保存信号掩码（需要系统调用）
2. 无栈协程不支持嵌套，类似于状态机状态转移 （https://zhuanlan.zhihu.com/p/347445164）, 不push、pop, 不破坏分支预测

