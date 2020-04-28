# Lab2 Test Report

> by 奥力给队

## 一、实验概要

### 1.程序编写策略

本次实验我们使用了Java的**Netty**构建高性能服务器。核心策略是用NIO编程代替BIO编程。

#### Netty

传统的阻塞式IO效率不高，经常多线程一起阻塞。由此为了解决高并发问题，Netty框架使用了NIO。

<img src="Lab2 Test Report.assets/image-20200423170747610.png" alt="image-20200423170747610" style="zoom: 67%;" />

- Netty抽象出2组线程池
    - BossGroup 负责客户端连接
    - WorkerGroup 负责事件操作
- 他们都是**NioEventLoopGroup**
    - 每个NioEventLoopGroup相当于一个事件循环组
    - 每组有多个事件循环NioEventLoop
    - **NioEventLoop**表示一个**不断循环的执行处理任务**的线程
        - 每个NioEventLoop绑定了一个selector
- **BossGroup**的运行过程
    1. 轮询accept事件
    2. 处理accept事件，并与client建立连接
        - 生成**NioScocketChannel**，并将其注册到某个worker Group NIOEventLoop上的**selector**
    3. 处理任务队列里的任务
- **Worker Group的某个NIOEventLoop**执行过程
    1. 轮询read、write事件
    2. 在对应**NioScocketChannel**处理I/O事件
    3. 处理任务队列的任务，即runAllTask
- 每个Worker NIOEventLoop处理业务时，会使用pipeline(管道),pipeline中包含了channel,即通过pipeline可以获取到对应通道,管道中维护了很多的处理器

### 2.输入输出

```shell
#编译
>make
#运行 ./文件名 [--ip 本机ip（默认localhost）] [--port 绑定本机端口（默认8080）]
>./httpserver --ip 127.0.0.1 --port 8080
#运行日志会输出到httpserver所在目录下的log文件里
```

## 二、性能分析

### 1.实验环境

#### 环境一

**操作系统：**CentOS Linux release 7.7.1908 (Core) 

**内核：** Linux 3.10.0-1062.18.1.el7

**CPU：**一个CPU，单核，Intel(R) Xeon(R) Platinum 8163 CPU @ 2.50GHz

#### 环境二

**操作系统：**

**内核：**

**CPU：**

### 2.性能指标

1.每秒能处理多少个HTTP请求

2.服务器能并行建立多少个TCP连接

### 3.性能分析



## 三、心得体会

### 陈晨

职责：根据netty框架设计本次实验，post代码实现，get代码部分实现

我们学习到，用传统的BIO编程，性能一定不高，于是我们搜寻资料后，觉得采用NIO模式编程，在网上搜寻得到Netty框架很符合我们的需求，于是对我们而言，高并发的问题比较好解决（调用Netty框架）。

当然这并不是意味着我们没有考虑高并发的问题。Netty框架的设计较为复杂，我们需要仔细了解其设计模式，也就是它是如何由Java的NIO构造成框架的。经过半个月的学习，我们终于掌握了Netty的大致框架，动手到实现上。

在构建好Netty框架后，我们面临的问题是如何处理HTTP的request，以及返回符合实验规则的response。这涉及到2个方面的问题：

1. HTTP请求与相应的**数据结构**与**通信策略**
2. 如何使用Netty的HTTP通信API

在其中，我们面临的最大问题是**API的调用**以及**Java相对路径访问文件**。

API的问题主要是官方文档没有样例，在**《Netty实战》**有部分样例，以及网上样例鱼龙混杂，容易造成混淆。但最后结果不断的尝试，我们成功形成一套正式的自己的方法。

Java相对路径访问文件的问题就很让人无奈，经过不断的试错，最终得出结论：Java的FIle相对路径为**项目所在位置**的相对路径，而不是**.java**的相对路径。

很可惜，我们没来得及实现Proxy功能，这个应该更有趣些。

### 周大为

职责：负责编写http服务器get请求
体会：刚开始一直纠结在响应消息上，导致get请求没有得到实际突破。网上资源比较多，也容易
找不到头脑，后在和队友的讨论中，慢慢明白了其过程：获取url,对url进行解析，一般默认访问的是
index.html，在可能访问的是子目录下的文件时，要考虑可能访问的文件不存在或者当前目录下是有index.html
然后在返回成功的信息中加入了各种属性信息，比如对html,jpg,css等的访问。

### 贺飓兮

职责：

### 王林钦

职责：编写启动器、性能测试、整合报告

