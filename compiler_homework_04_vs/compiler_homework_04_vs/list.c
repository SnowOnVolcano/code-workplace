/*****************************************************
 * @�ļ���: DoubleList.c
 * @�ļ�����: ���ļ�Ϊ˫��ѭ������⺯�������й��ܺ�����ʵ��
 * @�༭��: ��͢��
 * @�༭����: 2017-11-22
 * @�޸�����: 2018-2-1
 *****************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "list.h"

 /**
  * @������: initialList
  * @��������: ��ʼ������
  * @����: size: ����ڵ�Ŀռ��С
  * @����ֵ: ��ʼ���ɹ������������͵�ָ�룬ʧ�ܷ���NULL
  */
List_t* initialList(int size)
{
	List_t* temp;

	/* ��������ռ� */
	temp = malloc(sizeof(List_t));
	if (temp == NULL)
	{
		return NULL;
	}

	/* ��ʼ������ */
	temp->size = size;
	temp->n = 0;
	temp->head.next = &temp->head;
	temp->head.prev = &temp->head;

	return temp;
}

/**
 * @������: insertNode
 * @��������: ˫�������½ڵ���뺯����ֻ���ṩ�½ڵ��ǰ��ڵ��ַ
 *            �Ϳ��԰ѽڵ���뵽����֮��
 * @����: P:�½ڵ��ǰһ���ڵ�ĵ�ַ  N:�½ڵ�ĺ�һ���ڵ�ĵ�ַ
 *        newNode:�½ڵ��ַ
 * @����ֵ: void
 */
void insertNode(Node_t* P, Node_t* N, Node_t* newNode)
{
	/* �ȸı��Լ� */
	newNode->next = N;	// ����ָ��ָ���ڵ�
	newNode->prev = P;	// ǰ��ָ��ָ��ǰ�ڵ�

	/* �ٸı���� */
	N->prev = newNode;	// ��ڵ��ǰ��ָ��ָ���Լ�
	P->next = newNode;	// ǰ�ڵ�ĺ���ָ��ָ���Լ�
}

/**
 * @������: appendListTop
 * @��������: �ѽڵ���ͷ���뷽ʽ���ص�������
 * @����: list:�����ַ  data:���ݵ�ַ
 * @����ֵ: ����ɹ�����0��ʧ�ܷ��ط���
 */
int appendListTop(List_t* list, void* data)
{
	Node_t* newNode;

	/* ��һ���������½ڵ�ռ� */
	newNode = malloc(sizeof(Node_t));
	if (NULL == newNode)
	{
		return -1;
	}

	/* �ڶ��������ڵ��е����ݵ�ַ����ռ� */
	newNode->datap = malloc(list->size);
	if (NULL == newNode->datap)
	{
		free(newNode);  // ��Ҫ�����ͷ�ǰ������ɹ��Ŀռ�
		return -2;
	}

	/* ���������������ݸ��Ƶ��ڵ��е����ݵ�ַ */
	memmove(newNode->datap, data, list->size);

	/* ���Ĳ��������ݹ��ص�����ͷ */
	insertNode(&list->head, list->head.next, newNode);

	/* ���岽������ڵ���+1 */
	list->n += 1;

	return 0;	// �ɹ�����0
}

/**
 * @������: appendListTail
 * @��������: �ѽڵ���β���뷽ʽ���ص�������
 * @����: list:�����ַ  data:���ݵ�ַ
 * @����ֵ: ����ɹ�����0��ʧ�ܷ��ط���
 */
int appendListTail(List_t* list, void* data)
{
	Node_t* newNode;

	/* ��һ���������½ڵ�ռ� */
	newNode = malloc(sizeof(Node_t));
	if (NULL == newNode)
	{
		return -1;
	}

	/* �ڶ��������ڵ��е����ݵ�ַ����ռ� */
	newNode->datap = malloc(list->size);
	if (NULL == newNode->datap)
	{
		free(newNode);  // ��Ҫ�����ͷ�ǰ������ɹ��Ŀռ�
		return -2;
	}

	/* ���������������ݸ��Ƶ��ڵ��е����ݵ�ַ */
	memmove(newNode->datap, data, list->size);

	/* ���Ĳ��������ݹ��ص�����β */
	insertNode(list->head.prev, &list->head, newNode);

	/* ���岽������ڵ���+1 */
	list->n += 1;

	return 0;	// �ɹ�����0	
}

/**
 * @������: sortInsert
 * @��������: �Դ�С�����˳��ѽڵ���뵽������
 * @����: list:�����ַ  func:���ڱȽ����ݵĻص�����  data:���ݵ�ַ
 * @����ֵ: ����ɹ�����0��ʧ�ܷ��ط���
 */
