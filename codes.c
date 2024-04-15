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
  int dice;                        // ���ӵ���
  int dead;                        // ������ɫ��
  int onstage;                     // ������ɫ��ţ�0 - ����类���1 - ������2 - ���룩
  int hp[NUM_OF_CH];               // ����ɫ����ֵ
  int elem_code[NUM_OF_CH + 1];    // ����Ԫ�صĴ��루0 - �ޣ�1 - ����2 - ��3 - �ң�
  char element[NUM_OF_CH + 1][10]; // Ԫ�ظ���
  char name[NUM_OF_CH + 1][100];   // ��ɫ��
};

struct Info ply = { 0, 0, -1, { 0 }, { 0 }, { "  ", "  ", "  " }, { "类�", "����", "����" } }; // ply => player
struct Info cpt = { 0, 0, -1, { 0 }, { 0 }, { "  ", "  ", "  " }, { "类�", "����", "����" } }; // cpt => computer

int len, in_msg = 0;   // len: ѡ��Ľ�ɫ������in_msg: ս����־�ı��
int ch[NUM_OF_CH + 1]; // ch[]: �洢��ɫ
char msg[500][1000];   // msg => message, msg[][]: �洢ս����־

bool flag_ply_end_round = FALSE;  // flag_ply_end_round: �ҷ������غϺ�ʧȥ�ж�Ȩ
bool flag_cpt_end_round = FALSE;  // flag_cpt_end_round: �Է������غϺ�ʧȥ�ж�Ȩ
bool flag_ch_to_begin = FALSE;    // flag_ch_to_begin: ��ѡ�������ɫ�� ֻ���״ν��� ActionStage ʱִ��
bool flag_skip_printact = FALSE;  // flag_skip_printact: ���� PrintAct ����
bool flag_reaction = FALSE;       // flag_reaction: �Ƿ���Ԫ�ط�Ӧ

