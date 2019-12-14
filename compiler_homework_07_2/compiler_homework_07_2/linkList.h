/***************************************************************************************
 * @文件名: linkList.h
 * @文件描述: 该文件为双向循环链表库函数的头文件，包含所有功能函数的声明，链表和节点的封装等其它声明
 ***************************************************************************************/
#pragma once

 /* 链表节点封装 */
typedef struct node
{
	char name[0x800];	// 标识符名
	int data;			// 数据
	struct node* prev;	// 前一个节点
	struct node* next;	// 下一个节点
} Node_t;

/* 链表类型封装 */
typedef struct list
{
	int n;				// 链表中节点的个数
	struct node head;		// 链表头节点'
} List_t;


/********* 以下为功能函数声明 *********/

/**
 * @函数名: newList
 * @函数功能: 初始化链表
 * @参数: 无
 * @返回值: 初始化成功返回链表类型的指针，失败返回NULL
 */
List_t* newList();

/**
 * @函数名: insertNode
 * @函数功能: 双向链表新节点插入函数，只需提供新节点的前后节点地址
 *            就可以把节点插入到链表之中
 * @参数: P:新节点的前一个节点的地址  N:新节点的后一个节点的地址
 *        newNode:新节点地址
 * @返回值: void
 */
void insertNode(Node_t* P, Node_t* N, Node_t* newNode);

/**
 * @函数名: appendListTop
 * @函数功能: 把节点以头插入方式挂载到链表中
 * @参数: list:链表地址  data:数据地址
 * @返回值: 插入成功返回0，失败返回非零
 */
int appendListTop(List_t* list, int data);

/**
 * @函数名: appendListTail
 * @函数功能: 把节点以尾插入方式挂载到链表中
 * @参数: list:链表地址  data:数据地址
 * @返回值: 插入成功返回0，失败返回非零
 */
int appendListTail(List_t* list, int data);

/**
 * @函数名: deleteListTop
 * @函数功能: 删除链表头部节点
 * @参数: list:链表地址
 * @返回值: 删除成功返回0，失败返回非零
 */
int deleteListTop(List_t* list);

/**
 * @函数名: deleteListTail
 * @函数功能: 删除链表尾部节点
 * @参数: list:链表地址
 * @返回值: 删除成功返回0，失败返回非零
 */
int deleteListTail(List_t* list);

/**
 * @函数名: getListTopNode
 * @函数功能: 获得链表头部节点
 * @参数: list:链表地址
 * @返回值: Node_t*
 */
Node_t* getListTopNode(List_t* list);
/**
 * @函数名: getListTailNode
 * @函数功能: 获得链表尾部节点
 * @参数: list:链表地址
 * @返回值: Node_t*
 */
Node_t* getListTailNode(List_t* list);

/**
 * @函数名: getListNextNode
 * @函数功能: 获得当前节点的下一节点
 * @参数: list:链表地址
 * @返回值: Node_t*
 */
Node_t* getListNextNode(Node_t* node);

/**
 * @函数名: getListPrevNode
 * @函数功能: 获得当前节点的上一节点
 * @参数: node:当前节点
 * @返回值: Node_t*
 */
Node_t* getListPrevNode(Node_t* node);

/**
 * @函数名: isEmptyList
 * @函数功能: 判断链表是否为空
 * @参数: list:需要判断的链表地址
 * @返回值: 链表为空返回1，不为空返回0
 */
int isEmptyList(List_t* list);

/**
 * @函数名: destroyList
 * @函数功能: 销毁链表
 * @参数: list:需要销毁的链表地址
 * @返回值: 无
 */
void destroyList(List_t* list);


/* MY */
int appendListTailWithName(List_t* list, char* name, int data);

