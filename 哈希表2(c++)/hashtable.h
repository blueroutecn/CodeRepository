#pragma once
#include<string>
#include<iostream>
using std::string;
using std::cout;
using std::endl;
using std::cin;




/*--- ��ϣ��ʵ�ַ���1 ----------------------------*/

#define SUCCESS   1
#define UNSUCCESS 0
#define HASHSIZE 12         // ɢ�б�Ϊ���鳤
#define NULLKEY -32768      // ��ֵ
 
typedef int Status;


typedef struct HashTable
{
	int *elem;             // Ԫ�ش��ڶ�̬���顣
	int count;             // ��ǰԪ�ظ�����
};



int hash(int key);         // ɢ�к�������ģȡ�෨����

Status InitHashTable(HashTable *ht);          // ��ʼ��ɢ�б�

void InsertHashTable(HashTable *ht, int key); // ��ɢ�б����Ԫ�ء�

Status FindHashTable(HashTable *ht, int key, int *add); // ��ɢ���в���Ԫ�ء�



/*--- ��ϣ��ʵ�ַ���2 ----------------------------*/

#define HashMaxSize 1000

typedef int  KeyType;
typedef size_t ValType;


// ����һ������ָ�����ͣ�ָ��size_t function(KeyType key)���ͺ�����
typedef size_t(*HashFunc)(KeyType key);


typedef struct HashElem
// ��ϣ��Ԫ�ء�
// ����һ���ṹ������HashEelem,ͬʱһ��ͬ���ṹ�����HashEelem��
{
	KeyType key;            // ��Ϊ�����͡�
	ValType val;            // ֵΪ�ַ��͡�
	struct HashElem *next;  // ����ַ��ͻ������ͬ��ַ�ġ�
}HashElem;

typedef struct
// ����Ĺ�ϣ�����һ���ṹ���������������ṹ��
// ��ϣ���������HashMaxSize��
{
	HashElem* data[HashMaxSize];   // ����ָ�����鱣��Ԫ�ء�
	size_t size;                   // ��ǰԪ��������
	HashFunc func;                 // ��ǰ���õĹ�ϣ������
}HashTable2;


size_t hash_func(KeyType key);

void HashInit(HashTable2* ht, HashFunc hash_func);

void HashDestroy(HashTable2* ht);

HashElem* CreateNode(KeyType key, ValType val);

bool HashInsert(HashTable2* ht, HashElem elem);

ValType HashFind(HashTable2* ht, KeyType key);

bool  HashRemove(HashTable2* ht, KeyType key);

void HashPrint(HashTable2* ht);




