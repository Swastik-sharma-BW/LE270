使用方法：

1. APP1的代码中创建任务，调用APP2的启动函数：fibo_app_start

2. APP2 的入口函数为app_entry, 函数名不可改动；
   客户在APP2中，使用在APP1中已经封装提供给APP2使用的API接口，在APP2中使用上述API接口即可；
   
测试方法：
1. 划分分区
2. 编译底包
3. 实现APP1(封装给APP2的API，并调用启动APP2的函数)并编译
   测试编译指令(客户侧不用开启宏 -DAPP_DEMO_ENABLE=ON): cmake ../build -DAPP_DEMO_ENABLE=ON; make  
4. 实现APP2(调用APP1提供的API函数)
   测试编译指令(out目录下)：rm -rf * ; cmake ../build -DAPP2=ON; make
   
   