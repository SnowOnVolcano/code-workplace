#pragma once

#include "linkList.h"
#include "uthash-master/uthash.h"
#include "symbolTable.h"

#define TOKENSIZE 0x80
#define _NORMAL_EXIT 1
#define _ERROR -1

extern int lineNum;

// ��������
enum error_types
{
	// �Ƿ����Ż򲻷��ϴʷ�
	ERROR_A,
	// �����ض���
	ERROR_B,
	// δ���������
	ERROR_C,
	// ��������������ƥ��
	ERROR_D,
	// �����������Ͳ�ƥ��
	ERROR_E,
	// �����ж��г��ֲ��Ϸ�������
	ERROR_F,
	// �޷���ֵ�ĺ������ڲ�ƥ���return���
	ERROR_G,
	// �з���ֵ�ĺ���ȱ��return������ڲ�ƥ���return���
	ERROR_H,
	// ����Ԫ�ص��±�ֻ�������ͱ��ʽ
	ERROR_I,
	// ���ܸı䳣����ֵ
	ERROR_J,
	// ӦΪ�ֺ�
	ERROR_K,
	// ӦΪ��С����
	ERROR_L,
	// ӦΪ��������
	ERROR_M,
	// do-while�����ȱ�١�while��
	ERROR_N,
	// ���������С�=������ֻ�������ͻ��ַ��ͳ���
	ERROR_O
};

// ��������
extern int error(int etype);