int sortInsert(List_t* list, compare_t* func, void* data)
{
	Node_t* newNode;

	/* ��һ���������½ڵ�ռ� */
	newNode = malloc(sizeof(Node_t));
	if (NULL == newNode)
	{
		return -1;
	}

	/* �ڶ��������ڵ��е����ݵ�ַ����ռ� */
	newNode->datap = malloc(list->size);
	if (NULL == newNode->datap)
	{
		free(newNode);  // ��Ҫ�����ͷ�ǰ������ɹ��Ŀռ�
		return -2;
	}

	/* ���������������ݸ��Ƶ��ڵ��е����ݵ�ַ */
	memmove(newNode->datap, data, list->size);

	/* ���Ĳ����ҵ����ʵĲ���� */
	Node_t* i;
	for (i = list->head.next; i != &list->head; i = i->next)
	{
		if (func(i->datap, data) >= 0)
		{
			break;
		}
	}

	/* ���岽�����½ڵ���뵽���ʵĲ������ */
	insertNode(i->prev, i, newNode);

	/* ������������ڵ���+1 */
	list->n += 1;

	return 0;	// �ɹ�����0
}

/**
 * @������: searchListOneByItem
 * @��������: �����������ҽڵ㣬����ж��ƥ������ҵ�һ��
 * @����: list:�����ַ  func:���ڱȽ����ݵĻص�����  item:������ַ
 * @����ֵ: ���ҳɹ����ؽڵ��ַ��ʧ�ܷ���NULL
 */
void* searchListOneByItem(List_t* list, compare_t* func, void* item)
{
	Node_t* i;

	i = list->head.next;
	while (i != &list->head)
	{
		if (func(i->datap, item) == 1) // �ص���������1��ʾ����ƥ��
		{
			return i->datap;
		}
		i = i->next;
	}

	return NULL;
}

/**
 * @������: searchListByItem
 * @��������: �����������ҽڵ㣬����ж��ƥ����ȫ������
 * @����: list:�����ַ  func:���ڱȽ����ݵĻص�����  item:������ַ
 * @����ֵ: ���ҳɹ����ؽ�������ַ��ʧ�ܷ���NULL
 */
List_t* searchListByItem(List_t* list, compare_t* func, void* item)
{
	List_t* result;
	Node_t* i;

	/* �������ҵĽ������ */
	result = initialList(list->size);
	if (NULL == result)
	{
		return NULL;
	}

	/* ��������ƥ��Ľڵ� */
	i = list->head.next;
	while (i != &list->head)
	{
		if (func(i->datap, item) == 1)	// �ص���������1��ʾ����ƥ��
		{
			appendListTail(result, i->datap);
		}
		i = i->next;
	}

	/* �жϲ����Ƿ�ɹ� */
	if (0 == result->n)
	{
		return NULL;
	}

	return result;
}

/**
 * @������: deleteListOneByItem
 * @��������: ��������ɾ���ڵ㣬����ж��ƥ����ɾ����һ��
 * @����: list:�����ַ  func:���ڱȽ����ݵĻص�����  item:������ַ
 * @����ֵ: ɾ���ɹ�����0��ʧ�ܷ��ط���
 */
int deleteListOneByItem(List_t* list, compare_t* func, void* item)
{
	Node_t* i = list->head.next;		// ��һ���ڵ�

	/* ����ɾ�� */
	while (i != &list->head)
	{
		if (func(i->datap, item) == 1)	// �ص���������1��ʾ����ƥ��
		{
			i->next->prev = i->prev;
			i->prev->next = i->next;
			free(i->datap);				// ɾ���ڵ��е�����
			free(i);					// ɾ���ڵ㱾��
			list->n -= 1;				// �ǵ��޸Ľڵ����
			return 0;
		}
		i = i->next;
	}

	return -1;
}

/**
 * @������: deleteListByItem
 * @��������: ��������ɾ���ڵ㣬����ж��ƥ����ɾ��ȫ��
 * @����: list:�����ַ  func:���ڱȽ����ݵĻص�����  item:������ַ
 * @����ֵ: ���سɹ�ɾ���ĸ���
 */
int deleteListByItem(List_t* list, compare_t* func, void* item)
{
	int count = 0; 		 // ͳ��ɾ���ĸ���
	Node_t* i, * next;

	/* ����ɾ�� */
	i = list->head.next; //	��һ���ڵ�
	while (i != &list->head)
	{
		if (func(i->datap, item) == 1)	// �ص���������1��ʾ����ƥ��
		{
			next = i->next;				// �ȱ������һ���ڵ�

			/* ɾ�� */
			i->next->prev = i->prev;
			i->prev->next = i->next;
			free(i->datap);				// ɾ���ڵ��е�����
			free(i);					// ɾ���ڵ㱾��
			list->n -= 1;				// �ǵ��޸Ľڵ����

			count++;					// �ۼ�ɾ������
			i = next;					// ��һ������
			continue;
		}
		i = i->next;
	}

	return count;						// ���سɹ�ɾ���ĸ���
}

/**
 * @������: sortList
 * @��������: �������������
 * @����: list:�����ַ  func:���ڱȽ����ݵĻص�����
 * @����ֵ: void
 */
