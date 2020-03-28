# Lab1 test report

> by 奥利给队

## 一、实验概要

此次实验为学习与探索**多线程编程技术**，以陈果老师所讲的多线程编程的**三个基本策略**为核心，解数独题目为载体，进行多线程编程技术的探索。

### 1.程序编写策略

根据陈果老师上课所讲的多线程编写的三个基本策略，我们最终采取了如下组合进行编程实现：

1. 动态分配任务

    > 每个任务从一个输入队列里取元素执行任务

2. 每个线程做相同任务

    > 统一调用`solve(*Sudoku)`函数进行解题

3. 创建线程池

    > 线程池中部分代码参考老师的[ConcurrentWget-ThreadPool.c](https://github.com/1989chenguo/CloudComputingSlidesAndCodes/blob/master/CodeInSlides/chapter4/ConcurrentWget-ThreadPool.c)。

### 2.输入与输出

```shell
> make
> ./sudoku_slove n
./test1
./test2
^C

说明如下：
make后会生成sudoku_slove的二进制文件
运行sudoku_slove的参数n为创建线程池大小
之后是输入程序运行的文件路径名
运行后答案会输出到answer文件里
```

## 二、性能分析

### 1.实验环境



### 2.性能指标



### 3.性能测试



#### 1）线程池大小对程序性能分析

#### 2）数独量对程序性能分析

### 4.总结分析

## 三、心得体会

### 陈晨

负责文件输入与线程池创建

### 周大为

负责文件输入与结果测试

### 王林钦

负责数据结构构建与结果顺序输出

### 贺飓兮

负责解数独算法实现与结果测试