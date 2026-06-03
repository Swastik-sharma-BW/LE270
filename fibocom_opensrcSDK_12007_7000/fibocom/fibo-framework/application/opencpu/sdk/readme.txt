
此为Linux下编译工具

1. 环境安装
	1）建议安装 Ubentu 16.04.02 OS版本

2. 编译
	1）source fibo_env     /* 配置编译环境变量 */
	2）在src和inc中分别添加代码和头文件，注意app的入口函数为 FIBO_CALLBACK_T* fibo_main(void)
	3）cout
	4）cmake ../build 
	5）make
	若编译OK，在out下会产生app.zip文件，使用aboot烧录此zip，即可将主固件和app
	一同烧录入模块中
