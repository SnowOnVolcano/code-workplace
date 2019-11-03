#include "errorHandler.h"

FILE* fpError;

int init_errorHandler() {
	fpError = fopen("error.txt", "w");
	return 0;
}

int error(int etype) {
	puts("-----------");
	if (etype == ERROR_K) {
		printf("%d %c\n", lineNum - isMinuLine, etype + 'a');
		fprintf(fpError, "%d %c\n", lineNum, etype + 'a');
	}
	else if (etype >= 0) {
		printf("%d %c\n", lineNum, etype + 'a');
		fprintf(fpError, "%d %c\n", lineNum, etype + 'a');
	}
	else if (etype == -1) {
		printf("%d _ERROR\n", lineNum);
		fprintf(fpError, "%d _ERROR\n", lineNum);
	}
	else if (etype == -2) {
		printf("%d _ERROR_GET\n", lineNum);
		fprintf(fpError, "%d _ERROR_GET\n", lineNum);
	}
	
	/*switch (etype)
	{
	case _ERROR_GET:
		break;
	case _ERROR:
		break;
	case ERROR_A:// �Ƿ����Ż򲻷��ϴʷ�
		break;
	case ERROR_B:// �����ض���
		break;
	case ERROR_C:// δ���������
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
	}*/

	puts("-----------");
	return 0;
}