void sortList(List_t* list, compare_t* func)
{
	Node_t* i, * j;
	void* temp;

	/* ð������ */
	for (i = list->head.next; i != &list->head; i = i->next)
	{
		for (j = i->next; j != &list->head; j = j->next)
		{
			if (func(i->datap, j->datap) == 1)	// ����˳���лص���������
			{
				/* �����ڵ��е����ݵ�ַ */
				temp = i->datap;
				i->datap = j->datap;
				j->datap = temp;
			}
		}
	}
}

/**
 * @������: saveListToFile
 * @��������: �������浽�ļ���
 * @����: list:�����ַ  file:���ڱ���������ļ���
 * @����ֵ: �ɹ����淵��0��ʧ�ܷ��ط���
 */
int saveListToFile(List_t* list, char* file)
{
	FILE* fout;
	int ret;

	/* ���ļ�����д���� */
	fout = fopen(file, "w");
	if (NULL == fout)
	{
		return -1;
	}

	/* ��������ڵ��е����ݿռ��С */
	ret = fwrite(&list->size, sizeof(int), 1, fout);
	if (ret != 1)
	{
		fclose(fout);
		return -2;
	}

	/* ��������ڵ����� */
	Node_t* i = list->head.next;
	while (i != &list->head)
	{
		ret = fwrite(i->datap, list->size, 1, fout);
		if (ret != 1)
		{
			fclose(fout);
			return -3;
		}
		i = i->next;
	}

	/* �ر��ļ������˳� */
	fclose(fout);
	return 0;
}

/**
 * @������: loadListFromFile
 * @��������: ���ļ��м�������
 * @����: file:���ڼ���������ļ���
 * @����ֵ: ���سɹ����������ַ��ʧ�ܷ���NULL
 */
List_t* loadListFromFile(char* file)
{
	List_t* result;
	FILE* fin;
	int ret;

	/* ���ļ����ڼ����������� */
	fin = fopen(file, "r");
	if (NULL == fin)
	{
		goto exit1;
	}

	/* ��ʼ��������� */
	result = initialList(0);
	if (NULL == result)
	{
		goto exit2;
	}

	/* ��������ڵ��е����ݿռ��С */
	ret = fread(&result->size, sizeof(int), 1, fin);
	if (1 != ret)
	{
		goto exit3;
	}

	/* �������ݽڵ� */
	void* data = malloc(result->size);
	if (NULL == data)
	{
		goto exit3;
	}
	while (1)
	{
		ret = fread(data, result->size, 1, fin);

		/* �ж϶��Ƿ���������ļ�β�� */
		if (ferror(fin))
		{
			goto exit4;
		}
		else if (feof(fin))
		{
			break;
		}

		/* �ѽڵ���ӵ�����β */
		ret = appendListTail(result, data);
		if (ret != 0)
		{
			goto exit4;
		}
	}

	/* �������ݳɹ� */
	fclose(fin);
	free(data);

	return result;

	/* �����Ĳ���˳� */
exit4:
	free(data);
exit3:
	destroyList(result);
exit2:
	fclose(fin);
exit1:
	return NULL;
}

/**
 * @������: traverseListForward
 * @��������: ǰ���������
 * @����: list:�����ַ  func:���ڴ�ӡ���ݵĻص�����
 * @����ֵ: void
 */
void traverseListForward(List_t* list, void (*func)(void* data))
{
	Node_t* i;

	for (i = list->head.next; i != &list->head; i = i->next)
	{
		func(i->datap);		// ���ûص�������ӡ���
	}
}

/**
 * @������: traverseListBackward
 * @��������: �����������
 * @����: list:�����ַ  func:���ڴ�ӡ���ݵĻص�����
 * @����ֵ: void
 */
void traverseListBackward(List_t* list, void (*func)(void* data))
{
	Node_t* i;

	for (i = list->head.prev; i != &list->head; i = i->prev)
	{
		func(i->datap);		// ���ûص�������ӡ���
	}
}

/**
 * @������: isEmptyList
 * @��������: �ж������Ƿ�Ϊ��
 * @����: list:��Ҫ�жϵ������ַ
 * @����ֵ: ����Ϊ�շ���1����Ϊ�շ���0
 */
int isEmptyList(List_t* list)
{
	return list->head.next == &list->head &&
		list->head.prev == &list->head;
}

/**
 * @������: destroyList
 * @��������: ��������
 * @����: list:��Ҫ���ٵ������ַ
 * @����ֵ: ��
 */
void destroyList(List_t* list)
{
	Node_t* i, * next;

	i = list->head.next;
	while (i != &list->head)
	{
		next = i->next;		// �ȱ�����һ���ڵ�

		free(i->datap);		// �ͷŽڵ��е����ݿռ�
		free(i);			// �ͷŽڵ�

		i = next;
	}

	free(list);				// �ͷ�����
}
