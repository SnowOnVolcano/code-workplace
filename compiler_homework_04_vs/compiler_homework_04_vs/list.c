/*****************************************************
 * @文件名: DoubleList.c
 * @文件描述: 该文件为双向循环链表库函数中所有功能函数的实现
 * @编辑人: 王廷云
 * @编辑日期: 2017-11-22
 * @修改日期: 2018-2-1
 *****************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "list.h"

 /**
  * @函数名: initialList
  * @函数功能: 初始化链表
  * @参数: size: 链表节点的空间大小
  * @返回值: 初始化成功返回链表类型的指针，失败返回NULL
  */
List_t* initialList(int size)
{
	List_t* temp;

	/* 分配链表空间 */
	temp = malloc(sizeof(List_t));
	if (temp == NULL)
	{
		return NULL;
	}

	/* 初始化链表 */
	temp->size = size;
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
int appendListTop(List_t* list, void* data)
{
	Node_t* newNode;

	/* 第一步：分配新节点空间 */
	newNode = malloc(sizeof(Node_t));
	if (NULL == newNode)
	{
		return -1;
	}

	/* 第二步：给节点中的数据地址分配空间 */
	newNode->datap = malloc(list->size);
	if (NULL == newNode->datap)
	{
		free(newNode);  // 不要忘记释放前面申请成功的空间
		return -2;
	}

	/* 第三步：把新数据复制到节点中的数据地址 */
	memmove(newNode->datap, data, list->size);

	/* 第四步：把数据挂载到链表头 */
	insertNode(&list->head, list->head.next, newNode);

	/* 第五步：链表节点数+1 */
	list->n += 1;

	return 0;	// 成功返回0
}

/**
 * @函数名: appendListTail
 * @函数功能: 把节点以尾插入方式挂载到链表中
 * @参数: list:链表地址  data:数据地址
 * @返回值: 插入成功返回0，失败返回非零
 */
int appendListTail(List_t* list, void* data)
{
	Node_t* newNode;

	/* 第一步：分配新节点空间 */
	newNode = malloc(sizeof(Node_t));
	if (NULL == newNode)
	{
		return -1;
	}

	/* 第二步：给节点中的数据地址分配空间 */
	newNode->datap = malloc(list->size);
	if (NULL == newNode->datap)
	{
		free(newNode);  // 不要忘记释放前面申请成功的空间
		return -2;
	}

	/* 第三步：把新数据复制到节点中的数据地址 */
	memmove(newNode->datap, data, list->size);

	/* 第四步：把数据挂载到链表尾 */
	insertNode(list->head.prev, &list->head, newNode);

	/* 第五步：链表节点数+1 */
	list->n += 1;

	return 0;	// 成功返回0	
}

/**
 * @函数名: sortInsert
 * @函数功能: 以从小到大的顺序把节点插入到链表中
 * @参数: list:链表地址  func:用于比较数据的回调函数  data:数据地址
 * @返回值: 插入成功返回0，失败返回非零
 */
int sortInsert(List_t* list, compare_t* func, void* data)
{
	Node_t* newNode;

	/* 第一步：分配新节点空间 */
	newNode = malloc(sizeof(Node_t));
	if (NULL == newNode)
	{
		return -1;
	}

	/* 第二步：给节点中的数据地址分配空间 */
	newNode->datap = malloc(list->size);
	if (NULL == newNode->datap)
	{
		free(newNode);  // 不要忘记释放前面申请成功的空间
		return -2;
	}

	/* 第三步：把新数据复制到节点中的数据地址 */
	memmove(newNode->datap, data, list->size);

	/* 第四步：找到合适的插入点 */
	Node_t* i;
	for (i = list->head.next; i != &list->head; i = i->next)
	{
		if (func(i->datap, data) >= 0)
		{
			break;
		}
	}

	/* 第五步：把新节点插入到合适的插入点中 */
	insertNode(i->prev, i, newNode);

	/* 第六步：链表节点数+1 */
	list->n += 1;

	return 0;	// 成功返回0
}

/**
 * @函数名: searchListOneByItem
 * @函数功能: 根据条件查找节点，如果有多个匹配则查找第一个
 * @参数: list:链表地址  func:用于比较数据的回调函数  item:条件地址
 * @返回值: 查找成功返回节点地址，失败返回NULL
 */
void* searchListOneByItem(List_t* list, compare_t* func, void* item)
{
	Node_t* i;

	i = list->head.next;
	while (i != &list->head)
	{
		if (func(i->datap, item) == 1) // 回调函数返回1表示条件匹配
		{
			return i->datap;
		}
		i = i->next;
	}

	return NULL;
}

/**
 * @函数名: searchListByItem
 * @函数功能: 根据条件查找节点，如果有多个匹配则全部查找
 * @参数: list:链表地址  func:用于比较数据的回调函数  item:条件地址
 * @返回值: 查找成功返回结果链表地址，失败返回NULL
 */
List_t* searchListByItem(List_t* list, compare_t* func, void* item)
{
	List_t* result;
	Node_t* i;

	/* 创建查找的结果链表 */
	result = initialList(list->size);
	if (NULL == result)
	{
		return NULL;
	}

	/* 遍历查找匹配的节点 */
	i = list->head.next;
	while (i != &list->head)
	{
		if (func(i->datap, item) == 1)	// 回调函数返回1表示条件匹配
		{
			appendListTail(result, i->datap);
		}
		i = i->next;
	}

	/* 判断查找是否成功 */
	if (0 == result->n)
	{
		return NULL;
	}

	return result;
}

/**
 * @函数名: deleteListOneByItem
 * @函数功能: 根据条件删除节点，如果有多个匹配则删除第一个
 * @参数: list:链表地址  func:用于比较数据的回调函数  item:条件地址
 * @返回值: 删除成功返回0，失败返回非零
 */
int deleteListOneByItem(List_t* list, compare_t* func, void* item)
{
	Node_t* i = list->head.next;		// 第一个节点

	/* 查找删除 */
	while (i != &list->head)
	{
		if (func(i->datap, item) == 1)	// 回调函数返回1表示条件匹配
		{
			i->next->prev = i->prev;
			i->prev->next = i->next;
			free(i->datap);				// 删除节点中的数据
			free(i);					// 删除节点本身
			list->n -= 1;				// 记得修改节点个数
			return 0;
		}
		i = i->next;
	}

	return -1;
}

/**
 * @函数名: deleteListByItem
 * @函数功能: 根据条件删除节点，如果有多个匹配则删除全部
 * @参数: list:链表地址  func:用于比较数据的回调函数  item:条件地址
 * @返回值: 返回成功删除的个数
 */
int deleteListByItem(List_t* list, compare_t* func, void* item)
{
	int count = 0; 		 // 统计删除的个数
	Node_t* i, * next;

	/* 查找删除 */
	i = list->head.next; //	第一个节点
	while (i != &list->head)
	{
		if (func(i->datap, item) == 1)	// 回调函数返回1表示条件匹配
		{
			next = i->next;				// 先保存好下一个节点

			/* 删除 */
			i->next->prev = i->prev;
			i->prev->next = i->next;
			free(i->datap);				// 删除节点中的数据
			free(i);					// 删除节点本身
			list->n -= 1;				// 记得修改节点个数

			count++;					// 累加删除个数
			i = next;					// 下一个继续
			continue;
		}
		i = i->next;
	}

	return count;						// 返回成功删除的个数
}

/**
 * @函数名: sortList
 * @函数功能: 对链表进行排序
 * @参数: list:链表地址  func:用于比较数据的回调函数
 * @返回值: void
 */
void sortList(List_t* list, compare_t* func)
{
	Node_t* i, * j;
	void* temp;

	/* 冒泡排序法 */
	for (i = list->head.next; i != &list->head; i = i->next)
	{
		for (j = i->next; j != &list->head; j = j->next)
		{
			if (func(i->datap, j->datap) == 1)	// 排列顺序有回调函数决定
			{
				/* 交换节点中的数据地址 */
				temp = i->datap;
				i->datap = j->datap;
				j->datap = temp;
			}
		}
	}
}

/**
 * @函数名: saveListToFile
 * @函数功能: 把链表保存到文件中
 * @参数: list:链表地址  file:用于保存链表的文件名
 * @返回值: 成功保存返回0，失败返回非零
 */
int saveListToFile(List_t* list, char* file)
{
	FILE* fout;
	int ret;

	/* 打开文件用于写操作 */
	fout = fopen(file, "w");
	if (NULL == fout)
	{
		return -1;
	}

	/* 保存链表节点中的数据空间大小 */
	ret = fwrite(&list->size, sizeof(int), 1, fout);
	if (ret != 1)
	{
		fclose(fout);
		return -2;
	}

	/* 保存链表节点数据 */
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

	/* 关闭文件保存退出 */
	fclose(fout);
	return 0;
}

/**
 * @函数名: loadListFromFile
 * @函数功能: 从文件中加载链表
 * @参数: file:用于加载链表的文件名
 * @返回值: 加载成功返回链表地址，失败返回NULL
 */
List_t* loadListFromFile(char* file)
{
	List_t* result;
	FILE* fin;
	int ret;

	/* 打开文件用于加载链表数据 */
	fin = fopen(file, "r");
	if (NULL == fin)
	{
		goto exit1;
	}

	/* 初始化结果链表 */
	result = initialList(0);
	if (NULL == result)
	{
		goto exit2;
	}

	/* 加载链表节点中的数据空间大小 */
	ret = fread(&result->size, sizeof(int), 1, fin);
	if (1 != ret)
	{
		goto exit3;
	}

	/* 加载数据节点 */
	void* data = malloc(result->size);
	if (NULL == data)
	{
		goto exit3;
	}
	while (1)
	{
		ret = fread(data, result->size, 1, fin);

		/* 判断读是否出错或读到文件尾了 */
		if (ferror(fin))
		{
			goto exit4;
		}
		else if (feof(fin))
		{
			break;
		}

		/* 把节点添加到链表尾 */
		ret = appendListTail(result, data);
		if (ret != 0)
		{
			goto exit4;
		}
	}

	/* 加载数据成功 */
	fclose(fin);
	free(data);

	return result;

	/* 出错后的层次退出 */
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
 * @函数名: traverseListForward
 * @函数功能: 前向遍历链表
 * @参数: list:链表地址  func:用于打印数据的回调函数
 * @返回值: void
 */
void traverseListForward(List_t* list, void (*func)(void* data))
{
	Node_t* i;

	for (i = list->head.next; i != &list->head; i = i->next)
	{
		func(i->datap);		// 调用回调函数打印结果
	}
}

/**
 * @函数名: traverseListBackward
 * @函数功能: 后向遍历链表
 * @参数: list:链表地址  func:用于打印数据的回调函数
 * @返回值: void
 */
void traverseListBackward(List_t* list, void (*func)(void* data))
{
	Node_t* i;

	for (i = list->head.prev; i != &list->head; i = i->prev)
	{
		func(i->datap);		// 调用回调函数打印结果
	}
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

		free(i->datap);		// 释放节点中的数据空间
		free(i);			// 释放节点

		i = next;
	}

	free(list);				// 释放链表
}
