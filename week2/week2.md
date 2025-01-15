# 第二周课后作业以及学习笔记

[TOC]



## 任务要求

基于ue5官方的First Person模板，实现一个游戏demo，本课程作业为实现以下功能： 

物件规则： 

1. 射击命中方块，获得积分X分 (clear)

2. 子弹命中方块后销毁 (clear)

3. 方块被子弹命中后，缩放为Y倍，再次被命中后销毁 (clear)

   

游戏流程：

1. 游戏开始时随机N个方块成为“重要目标”，射击命中后获得双倍积分 (clear)
2. 游戏开始后限时T秒，时间到后游戏结算，打印日志输出每个玩家获得的积分和所有玩家获得的总积分(not yet)  



附加题，Nice to have 

1.利用UMG制作结算UI替代日志打印 (in plan)

2.支持多人联机(no plan)



## 内容实现

### Gamemode

gamemode的问题一直很困扰，内容填写不当很容易使游戏崩溃

解决了游戏限时问题，写一个timer函数，20秒结束后触发结束函数，打印角色分数和总分数。

解决了gamereset问题

现在可以进行多轮游戏，每次重置方块和玩家，但不重置总分。

### 方块类

事实上方块类只做了一半，没有对其进行物理上的设定和修改，出现和摆放也是纯手动摆的。

## Bonus

### 一些问题

**UE5显示:Video memory has been exhausted**

问题描述：会造成editor卡顿，进入运行模式后一开枪就会很卡不

解决方案：Edit → Project settings → Platforms → Windows → Default RHI设置为DirectX 11即可。

参考连接：[UE5显示:Video memory has been exhausted - 哔哩哔哩 (bilibili.com)](https://www.bilibili.com/opus/792421401893011472#:~:text=解决%3AEdit → Project settings → Platforms,→ Windows → Default RHI设置为DirectX 11即可。 加载中…)



但是更改为directx11后材质却不能正确加载，有待继续研究。



### 知识点

#### spawn（实例化）

动态生成对象

**常见的 Spawn 对象类型**

1. **Actors**
   大部分需要生成的对象，比如角色、拾取物等，都是继承自 `AActor` 的类。
2. **Components**
   比如粒子效果、声音等子对象，可以通过 `AttachToComponent` 挂载到已存在的对象上。
3. **Pawn/Character**
   用于生成玩家或 AI 控制的角色。
4. **Effects（特效）**
   用于生成动态效果，比如爆炸、烟雾等。

#### BeginPlay VS StartPlay

> BeginPlay 是在打开的关卡开始时调用的事件。Start Play 是一个您可以使用 GameMode 调用的函数，从它所说的“Trasitions to WaitingToStart and calls BeginPlay on actors”来看，我想说它可能就是这样做的。希望这有帮助。

以上内容摘自epic官方社区。

#### GameState

管理客户端信息，而具体的游戏实现和流程还在gamemode里。

字面意思，Game State 就是指游戏状态。它管理了所有已连接的客户端，并且实时追踪游戏层面的属性并把它们分发给远程客户端。有别于Play State，GS（GameState）主要是负责游戏全局属性，比如5V5Moba游戏中的红蓝双方防御塔的剩余数量，游戏当前进行的时间，大小龙击杀的情况，红蓝阵营野怪刷新情况等等。而PS（Player State）则是记录单个玩家的属性和状态，比如补了多少刀，出了什么状态，身上有多少钱，技能冷却时间等等。

[Unreal Engine的Gameplay框架和重点 - 知乎 (zhihu.com)](https://zhuanlan.zhihu.com/p/612837045)

### 3C

Character，Control，Camera





### Component

Actor可以说是由Component组成的，所以Component其实是我们[对象树](https://zhida.zhihu.com/search?content_id=1514500&content_type=Article&match_order=1&q=对象树&zhida_source=entity)里最底层的员工了。在UE里，Component表达的是“功能”的概念。比如说你要实现一个可以响应的WASD移动的功能，或者是VR里抓取的功能，甚至是嵌套另一个Actor的功能，这些都是一个个组件。正确理解“功能”和“游戏业务逻辑”的区分是理解Component的关键要点。
所以我们在这一个层级上要编写的逻辑，是实现一个个**“与特定游戏无关”**的功能。理想情况下，等你的一个游戏完成，你那些已经实现完成的Components是可以无痛迁移到下一个游戏中用的。换言之，一旦你发现你在Component中含有游戏的业务逻辑代码，这就是所谓的“Bad Smell”了，要警惕游戏架构是否恰当，是否没有很清晰的概念划分。

[《InsideUE4》GamePlay架构（四）Pawn - 知乎 (zhihu.com)](https://zhuanlan.zhihu.com/p/23321666?refer=insideue4)