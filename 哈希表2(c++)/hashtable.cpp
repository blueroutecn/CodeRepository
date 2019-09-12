#include"head.h"




/*--- 哈希表实现方案1 ----------------------------*/

int m = 12;                 // 散列表长，全局变量。

int hash(int key)
{
	return key % m;
}

Status InitHashTable(HashTable *ht)
{
	m = HASHSIZE;
	ht->count = m;
	ht->elem = new int[m];

	if (ht->elem == nullptr) return UNSUCCESS;

	// 指针指向的HASHSIZE内存初始化为空值。
	for (int i = 0; i < m; i++)
		ht->elem[i] = NULLKEY;      
	return SUCCESS;
}

void InsertHashTable(HashTable *ht, int key)
{
	int add = hash(key);

	// 采用线性探查法检查地址是否冲突。
	// 这里不检查key是否已经存在。
	while (ht->elem[add] != NULLKEY )
	{
		// 这里依然采用取余的方法，是为了避免越界。
		add = (add + 1) % m;

		// 避免陷入无限循环。
		if (add == hash(key))
		{
			cout << "don't have mem for " << key << endl;
			return;
		}
	}

	ht->elem[add] = key;
}

Status FindHashTable(HashTable *ht, int key, int *add)
{
	*add = hash(key);

	// 检查地址是否冲突，往后查找。
	while (ht->elem[*add] != key )
	{
		*add = (*add + 1) % m;

		// 下一位是空值，即说明内存未满但没有key。
		// 地址回到原点，即说明遍历内存但没有key。
		if (ht->elem[*add] == NULLKEY || *add == hash(key))
			return UNSUCCESS;
	}

	return SUCCESS;
}




/*--- 哈希表实现方案2 ----------------------------*/

size_t hash_func(KeyType key)
{
	return key % HashMaxSize;
}


void HashInit(HashTable2 *ht, HashFunc func)
{
	if (ht == nullptr) ht = (HashTable2*)malloc(sizeof(HashTable2));

	for (int i = 0; i < HashMaxSize; i++)
	{
		ht->data[i] = nullptr;
	}

	ht->size = 0;
	ht->func = func;
}


void HashDestroy(HashTable2 *ht)
{
	if (ht == nullptr) return;

	for (int i = 0; i < HashMaxSize; i++)
	{
		HashElem *cur = ht->data[i];
		HashElem *to_del = nullptr;

		while (cur)
		{
			to_del = cur->next;
			delete cur;

			cur = to_del;
			to_del = nullptr;   // 将其置为空，保险起见。
		}

		ht->size = 0;
	}
}


HashElem* CreateNode(KeyType key, ValType val)
{
	//HashElem *elem = (HashElem*)malloc(sizeof(HashElem));
	//if (elem == NULL) return NULL;

	HashElem *elem =( HashElem*)malloc(sizeof(HashElem));
	if (elem == nullptr) return nullptr;

	elem->key = key; 
	elem->val = val; 

	// 一般若地址不冲突，则每个元素就是一个节点（所以哈希表中的节点间无连接关系）；
	// 地址冲突时，将相同地址的元素都保存在该节点，通过在该节点处开辟链表来存储
	// 所以每个HashElem代表所有相同地址的哈希表元素。
	elem->next = nullptr;

	return elem;
}


bool HashInsert(HashTable2 *ht, HashElem elem)
{
	if (ht == nullptr) return false; // 无法初始化，只能返回。

	// 利用哈希函数计算键值（地址，编号）。
	// 不检查键key是否有重复。
	int offset = ht->func(elem.key);

	// 如果该地址未使用，则插入在该点。
	if (ht->data[offset] == nullptr)
	{
		ht->data[offset] = CreateNode(elem.key, elem.val);
	}
	// 如果该处已有数据（地址冲突），则在该节点处开辟链表。
	else
	{
		HashElem *node = CreateNode(elem.key, elem.val);

		node->next = ht->data[offset];
		ht->data[offset] = node; 
	}

	ht->size++;
}


ValType HashFind(HashTable2 *ht, KeyType key)
{
	if (ht == nullptr) return -1;

	size_t offset = ht->func(key); 
	HashElem *cur = ht->data[offset];

	// 在该节点处搜索相同地址的所有元素。
	while (cur)
	{
		if (cur->key == key)return cur->val;

		// 链表结构，不需要检查边界。
		cur = cur->next; 
	}

	cout << key << " :not found in the hashtable." << endl;
	return -1;
}


bool HashRemove(HashTable2 *ht, KeyType key)
{
	if (ht == NULL) return false;

	size_t offset = ht->func(key);
	HashElem *cur = ht->data[offset];
	HashElem *pre_cur = NULL;

	while (cur)
	{
		if (cur->key == key)break;

		pre_cur = cur;
		cur = cur->next;
	}

	if (cur == NULL)
	{
		cout << "not found --" << key << endl;
		return false;
	}
	else
	{		
		if(cur->next)  // 如果该节点还有其他数据（同地址数据），则保存地址。
			pre_cur = cur->next;

		free(cur);
		cur = NULL;    // free（）之后，指针依然可用，故设为空指针。

		ht->data[offset] = pre_cur;  // 更新节点元素。
		ht->size--;

		return true;
	}
}


void HashPrint(HashTable2 *ht)
{
	if (ht == NULL) return;

	cout << endl;
	cout << "hashtable size is " << ht->size << endl;
	for (int i = 0; i < HashMaxSize; i++)
	{
		if (ht->data[i] == NULL) continue;

		cout << "node " << i << ": data below:" << endl;
		HashElem *cur = ht->data[i];

		while (cur)
		{
			cout << cur->key << " ; " << cur->val << endl;
			cur = cur->next;
		}
	}
	cout << endl;
}


