#pragma once

#include "head.h"

extern FILE* fpError;

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

extern int init_errorHandler();

// ��������
extern int error(int etype);
