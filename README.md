# 开源鸿蒙开发者大赛开发计划
## 项目介绍
由于整个小凌派的项目太大，不方便全部上传到github，这里上传自己修改的代码。
当前目录在小凌派RK2206开源项目的位置是`lockzhiner-rk2206-openharmony3.0lts/vendor/lockzhiner/rk2206/samples/`，使用的时候只需要把当前项目Clone到该目录下即可.
## 计划
本项目研发计划主要分几个阶段。

| 阶段 | 简介 |
| --- | --- |
| 第一阶段 | 环境搭建，测试样例。|
| 第二阶段 | 修改代码，尝试多个样例组合使用|
| 第三阶段 | 实践项目|
## 第一阶段
## 环境搭建
### 编译调试

#### 修改 BUILD.gn 文件

修改 `vendor/lockzhiner/rk2206/sample` 路径下 BUILD.gn 文件，指定目录下的BUILD.gn文件里的包名参与编译。例如`c3_e53_intelligent_street_lamp`

```r
"./c3_e53_intelligent_street_lamp:e53_isl_example",
```

修改 `device/lockzhiner/rk2206/sdk_liteos` 路径下 Makefile 文件，添加 `-l xxx` 参与编译。例如`e53_isl_example`

```r
hardware_LIBS = -lhal_iothardware -lhardware -le53_isl_example
```

```r
# hb路径设置第一次需要加-root
hb set -root <根目录>
# 选择开发版 
hb set
# 清除旧文件
hb clean
# 编译，加 -f 全部编译
hb build -f
```
### 样例运行
***2022年10月8日成功运行智慧路灯样例***
## 第二阶段
### 多个样例运行
1. 尝试同时使用两个样例：智慧路灯和智能手势

#### 10.16同时烧入两个样例成功
智能手势使用成功，在串口调试时同时出现智慧路灯和智能手势的调试信息。
