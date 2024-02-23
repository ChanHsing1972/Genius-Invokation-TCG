#pragma once
#ifndef C_Users_14229_source_repos_Genius_Invokation_TCG_2
#define C_Users_14229_source_repos_Genius_Invokation_TCG_2

void BeforeGame();													// 开局准备
int DuringGame();														// 对局回合
void ActionStage();													// 对局回合 => 行动阶段
void ChangeCh(int row, int col, int stage); // 换人的核心操作
int IsValidChange(int cmd_act2);						// 主动换人过程，判断是否有效
void DieAndChange();												// 因角色倒下，被动换人过程
int Attack(int para);												// 我方攻击
int Rival();																// 对手的牌组逻辑
void PrintInterface(int stage);							// 打印牌组界面
void PrintAct();														// 打印行动指示
void PrintChInfo(int onstage);							// 打印角色介绍
void PrintYuan();														// 打印原神启动界面
void BGM1();															  // 循环播放 BGM
void BGM2();															  // 循环播放 BGM

#endif C_Users_14229_source_repos_Genius_Invokation_TCG_2
