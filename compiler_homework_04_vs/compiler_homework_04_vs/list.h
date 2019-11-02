/***************************************************************************************
 * @�ļ���: list.h
 * @�ļ�����: ���ļ�Ϊ˫��ѭ������⺯����ͷ�ļ����������й��ܺ���������������ͽڵ�ķ�װ����������
 ***************************************************************************************/
#ifndef _LIST_H_
#define _LIST_H_

 /* ����ڵ��װ */
typedef struct node
{
	void* datap;		// ���ݵ�ַ
	struct node* prev;	// ǰһ���ڵ�
	struct node* next;	// ��һ���ڵ�
} Node_t;

/* �������ͷ�װ */
typedef struct list
{
	int n;				// �����нڵ�ĸ���
	int size;			// ����ڵ��д�����ݵĿռ��С
	Node_t head;		// ����ͷ�ڵ�
} List_t;

/* �ص��������Ͷ��� */
typedef int compare_t(void* data1, void* data2);

/********* ����Ϊ���ܺ������� *********/

/**
 * @������: initialList
 * @��������: ��ʼ������
 * @����: size: ����ڵ�Ŀռ��С
 * @����ֵ: ��ʼ���ɹ������������͵�ָ�룬ʧ�ܷ���NULL
 */
List_t* initialList(int size);

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
int appendListTop(List_t* list, void* data);

/**
 * @������: appendListTail
 * @��������: �ѽڵ���β���뷽ʽ���ص�������
 * @����: list:�����ַ  data:���ݵ�ַ
 * @����ֵ: ����ɹ�����0��ʧ�ܷ��ط���
 */
int appendListTail(List_t* list, void* data);

/**
 * @������: sortInsert
 * @��������: �Դ�С�����˳��ѽڵ���뵽������
 * @����: list:�����ַ  func:���ڱȽ����ݵĻص�����  data:���ݵ�ַ
 * @����ֵ: ����ɹ�����0��ʧ�ܷ��ط���
 */
int sortInsert(List_t* list, compare_t* func, void* data);

/**
 * @������: searchListOneByItem
 * @��������: �����������ҽڵ㣬����ж��ƥ������ҵ�һ��
 * @����: list:�����ַ  func:���ڱȽ����ݵĻص�����  item:������ַ
 * @����ֵ: ���ҳɹ����ؽڵ��ַ��ʧ�ܷ���NULL
 */
void* searchListOneByItem(List_t* list, compare_t* func, void* item);

/**
 * @������: searchListByItem
 * @��������: �����������ҽڵ㣬����ж��ƥ����ȫ������
 * @����: list:�����ַ  func:���ڱȽ����ݵĻص�����  item:������ַ
 * @����ֵ: ���ҳɹ����ؽ�������ַ��ʧ�ܷ���NULL
 */
List_t* searchListByItem(List_t* list, compare_t* func, void* item);

/**
 * @������: deleteListOneByItem
 * @��������: ��������ɾ���ڵ㣬����ж��ƥ����ɾ����һ��
 * @����: list:�����ַ  func:���ڱȽ����ݵĻص�����  item:������ַ
 * @����ֵ: ɾ���ɹ�����0��ʧ�ܷ��ط���
 */
int deleteListOneByItem(List_t* list, compare_t* func, void* item);

/**
 * @������: deleteListByItem
 * @��������: ��������ɾ���ڵ㣬����ж��ƥ����ɾ��ȫ��
 * @����: list:�����ַ  func:���ڱȽ����ݵĻص�����  item:������ַ
 * @����ֵ: ���سɹ�ɾ���ĸ���
 */
int deleteListByItem(List_t* list, compare_t* func, void* item);

/**
 * @������: sortList
 * @��������: �������������
 * @����: list:�����ַ  func:���ڱȽ����ݵĻص�����
 * @����ֵ: void
 */
void sortList(List_t* list, compare_t* func);

/**
 * @������: saveListToFile
 * @��������: �������浽�ļ���
 * @����: list:�����ַ  file:���ڱ���������ļ���
 * @����ֵ: �ɹ����淵��0��ʧ�ܷ��ط���
 */
int saveListToFile(List_t* list, char* file);

/**
 * @������: loadListFromFile
 * @��������: ���ļ��м�������
 * @����: file:���ڼ���������ļ���
 * @����ֵ: ���سɹ����������ַ��ʧ�ܷ���NULL
 */
List_t* loadListFromFile(char* file);

/**
 * @������: traverseListForward
 * @��������: ǰ���������
 * @����: list:�����ַ  func:���ڴ�ӡ���ݵĻص�����
 * @����ֵ: void
 */
void traverseListForward(List_t* list, void (*func)(void* data));

/**
 * @������: traverseListBackward
 * @��������: �����������
 * @����: list:�����ַ  func:���ڴ�ӡ���ݵĻص�����
 * @����ֵ: void
 */
void traverseListBackward(List_t* list, void (*func)(void* data));

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

#endif //_LIST_H_