int main()
{
  BGM1();
  printf("\n\n        ��ʾ��\
            \n        �� ���� F11 �Ի����ѹ۸С�\
            \n        �� ����ʹ�÷ֱ��ʸ��� 1920 * 1400 ����ʾ����\
          \n\n        ������...");
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
  int cmd_ch;  // ѡ���ɫ
  char cmd_yn; // yes or no
  printf("\n\n          WELCOME TO GENIUS INVOKATION TCG.\
            \n          ��ӭ���� Gayshin Impart ��ʥ�ٻ���\
          \n\n          ���б���ѡ�� 1 - 3 λ��ɫ��\
            \n          ( ���ǵ���Ϸƽ���ԣ�����ȫѡ )\
          \n\n          1 - ����类�          ������\
            \n          2 - ����              ����\
            \n          3 - ����              ���ң�\
            \n          4 -���������ų���ģ�\
            \n          5 -����Ҳ�����ų���ģ�\
            \n          6 -���һ������ų���ģ�\
            \n          ...\
          \n\n          ���ѡ��(e.g.\"1 2 3\\n\")��");

  // ѡ�����飺��������ɫ��ѡ������һ����ɫ������������ɫ
  while (1)
  { 
    int i = 1;

    scanf("%d", &cmd_ch);
    printf("          ȷ��ѡ��%s", ply.name[cmd_ch - 1]);
    ch[0] = cmd_ch - 1; len = 1;

    while (getchar() != '\n')
    {
      scanf("%d", &cmd_ch);
      printf(" & %s", ply.name[cmd_ch - 1]);
      ch[i++] = cmd_ch - 1; len++;
    }
    printf("��(Y/N)");

    cmd_yn = getch();
    if ((cmd_yn == 'Y') || (cmd_yn == 'y'))
    {
      printf("\n          ������...");
      Sleep(500); system("cls"); break;
    }
    else if ((cmd_yn == 'N') || (cmd_yn == 'n'))
    {
      printf("\n\n          ����ѡ��");
      continue;
    }
  }

   // ѡ���ؿ���ֻ��һ���ؿ�
   int cmd_lvl;
   printf("\n\n          WELCOME TO GENIUS INVOKATION TCG.\
             \n          ��ӭ���� Gayshin Impart ��ʥ�ٻ���\
           \n\n          ѡ��ؿ���û��ѡ����\
           \n\n          +---+  +---+  +---+  +---+  +---+  \
             \n          | 1 |  |XXX|  |XXX|  |XXX|  |XXX|  \
             \n          +---+  +---+  +---+  +---+  +---+  \
           \n\n          ���ѡ��");
   getch();
   printf("\n          ������...");
   Sleep(500); system("cls");
}
int DuringGame()
{
  while (1)
  {
    // Ͷ���׶Σ�ͨ��Ͷ����ͨɫ��ȷ���ж�����
    PrintInterface(1); Sleep(500);
    for (int i = 0; i < 50; i++) // ���ӿ��ٱ仯Ч��
    {
      ply.dice = rand() % 6 + 5;
      cpt.dice = rand() % 6 + 5;
      printf("\n\n          �������ӣ����ӵ���Ϊ��%d", ply.dice); Sleep(20);
      PrintInterface(1);
    }
    printf("  \n\n          �������ӣ����ӵ���Ϊ��%d", ply.dice); Sleep(1000);
    printf("    \n          �ʱ��غ��У�����ж�����Ϊ %d��", ply.dice); Sleep(500);
    printf("  \n\n          �����������ӣ����ӵ���Ϊ��%d", cpt.dice); Sleep(500);
    printf("    \n          �ʱ��غ��У����ֵ��ж�����Ϊ %d��", cpt.dice); Sleep(500);
    printf("  \n\n          �������������");
    getch();

    // ply.dice = 10;
    // cpt.dice = 10;

    // �ж��׶Σ�ͨ�������ж����������ͨ������Ԫ��ս�����滻��ս��ɫ�ȶ�������������Ԫ�ط�Ӧ��
    flag_ply_end_round = FALSE; // flag_ply_end_round: �ҷ������غϺ�ʧȥ�ж�Ȩ����ʱΪ�ر�״̬��
    flag_cpt_end_round = FALSE; // flag_cpt_end_round: �Է������غϺ�ʧȥ�ж�Ȩ����ʱΪ�ر�״̬��
    PrintInterface(2); Sleep(500);

    ActionStage();

    printf("\n          �������������");
    getch();

    // �����׶Σ�����ֳ���ʤ������һ�����н�ɫȫ�����£�����ô��ʾʤ��������ص�Ͷ���׶�
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
      printf("\n\n          ������\n\n"); return 0;
      for (int i = 0; i <= 99; i++)
      {
        printf("\n          ");
        for (int j = 0; j <= 15; j++)
        {
          printf("�� ");
          Sleep(50);
        }
      }
    }
    else if (cpt.dead == len)
    {
      printf("\n\n          Ӯ���ˣ�����\n\n"); return 0;
      for (int i = 0; i <= 99; i++)
      {
        printf("\n          ");
        for (int j = 0; j <= 15; j++)
        {
          printf("Ӯ ");
          Sleep(50);
        }
      }
    }
    else
    {
      printf("\n\n          ���ڼ�����һ�غ�...");
      Sleep(500); continue;
    }
  }
}
void ActionStage()
{
  // P1 - ѡ�������ɫ
  int cmd_onstage;

  if (flag_ch_to_begin == FALSE) // flag_ch_to_begin: ��ѡ�������ɫ�� ֻ���״ν��� ActionStage ʱִ��
  {
    printf("\n\n          |  ");
    for (int i = 0; i < len; i++)
      printf("%d - %s  |  ", ch[i] + 1, ply.name[ch[i]]);
    printf("\n          ѡ��һλ������ɫ��");
    cmd_onstage = getch() - '0';

    ply.onstage = cmd_onstage - 1;
    cpt.onstage = (ply.onstage + 1) % 3;
    sprintf(msg[in_msg++], "[ �� ָ��������ɫ�� %s�� ]", ply.name[ply.onstage]);
    sprintf(msg[in_msg++], "[ ���� ָ��������ɫ��%s�� ]", cpt.name[cpt.onstage]);
    ChangeCh(1, cpt.onstage, 2);
    printf("\n\n          %s", msg[in_msg - 2]);
    printf("  \n          %s", msg[in_msg - 1]);
  }
  flag_ch_to_begin = TRUE;

  // P2 - ��ʼ�ж�
  int cmd_act, cmd_act2;

  if(flag_skip_printact == FALSE) // flag_skip_printact: ���� PrintAct ����
    PrintAct(); 
  flag_skip_printact = FALSE;

  while (flag_ply_end_round == FALSE)
  {
    printf("\n          ������Ӧ����ִ�в�����");
    cmd_act = getch() - '0';
    switch (cmd_act)
    {
    case 1:
      printf(" \n          |  ");
      for (int i = 0; i < len; i++)
        printf("%d - %s  |  ", ch[i] + 1, ply.name[ch[i]]);
      printf("\n          ������ս��ɫΪ"); break;
    case 2: 
      printf("\n          ������ 2 ���ж��������� 1 ȷ��"); break;
    case 3: 
      printf("\n          ������ 3 ���ж��������� 1 ȷ��"); break;
    case 4: 
      printf("\n          �� 1 ȷ��"); break;
    case 5:
      printf("\n          |  ");
      for (int i = 0; i < len; i++)
        printf("%d - %s  |  ", ch[i] + 1, ply.name[ch[i]]);
      printf("\n          ѡ���ɫ�鿴��Ϣ"); break;
    default: break;
    }
    printf("���� 0 ������");
    cmd_act2 = getch() - '0';

    if (cmd_act2 == 0) continue;
    else
    {
      switch (cmd_act)
      {
      case 1: 
        IsValidChange(cmd_act2); 
        if (flag_cpt_end_round == FALSE)
          printf("\n          [ �ȴ������ж�... ]");
        Rival(); break;
      case 2: 
        Attack(PUGONG); break;
      case 3: 
        Attack(ZHANJI); break;
      case 4: 
        sprintf(msg[in_msg++], "[ �� ���������غϡ� ]");
        PrintInterface(2); PrintAct();
        printf("\n          %s", msg[in_msg - 1]);
        if(flag_cpt_end_round == FALSE)
          printf("\n          [ �ȴ������ж�... ]");
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
    printf("\n\n          [ �ý�ɫ�ѵ��£��޷������� ]");
  else if (ply.dice < 1)
    printf("\n\n          [ �ж��������㣬�޷������� ]");
  else if (cmd_act2 - 1 == ply.onstage)
    printf("\n\n          [ ��ָ���Ľ�ɫ�뵱ǰ��ս��ɫ��ͬ���޷������� ]");
  else
  {
    sprintf(msg[in_msg++],"[ �� ������ս��ɫΪ%s�� ]", ply.name[cmd_act2 - 1]);
    ply.dice--; ChangeCh(0, cmd_act2 - 1, 2); PrintAct();
    printf("\n          [ �� ������ս��ɫΪ%s�� ]", ply.name[cmd_act2 - 1]);
    return 0;
  }
  flag_skip_printact = TRUE;
  ActionStage();
}
void DieAndChange()
{
  ply.dead = cpt.dead = 0;

  sprintf(msg[in_msg++], "[ �� ��%s�ѵ��¡�  ]", ply.name[ply.onstage]);
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
    printf("\n\n          ������\n\n");
    for (int i = 0; i <= 99; i++)
    {
      printf("\n          ");
      for (int j = 0; j <= 15; j++)
      {
        printf("�� ");
        Sleep(50);
      }
    } exit(0);
  }
  else if (cpt.dead == len)
  {
    PrintInterface(3); Sleep(500);
    printf("\n\n          Ӯ���ˣ�����\n\n"); 
    for (int i = 0; i <= 99; i++)
    {
      printf("\n          ");
      for (int j = 0; j <= 15; j++)
      {
        printf("Ӯ ");
        Sleep(50);
      }
    } exit(0);
  }

  printf("\n          [ ����ѡ��һ����ս��ɫ�Լ����ж��� ]");
  printf("\n\n          |  ");
  for (int i = 0; i < len; i++)
    printf("%d - %s  |  ", ch[i] + 1, ply.name[ch[i]]);

  int temp;
  while (1)
  {
    printf("\n          ������ս��ɫΪ���˴θ����������ж�������");
    temp = getch() - '0';
    ply.dice++;
    if (ply.hp[temp - 1] == 0)
    {
      printf("\n          [ �ý�ɫ�ѵ��¡� ]"); continue;
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
    printf("\n          [ �ж��������㡣 ]");
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
        sprintf(msg[in_msg++],"[ �� ��%sʹ�� ��ͨ���� �� ���ֵ�%s��� 2 ���˺��� ]", ply.name[ply.onstage], cpt.name[cpt.onstage]);
        PrintInterface(2); PrintAct();
        printf("\n          %s", msg[in_msg - 1]);
      }
      else if (para == ZHANJI)
      {
        switch (ply.onstage + 1)
        {
        case 1:
          sprintf(msg[in_msg++], "[ �� ��类�ʹ�� ������������ �� ���ֵ�%s��� 4 ���˺��� ]", cpt.name[cpt.onstage]);
          sprintf(cpt.element[cpt.onstage], "��");
          cpt.elem_code[cpt.onstage] = 1; break;
        case 2:
          sprintf(msg[in_msg++], "[ �� ������ʹ�� ����ͥ���� �� ���ֵ�%s��� 4 ���˺��� ]", cpt.name[cpt.onstage]);
          sprintf(cpt.element[cpt.onstage], "��");
          cpt.elem_code[cpt.onstage] = 2; break;
        case 3:
          sprintf(msg[in_msg++], "[ �� ������ʹ�� ���� �� ���ֵ�%s��� 4 ���˺��� ]", cpt.name[cpt.onstage]);
          sprintf(cpt.element[cpt.onstage], "��");
          cpt.elem_code[cpt.onstage] = 3; break;
        default: break;
        }
        PrintInterface(2); PrintAct();
        printf("\n          %s", msg[in_msg - 1]);

        if (flag_reaction == TRUE)
        {
          if (cpt.hp[cpt.onstage] > 2) cpt.hp[cpt.onstage] -= 2;
          else cpt.hp[cpt.onstage] = 0;
          sprintf(msg[in_msg++], "[ �� �����ڻ���Ӧ���˺� +2�� ]");
          PrintInterface(2); PrintAct();
          printf("\n          %s", msg[in_msg - 2]);
          printf("\n          %s", msg[in_msg - 1]);
        }
      }
    }

    else if (cpt.hp[cpt.onstage] <= dmg)
    {
      if (flag_reaction == TRUE)
        sprintf(msg[in_msg++], "[ �� �����ڻ���Ӧ���˺� +2�� ]");
      sprintf(msg[in_msg++], "[ ���� ��%s�ѵ��¡� ]", cpt.name[cpt.onstage]);
      cpt.hp[cpt.onstage] = 0;
      PrintInterface(2); PrintAct();
      if (flag_reaction == TRUE)
        printf("\n          %s", msg[in_msg - 2]);
      printf("\n          %s", msg[in_msg - 1]); Sleep(1500);
    }

    if (ply.hp[0] == 0 && ply.hp[1] == 0 && ply.hp[2] == 0) return 0;
    if (flag_cpt_end_round == FALSE)
      printf("\n          [ �ȴ������ж�... ]");
    Rival();
  }
}
int Rival()
{
  // �����ж϶��ֽ�ɫ�Ƿ��£��Ƿ���Ҫ����
  if (cpt.hp[cpt.onstage] == 0)
  {
    for (int i = 0; i < len; i++)
    {
      if (cpt.hp[i] != 0)
      {
        cpt.onstage = i; Sleep(2000);
        sprintf(msg[in_msg++], "[ ���� ����������ɫΪ%s�� ]", cpt.name[cpt.onstage]);
        PrintInterface(2); PrintAct();
        printf("\n          %s", msg[in_msg - 1]);
        break;
      }
    }
  }

  if (cpt.hp[0] == 0 && cpt.hp[1] == 0 && cpt.hp[2] == 0)
  {
    PrintInterface(3); Sleep(500);
    printf("\n\n          Ӯ���ˣ�����\n\n"); 
    for (int i = 0; i <= 99; i++)
    {
      printf("\n          ");
      for (int j = 0; j <= 15; j++)
      {
        printf("Ӯ ");
        Sleep(50);
      }
    } exit(0);
  }

  // ���������߼�
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
        sprintf(msg[in_msg++], "[ ���� ��类�ʹ�� ������������ �� �� ��%s��� 4 ���˺��� ]", ply.name[ply.onstage]); 
        ply.elem_code[ply.onstage] = 1;
        sprintf(ply.element[ply.onstage], "��"); break;
      case 2: 
        sprintf(msg[in_msg++], "[ ���� ������ʹ�� ����ͥ���� �� �� ��%s��� 4 ���˺��� ]", ply.name[ply.onstage]);
        ply.elem_code[ply.onstage] = 2;
        sprintf(ply.element[ply.onstage], "��"); break;
      case 3: 
        sprintf(msg[in_msg++], "[ ���� ������ʹ�� ���� �� �� ��%s��� 4 ���˺��� ]", ply.name[ply.onstage]);
        ply.elem_code[ply.onstage] = 3;
        sprintf(ply.element[ply.onstage], "��"); break;
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
          sprintf(msg[in_msg++], "[ ���� �����ڻ���Ӧ���˺� +2�� ]");
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
      sprintf(msg[in_msg++], "[ ���� ��%sʹ�� ��ͨ���� �� �� ��%s��� 2 ���˺��� ]", cpt.name[cpt.onstage], ply.name[ply.onstage]);

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
      sprintf(msg[in_msg++], "[ ���� ���������غϡ� ]");
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
void PrintInterface(int stage)// ��ӡ����
{
  int out_msg = 0, temp = 0;

  if (in_msg > 21)
    out_msg = out_msg + in_msg - 21;
  system("cls");
  printf("\n\n          GENIUS INVOKATION TCG\
            \n          ��ʥ�ٻ�\n\
            \n          ��                                         ����        ս����־\n");
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
  printf("\n\n          �ж�����            %.2d  |  %.2d          �ж�����", ply.dice, cpt.dice);

  switch (stage)
  {
  case 1: printf("\n\n\n          ==================== Ͷ���׶� ================="); break;
  case 2: printf("\n\n\n          ==================== �ж��׶� ================="); break;
  case 3: printf("\n\n\n          ==================== �����׶� ================="); break;
  default: break;
  }
}
void PrintAct()
{
  printf("\n\n          ����Բ�ȡ�����ж���\
            \n          1 - ������ɫ������ 1 ���ж�������������ǰ��ս�Ľ�ɫ��\
            \n          2 - ��ͨ���������� 2 ���ж������������ͨ�˺���\
            \n          3 - Ԫ��ս�������� 3 ���ж���������ɽϸ��˺���Ԫ�ظ��š�\
            \n          4 - �����غϣ������������غ��ж���\
            \n          5 - �鿴��Ϣ���鿴������ɫ����ϸ��Ϣ��\n");
}
void PrintChInfo(int onstage)
{
  switch (onstage)
  {
  case 1:
    printf("\n          +--                                                       --+\
            \n          | ����类�                                                  |\
            \n            Kamisato Ayaka\
            \n            ---\
            \n            ���� ������С� ����ҵĴ�С�㡣\
            \n            ���˶�����Ʒ�и߽ࣻ��ׯ�����ţ��ϻ��ּ��͡�\
            \n            �����������а��ȵ�ʱ�⣬����ٶ�һ��ͺ��ˡ�������̰�İ�����\
            \n            ---\
            \n            ��ͨ���� | ����������\
            \n            ����������ε�������������� 2 ���˺������� 2 ���ж�������\
            \n            Ԫ��ս�� | ������������\
            \n          | ����ʢ���ı�֮������� 4 ���Ԫ���˺������� 3 ���ж�������|\
            \n          +--                                                       --+\n"); break;
  case 2:
    printf("\n          +--                                                           --+\
            \n          | ��Ұԭ����                                                    |\
            \n            Naganohara Yoimiya\
            \n            ---\
            \n            �Ż�������̻�����������Ұԭ�̻��ꡱ ���ε�����\
            \n            δ����ͯ���뽳�˵�ִ���������Ͻ�֯�����������ɫ��Ӧ��\
            \n            ���̻����ţ����鳤�档��\
            \n            ---\
            \n            ��ͨ���� | �̻����\
            \n            ����������ε����������������� 2 ���˺������� 2 ���ж�������\
            \n            Ԫ��ս�� | ����ͥ����\
            \n          | �趯�����̻���� 4 ���Ԫ���˺������� 3 ���ж�������        |\
            \n          +--                                                           --+\n"); break;
  case 3:
    printf("\n          +--                                                             --+\
            \n          | ����                                                            |\
            \n            Zhong Li\
            \n            ---\
            \n            Ӧ �������á� ������������ؿ��䡣\
            \n            ��ò��������ֹ���ţ�֪����������أ�ӵ��Զ�����˵�ѧʶ��\
            \n            ���ó�����һ���ӭ���յ㣬���ش�æ����\
            \n            ---\
            \n            ��ͨ���� | ����\
            \n            �����������ε�����ǹ������� 2 ���˺������� 2 ���ж�������\
            \n            Ԫ��ս�� | ����\
            \n          | ������Ԫ�ػ��۽�Ϊ�Ҽ������ 4 ����Ԫ���˺������� 3 ���ж�������|\
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
  printf("\n\n                                                            �������������");
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