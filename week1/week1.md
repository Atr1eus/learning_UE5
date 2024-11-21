第一次编译和构建十分顺利，没有出过环境问题或者是其它报错（可能是之前使用了一段时间商城发行版，已经配置好了所有的运行环境）

第一次编译11小时，起床发现硬盘空间一比特不剩，于是删除了硬盘中留存的商城发行版，取消编译后再次编译，编译成功：

![image-20241119092404287](C:\Users\86158\AppData\Roaming\Typora\typora-user-images\image-20241119092404287.png)

删除了plugin/exprimental文件夹，结果生成的时候出现报错：

```txt
1>Unable to find plugin 'PythonScriptPlugin' (referenced via default plugins -> AnimationData.uplugin -> ControlRig.uplugin). Install it and try again, or remove it from the required plugin list.
```

查找后发现是experimental中的插件，而且再default设置中，由于不了解插件的特性，不知道是否需要重新setup或者编译，于是决定重装/(ㄒoㄒ)/~~

第二次编译时actions数目多了600，不知道是为什么。

![image-20241120085049197](C:\Users\86158\AppData\Roaming\Typora\typora-user-images\image-20241120085049197.png)

第二次编译成功，开始配置安卓打包环境。

按照官方文档设置和下载AS，配置NDK和SDK

新建项目week1，关闭live coding

稍微设置了一下场景，开始打包

![image-20241120171805771](C:\Users\86158\AppData\Roaming\Typora\typora-user-images\image-20241120171805771.png)

打包也花了几个小时

![image-20241120214209769](C:\Users\86158\AppData\Roaming\Typora\typora-user-images\image-20241120214209769.png)

结果打包失败unkonwn error

重新打包时，电脑突然多出一个新加卷Z，点进去是grandle

![image-20241120230523950](C:\Users\86158\AppData\Roaming\Typora\typora-user-images\image-20241120230523950.png)

然而什么都没改换成全局代理重新编译一次就成功了

![image-20241120231712040](C:\Users\86158\AppData\Roaming\Typora\typora-user-images\image-20241120231712040.png)

在移动平台上运行

![image-20241121085207068](C:\Users\86158\AppData\Roaming\Typora\typora-user-images\image-20241121085207068.png)

无法执行，可能是未勾选在apk中打包（因为使用的鸿蒙系统，无法识别googleplay所需的obb文件）

勾选后重新打包并运行：

![e49cc0510feafdbc8a415effa36ee5e6_720](D:\QQChat\Tencent Files\1582329292\nt_qq\nt_data\Pic\2024-11\Thumb\e49cc0510feafdbc8a415effa36ee5e6_720.jpg)