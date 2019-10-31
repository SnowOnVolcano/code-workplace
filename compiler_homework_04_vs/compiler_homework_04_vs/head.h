#ifndef _HEAD_H_
#define _HEAD_H_

typedef int bool;

#include "linklist.h"

#define TOKENSIZE 0x80
#define _NORMAL_EXIT 1
#define _ERROR -1
#define _ERROR_STATEMENT -2
#define true 1
#define false 0

// �����к�
extern int line_num; 
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
extern void error(int etype);

#endif // !_HEAD_H_
