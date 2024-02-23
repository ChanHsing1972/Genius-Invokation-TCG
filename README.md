# **一个简陋的七圣召唤**  
*Created by Samuel Chan*  
## **基本信息**  
&emsp;&emsp;**平台**：Windows 11，使用 Visual Studio 2022 进行编译  
&emsp;&emsp;**C 语言环境**：gcc version 8.1.0 ( x86_64-posix-seh-rev0, Built by MinGW-W64 project ) ( mingw64 )  
&emsp;&emsp;**编译选项**：默认  
&emsp;&emsp;**使用的第三方库**：未使用第三方库  
&emsp;&emsp;**一点提示**：按下 F11 以获得最佳观感。建议使用**分辨率高于 1920 × 1400** 的显示屏。
 
## **游戏方式与内容**
&emsp;&emsp;用命令行实现了七圣召唤的基础功能。   
&emsp;&emsp;**基本规则**：充分利用行动点数和元素反应，打败对手所有角色，取得胜利。  
&emsp;&emsp;**交互方式**：按下相应字符即可进行操作，无需输入。提供五种操作，分别对应数字 1 - 5。界面如图。
<div align=middle>
<img src="界面设计 (1).jpg" width=320 height=auto>
<img src="界面设计 (2).jpg" width=320 height=auto>
</div>  

> &emsp;你可以采取以下行动：  
&emsp;1 - 更换角色：消耗 1 点行动点数，更换当前出战的角色。  
&emsp;2 - 普通攻击：消耗 2 点行动点数，造成普通伤害。  
&emsp;3 - 元素战技：消耗 3 点行动点数，造成较高伤害及元素附着。  
&emsp;4 - 结束回合：宣布结束本回合行动。  
&emsp;5 - 查看信息：查看各个角色的详细信息。  

&emsp;&emsp;注意，每次采取 1、2、3 三种行动后，会将行动权移交给对手。采取 “4 - 结束回合” 行动后，你宣告回合结束，将失去本回合的行动权。另外，不同于原作，在本游戏中，每一回合你均为先手。  
&emsp;&emsp;**角色设计**：提供三位角色，初始生命值均为 10，显示于卡片左上角。每位角色拥有相应的普通攻击及元素战技，可在游戏中输入 “5” 查看关于角色的具体介绍。  
&emsp;&emsp;**关卡设计**：提供一个关卡。在该关卡中，对手将充分利用其行动点数攻击你的角色。  
&emsp;&emsp;**元素设计**：三位角色对应元素分别为：绫华 => 冰元素，霄宫 => 火元素，钟离 => 岩元素。角色使用元素战技后，目标将附着对应的元素，附着元素种类显示于卡片右下角（注意：此处显示的并非角色自身元素属性）。本游戏提供冰、火、岩三种元素附着，持续时间均为一回合。其中，冰、火元素可发生元素反应 “融化”，并带来高额伤害增幅（+2 点伤害）。  
## **设计亮点**
1. 有 BGM……  
2. 投掷骰子时点数随机（范围：5 - 10），伴随数字快速变化效果，以模拟投掷过程。  
3. 当前出战角色将以卡牌突出形式显示。  
4. 实现了元素附着、元素反应及增伤效果。  
5. 提供战斗日志功能（可滚动）。  
6. 每位角色均配有简要介绍。  
7. 按下相应字符即可进行操作，无需输入。
8. ~~制作了原神启动的字符画。~~  

<div align=middle>
<img src="角色信息.jpg" width=640 height=auto>
</div>