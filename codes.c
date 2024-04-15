/**************************
 * Created by Samuel Chan *
 **************************/

#define _CRT_SECURE_NO_WARNINGS 1
#define NUM_OF_CH 3
#define PUGONG 0
#define ZHANJI 1
#define OUTMSG out_msg + (temp++)
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <windows.h>
#include <mmsystem.h>
#include <stdbool.h>
#include "header.h"
#pragma comment(lib,"Winmm.lib")

struct Info
{
  int dice;                        // 骰子点数
  int dead;                        // 阵亡角色数
  int onstage;                     // 出场角色编号（0 - 神里绫华，1 - 宵宫，2 - 钟离）
  int hp[NUM_OF_CH];               // 各角色生命值
  int elem_code[NUM_OF_CH + 1];    // 附着元素的代码（0 - 无，1 - 冰，2 - 火，3 - 岩）
  char element[NUM_OF_CH + 1][10]; // 元素附着
  char name[NUM_OF_CH + 1][100];   // 角色名
};

struct Info ply = { 0, 0, -1, { 0 }, { 0 }, { "  ", "  ", "  " }, { "绫华", "霄宫", "钟离" } }; // ply => player
struct Info cpt = { 0, 0, -1, { 0 }, { 0 }, { "  ", "  ", "  " }, { "绫华", "霄宫", "钟离" } }; // cpt => computer

int len, in_msg = 0;   // len: 选择的角色数量，in_msg: 战斗日志的编号
int ch[NUM_OF_CH + 1]; // ch[]: 存储角色
char msg[500][1000];   // msg => message, msg[][]: 存储战斗日志

bool flag_ply_end_round = FALSE;  // flag_ply_end_round: 我方结束回合后，失去行动权
bool flag_cpt_end_round = FALSE;  // flag_cpt_end_round: 对方结束回合后，失去行动权
bool flag_ch_to_begin = FALSE;    // flag_ch_to_begin: “选择出场角色” 只在首次进入 ActionStage 时执行
bool flag_skip_printact = FALSE;  // flag_skip_printact: 跳过 PrintAct 函数
bool flag_reaction = FALSE;       // flag_reaction: 是否发生元素反应

