#pragma once
#include<string>
#include<iostream>
using std::string;
using std::cout;
using std::endl;
using std::cin;




/*--- 哈希表实现方案1 ----------------------------*/

#define SUCCESS   1
#define UNSUCCESS 0
#define HASHSIZE 12         // 散列表长为数组长
#define NULLKEY -32768      // 空值
 
typedef int Status;


typedef struct HashTable
{
	int *elem;             // 元素存在动态数组。
	int count;             // 当前元素个数。
};



int hash(int key);         // 散列函数（求模取余法）。

Status InitHashTable(HashTable *ht);          // 初始化散列表。

void InsertHashTable(HashTable *ht, int key); // 向散列表插入元素。

Status FindHashTable(HashTable *ht, int key, int *add); // 在散列中查找元素。



/*--- 哈希表实现方案2 ----------------------------*/

#define HashMaxSize 1000

typedef int  KeyType;
typedef size_t ValType;


// 定义一个函数指针类型，指向size_t function(KeyType key)类型函数。
typedef size_t(*HashFunc)(KeyType key);


typedef struct HashElem
// 哈希表元素。
// 定义一个结构体类型HashEelem,同时一个同名结构体变量HashEelem。
{
	KeyType key;            // 键为整数型。
	ValType val;            // 值为字符型。
	struct HashElem *next;  // 若地址冲突，则形同地址的。
}HashElem;

typedef struct
// 这里的哈希表就是一个结构体变量，而非链表结构。
// 哈希表最大容量HashMaxSize。
{
	HashElem* data[HashMaxSize];   // 采用指针数组保存元素。
	size_t size;                   // 当前元素数量。
	HashFunc func;                 // 当前采用的哈希函数。
}HashTable2;


size_t hash_func(KeyType key);

void HashInit(HashTable2* ht, HashFunc hash_func);

void HashDestroy(HashTable2* ht);

HashElem* CreateNode(KeyType key, ValType val);

bool HashInsert(HashTable2* ht, HashElem elem);

ValType HashFind(HashTable2* ht, KeyType key);

bool  HashRemove(HashTable2* ht, KeyType key);

void HashPrint(HashTable2* ht);




