#include"head.h"




/*--- ��ϣ��ʵ�ַ���1 ----------------------------*/

int m = 12;                 // ɢ�б���ȫ�ֱ�����

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

	// ָ��ָ���HASHSIZE�ڴ��ʼ��Ϊ��ֵ��
	for (int i = 0; i < m; i++)
		ht->elem[i] = NULLKEY;      
	return SUCCESS;
}

void InsertHashTable(HashTable *ht, int key)
{
	int add = hash(key);

	// ��������̽�鷨����ַ�Ƿ��ͻ��
	// ���ﲻ���key�Ƿ��Ѿ����ڡ�
	while (ht->elem[add] != NULLKEY )
	{
		// ������Ȼ����ȡ��ķ�������Ϊ�˱���Խ�硣
		add = (add + 1) % m;

		// ������������ѭ����
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

	// ����ַ�Ƿ��ͻ��������ҡ�
	while (ht->elem[*add] != key )
	{
		*add = (*add + 1) % m;

		// ��һλ�ǿ�ֵ����˵���ڴ�δ����û��key��
		// ��ַ�ص�ԭ�㣬��˵�������ڴ浫û��key��
		if (ht->elem[*add] == NULLKEY || *add == hash(key))
			return UNSUCCESS;
	}

	return SUCCESS;
}




/*--- ��ϣ��ʵ�ַ���2 ----------------------------*/

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
			to_del = nullptr;   // ������Ϊ�գ����������
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

	// һ������ַ����ͻ����ÿ��Ԫ�ؾ���һ���ڵ㣨���Թ�ϣ���еĽڵ�������ӹ�ϵ����
	// ��ַ��ͻʱ������ͬ��ַ��Ԫ�ض������ڸýڵ㣬ͨ���ڸýڵ㴦�����������洢
	// ����ÿ��HashElem����������ͬ��ַ�Ĺ�ϣ��Ԫ�ء�
	elem->next = nullptr;

	return elem;
}


bool HashInsert(HashTable2 *ht, HashElem elem)
{
	if (ht == nullptr) return false; // �޷���ʼ����ֻ�ܷ��ء�

	// ���ù�ϣ���������ֵ����ַ����ţ���
	// ������key�Ƿ����ظ���
	int offset = ht->func(elem.key);

	// ����õ�ַδʹ�ã�������ڸõ㡣
	if (ht->data[offset] == nullptr)
	{
		ht->data[offset] = CreateNode(elem.key, elem.val);
	}
	// ����ô��������ݣ���ַ��ͻ�������ڸýڵ㴦��������
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

	// �ڸýڵ㴦������ͬ��ַ������Ԫ�ء�
	while (cur)
	{
		if (cur->key == key)return cur->val;

		// ����ṹ������Ҫ���߽硣
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
		if(cur->next)  // ����ýڵ㻹���������ݣ�ͬ��ַ���ݣ����򱣴��ַ��
			pre_cur = cur->next;

		free(cur);
		cur = NULL;    // free����֮��ָ����Ȼ���ã�����Ϊ��ָ�롣

		ht->data[offset] = pre_cur;  // ���½ڵ�Ԫ�ء�
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


