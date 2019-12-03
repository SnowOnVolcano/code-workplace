/*****************************************************
 * @文件名: linkList.c
 * @文件描述: 该文件为双向循环链表库函数中所有功能函数的实现
 *****************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "linkList.h"

 /**
  * @函数名: newList
  * @函数功能: 初始化链表
  * @参数: size: 链表节点的空间大小
  * @返回值: 初始化成功返回链表类型的指针，失败返回NULL
  */
List_t* newList()
{
	List_t* temp;

	/* 分配链表空间 */
	temp = malloc(sizeof(List_t));
	if (temp == NULL)
	{
		return NULL;
	}

	/* 初始化链表 */
	temp->n = 0;
	temp->head.next = &temp->head;
	temp->head.prev = &temp->head;

	return temp;
}

/**
 * @函数名: insertNode
 * @函数功能: 双向链表新节点插入函数，只需提供新节点的前后节点地址
 *            就可以把节点插入到链表之中
 * @参数: P:新节点的前一个节点的地址  N:新节点的后一个节点的地址
 *        newNode:新节点地址
 * @返回值: void
 */
void insertNode(Node_t* P, Node_t* N, Node_t* newNode)
{
	/* 先改变自己 */
	newNode->next = N;	// 后向指针指向后节点
	newNode->prev = P;	// 前向指针指向前节点

	/* 再改变别人 */
	N->prev = newNode;	// 后节点的前向指针指向自己
	P->next = newNode;	// 前节点的后向指针指向自己
}



/**
 * @函数名: appendListTop
 * @函数功能: 把节点以头插入方式挂载到链表中
 * @参数: list:链表地址  data:数据地址
 * @返回值: 插入成功返回0，失败返回非零
 */
int appendListTop(List_t* list, int data)
{
	Node_t* newNode;

	/* 第一步：分配新节点空间 */
	newNode = malloc(sizeof(Node_t));
	if (NULL == newNode)
	{
		return -1;
	}

	/* 第二步：给节点中的数据赋值 */
	newNode->data = data;

	/* 第三步：把新节点挂载到链表头 */
	insertNode(&list->head, list->head.next, newNode);

	/* 第四步：链表节点数+1 */
	list->n += 1;

	return 0;	// 成功返回0
}

/**
 * @函数名: appendListTail
 * @函数功能: 把节点以尾插入方式挂载到链表中
 * @参数: list:链表地址  data:数据
 * @返回值: 插入成功返回0，失败返回非零
 */
int appendListTail(List_t* list, int data)
{
	Node_t* newNode;

	/* 第一步：分配新节点空间 */
	newNode = malloc(sizeof(Node_t));
	if (NULL == newNode)
	{
		return -1;
	}

	/* 第二步：给节点中的数据赋值 */
	newNode->data = data;

	/* 第三步：把数据挂载到链表尾 */
	insertNode(list->head.prev, &list->head, newNode);

	/* 第四步：链表节点数+1 */
	list->n += 1;

	return 0;	// 成功返回0	
}

/**
 * @函数名: deleteListTop
 * @函数功能: 删除链表头部节点
 * @参数: list:链表地址
 * @返回值: 删除成功返回0，失败返回非零
 */
int deleteListTop(List_t* list) {
	Node_t* i = list->head.next;
	i->next->prev = i->prev;
	i->prev->next = i->next;
	free(i);					// 删除节点本身
	list->n -= 1;				// 记得修改节点个数
	return 0;
}

/**
 * @函数名: deleteListTail
 * @函数功能: 删除链表尾部节点
 * @参数: list:链表地址  
 * @返回值: 删除成功返回0，失败返回非零
 */
int deleteListTail(List_t* list) {
	Node_t* i = list->head.prev;
	i->next->prev = i->prev;
	i->prev->next = i->next;
	free(i);					// 删除节点本身
	list->n -= 1;				// 记得修改节点个数
	return 0;
}

/**
 * @函数名: getListTopNode
 * @函数功能: 获得链表头部节点
 * @参数: list:链表地址
 * @返回值: Node_t*
 */
Node_t* getListTopNode(List_t* list) {
	if (list == NULL) { return NULL; }
	else { return list->head.next; }
}

/**
 * @函数名: getListTailNode
 * @函数功能: 获得链表尾部节点
 * @参数: list:链表地址
 * @返回值: Node_t*
 */
Node_t* getListTailNode(List_t* list) {
	if (list == NULL) { return NULL; }
	else { return list->head.prev; }
}

/**
 * @函数名: getListNextNode
 * @函数功能: 获得当前节点的下一节点
 * @参数: list:链表地址
 * @返回值: Node_t*
 */
Node_t* getListNextNode(Node_t* node) {
	if (node == NULL) { return NULL; }
	else { return node->next; }
}

/**
 * @函数名: getListPrevNode
 * @函数功能: 获得当前节点的上一节点
 * @参数: node:当前节点
 * @返回值: Node_t*
 */
Node_t* getListPrevNode(Node_t* node) {
	if (node == NULL) { return NULL; }
	else { return node->prev; }
}

/**
 * @函数名: isEmptyList
 * @函数功能: 判断链表是否为空
 * @参数: list:需要判断的链表地址
 * @返回值: 链表为空返回1，不为空返回0
 */
int isEmptyList(List_t* list)
{
	return list->head.next == &list->head &&
		list->head.prev == &list->head;
}

/**
 * @函数名: destroyList
 * @函数功能: 销毁链表
 * @参数: list:需要销毁的链表地址
 * @返回值: 无
 */
void destroyList(List_t* list)
{
	Node_t* i, * next;

	i = list->head.next;
	while (i != &list->head)
	{
		next = i->next;		// 先保存下一个节点

		free(i);			// 释放节点

		i = next;
	}

	free(list);				// 释放链表
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