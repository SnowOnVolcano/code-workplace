/***************************************************************************************
 * @�ļ���: linkList.h
 * @�ļ�����: ���ļ�Ϊ˫��ѭ������⺯����ͷ�ļ����������й��ܺ���������������ͽڵ�ķ�װ����������
 ***************************************************************************************/
#pragma once

 /* ����ڵ��װ */
typedef struct node
{
	char name[0x800];	// ��ʶ����
	int data;			// ����
	struct node* prev;	// ǰһ���ڵ�
	struct node* next;	// ��һ���ڵ�
} Node_t;

/* �������ͷ�װ */
typedef struct list
{
	int n;				// �����нڵ�ĸ���
	struct node head;		// ����ͷ�ڵ�'
} List_t;


/********* ����Ϊ���ܺ������� *********/

/**
 * @������: newList
 * @��������: ��ʼ������
 * @����: ��
 * @����ֵ: ��ʼ���ɹ������������͵�ָ�룬ʧ�ܷ���NULL
 */
List_t* newList();

/**
 * @������: insertNode
 * @��������: ˫�������½ڵ���뺯����ֻ���ṩ�½ڵ��ǰ��ڵ��ַ
 *            �Ϳ��԰ѽڵ���뵽����֮��
 * @����: P:�½ڵ��ǰһ���ڵ�ĵ�ַ  N:�½ڵ�ĺ�һ���ڵ�ĵ�ַ
 *        newNode:�½ڵ��ַ
 * @����ֵ: void
 */
void insertNode(Node_t* P, Node_t* N, Node_t* newNode);

/**
 * @������: appendListTop
 * @��������: �ѽڵ���ͷ���뷽ʽ���ص�������
 * @����: list:�����ַ  data:���ݵ�ַ
 * @����ֵ: ����ɹ�����0��ʧ�ܷ��ط���
 */
int appendListTop(List_t* list, int data);

/**
 * @������: appendListTail
 * @��������: �ѽڵ���β���뷽ʽ���ص�������
 * @����: list:�����ַ  data:���ݵ�ַ
 * @����ֵ: ����ɹ�����0��ʧ�ܷ��ط���
 */
int appendListTail(List_t* list, int data);

/**
 * @������: deleteListTop
 * @��������: ɾ������ͷ���ڵ�
 * @����: list:�����ַ
 * @����ֵ: ɾ���ɹ�����0��ʧ�ܷ��ط���
 */
int deleteListTop(List_t* list);

/**
 * @������: deleteListTail
 * @��������: ɾ������β���ڵ�
 * @����: list:�����ַ
 * @����ֵ: ɾ���ɹ�����0��ʧ�ܷ��ط���
 */
int deleteListTail(List_t* list);

/**
 * @������: getListTopNode
 * @��������: �������ͷ���ڵ�
 * @����: list:�����ַ
 * @����ֵ: Node_t*
 */
Node_t* getListTopNode(List_t* list);
/**
 * @������: getListTailNode
 * @��������: �������β���ڵ�
 * @����: list:�����ַ
 * @����ֵ: Node_t*
 */
Node_t* getListTailNode(List_t* list);

/**
 * @������: getListNextNode
 * @��������: ��õ�ǰ�ڵ����һ�ڵ�
 * @����: list:�����ַ
 * @����ֵ: Node_t*
 */
Node_t* getListNextNode(Node_t* node);

/**
 * @������: getListPrevNode
 * @��������: ��õ�ǰ�ڵ����һ�ڵ�
 * @����: node:��ǰ�ڵ�
 * @����ֵ: Node_t*
 */
Node_t* getListPrevNode(Node_t* node);

/**
 * @������: isEmptyList
 * @��������: �ж������Ƿ�Ϊ��
 * @����: list:��Ҫ�жϵ������ַ
 * @����ֵ: ����Ϊ�շ���1����Ϊ�շ���0
 */
int isEmptyList(List_t* list);

/**
 * @������: destroyList
 * @��������: ��������
 * @����: list:��Ҫ���ٵ������ַ
 * @����ֵ: ��
 */
void destroyList(List_t* list);


/* MY */
int appendListTailWithName(List_t* list, char* name, int data);

