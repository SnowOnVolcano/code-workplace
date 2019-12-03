/*****************************************************
 * @�ļ���: linkList.c
 * @�ļ�����: ���ļ�Ϊ˫��ѭ������⺯�������й��ܺ�����ʵ��
 *****************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "linkList.h"

 /**
  * @������: newList
  * @��������: ��ʼ������
  * @����: size: ����ڵ�Ŀռ��С
  * @����ֵ: ��ʼ���ɹ������������͵�ָ�룬ʧ�ܷ���NULL
  */
List_t* newList()
{
	List_t* temp;

	/* ��������ռ� */
	temp = malloc(sizeof(List_t));
	if (temp == NULL)
	{
		return NULL;
	}

	/* ��ʼ������ */
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
int appendListTop(List_t* list, int data)
{
	Node_t* newNode;

	/* ��һ���������½ڵ�ռ� */
	newNode = malloc(sizeof(Node_t));
	if (NULL == newNode)
	{
		return -1;
	}

	/* �ڶ��������ڵ��е����ݸ�ֵ */
	newNode->data = data;

	/* �����������½ڵ���ص�����ͷ */
	insertNode(&list->head, list->head.next, newNode);

	/* ���Ĳ�������ڵ���+1 */
	list->n += 1;

	return 0;	// �ɹ�����0
}

/**
 * @������: appendListTail
 * @��������: �ѽڵ���β���뷽ʽ���ص�������
 * @����: list:�����ַ  data:����
 * @����ֵ: ����ɹ�����0��ʧ�ܷ��ط���
 */
int appendListTail(List_t* list, int data)
{
	Node_t* newNode;

	/* ��һ���������½ڵ�ռ� */
	newNode = malloc(sizeof(Node_t));
	if (NULL == newNode)
	{
		return -1;
	}

	/* �ڶ��������ڵ��е����ݸ�ֵ */
	newNode->data = data;

	/* �������������ݹ��ص�����β */
	insertNode(list->head.prev, &list->head, newNode);

	/* ���Ĳ�������ڵ���+1 */
	list->n += 1;

	return 0;	// �ɹ�����0	
}

/**
 * @������: deleteListTop
 * @��������: ɾ������ͷ���ڵ�
 * @����: list:�����ַ
 * @����ֵ: ɾ���ɹ�����0��ʧ�ܷ��ط���
 */
int deleteListTop(List_t* list) {
	Node_t* i = list->head.next;
	i->next->prev = i->prev;
	i->prev->next = i->next;
	free(i);					// ɾ���ڵ㱾��
	list->n -= 1;				// �ǵ��޸Ľڵ����
	return 0;
}

/**
 * @������: deleteListTail
 * @��������: ɾ������β���ڵ�
 * @����: list:�����ַ  
 * @����ֵ: ɾ���ɹ�����0��ʧ�ܷ��ط���
 */
int deleteListTail(List_t* list) {
	Node_t* i = list->head.prev;
	i->next->prev = i->prev;
	i->prev->next = i->next;
	free(i);					// ɾ���ڵ㱾��
	list->n -= 1;				// �ǵ��޸Ľڵ����
	return 0;
}

/**
 * @������: getListTopNode
 * @��������: �������ͷ���ڵ�
 * @����: list:�����ַ
 * @����ֵ: Node_t*
 */
Node_t* getListTopNode(List_t* list) {
	if (list == NULL) { return NULL; }
	else { return list->head.next; }
}

/**
 * @������: getListTailNode
 * @��������: �������β���ڵ�
 * @����: list:�����ַ
 * @����ֵ: Node_t*
 */
Node_t* getListTailNode(List_t* list) {
	if (list == NULL) { return NULL; }
	else { return list->head.prev; }
}

/**
 * @������: getListNextNode
 * @��������: ��õ�ǰ�ڵ����һ�ڵ�
 * @����: list:�����ַ
 * @����ֵ: Node_t*
 */
Node_t* getListNextNode(Node_t* node) {
	if (node == NULL) { return NULL; }
	else { return node->next; }
}

/**
 * @������: getListPrevNode
 * @��������: ��õ�ǰ�ڵ����һ�ڵ�
 * @����: node:��ǰ�ڵ�
 * @����ֵ: Node_t*
 */
Node_t* getListPrevNode(Node_t* node) {
	if (node == NULL) { return NULL; }
	else { return node->prev; }
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

		free(i);			// �ͷŽڵ�

		i = next;
	}

	free(list);				// �ͷ�����
}



/* MY */
int appendListTailWithName(List_t* list, char* name, int data)
{
	Node_t* newNode;
	newNode = malloc(sizeof(Node_t));
	
	if (NULL == newNode) { return -1; }

	strcpy(newNode->name, name);
	newNode->data = data;

	insertNode(list->head.prev, &list->head, newNode);
	list->n += 1;

	return 0;	
}