int main()
{
  BGM1();
  printf("\n\n        提示：\
            \n        · 按下 F11 以获得最佳观感。\
            \n        · 建议使用分辨率高于 1920 * 1400 的显示屏。\
          \n\n        加载中...");
  Sleep(4000); system("cls"); Sleep(500);
  PrintYuan();

  srand((unsigned int)time);
  for (int i = 0; i < NUM_OF_CH; i++)
    ply.hp[i] = cpt.hp[i] = 10;

  BGM2();
  BeforeGame();
  DuringGame();
   
  return 0;
}
void BeforeGame()
{
  int cmd_ch;  // 选择角色
  char cmd_yn; // yes or no
  printf("\n\n          WELCOME TO GENIUS INVOKATION TCG.\
            \n          欢迎来到 Gayshin Impart 七圣召唤。\
          \n\n          从列表中选择 1 - 3 位角色：\
            \n          ( 考虑到游戏平衡性，建议全选 )\
          \n\n          1 - 神里绫华          （冰）\
            \n          2 - 宵宫              （火）\
            \n          3 - 钟离              （岩）\
            \n          4 -（我是来撑场面的）\
            \n          5 -（我也是来撑场面的）\
            \n          6 -（我还是来撑场面的）\
            \n          ...\
          \n\n          你的选择(e.g.\"1 2 3\\n\")：");

  // 选定牌组：从三个角色中选择至少一个角色，至多三个角色
  while (1)
  { 
    int i = 1;

    scanf("%d", &cmd_ch);
    printf("          确认选择：%s", ply.name[cmd_ch - 1]);
    ch[0] = cmd_ch - 1; len = 1;

    while (getchar() != '\n')
    {
      scanf("%d", &cmd_ch);
      printf(" & %s", ply.name[cmd_ch - 1]);
      ch[i++] = cmd_ch - 1; len++;
    }
    printf("？(Y/N)");

    cmd_yn = getch();
    if ((cmd_yn == 'Y') || (cmd_yn == 'y'))
    {
      printf("\n          加载中...");
      Sleep(500); system("cls"); break;
    }
    else if ((cmd_yn == 'N') || (cmd_yn == 'n'))
    {
      printf("\n\n          重新选择：");
      continue;
    }
  }

   // 选定关卡：只有一个关卡
   int cmd_lvl;
   printf("\n\n          WELCOME TO GENIUS INVOKATION TCG.\
             \n          欢迎来到 Gayshin Impart 七圣召唤。\
           \n\n          选择关卡（没得选）：\
           \n\n          +---+  +---+  +---+  +---+  +---+  \
             \n          | 1 |  |XXX|  |XXX|  |XXX|  |XXX|  \
             \n          +---+  +---+  +---+  +---+  +---+  \
           \n\n          你的选择？");
   getch();
   printf("\n          加载中...");
   Sleep(500); system("cls");
}
int DuringGame()
{
  while (1)
  {
    // 投掷阶段：通过投掷普通色子确定行动点数
    PrintInterface(1); Sleep(500);
    for (int i = 0; i < 50; i++) // 骰子快速变化效果
    {
      ply.dice = rand() % 6 + 5;
      cpt.dice = rand() % 6 + 5;
      printf("\n\n          掷出骰子，骰子点数为：%d", ply.dice); Sleep(20);
      PrintInterface(1);
    }
    printf("  \n\n          掷出骰子，骰子点数为：%d", ply.dice); Sleep(1000);
    printf("    \n          故本回合中，你的行动点数为 %d。", ply.dice); Sleep(500);
    printf("  \n\n          对手掷出骰子，骰子点数为：%d", cpt.dice); Sleep(500);
    printf("    \n          故本回合中，对手的行动点数为 %d。", cpt.dice); Sleep(500);
    printf("  \n\n          按任意键继续。");
    getch();

    // ply.dice = 10;
    // cpt.dice = 10;

    // 行动阶段：通过消耗行动点数完成普通攻击，元素战技，替换出战角色等动作（可以利用元素反应）
    flag_ply_end_round = FALSE; // flag_ply_end_round: 我方结束回合后，失去行动权。此时为关闭状态。
    flag_cpt_end_round = FALSE; // flag_cpt_end_round: 对方结束回合后，失去行动权。此时为关闭状态。
    PrintInterface(2); Sleep(500);

    ActionStage();

    printf("\n          按任意键继续。");
    getch();

    // 结束阶段：如果分出了胜负（有一方所有角色全部倒下），那么显示胜负，否则回到投掷阶段
    for (int i = 0; i < len; i++)
    {
      ply.element[i][0] = ply.element[i][1] = cpt.element[i][0] = cpt.element[i][1] = ' ';
      ply.elem_code[i] = cpt.elem_code[i] = 0;
    }
    PrintInterface(3); Sleep(500);

    ply.dead = cpt.dead = 0;

    for (int i = 0; i < len; i++)
    {
      if (ply.hp[i] == 0) ply.dead++;
      if (cpt.hp[i] == 0) cpt.dead++;
    }
    if (ply.dead == len)
    {
      printf("\n\n          输麻了\n\n"); return 0;
      for (int i = 0; i <= 99; i++)
      {
        printf("\n          ");
        for (int j = 0; j <= 15; j++)
        {
          printf("输 ");
          Sleep(50);
        }
      }
    }
    else if (cpt.dead == len)
    {
      printf("\n\n          赢麻了！！！\n\n"); return 0;
      for (int i = 0; i <= 99; i++)
      {
        printf("\n          ");
        for (int j = 0; j <= 15; j++)
        {
          printf("赢 ");
          Sleep(50);
        }
      }
    }
    else
    {
      printf("\n\n          正在加载下一回合...");
      Sleep(500); continue;
    }
  }
}
void ActionStage()
{
  // P1 - 选择出场角色
  int cmd_onstage;

  if (flag_ch_to_begin == FALSE) // flag_ch_to_begin: “选择出场角色” 只在首次进入 ActionStage 时执行
  {
    printf("\n\n          |  ");
    for (int i = 0; i < len; i++)
      printf("%d - %s  |  ", ch[i] + 1, ply.name[ch[i]]);
    printf("\n          选择一位出场角色。");
    cmd_onstage = getch() - '0';

    ply.onstage = cmd_onstage - 1;
    cpt.onstage = (ply.onstage + 1) % 3;
    sprintf(msg[in_msg++], "[ 你 指定出场角色： %s。 ]", ply.name[ply.onstage]);
    sprintf(msg[in_msg++], "[ 对手 指定出场角色：%s。 ]", cpt.name[cpt.onstage]);
    ChangeCh(1, cpt.onstage, 2);
    printf("\n\n          %s", msg[in_msg - 2]);
    printf("  \n          %s", msg[in_msg - 1]);
  }
  flag_ch_to_begin = TRUE;

  // P2 - 开始行动
  int cmd_act, cmd_act2;

  if(flag_skip_printact == FALSE) // flag_skip_printact: 跳过 PrintAct 函数
    PrintAct(); 
  flag_skip_printact = FALSE;

  while (flag_ply_end_round == FALSE)
  {
    printf("\n          按下相应键以执行操作。");
    cmd_act = getch() - '0';
    switch (cmd_act)
    {
    case 1:
      printf(" \n          |  ");
      for (int i = 0; i < len; i++)
        printf("%d - %s  |  ", ch[i] + 1, ply.name[ch[i]]);
      printf("\n          更换出战角色为"); break;
    case 2: 
      printf("\n          将消耗 2 点行动点数，按 1 确认"); break;
    case 3: 
      printf("\n          将消耗 3 点行动点数，按 1 确认"); break;
    case 4: 
      printf("\n          按 1 确认"); break;
    case 5:
      printf("\n          |  ");
      for (int i = 0; i < len; i++)
        printf("%d - %s  |  ", ch[i] + 1, ply.name[ch[i]]);
      printf("\n          选择角色查看信息"); break;
    default: break;
    }
    printf("（按 0 撤销）");
    cmd_act2 = getch() - '0';

    if (cmd_act2 == 0) continue;
    else
    {
      switch (cmd_act)
      {
      case 1: 
        IsValidChange(cmd_act2); 
        if (flag_cpt_end_round == FALSE)
          printf("\n          [ 等待对手行动... ]");
        Rival(); break;
      case 2: 
        Attack(PUGONG); break;
      case 3: 
        Attack(ZHANJI); break;
      case 4: 
        sprintf(msg[in_msg++], "[ 你 宣布结束回合。 ]");
        PrintInterface(2); PrintAct();
        printf("\n          %s", msg[in_msg - 1]);
        if(flag_cpt_end_round == FALSE)
          printf("\n          [ 等待对手行动... ]");
        flag_ply_end_round = TRUE;
        Rival(); break;
      case 5:
        PrintChInfo(cmd_act2);
      default: break;
      }
    }
    if (flag_ply_end_round == TRUE) break;
    if (flag_cpt_end_round == TRUE) continue;
  }
}
void ChangeCh(int who, int index, int stage)
{
  if (who == 0) ply.onstage = index;
  if (who == 1) cpt.onstage = index;
  PrintInterface(stage);
}
int IsValidChange(int cmd_act2)
{
  int temp;

  if (ply.hp[cmd_act2 - 1] == 0)
    printf("\n\n          [ 该角色已倒下，无法更换。 ]");
  else if (ply.dice < 1)
    printf("\n\n          [ 行动点数不足，无法更换。 ]");
  else if (cmd_act2 - 1 == ply.onstage)
    printf("\n\n          [ 你指定的角色与当前出战角色相同，无法更换。 ]");
  else
  {
    sprintf(msg[in_msg++],"[ 你 更换出战角色为%s。 ]", ply.name[cmd_act2 - 1]);
    ply.dice--; ChangeCh(0, cmd_act2 - 1, 2); PrintAct();
    printf("\n          [ 你 更换出战角色为%s。 ]", ply.name[cmd_act2 - 1]);
    return 0;
  }
  flag_skip_printact = TRUE;
  ActionStage();
}
void DieAndChange()
{
  ply.dead = cpt.dead = 0;

  sprintf(msg[in_msg++], "[ 你 的%s已倒下。  ]", ply.name[ply.onstage]);
  PrintInterface(2); PrintAct();
  printf("\n          %s\n          %s", msg[in_msg - 2], msg[in_msg - 1]);

  for (int i = 0; i < len; i++)
  {
    if (ply.hp[i] == 0) ply.dead++;
    if (cpt.hp[i] == 0) cpt.dead++;
  }
  if (ply.dead == len)
  {
    PrintInterface(3); Sleep(500);
    printf("\n\n          输麻了\n\n");
    for (int i = 0; i <= 99; i++)
    {
      printf("\n          ");
      for (int j = 0; j <= 15; j++)
      {
        printf("输 ");
        Sleep(50);
      }
    } exit(0);
  }
  else if (cpt.dead == len)
  {
    PrintInterface(3); Sleep(500);
    printf("\n\n          赢麻了！！！\n\n"); 
    for (int i = 0; i <= 99; i++)
    {
      printf("\n          ");
      for (int j = 0; j <= 15; j++)
      {
        printf("赢 ");
        Sleep(50);
      }
    } exit(0);
  }

  printf("\n          [ 重新选择一个出战角色以继续行动。 ]");
  printf("\n\n          |  ");
  for (int i = 0; i < len; i++)
    printf("%d - %s  |  ", ch[i] + 1, ply.name[ch[i]]);

  int temp;
  while (1)
  {
    printf("\n          更换出战角色为（此次更换不消耗行动点数）");
    temp = getch() - '0';
    ply.dice++;
    if (ply.hp[temp - 1] == 0)
    {
      printf("\n          [ 该角色已倒下。 ]"); continue;
    }
    IsValidChange(temp); break;
  }
}
int Attack(int para)
{
  int pnt, dmg; // pnt => point, dmg => damage
  
  if (para == PUGONG)
  {
    pnt = 2; dmg = 2;
  }
  else if (para == ZHANJI)
  {
    pnt = 3; dmg = 4;
  }

  if (ply.dice < pnt)
    printf("\n          [ 行动点数不足。 ]");
  else if (ply.dice >= pnt)
  {
    ply.dice -= pnt;

    flag_reaction = FALSE;
    if ((cpt.elem_code[cpt.onstage] == 2 && ply.onstage == 0) \
      || (cpt.elem_code[cpt.onstage] == 1 && ply.onstage == 1))
      flag_reaction = TRUE;

    if (cpt.hp[cpt.onstage] > dmg)
    {
      cpt.hp[cpt.onstage] -= dmg;
      if (para == PUGONG)
      {
        sprintf(msg[in_msg++],"[ 你 的%s使用 普通攻击 对 对手的%s造成 2 点伤害。 ]", ply.name[ply.onstage], cpt.name[cpt.onstage]);
        PrintInterface(2); PrintAct();
        printf("\n          %s", msg[in_msg - 1]);
      }
      else if (para == ZHANJI)
      {
        switch (ply.onstage + 1)
        {
        case 1:
          sprintf(msg[in_msg++], "[ 你 的绫华使用 神里流·冰华 对 对手的%s造成 4 点伤害。 ]", cpt.name[cpt.onstage]);
          sprintf(cpt.element[cpt.onstage], "冰");
          cpt.elem_code[cpt.onstage] = 1; break;
        case 2:
          sprintf(msg[in_msg++], "[ 你 的霄宫使用 焰硝庭火舞 对 对手的%s造成 4 点伤害。 ]", cpt.name[cpt.onstage]);
          sprintf(cpt.element[cpt.onstage], "火");
          cpt.elem_code[cpt.onstage] = 2; break;
        case 3:
          sprintf(msg[in_msg++], "[ 你 的钟离使用 地心 对 对手的%s造成 4 点伤害。 ]", cpt.name[cpt.onstage]);
          sprintf(cpt.element[cpt.onstage], "岩");
          cpt.elem_code[cpt.onstage] = 3; break;
        default: break;
        }
        PrintInterface(2); PrintAct();
        printf("\n          %s", msg[in_msg - 1]);

        if (flag_reaction == TRUE)
        {
          if (cpt.hp[cpt.onstage] > 2) cpt.hp[cpt.onstage] -= 2;
          else cpt.hp[cpt.onstage] = 0;
          sprintf(msg[in_msg++], "[ 你 触发融化反应，伤害 +2。 ]");
          PrintInterface(2); PrintAct();
          printf("\n          %s", msg[in_msg - 2]);
          printf("\n          %s", msg[in_msg - 1]);
        }
      }
    }

    else if (cpt.hp[cpt.onstage] <= dmg)
    {
      if (flag_reaction == TRUE)
        sprintf(msg[in_msg++], "[ 你 触发融化反应，伤害 +2。 ]");
      sprintf(msg[in_msg++], "[ 对手 的%s已倒下。 ]", cpt.name[cpt.onstage]);
      cpt.hp[cpt.onstage] = 0;
      PrintInterface(2); PrintAct();
      if (flag_reaction == TRUE)
        printf("\n          %s", msg[in_msg - 2]);
      printf("\n          %s", msg[in_msg - 1]); Sleep(1500);
    }

    if (ply.hp[0] == 0 && ply.hp[1] == 0 && ply.hp[2] == 0) return 0;
    if (flag_cpt_end_round == FALSE)
      printf("\n          [ 等待对手行动... ]");
    Rival();
  }
}
int Rival()
{
  // 首先判断对手角色是否倒下，是否需要更换
  if (cpt.hp[cpt.onstage] == 0)
  {
    for (int i = 0; i < len; i++)
    {
      if (cpt.hp[i] != 0)
      {
        cpt.onstage = i; Sleep(2000);
        sprintf(msg[in_msg++], "[ 对手 更换出场角色为%s。 ]", cpt.name[cpt.onstage]);
        PrintInterface(2); PrintAct();
        printf("\n          %s", msg[in_msg - 1]);
        break;
      }
    }
  }

  if (cpt.hp[0] == 0 && cpt.hp[1] == 0 && cpt.hp[2] == 0)
  {
    PrintInterface(3); Sleep(500);
    printf("\n\n          赢麻了！！！\n\n"); 
    for (int i = 0; i <= 99; i++)
    {
      printf("\n          ");
      for (int j = 0; j <= 15; j++)
      {
        printf("赢 ");
        Sleep(50);
      }
    } exit(0);
  }

  // 对手牌组逻辑
  while (flag_cpt_end_round == FALSE)
  {
    Sleep(2000);

    if (cpt.dice > 2)
    {
      flag_reaction = FALSE;
      if ((ply.elem_code[ply.onstage] == 1 && cpt.onstage == 1) \
        || (ply.elem_code[ply.onstage] == 2 && cpt.onstage == 0))
        flag_reaction = TRUE;

      cpt.dice -= 3;
      switch (cpt.onstage + 1)
      {
      case 1: 
        sprintf(msg[in_msg++], "[ 对手 的绫华使用 神里流·冰华 对 你 的%s造成 4 点伤害。 ]", ply.name[ply.onstage]); 
        ply.elem_code[ply.onstage] = 1;
        sprintf(ply.element[ply.onstage], "冰"); break;
      case 2: 
        sprintf(msg[in_msg++], "[ 对手 的霄宫使用 焰硝庭火舞 对 你 的%s造成 4 点伤害。 ]", ply.name[ply.onstage]);
        ply.elem_code[ply.onstage] = 2;
        sprintf(ply.element[ply.onstage], "火"); break;
      case 3: 
        sprintf(msg[in_msg++], "[ 对手 的钟离使用 地心 对 你 的%s造成 4 点伤害。 ]", ply.name[ply.onstage]);
        ply.elem_code[ply.onstage] = 3;
        sprintf(ply.element[ply.onstage], "岩"); break;
      default: break;
      }

      if (flag_reaction == FALSE)
      {
        if(ply.hp[ply.onstage] > 4)
        {
          ply.hp[ply.onstage] -= 4;
          PrintInterface(2); PrintAct();
          printf("\n          %s", msg[in_msg - 1]);
        }
        else if (ply.hp[ply.onstage] <= 4)
        {
          ply.hp[ply.onstage] = 0;
          Sleep(1500); DieAndChange();
        }
      }
      else if (flag_reaction == TRUE)
      {
        if (ply.hp[ply.onstage] > 6)
        {
          ply.hp[ply.onstage] -= 4;
          PrintInterface(2); PrintAct();
          printf("\n          %s", msg[in_msg - 1]);
        }
        else if (ply.hp[ply.onstage] <= 6)
        {
          ply.hp[ply.onstage] = 0;
          sprintf(msg[in_msg++], "[ 对手 触发融化反应，伤害 +2。 ]");
          PrintInterface(2); PrintAct();
          printf("\n          %s", msg[in_msg - 2]);
          printf("\n          %s", msg[in_msg - 1]);
          Sleep(1500); DieAndChange();
        }
      }
    }

    else if (cpt.dice == 2)
    {
      cpt.dice -= 2;
      sprintf(msg[in_msg++], "[ 对手 的%s使用 普通攻击 对 你 的%s造成 2 点伤害。 ]", cpt.name[cpt.onstage], ply.name[ply.onstage]);

      if (ply.hp[ply.onstage] > 2)
      {
        ply.hp[ply.onstage] -= 2;
        PrintInterface(2); PrintAct();
        printf("\n          %s", msg[in_msg - 1]);
      }
      else
      {
        ply.hp[ply.onstage] = 0;
        DieAndChange();
      }
    }

    else if (cpt.dice <= 2)
    {
      sprintf(msg[in_msg++], "[ 对手 宣布结束回合。 ]");
      PrintInterface(2); PrintAct();
      printf("\n          %s", msg[in_msg - 2]);
      printf("\n          %s", msg[in_msg - 1]);
      flag_cpt_end_round = TRUE;
      flag_skip_printact = TRUE;
      ActionStage(); break;
    }

    if (flag_ply_end_round == TRUE) continue;
    else break;
  }
}
void PrintInterface(int stage)// 打印界面
{
  int out_msg = 0, temp = 0;

  if (in_msg > 21)
    out_msg = out_msg + in_msg - 21;
  system("cls");
  printf("\n\n          GENIUS INVOKATION TCG\
            \n          七圣召唤\n\
            \n          你                                         对手        战斗日志\n");
  for (int i = 0; i < len; i++)
  {
    if (ply.onstage != i)
    {
      if (cpt.onstage != (i + 1) % 3)
      {
        printf("\n          +--------+                           +--------+        %s", msg[OUTMSG]);
        printf("\n          |%.2d      |                           |%.2d      |        %s", ply.hp[i], cpt.hp[(i + 1) % 3], msg[OUTMSG]);
        printf("\n          |        |                           |        |        %s", msg[OUTMSG]);
        printf("\n          |  %s  |                           |  %s  |        %s", ply.name[ch[i]], cpt.name[(ch[i] + 1) % 3], msg[OUTMSG]);
        printf("\n          |        |                           |        |        %s", msg[OUTMSG]);
        printf("\n          |      %s|                           |      %s|        %s", ply.element[i], cpt.element[(i + 1) % 3], msg[OUTMSG]);
        printf("\n          +--------+                           +--------+        %s", msg[OUTMSG]);
      }
      else if(cpt.onstage == (i + 1) % 3)
      {
        printf("\n          +--------+                        +--------+           %s", msg[OUTMSG]);
        printf("\n          |%.2d      |                        |%.2d      |           %s", ply.hp[i], cpt.hp[(i + 1) % 3], msg[OUTMSG]);
        printf("\n          |        |                        |        |           %s", msg[OUTMSG]);
        printf("\n          |  %s  |                        |  %s  |           %s", ply.name[ch[i]], cpt.name[(ch[i] + 1) % 3], msg[OUTMSG]);
        printf("\n          |        |                        |        |           %s", msg[OUTMSG]);
        printf("\n          |      %s|                        |      %s|           %s", ply.element[i], cpt.element[(i + 1) % 3], msg[OUTMSG]);
        printf("\n          +--------+                        +--------+           %s", msg[OUTMSG]);
      }
    }

    else if (ply.onstage == i)
    {
      if (cpt.onstage != (i + 1) % 3)
      {
        printf("\n             +--------+                        +--------+        %s", msg[OUTMSG]);
        printf("\n             |%.2d      |                        |%.2d      |        %s", ply.hp[i], cpt.hp[(i + 1) % 3], msg[OUTMSG]);
        printf("\n             |        |                        |        |        %s", msg[OUTMSG]);
        printf("\n             |  %s  |                        |  %s  |        %s", ply.name[ch[i]], cpt.name[(ch[i] + 1) % 3], msg[OUTMSG]);
        printf("\n             |        |                        |        |        %s", msg[OUTMSG]);
        printf("\n             |      %s|                        |      %s|        %s", ply.element[i], cpt.element[(i + 1) % 3], msg[OUTMSG]);
        printf("\n             +--------+                        +--------+        %s", msg[OUTMSG]);
      }
      else if (cpt.onstage == (i + 1) % 3)
      {
        printf("\n             +--------+                     +--------+           %s", msg[OUTMSG]);
        printf("\n             |%.2d      |                     |%.2d      |           %s", ply.hp[i], cpt.hp[(i + 1) % 3], msg[OUTMSG]);
        printf("\n             |        |                     |        |           %s", msg[OUTMSG]);
        printf("\n             |  %s  |                     |  %s  |           %s", ply.name[ch[i]], cpt.name[(ch[i] + 1) % 3], msg[OUTMSG]);
        printf("\n             |        |                     |        |           %s", msg[OUTMSG]);
        printf("\n             |      %s|                     |      %s|           %s", ply.element[i], cpt.element[(i + 1) % 3], msg[OUTMSG]);
        printf("\n             +--------+                     +--------+           %s", msg[OUTMSG]);
      }
    }
  }
  printf("\n\n          行动点数            %.2d  |  %.2d          行动点数", ply.dice, cpt.dice);

  switch (stage)
  {
  case 1: printf("\n\n\n          ==================== 投掷阶段 ================="); break;
  case 2: printf("\n\n\n          ==================== 行动阶段 ================="); break;
  case 3: printf("\n\n\n          ==================== 结束阶段 ================="); break;
  default: break;
  }
}
void PrintAct()
{
  printf("\n\n          你可以采取以下行动：\
            \n          1 - 更换角色：消耗 1 点行动点数，更换当前出战的角色。\
            \n          2 - 普通攻击：消耗 2 点行动点数，造成普通伤害。\
            \n          3 - 元素战技：消耗 3 点行动点数，造成较高伤害及元素附着。\
            \n          4 - 结束回合：宣布结束本回合行动。\
            \n          5 - 查看信息：查看各个角色的详细信息。\n");
}
void PrintChInfo(int onstage)
{
  switch (onstage)
  {
  case 1:
    printf("\n          +--                                                       --+\
            \n          | 神里绫华                                                  |\
            \n            Kamisato Ayaka\
            \n            ---\
            \n            稻妻 “社奉行” 神里家的大小姐。\
            \n            容姿端丽，品行高洁；端庄而文雅，聪慧又坚韧。\
            \n            “像这样悠闲安稳的时光，如果再多一点就好了……我真贪心啊。”\
            \n            ---\
            \n            普通攻击 | 神里流·倾\
            \n            进行至多五段的连续剑击，造成 2 点伤害。消耗 2 点行动点数。\
            \n            元素战技 | 神里流·冰华\
            \n          | 唤起盛开的冰之华，造成 4 点冰元素伤害。消耗 3 点行动点数。|\
            \n          +--                                                       --+\n"); break;
  case 2:
    printf("\n          +--                                                           --+\
            \n          | 长野原霄宫                                                    |\
            \n            Naganohara Yoimiya\
            \n            ---\
            \n            才华横溢的烟花工匠，“长野原烟花店” 现任店主。\
            \n            未泯的童心与匠人的执着在她身上交织出了奇妙的焰色反应。\
            \n            “烟花易逝，人情长存。”\
            \n            ---\
            \n            普通攻击 | 烟火打扬\
            \n            进行至多五段的连续弓箭射击，造成 2 点伤害。消耗 2 点行动点数。\
            \n            元素战技 | 焰硝庭火舞\
            \n          | 舞动线香烟火，造成 4 点火元素伤害。消耗 3 点行动点数。        |\
            \n          +--                                                           --+\n"); break;
  case 3:
    printf("\n          +--                                                             --+\
            \n          | 钟离                                                            |\
            \n            Zhong Li\
            \n            ---\
            \n            应 “往生堂” 邀请而来的神秘客卿。\
            \n            样貌俊美，举止高雅，知礼数、晓规矩，拥有远超常人的学识。\
            \n            “旅程总有一天会迎来终点，不必匆忙。”\
            \n            ---\
            \n            普通攻击 | 岩雨\
            \n            进行至多六段的连续枪击，造成 2 点伤害。消耗 2 点行动点数。\
            \n            元素战技 | 地心\
            \n          | 命令岩元素积聚结为岩脊，造成 4 点岩元素伤害。消耗 3 点行动点数。|\
            \n          +--                                                             --+\n"); break;
  default: break;
  }
}
void PrintYuan()
{
  printf("\
................................................................................................................!i......................\n\
................................................................................................................/*:.....................\n\
...............................................................................................................`IF'.....................\n\
...............................................................................................................'F&i.....................\n\
...............................................................................................................1&&I`....................\n\
..............................................................................................................`I&&F~....................\n\
..............................................................................................................+&&&&]:...................\n\
.............::..............................................................::..............................`I&&&&X'...................\n\
............'XI,............................................................;XY`.............................}&&&&&&I`..................\n\
...........`*&&]:..........................................................:*&&]:...........................FF&&&&&&&1..................\n\
........:~*&&&&&F]':....................................................:'*&&&&&Fl'......................:iF&&&&&&&&&&F1`...............\n\
...:~/FF&&&&&&&&&&&&FXIII******************************************IIYXF&&&&&&&&&&&&Xl+':.............,iIF&&&&&&&&&&&&&&&Y/':...........\n\
......:YY&&&&&&&&&&*~:................,l&&Xi:.........................:,>lF&&&&&&F]!:...................:'/YF&&&&&&&&&X}!`..............\n\
........~F&&&&&&&&X;................,lF&&&&&X>............................`]&&&&1:..::......................:iF&&&&&1`..................\n\
........'F&&&&&&&&X;.;Y&FFY]i'`......:XX&&&l,......;]F&&X]!:................!FF';]F&&&&*;...~F&&FX*+':........:}&&I,.......`}F&&&&FY/;..\n\
........'F&&&&&&&&X;.'X&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&Yi:.+F&&&&&&&&&&&&&&&&&&&&&&F+..!F&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&F]\n\
........'F&&&&&&&&X;.'X&&&&&&&&}..................;X&&&&&&&&Y'..........:`Y&&&&&&&&&&&&&&&F>!F&&&&&&&&I,......iF&&&}:.....:]&&&&&&&&&X!.\n\
........'F&&&&&&&&X;.'X&&&&&&&&}..................;X&&&&&&&&]:...1&&&&&F~`Y&&&&&&F*******l+`!F&&&&&&&&I,....:/F&&&&&]`....:]&&&&&&&&&/..\n\
........'F&&&&&&&&X;.'X&&&&&&&&&FFFFFFFFFFFFFFFF/.;X&&&&&&&&]:...1&&&&&F~`Y&&&&&&*:.::::::..!F&&&&&&&&Y~>lX&&&&&&&&&&&&F*+']&&&&&&&&&/..\n\
........'F&&&&&&&&X;.'X&&&&&&&&],,,,,,,,,,,,,,,,`.;X&&&&&&&&]:...1&&&&&F~`Y&&&&&&*:>&&&&&&i.!F&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&/..\n\
........'F&&&&&&&&Y`.'X&&&&&&&&}..................;X&&&&&&&&}:...1&&&&&F~`Y&&&&&&*:>&&&&&&i.!F&&&&&&&&I,.:**F&&&&&&&&&Yi`.:]&&&&&&&&&/..\n\
........~F&&&&&&&&l:.'X&&&&&&&&I!!!!!!!!!!!!!!!!!!+F&&&&&&&&}:...1&&&&&F~`Y&&&&&&*:>&&&&&&i.!F&&&&&&&&I,....:}&&&&&&l,....:]&&&&&&&&&/..\n\
.........&&&&&&&&X'..'X&&&&&F*!'''''1&&&&&&&&Y!''''X&&&&&&&Xi....1&&&&&F~`Y&&&&&&*:>&&&&&&i.!F&&&&&&&&Y;::::::;Y&&F:::::::`l&&&&&&&&&/..\n\
.......:1&&&&&&&&+...'XFFI+,........>&&&&&&&&I`...,]l}+;;:.......1&&&&&F~`Y&&&&&&*:>&&&&&Y;.!F&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&/..\n\
.......FF&&&&&&X~......:,,}IXFF>....>&&&&&&&&I`...:lFFYl/~`.....:]&&&&&X,`Y&&&&&&*:>&&&*'...!F&&&&&&&]`.......,Y&&F!.......:/F&&&&&&&+..\n\
......;Y&&&&&&Y'..:~1YF&&&&&&&&Fi:..>&&&&&&&&I`..,l&&&&&&&&&F*i,,Y&&&&&}.`Y&&&&&&*:**i`.....!F&&&&Yi:........;I&&&&X!........:!IF&&&&+..\n\
....:/&&&&&&l,`1F&&&&&&&&&&&&&FFXYl;>&&&&&&&&I,iIYFF&&&&&&&&&&&&&&&&&Fi..`Y&&&&&&*:..........`:.........`~}X&&&&&&&&&&Fll,.........:`:..\n\
...:]&&&&&*;..FF&&&&&&&Xli',:.......>&&&&&&&&I`......:`;!/IF&&&&&&&&Y;...`Y&&&&&&*:.................:}XF&&&&&&&&&&&&&&&&&&&F*;..........\n\
.`l&&F}'........`+Y&&I::............>&&&&&&&&*:............,1F&F*::......`Y&&&&&&]:.....................:YY&&&&&&&&&&&&F+`..............\n\
>Y]!:...............,!/YYI}>'`......>&&&&&&&X~......:,!+lYI]i':..........`Y&&&&&F~.........................iF&&&&&&&&&]:................\n\
....................................>&&&&&I'.............................`Y&&&Fi............................;X&&&&&&F...................\n\
....................................>&&F},...............................`Y&X+:............................../&&&&&&l:..................\n\
.............................................................................................................`*&&&&X'...................\n\
............................................................................................................../&&&&l:...................\n\
..............................................................................................................;X&&FF....................\n\
..............................................................................................................:*&&X;....................\n\
...............................................................................................................+&&l:....................\n\
...............................................................................................................!&&/.....................\n\
...............................................................................................................,XF!.....................\n\
...............................................................................................................`IF;.....................\n\
...............................................................................................................:}Y:.....................\n\
................................................................................................................/::.....................\n\
................................................................................................................!i......................\n");
  char temp;
  printf("\n\n                                                            按任意键继续。");
  getch();
  system("cls");
}
void BGM1()
{
  PlaySound(TEXT("Dream Aria.wav"), NULL, SND_LOOP | SND_ASYNC);
}
void BGM2()
{
  PlaySound(TEXT("His Solution.wav"), NULL, SND_LOOP | SND_ASYNC);
}