#include "head.h"


int error(int etype) {
	switch (etype)
	{
	case _ERROR:
		break;


	case ERROR_A:// �Ƿ����Ż򲻷��ϴʷ�
		break;


	case ERROR_B:// �����ض���
		printf("b\n");
		break;


	case ERROR_C:// δ���������
		printf("c\n");
		break;


	case ERROR_D:// ��������������ƥ��
		break;


	case ERROR_E:// �����������Ͳ�ƥ��
		break;


	case ERROR_F:// �����ж��г��ֲ��Ϸ�������
		break;


	case ERROR_G:// �޷���ֵ�ĺ������ڲ�ƥ���return���
		break;


	case ERROR_H:// �з���ֵ�ĺ���ȱ��return������ڲ�ƥ���return���
		break;


	case ERROR_I:// ����Ԫ�ص��±�ֻ�������ͱ��ʽ
		break;


	case ERROR_J:// ���ܸı䳣����ֵ
		break;


	case ERROR_K:// ӦΪ�ֺ�
		break;


	case ERROR_L:// ӦΪ��С����
		break;


	case ERROR_M:// ӦΪ��������
		break;


	case ERROR_N:// do-while�����ȱ�١�while��
		break;


	case ERROR_O:// ���������С�=������ֻ�������ͻ��ַ��ͳ���
		break;

	default:
		break;
	}
}

