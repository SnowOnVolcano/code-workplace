/***************************************************************************************
 * @文件名: list.h
 * @文件描述: 该文件为双向循环链表库函数的头文件，包含所有功能函数的声明，链表和节点的封装等其它声明
 ***************************************************************************************/
#ifndef _LIST_H_
#define _LIST_H_

 /* 链表节点封装 */
typedef struct node
{
	void* datap;		// 数据地址
	struct node* prev;	// 前一个节点
	struct node* next;	// 下一个节点
} Node_t;

/* 链表类型封装 */
typedef struct list
{
	int n;				// 链表中节点的个数
	int size;			// 链表节点中存放数据的空间大小
	Node_t head;		// 链表头节点
} List_t;

/* 回调函数类型定义 */
typedef int compare_t(void* data1, void* data2);

/********* 以下为功能函数声明 *********/

/**
 * @函数名: initialList
 * @函数功能: 初始化链表
 * @参数: size: 链表节点的空间大小
 * @返回值: 初始化成功返回链表类型的指针，失败返回NULL
 */
List_t* initialList(int size);

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
int appendListTop(List_t* list, void* data);

/**
 * @函数名: appendListTail
 * @函数功能: 把节点以尾插入方式挂载到链表中
 * @参数: list:链表地址  data:数据地址
 * @返回值: 插入成功返回0，失败返回非零
 */
int appendListTail(List_t* list, void* data);

/**
 * @函数名: sortInsert
 * @函数功能: 以从小到大的顺序把节点插入到链表中
 * @参数: list:链表地址  func:用于比较数据的回调函数  data:数据地址
 * @返回值: 插入成功返回0，失败返回非零
 */
int sortInsert(List_t* list, compare_t* func, void* data);

/**
 * @函数名: searchListOneByItem
 * @函数功能: 根据条件查找节点，如果有多个匹配则查找第一个
 * @参数: list:链表地址  func:用于比较数据的回调函数  item:条件地址
 * @返回值: 查找成功返回节点地址，失败返回NULL
 */
void* searchListOneByItem(List_t* list, compare_t* func, void* item);

/**
 * @函数名: searchListByItem
 * @函数功能: 根据条件查找节点，如果有多个匹配则全部查找
 * @参数: list:链表地址  func:用于比较数据的回调函数  item:条件地址
 * @返回值: 查找成功返回结果链表地址，失败返回NULL
 */
List_t* searchListByItem(List_t* list, compare_t* func, void* item);

/**
 * @函数名: deleteListOneByItem
 * @函数功能: 根据条件删除节点，如果有多个匹配则删除第一个
 * @参数: list:链表地址  func:用于比较数据的回调函数  item:条件地址
 * @返回值: 删除成功返回0，失败返回非零
 */
int deleteListOneByItem(List_t* list, compare_t* func, void* item);

/**
 * @函数名: deleteListByItem
 * @函数功能: 根据条件删除节点，如果有多个匹配则删除全部
 * @参数: list:链表地址  func:用于比较数据的回调函数  item:条件地址
 * @返回值: 返回成功删除的个数
 */
int deleteListByItem(List_t* list, compare_t* func, void* item);

/**
 * @函数名: sortList
 * @函数功能: 对链表进行排序
 * @参数: list:链表地址  func:用于比较数据的回调函数
 * @返回值: void
 */
void sortList(List_t* list, compare_t* func);

/**
 * @函数名: saveListToFile
 * @函数功能: 把链表保存到文件中
 * @参数: list:链表地址  file:用于保存链表的文件名
 * @返回值: 成功保存返回0，失败返回非零
 */
int saveListToFile(List_t* list, char* file);

/**
 * @函数名: loadListFromFile
 * @函数功能: 从文件中加载链表
 * @参数: file:用于加载链表的文件名
 * @返回值: 加载成功返回链表地址，失败返回NULL
 */
List_t* loadListFromFile(char* file);

/**
 * @函数名: traverseListForward
 * @函数功能: 前向遍历链表
 * @参数: list:链表地址  func:用于打印数据的回调函数
 * @返回值: void
 */
void traverseListForward(List_t* list, void (*func)(void* data));

/**
 * @函数名: traverseListBackward
 * @函数功能: 后向遍历链表
 * @参数: list:链表地址  func:用于打印数据的回调函数
 * @返回值: void
 */
void traverseListBackward(List_t* list, void (*func)(void* data));

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

#endif //_LIST_H_
