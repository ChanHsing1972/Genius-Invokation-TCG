#pragma once
#ifndef C_Users_14229_source_repos_Genius_Invokation_TCG_2
#define C_Users_14229_source_repos_Genius_Invokation_TCG_2

void BeforeGame();													// ����׼��
int DuringGame();														// �Ծֻغ�
void ActionStage();													// �Ծֻغ� => �ж��׶�
void ChangeCh(int row, int col, int stage); // ���˵ĺ��Ĳ���
int IsValidChange(int cmd_act2);						// �������˹��̣��ж��Ƿ���Ч
void DieAndChange();												// ���ɫ���£��������˹���
int Attack(int para);												// �ҷ�����
int Rival();																// ���ֵ������߼�
void PrintInterface(int stage);							// ��ӡ�������
void PrintAct();														// ��ӡ�ж�ָʾ
void PrintChInfo(int onstage);							// ��ӡ��ɫ����
void PrintYuan();														// ��ӡԭ����������
void BGM1();															  // ѭ������ BGM
void BGM2();															  // ѭ������ BGM

#endif C_Users_14229_source_repos_Genius_Invokation_TCG_2
