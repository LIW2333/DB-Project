#include "IndexManager.h"

IndexManager::IndexManager() {
	file_name = "";
	indexFileName = "";
	configFileName = "";
	root = -1;
}

IndexManager::~IndexManager() {
	if (file_name != "")
		emptyBplusTree();
	// cout << "~IndexManager()" << endl;
	// flushAll();
}

void IndexManager::showCurrentTree() {
	cout << "file_name: " << file_name << endl;
	cout << "indexFileName: " << indexFileName << endl;
	cout << "configFileName: " << configFileName << endl;
	cout << "root: " << root << endl;
}

int IndexManager::createBplusTree(TargetInt t)
{
	if (file_name != "")
		emptyBplusTree();
	string name = t.indexFileName;
	string iName = name + ".index";
	string cName = name + ".config";

/*	if (_access(iName.c_str(), F_OK) == 0)
	{
		cout << "The index file has already existed!" << endl;
		return -1;
	}*/
	
	{
		FILE* BFile;
		FILE* configFile;
	
				file_name = name;
				indexFileName = iName;
				configFileName = cName;

				emptyNodeList ENL1 = { 1, 0 };
				BufferNode& temp_bf = getBlock(INDEX, configFileName, 0);
				temp_bf.writeBlock(ENL1, sizeof(emptyNodeList), 0);

				emptyNodeList ENL2 = { 0, 1, 0 };
				BufferNode& temp_bf2 = getBlock(INDEX, configFileName, 1);
				temp_bf2.writeBlock(ENL2, sizeof(emptyNodeList), 0);

				root = 1;

				BplusNodeInt r = { 0 };
				r.nkey = 0;
				r.isleaf = true;
				writeBplusNode(root, r);

				rootTable[file_name] = root;


				return 0;

			
	}
}

int IndexManager::createBplusTree(TargetFloat t)
{
	if (file_name != "")
		emptyBplusTree();
	string name = t.indexFileName;
	string iName = name + ".index";
	string cName = name + ".config";


				file_name = name;
				indexFileName = iName;
				configFileName = cName;

				emptyNodeList ENL1 = { 1, 0 };
				BufferNode& temp_bf = getBlock(INDEX, configFileName, 0);
				temp_bf.writeBlock(ENL1, sizeof(emptyNodeList), 0);

				emptyNodeList ENL2 = { 0, 1, 0 };
				BufferNode& temp_bf2 = getBlock(INDEX, configFileName, 1);
				temp_bf2.writeBlock(ENL2, sizeof(emptyNodeList), 0);

				root = 1;
				BplusNodeFloat r = { 0 };
				r.nkey = 0;
				r.isleaf = true;
				writeBplusNode(root, r);

				rootTable[file_name] = root;


				return 0;

		
}

int IndexManager::createBplusTree(TargetChar t)
{
	if (file_name != "")
		emptyBplusTree();
	string name = t.indexFileName;
	string iName = name + ".index";
	string cName = name + ".config";
	
				file_name = name;
				indexFileName = iName;
				configFileName = cName;

				emptyNodeList ENL1 = { 1, 0 };
				BufferNode& temp_bf = getBlock(INDEX, configFileName, 0);
				temp_bf.writeBlock(ENL1, sizeof(emptyNodeList), 0);

				emptyNodeList ENL2 = { 0, 1, 0 };
				BufferNode& temp_bf2 = getBlock(INDEX, configFileName, 1);
				temp_bf2.writeBlock(ENL2, sizeof(emptyNodeList), 0);

				root = 1;

				BplusNodeChar r = { 0 };
				r.nkey = 0;
				r.isleaf = true;
				writeBplusNode(root, r);

				rootTable[file_name] = root;

				
				return 0;

		
}


void IndexManager::emptyBplusTree()
{
	if (file_name != "")
	{
		BufferNode& temp_bf = getBlock(INDEX, configFileName, 0);
		temp_bf.writeBlock(root, sizeof(ADDRESS), 0);
		rootTable[file_name] = root;
	}

	file_name = "";
	indexFileName = "";
	configFileName = "";
	root = -1;
}

int  IndexManager::switchBplusTree(string name)
{
	if (file_name != name)
	{
		emptyBplusTree();

		if (rootTable.count(name) == 1)
		{
			file_name = name;
			indexFileName = name + ".index";
			configFileName = name + ".config";
			root = rootTable[file_name];
			return 0;
		}
		else
		{
			string iName = name + ".index";
			if (_access(iName.c_str(), F_OK) == 0)
			{
				file_name = name;
				indexFileName = iName;
				configFileName = name + ".config";
				BufferNode& temp_bf = getBlock(INDEX, configFileName, 0);
				temp_bf.readBlock(root, sizeof(ADDRESS), 0);
				rootTable[file_name] = root;

				return 0;
			}
			else
			{
				cout << "The index file dose not exist!" << endl;
				return -1;
			}
		}
	}
	else
		return 0;
}

void IndexManager::readBplusNode(ADDRESS add, BplusNodeInt& node)
{
	if (add <= 0)
	{
		cerr << "the node you are to read dose not exist!" << endl;
		cout << "node ID: " << add;
		cout << "file name: " << file_name;
	}
	else
	{
		BufferNode& temp_bf = getBlock(INDEX, indexFileName, add - 1);
		temp_bf.readBlock(node, sizeof(BplusNodeInt), 0);
	}
}

void IndexManager::readBplusNode(ADDRESS add, BplusNodeFloat& node)
{
	if (add <= 0)
	{
		cerr << "the node you are to read dose not exist!" << endl;
		cout << "node ID: " << add;
		cout << "file name: " << file_name;
	}
	else
	{
		BufferNode& temp_bf = getBlock(INDEX, indexFileName, add - 1);
		temp_bf.readBlock(node, sizeof(BplusNodeFloat), 0);
	}
}
void IndexManager::readBplusNode(ADDRESS add, BplusNodeChar& node)
{
	if (add <= 0)
	{
		cerr << "the node you are to read dose not exist!" << endl;
		cout << "node ID: " << add;
		cout << "file name: " << file_name;
	}
	else
	{
		BufferNode& temp_bf = getBlock(INDEX, indexFileName, add - 1);
		temp_bf.readBlock(node, sizeof(BplusNodeChar), 0);
	}
}

void IndexManager::writeBplusNode(ADDRESS add, BplusNodeInt& node)
{
	if (add <= 0)
	{
		cerr << "the node you are to write dose not exist!" << endl;
		cout << "node ID: " << add;
		cout << "file name: " << file_name;
	}
	else
	{
		BufferNode& temp_bf = getBlock(INDEX, indexFileName, add - 1);
		temp_bf.writeBlock(node, sizeof(BplusNodeInt), 0);
	}
}

void IndexManager::writeBplusNode(ADDRESS add, BplusNodeFloat& node)
{
	if (add <= 0)
	{
		cerr << "the node you are to write dose not exist!" << endl;
		cout << "node ID: " << add;
		cout << "file name: " << file_name;
	}
	else
	{
		BufferNode& temp_bf = getBlock(INDEX, indexFileName, add - 1);
		temp_bf.writeBlock(node, sizeof(BplusNodeFloat), 0);
	}
}
void IndexManager::writeBplusNode(ADDRESS add, BplusNodeChar& node)
{
	if (add <= 0)
	{
		cerr << "the node you are to write dose not exist!" << endl;
		cout << "node ID: " << add;
		cout << "file name: " << file_name;
	}
	else
	{
		BufferNode& temp_bf = getBlock(INDEX, indexFileName, add - 1);
		temp_bf.writeBlock(node, sizeof(BplusNodeChar), 0);
	}
}

void IndexManager::insertBplusTree(TargetInt& tar)
{
	BplusNodeInt r;
	readBplusNode(root, r);

	if (r.nkey == MAX_INT)
	{
		BplusNodeInt newRoot;
		newRoot.nkey = 0;
		newRoot.isleaf = false;
		newRoot.Pointer[0] = root;

		splitBplusNodeInt(newRoot, r, 0);
		writeBplusNode(root, r);

		root = newBplusNode();

		writeBplusNode(root, newRoot);

		//???????????????
	}
	insertKey(root, tar);
}

void IndexManager::insertBplusTree(TargetFloat& tar)
{
	BplusNodeFloat r;
	readBplusNode(root, r);

	if (r.nkey == MAX_FLOAT)
	{
		BplusNodeFloat newRoot;
		newRoot.nkey = 0;
		newRoot.isleaf = false;
		newRoot.Pointer[0] = root;

		splitBplusNodeFloat(newRoot, r, 0);
		writeBplusNode(root, r);

		root = newBplusNode();

		writeBplusNode(root, newRoot);
		//???????????????
	}
	insertKey(root, tar);
}

void IndexManager::insertBplusTree(TargetChar& tar)
{
	BplusNodeChar r;
	readBplusNode(root, r);

	if (r.nkey == MAX_CHAR)
	{
		BplusNodeChar newRoot;
		newRoot.nkey = 0;
		newRoot.isleaf = false;
		newRoot.Pointer[0] = root;

		splitBplusNodeChar(newRoot, r, 0);
		writeBplusNode(root, r);

		root = newBplusNode();

		writeBplusNode(root, newRoot);

		//???????????????
	}
	insertKey(root, tar);
}

void IndexManager::insertKey(ADDRESS current, TargetInt& tar)
{
	BplusNodeInt x;
	readBplusNode(current, x);

	int i;
	for (i = 0; i < x.nkey && x.key[i] < tar.key; i++);

	if (i < x.nkey && x.isleaf && x.key[i] == tar.key)  //???B+????????????????????????????????????
	{
		//?????????????????????
		cerr << "The key '" << tar.key << "' has already existed in the index!" << endl;
		return;
	}

	if (!x.isleaf)    //?????????????????????
	{
		BplusNodeInt y;
		readBplusNode(x.Pointer[i], y);

		if (y.nkey == MAX_INT)     //??????x?????????????????????????????????????????????
		{
			splitBplusNodeInt(x, y, i);
			writeBplusNode(current, x);
			writeBplusNode(x.Pointer[i], y);
		}
		if (tar.key <= x.key[i] || i == x.nkey)
		{
			insertKey(x.Pointer[i], tar);
		}
		else
		{
			insertKey(x.Pointer[i + 1], tar);
		}
	}
	else          //??????????????????,???????????????????????????key?????????
	{

		for (int j = x.nkey; j > i; j--)
		{
			x.key[j] = x.key[j - 1];
			x.Pointer[j] = x.Pointer[j - 1];
		}
		x.key[i] = tar.key;
		x.nkey++;

		//????????????????????????x.Pointer[i]

		x.Pointer[i] = tar.index.offset + (tar.index.num << 12);

		writeBplusNode(current, x);

	}
}


void IndexManager::insertKey(ADDRESS current, TargetFloat& tar)
{
	BplusNodeFloat x;
	readBplusNode(current, x);

	int i;
	for (i = 0; i < x.nkey && x.key[i] < tar.key; i++);

	if (i < x.nkey && x.isleaf && x.key[i] == tar.key)  //???B+????????????????????????????????????
	{
		//?????????????????????
		cerr << "The key '" << tar.key << "' has already existed in the index!" << endl;
		return;
	}

	if (!x.isleaf)    //?????????????????????
	{
		BplusNodeFloat y;
		readBplusNode(x.Pointer[i], y);

		if (y.nkey == MAX_FLOAT)     //??????x?????????????????????????????????????????????
		{
			splitBplusNodeFloat(x, y, i);
			writeBplusNode(current, x);
			writeBplusNode(x.Pointer[i], y);
		}
		if (tar.key <= x.key[i] || i == x.nkey)
		{
			insertKey(x.Pointer[i], tar);
		}
		else
		{
			insertKey(x.Pointer[i + 1], tar);
		}

	}
	else          //??????????????????,???????????????????????????key?????????
	{

		for (int j = x.nkey; j > i; j--)
		{
			x.key[j] = x.key[j - 1];
			x.Pointer[j] = x.Pointer[j - 1];
		}
		x.key[i] = tar.key;
		x.nkey++;

		//????????????????????????x.Pointer[i]

		x.Pointer[i] = tar.index.offset + (tar.index.num << 12);

		writeBplusNode(current, x);

	}
}

void IndexManager::insertKey(ADDRESS current, TargetChar& tar)
{
	BplusNodeChar x;
	readBplusNode(current, x);

	int i;
	for (i = 0; i < x.nkey && toString(x.key[i]) < tar.key; i++);

	if (i < x.nkey && x.isleaf && toString(x.key[i]) == tar.key)  //???B+????????????????????????????????????
	{
		//?????????????????????
		cerr << "The key '" << tar.key << "' has already existed in the index!" << endl;
		return;
	}

	if (!x.isleaf)    //?????????????????????
	{
		BplusNodeChar y;
		readBplusNode(x.Pointer[i], y);

		if (y.nkey == MAX_CHAR)     //??????x?????????????????????????????????????????????
		{
			splitBplusNodeChar(x, y, i);
			writeBplusNode(current, x);
			writeBplusNode(x.Pointer[i], y);
		}
		if (tar.key <= toString(x.key[i]) || i == x.nkey)
		{
			insertKey(x.Pointer[i], tar);
		}
		else
		{
			insertKey(x.Pointer[i + 1], tar);
		}

	}
	else          //??????????????????,???????????????????????????key?????????
	{

		for (int j = x.nkey; j > i; j--)
		{
			// x.key[j] = x.key[j-1] ;
			memcpy(x.key[j], x.key[j - 1], 255);
			x.Pointer[j] = x.Pointer[j - 1];
		}
		// x.key[i] = tar.key;
		memcpy(x.key[i], tar.key.c_str(), tar.key.length());
		x.key[i][tar.key.length()] = '\0';
		x.nkey++;

		//????????????????????????x.Pointer[i]

		x.Pointer[i] = tar.index.offset + (tar.index.num << 12);

		writeBplusNode(current, x);

	}
}


void IndexManager::splitBplusNodeInt(BplusNodeInt &father, BplusNodeInt &current, const int childnum)
{
	int half = MAX_INT / 2;

	int i;

	for (i = father.nkey; i > childnum; i--)
	{
		father.key[i] = father.key[i - 1];
		father.Pointer[i + 1] = father.Pointer[i];
	}
	father.nkey++;

	BplusNodeInt t;

	ADDRESS address = newBplusNode();

	father.key[childnum] = current.key[half];
	father.Pointer[childnum + 1] = address;

	for (i = half + 1; i < MAX_INT; i++)
	{
		t.key[i - half - 1] = current.key[i];
		t.Pointer[i - half - 1] = current.Pointer[i];
	}

	t.nkey = MAX_INT - half - 1;
	t.Pointer[t.nkey] = current.Pointer[MAX_INT];

	t.isleaf = current.isleaf;

	current.nkey = half;

	if (current.isleaf)   //????????????????????????????????????
	{
		current.nkey++;
		t.Pointer[MAX_INT] = current.Pointer[MAX_INT];
		current.Pointer[MAX_INT] = address;
	}

	writeBplusNode(address, t);
}

void IndexManager::splitBplusNodeFloat(BplusNodeFloat &father, BplusNodeFloat &current, const int childnum)
{
	int half = MAX_FLOAT / 2;

	int i;

	for (i = father.nkey; i > childnum; i--)
	{
		father.key[i] = father.key[i - 1];
		father.Pointer[i + 1] = father.Pointer[i];
	}
	father.nkey++;

	BplusNodeFloat t;

	ADDRESS address = newBplusNode();

	father.key[childnum] = current.key[half];
	father.Pointer[childnum + 1] = address;

	for (i = half + 1; i < MAX_FLOAT; i++)
	{
		t.key[i - half - 1] = current.key[i];
		t.Pointer[i - half - 1] = current.Pointer[i];
	}

	t.nkey = MAX_FLOAT - half - 1;
	t.Pointer[t.nkey] = current.Pointer[MAX_FLOAT];

	t.isleaf = current.isleaf;

	current.nkey = half;

	if (current.isleaf)   //????????????????????????????????????
	{
		current.nkey++;
		t.Pointer[MAX_FLOAT] = current.Pointer[MAX_FLOAT];
		current.Pointer[MAX_FLOAT] = address;
	}

	writeBplusNode(address, t);
}

void IndexManager::splitBplusNodeChar(BplusNodeChar &father, BplusNodeChar &current, const int childnum)
{
	int half = MAX_CHAR / 2;

	int i;

	for (i = father.nkey; i > childnum; i--)
	{
		// father.key[i] = father.key[i-1] ;
		memcpy(father.key[i], father.key[i - 1], 255);
		father.Pointer[i + 1] = father.Pointer[i];
	}
	father.nkey++;

	BplusNodeChar t;

	ADDRESS address = newBplusNode();

	// father.key[childnum] = current.key[half] ;
	memcpy(father.key[childnum], current.key[half], 255);
	father.Pointer[childnum + 1] = address;

	for (i = half + 1; i < MAX_CHAR; i++)
	{
		// t.key[i-half-1] = current.key[i] ;
		memcpy(t.key[i - half - 1], current.key[i], 255);
		t.Pointer[i - half - 1] = current.Pointer[i];
	}

	t.nkey = MAX_CHAR - half - 1;
	t.Pointer[t.nkey] = current.Pointer[MAX_CHAR];

	t.isleaf = current.isleaf;

	current.nkey = half;

	if (current.isleaf)   //????????????????????????????????????
	{
		current.nkey++;
		t.Pointer[MAX_CHAR] = current.Pointer[MAX_CHAR];
		current.Pointer[MAX_CHAR] = address;
	}

	writeBplusNode(address, t);
}

int IndexManager::newBplusNode()
{
	BufferNode& temp_bf = getBlock(INDEX, configFileName, 0);
	emptyNodeList overview;
	temp_bf.readBlock(overview, sizeof(emptyNodeList), 0);

	int page;
	for (page = 0; page < BLOCK_SIZE / sizeof(int)-1; page++)
	if (overview.isUsed[page] == 0)
		break;

	if (page == BLOCK_SIZE / sizeof(int)-1)
	{
		cout << "The index file is full!" << endl;
		return -1;
	}
	else
	{
		BufferNode& temp_bf2 = getBlock(INDEX, configFileName, page + 1);
		emptyNodeList ENL;
		temp_bf2.readBlock(ENL, sizeof(emptyNodeList), 0);
		int block;
		for (block = 0; block < BLOCK_SIZE / sizeof(int)-1; block++)
		if (ENL.isUsed[block] == 0)
			break;
		ENL.isUsed[block] = 1;

		if (block == (BLOCK_SIZE / sizeof(int)-2))
		{
			overview.isUsed[page] = 1;
			temp_bf.writeBlock(overview, sizeof(emptyNodeList), 0);
		}
		temp_bf2.writeBlock(ENL, sizeof(emptyNodeList), 0);

		return page*(BLOCK_SIZE / sizeof(int)-1) + block + 1;
	}
}

void  IndexManager::enumLeafKey(TargetInt tar)
{
	BplusNodeInt head;

	readBplusNode(root, head);

	while (!head.isleaf)
	{
		readBplusNode(head.Pointer[0], head);
	}

	while (1)
	{
		for (int i = 0; i < head.nkey; i++)
		{
			cout << "key = " << head.key[i];
			cout << "block num = " << (head.Pointer[i] >> 12);
			cout << "offset = " << toOffset(head.Pointer[i]) << endl;
		}
		if (head.Pointer[MAX_INT] == 0)
			break;

		readBplusNode(head.Pointer[MAX_INT], head);
	}
}

void  IndexManager::enumLeafKey(TargetFloat tar)
{
	BplusNodeFloat head;

	readBplusNode(root, head);

	while (!head.isleaf)
	{
		readBplusNode(head.Pointer[0], head);
	}

	while (1)
	{
		for (int i = 0; i < head.nkey; i++)
		{
			printf("key = %f block num = %d offset = %d\n", head.key[i], head.Pointer[i] >> 12, toOffset(head.Pointer[i]));
			//     cout << "key = " << head.key[i];
			//     cout << "block num = " << (head.Pointer[i]>>12);
			//     cout << "offset = " << toOffset(head.Pointer[i]) << endl; 
		}
		if (head.Pointer[MAX_FLOAT] == 0)
			break;

		readBplusNode(head.Pointer[MAX_FLOAT], head);
	}
}

void  IndexManager::enumLeafKey(TargetChar tar)
{
	BplusNodeChar head;

	readBplusNode(root, head);

	while (!head.isleaf)
	{
		readBplusNode(head.Pointer[0], head);
	}

	// unsigned int temp;
	while (1)
	{
		for (int i = 0; i < head.nkey; i++)
		{
			// printf("key = %s block num = %d offset = %d\n",head.key[i], head.Pointer[i]>>12, toOffset(head.Pointer[i]) );
			//cout << "key = " << head.key[i];
			cout << "key = " << head.key[i];
			cout << " block num = " << (head.Pointer[i] >> 12);
			cout << " offset = " << toOffset(head.Pointer[i]) << endl;
		}
		if (head.Pointer[MAX_CHAR] == 0)
			break;

		readBplusNode(head.Pointer[MAX_CHAR], head);
	}
}

vector<index_info> IndexManager::searchBplusTree(TargetInt& tar, ADDRESS& resultAdd)
{
	int i;
	int flagNode = -1, flagIndex = -1;

	vector<index_info> result;
	index_info result_temp;
	ADDRESS current = root;
	ADDRESS leafNode;

	BplusNodeInt a;
	do
	{
		leafNode = current; //record the node ID of the leaf node 
		readBplusNode(current, a);

		for (i = 0; i < a.nkey && tar.key > a.key[i]; i++);

		if (i < a.nkey && a.isleaf && tar.key == a.key[i])       //???B+???????????????????????????????????????  
		{
			resultAdd = current; //?????????????????????????????????????????????  
			if (tar.type == SINGLE)
			{
				result_temp.num = a.Pointer[i] >> 12;
				result_temp.offset = toOffset(a.Pointer[i]);
				result.push_back(result_temp);
			}
			else if (tar.type == NOTLARGER)
			{
				flagIndex = i;
				flagNode = current;
			}
			else if (tar.type == LARGER)
			{
				i++;
			}
		}
		current = a.Pointer[i];
	} while (!a.isleaf);

	if (tar.type == NOTLESS || tar.type == LARGER)
	{
		for (; i < a.nkey; i++)
		{
			result_temp.num = a.Pointer[i] >> 12;
			result_temp.offset = toOffset(a.Pointer[i]);
			result.push_back(result_temp);
		}
		while (i == a.nkey && a.Pointer[MAX_INT] > 0)
		{
			current = a.Pointer[MAX_INT];
			readBplusNode(current, a);
			for (i = 0; i < a.nkey; i++)
			{
				result_temp.num = a.Pointer[i] >> 12;
				result_temp.offset = toOffset(a.Pointer[i]);
				result.push_back(result_temp);
			}
		}
	}
	else if (tar.type == NOTLARGER || tar.type == LESS)
	{
		if (flagIndex < 0)
		{
			flagNode = leafNode;
			flagIndex = i - 1;
		}
		BplusNodeInt head;
		ADDRESS headNode = root;

		readBplusNode(root, head);

		while (!head.isleaf)
		{
			headNode = head.Pointer[0];
			readBplusNode(head.Pointer[0], head);
		}
		// current = headNode;
		i = 0;
		while ((headNode != flagNode) || (headNode == flagNode && i <= flagIndex))
		{
			if (i == head.nkey)
			{
				i = 0;
				headNode = head.Pointer[MAX_INT];
				readBplusNode(headNode, head);
			}
			else
			{
				result_temp.num = head.Pointer[i] >> 12;
				result_temp.offset = toOffset(head.Pointer[i]);
				result.push_back(result_temp);
				i++;
			}
		}
	}
	return result;
}

vector<index_info> IndexManager::searchBplusTree(TargetFloat& tar, ADDRESS& resultAdd)
{
	int i;
	int flagNode = -1, flagIndex = -1;

	vector<index_info> result;
	index_info result_temp;
	ADDRESS current = root;
	ADDRESS leafNode;

	BplusNodeFloat a;
	do
	{
		leafNode = current; //record the node ID of the leaf node 
		readBplusNode(current, a);

		for (i = 0; i < a.nkey && tar.key > a.key[i]; i++);

		if (i < a.nkey && a.isleaf && tar.key == a.key[i])       //???B+???????????????????????????????????????  
		{
			resultAdd = current; //?????????????????????????????????????????????  
			if (tar.type == SINGLE)
			{
				result_temp.num = a.Pointer[i] >> 12;
				result_temp.offset = toOffset(a.Pointer[i]);
				result.push_back(result_temp);
			}
			else if (tar.type == NOTLARGER)
			{
				flagIndex = i;
				flagNode = current;
			}
			else if (tar.type == LARGER)
			{
				i++;
			}
		}
		current = a.Pointer[i];
	} while (!a.isleaf);

	if (tar.type == NOTLESS || tar.type == LARGER)
	{
		for (; i < a.nkey; i++)
		{
			result_temp.num = a.Pointer[i] >> 12;
			result_temp.offset = toOffset(a.Pointer[i]);
			result.push_back(result_temp);
		}
		while (i == a.nkey && a.Pointer[MAX_FLOAT] > 0)
		{
			current = a.Pointer[MAX_FLOAT];
			readBplusNode(current, a);
			for (i = 0; i < a.nkey; i++)
			{
				result_temp.num = a.Pointer[i] >> 12;
				result_temp.offset = toOffset(a.Pointer[i]);
				result.push_back(result_temp);
			}
		}
	}
	else if (tar.type == NOTLARGER || tar.type == LESS)
	{
		if (flagIndex < 0)
		{
			flagNode = leafNode;
			flagIndex = i - 1;
		}
		BplusNodeFloat head;
		ADDRESS headNode = root;

		readBplusNode(root, head);

		while (!head.isleaf)
		{
			headNode = head.Pointer[0];
			readBplusNode(head.Pointer[0], head);
		}
		// current = headNode;
		i = 0;
		while ((headNode != flagNode) || (headNode == flagNode && i <= flagIndex))
		{
			if (i == head.nkey)
			{
				i = 0;
				headNode = head.Pointer[MAX_FLOAT];
				readBplusNode(headNode, head);
			}
			else
			{
				result_temp.num = head.Pointer[i] >> 12;
				result_temp.offset = toOffset(head.Pointer[i]);
				result.push_back(result_temp);
				i++;
			}
		}
	}

	return result;
}

vector<index_info> IndexManager::searchBplusTree(TargetChar& tar, ADDRESS& resultAdd)
{
	int i;
	int flagNode = -1, flagIndex = -1;

	vector<index_info> result;
	index_info result_temp;
	ADDRESS current = root;
	ADDRESS leafNode;

	BplusNodeChar a;
	do
	{
		leafNode = current; //record the node ID of the leaf node 
		readBplusNode(current, a);

		for (i = 0; i < a.nkey && tar.key > toString(a.key[i]); i++);

		if (i < a.nkey && a.isleaf && tar.key == toString(a.key[i]))       //???B+???????????????????????????????????????
		{
			resultAdd = current; //?????????????????????????????????????????????  
			if (tar.type == SINGLE)
			{
				result_temp.num = a.Pointer[i] >> 12;
				result_temp.offset = toOffset(a.Pointer[i]);
				result.push_back(result_temp);
			}
			else if (tar.type == NOTLARGER)
			{
				flagIndex = i;
				flagNode = current;
			}
			else if (tar.type == LARGER)
			{
				i++;
			}
		}
		current = a.Pointer[i];
	} while (!a.isleaf);

	if (tar.type == NOTLESS || tar.type == LARGER)
	{
		for (; i < a.nkey; i++)
		{
			result_temp.num = a.Pointer[i] >> 12;
			result_temp.offset = toOffset(a.Pointer[i]);
			result.push_back(result_temp);
		}
		while (i == a.nkey && a.Pointer[MAX_CHAR] > 0)
		{
			current = a.Pointer[MAX_CHAR];
			readBplusNode(current, a);
			for (i = 0; i < a.nkey; i++)
			{
				result_temp.num = a.Pointer[i] >> 12;
				result_temp.offset = toOffset(a.Pointer[i]);
				result.push_back(result_temp);
			}
		}
	}
	else if (tar.type == NOTLARGER || tar.type == LESS)
	{
		if (flagIndex < 0)
		{
			flagNode = leafNode;
			flagIndex = i - 1;
		}
		BplusNodeChar head;
		ADDRESS headNode = root;

		readBplusNode(root, head);

		while (!head.isleaf)
		{
			headNode = head.Pointer[0];
			readBplusNode(head.Pointer[0], head);
		}
		// current = headNode;
		i = 0;
		while ((headNode != flagNode) || (headNode == flagNode && i <= flagIndex))
		{
			if (i == head.nkey)
			{
				i = 0;
				headNode = head.Pointer[MAX_CHAR];
				readBplusNode(headNode, head);
			}
			else
			{
				result_temp.num = head.Pointer[i] >> 12;
				result_temp.offset = toOffset(head.Pointer[i]);
				result.push_back(result_temp);
				i++;
			}
		}
	}

	return result;
}

int IndexManager::toOffset(int x)
{
	unsigned int temp;
	temp = x;
	temp = temp << 20;
	temp = temp >> 1;
	temp = temp & 0x7FFFFFFF;
	temp = temp >> 19;
	int offset = temp;
	return offset;
}

string IndexManager::toString(char* c)
{
	string s(c);
	return s;
}


void IndexManager::deleteBplusTree(TargetInt& tar)
{
	deleteKey(root, tar);
	BplusNodeInt rootnode;
	readBplusNode(root, rootnode);

	if (!rootnode.isleaf && rootnode.nkey == 0)    //???????????????????????????????????????????????????????????????????????????0??????????????????????????????  
	{
		//??????ROOT?????????????????????
		setNodeEmpty(root);
		root = rootnode.Pointer[0];         //???????????????,B+????????????1               
	}
}

void IndexManager::deleteBplusTree(TargetFloat& tar)
{
	deleteKey(root, tar);
	BplusNodeFloat rootnode;
	readBplusNode(root, rootnode);

	if (!rootnode.isleaf && rootnode.nkey == 0)    //???????????????????????????????????????????????????????????????????????????0??????????????????????????????  
	{
		//??????ROOT?????????????????????
		setNodeEmpty(root);
		root = rootnode.Pointer[0];         //???????????????,B+????????????1               
	}
}

void IndexManager::deleteBplusTree(TargetChar& tar)
{
	deleteKey(root, tar);
	BplusNodeChar rootnode;
	readBplusNode(root, rootnode);

	if (!rootnode.isleaf && rootnode.nkey == 0)    //???????????????????????????????????????????????????????????????????????????0??????????????????????????????  
	{
		//??????ROOT?????????????????????
		setNodeEmpty(root);
		root = rootnode.Pointer[0];         //???????????????,B+????????????1               
	}
}

void IndexManager::deleteKey(ADDRESS current, TargetInt& tar)
{
	int i, j;
	BplusNodeInt x;
	readBplusNode(current, x);


	for (i = 0; i < x.nkey && tar.key > x.key[i]; i++);

	if (i < x.nkey && x.key[i] == tar.key)  //??????????????????????????????  
	{

		if (!x.isleaf)     //???????????????????????????  
		{
			BplusNodeInt child;
			ADDRESS childAdd = x.Pointer[i];
			readBplusNode(x.Pointer[i], child);

			if (child.isleaf)     //????????????????????????  
			{
				if (child.nkey > MAX_INT / 2)      //??????A  
				{
					x.key[i] = child.key[child.nkey - 2];
					child.nkey--;

					writeBplusNode(current, x);
					writeBplusNode(x.Pointer[i], child);

					return;
				}
				else    //?????????????????????????????????????????????  
				{
					if (i > 0)      //??????????????????  
					{
						BplusNodeInt lbchild;
						readBplusNode(x.Pointer[i - 1], lbchild);

						if (lbchild.nkey > MAX_INT / 2)        //??????B  
						{
							for (j = child.nkey; j > 0; j--)
							{
								child.key[j] = child.key[j - 1];
								child.Pointer[j] = child.Pointer[j - 1];
							}

							child.key[0] = x.key[i - 1];
							child.Pointer[0] = lbchild.Pointer[lbchild.nkey - 1];

							child.nkey++;

							lbchild.nkey--;

							x.key[i - 1] = lbchild.key[lbchild.nkey - 1];
							x.key[i] = child.key[child.nkey - 2];

							writeBplusNode(current, x);
							writeBplusNode(x.Pointer[i - 1], lbchild);
							writeBplusNode(x.Pointer[i], child);

						}
						else    //??????C  
						{
							for (j = 0; j < child.nkey; j++)
							{
								lbchild.key[lbchild.nkey + j] = child.key[j];
								lbchild.Pointer[lbchild.nkey + j] = child.Pointer[j];
							}
							lbchild.nkey += child.nkey;

							lbchild.Pointer[MAX_INT] = child.Pointer[MAX_INT];

							setNodeEmpty(childAdd);
							//??????child?????????????????????x.Pointer[i]  

							for (j = i - 1; j < x.nkey - 1; j++)
							{
								x.key[j] = x.key[j + 1];
								x.Pointer[j + 1] = x.Pointer[j + 2];
							}
							x.nkey--;

							x.key[i - 1] = lbchild.key[lbchild.nkey - 2];

							writeBplusNode(current, x);
							writeBplusNode(x.Pointer[i - 1], lbchild);

							i--;

						}


					}
					else      //?????????????????????  
					{
						BplusNodeInt rbchild;
						ADDRESS rbchildAdd = x.Pointer[i + 1];
						readBplusNode(x.Pointer[i + 1], rbchild);

						if (rbchild.nkey > MAX_INT / 2)        //??????D  
						{
							x.key[i] = rbchild.key[0];
							child.key[child.nkey] = rbchild.key[0];
							child.Pointer[child.nkey] = rbchild.Pointer[0];
							child.nkey++;

							for (j = 0; j < rbchild.nkey - 1; j++)
							{
								rbchild.key[j] = rbchild.key[j + 1];
								rbchild.Pointer[j] = rbchild.Pointer[j + 1];
							}

							rbchild.nkey--;

							writeBplusNode(current, x);
							writeBplusNode(x.Pointer[i], child);
							writeBplusNode(x.Pointer[i + 1], rbchild);

						}
						else    //??????E  
						{
							for (j = 0; j < rbchild.nkey; j++)
							{
								child.key[child.nkey + j] = rbchild.key[j];
								child.Pointer[child.nkey + j] = rbchild.Pointer[j];
							}
							child.nkey += rbchild.nkey;

							child.Pointer[MAX_INT] = rbchild.Pointer[MAX_INT];

							setNodeEmpty(rbchildAdd);
							//??????rbchild???????????????x.Pointer[i+1]  

							for (j = i; j < x.nkey - 1; j++)
							{
								x.key[j] = x.key[j + 1];
								x.Pointer[j + 1] = x.Pointer[j + 2];
							}
							x.nkey--;

							writeBplusNode(current, x);
							writeBplusNode(x.Pointer[i], child);

						}

					}

				}

			}
			else      //??????F  
			{

				//??????key???B+?????????????????????????????????,????????????????????????key?????????  

				TargetInt ttar;
				ttar.indexFileName = tar.indexFileName;
				ttar.key = tar.key;
				ttar.type = SINGLE;
				ADDRESS resultAdd;
				vector<index_info> result;
				result = searchBplusTree(ttar, resultAdd);

				BplusNodeInt last;

				readBplusNode(resultAdd, last);

				x.key[i] = last.key[last.nkey - 2];

				writeBplusNode(current, x);


				if (child.nkey > MAX_INT / 2)        //??????H  
				{

				}
				else          //?????????????????????????????????????????????,???????????????????????????????????????????????????  
				{
					if (i > 0)  //x.key[i]????????????  
					{
						BplusNodeInt lbchild;
						readBplusNode(x.Pointer[i - 1], lbchild);

						if (lbchild.nkey > MAX_INT / 2)       //??????I  
						{
							for (j = child.nkey; j > 0; j--)
							{
								child.key[j] = child.key[j - 1];
								child.Pointer[j + 1] = child.Pointer[j];
							}
							child.Pointer[1] = child.Pointer[0];
							child.key[0] = x.key[i - 1];
							child.Pointer[0] = lbchild.Pointer[lbchild.nkey];

							child.nkey++;

							x.key[i - 1] = lbchild.key[lbchild.nkey - 1];
							lbchild.nkey--;

							writeBplusNode(current, x);
							writeBplusNode(x.Pointer[i - 1], lbchild);
							writeBplusNode(x.Pointer[i], child);
						}
						else        //??????J  
						{
							lbchild.key[lbchild.nkey] = x.key[i - 1];   //?????????????????????????????????????????????  
							lbchild.nkey++;

							for (j = 0; j < child.nkey; j++)      //???child???????????????lbchild???????????????,  
							{
								lbchild.key[lbchild.nkey + j] = child.key[j];
								lbchild.Pointer[lbchild.nkey + j] = child.Pointer[j];
							}
							lbchild.Pointer[lbchild.nkey + j] = child.Pointer[j];
							lbchild.nkey += child.nkey;        //?????????child?????????lbchild??????  

							setNodeEmpty(childAdd);
							//??????child?????????????????????,x.Pointer[i]  


							//???????????????????????????child??????????????????????????????lbchild?????????,?????????????????????,???????????????????????????????????????  
							for (j = i - 1; j < x.nkey - 1; j++)
							{
								x.key[j] = x.key[j + 1];
								x.Pointer[j + 1] = x.Pointer[j + 2];
							}
							x.nkey--;

							writeBplusNode(current, x);
							writeBplusNode(x.Pointer[i - 1], lbchild);

							i--;

						}

					}
					else        //??????x.key[i]???????????????  
					{
						BplusNodeInt rbchild;
						ADDRESS rbchildAdd = x.Pointer[i + 1];
						readBplusNode(x.Pointer[i + 1], rbchild);

						if (rbchild.nkey > MAX_INT / 2)     //??????K  
						{

							child.key[child.nkey] = x.key[i];
							child.nkey++;

							child.Pointer[child.nkey] = rbchild.Pointer[0];
							x.key[i] = rbchild.key[0];

							for (j = 0; j < rbchild.nkey - 1; j++)
							{
								rbchild.key[j] = rbchild.key[j + 1];
								rbchild.Pointer[j] = rbchild.Pointer[j + 1];
							}
							rbchild.Pointer[j] = rbchild.Pointer[j + 1];
							rbchild.nkey--;

							writeBplusNode(current, x);
							writeBplusNode(x.Pointer[i], child);
							writeBplusNode(x.Pointer[i + 1], rbchild);

						}
						else        //??????L  
						{
							child.key[child.nkey] = x.key[i];
							child.nkey++;

							for (j = 0; j < rbchild.nkey; j++)     //???rbchild???????????????child?????????  
							{
								child.key[child.nkey + j] = rbchild.key[j];
								child.Pointer[child.nkey + j] = rbchild.Pointer[j];
							}
							child.Pointer[child.nkey + j] = rbchild.Pointer[j];

							child.nkey = child.nkey + rbchild.nkey;

							setNodeEmpty(rbchildAdd);
							//??????rbchild????????????????????????,x,Pointer[i+1]  

							for (j = i; j < x.nkey - 1; j++)    //????????????????????????????????????????????????,????????????????????????????????????  
							{
								x.key[j] = x.key[j + 1];
								x.Pointer[j + 1] = x.Pointer[j + 2];
							}
							x.nkey--;

							writeBplusNode(current, x);
							writeBplusNode(x.Pointer[i], child);

						}

					}
				}

			}

			deleteKey(x.Pointer[i], tar);

		}
		else  //??????G  
		{
			for (j = i; j < x.nkey - 1; j++)
			{
				x.key[j] = x.key[j + 1];
				x.Pointer[j] = x.Pointer[j + 1];
			}
			x.nkey--;

			writeBplusNode(current, x);

			return;
		}

	}
	else        //?????????????????????????????????     
	{
		if (!x.isleaf)    //??????????????????,??????????????????????????????Pointer[i]??????????????????  
		{
			BplusNodeInt child;
			ADDRESS childAdd = x.Pointer[i];
			readBplusNode(x.Pointer[i], child);

			if (!child.isleaf)      //?????????????????????????????????  
			{
				if (child.nkey > MAX_INT / 2)        //??????H  
				{

				}
				else          //?????????????????????????????????????????????,???????????????????????????????????????????????????  
				{
					if (i > 0)  //x.key[i]????????????  
					{
						BplusNodeInt lbchild;
						readBplusNode(x.Pointer[i - 1], lbchild);

						if (lbchild.nkey > MAX_INT / 2)       //??????I  
						{
							for (j = child.nkey; j > 0; j--)
							{
								child.key[j] = child.key[j - 1];
								child.Pointer[j + 1] = child.Pointer[j];
							}
							child.Pointer[1] = child.Pointer[0];
							child.key[0] = x.key[i - 1];
							child.Pointer[0] = lbchild.Pointer[lbchild.nkey];

							child.nkey++;

							x.key[i - 1] = lbchild.key[lbchild.nkey - 1];
							lbchild.nkey--;

							writeBplusNode(current, x);
							writeBplusNode(x.Pointer[i - 1], lbchild);
							writeBplusNode(x.Pointer[i], child);
						}
						else        //??????J  
						{
							lbchild.key[lbchild.nkey] = x.key[i - 1];   //?????????????????????????????????????????????  
							lbchild.nkey++;

							for (j = 0; j < child.nkey; j++)      //???child???????????????lbchild???????????????,  
							{
								lbchild.key[lbchild.nkey + j] = child.key[j];
								lbchild.Pointer[lbchild.nkey + j] = child.Pointer[j];
							}
							lbchild.Pointer[lbchild.nkey + j] = child.Pointer[j];
							lbchild.nkey += child.nkey;        //?????????child?????????lbchild??????  


							setNodeEmpty(childAdd);
							//??????child?????????????????????,x.Pointer[i]  


							//???????????????????????????child??????????????????????????????lbchild?????????,?????????????????????,???????????????????????????????????????  
							for (j = i - 1; j < x.nkey - 1; j++)
							{
								x.key[j] = x.key[j + 1];
								x.Pointer[j + 1] = x.Pointer[j + 2];
							}
							x.nkey--;

							writeBplusNode(current, x);
							writeBplusNode(x.Pointer[i - 1], lbchild);

							i--;

						}

					}
					else        //??????x.key[i]???????????????  
					{
						BplusNodeInt rbchild;
						ADDRESS rbchildAdd = x.Pointer[i + 1];
						readBplusNode(x.Pointer[i + 1], rbchild);

						if (rbchild.nkey > MAX_INT / 2)     //??????K  
						{

							child.key[child.nkey] = x.key[i];
							child.nkey++;

							child.Pointer[child.nkey] = rbchild.Pointer[0];
							x.key[i] = rbchild.key[0];

							for (j = 0; j < rbchild.nkey - 1; j++)
							{
								rbchild.key[j] = rbchild.key[j + 1];
								rbchild.Pointer[j] = rbchild.Pointer[j + 1];
							}
							rbchild.Pointer[j] = rbchild.Pointer[j + 1];
							rbchild.nkey--;

							writeBplusNode(current, x);
							writeBplusNode(x.Pointer[i], child);
							writeBplusNode(x.Pointer[i + 1], rbchild);

						}
						else        //??????L  
						{
							child.key[child.nkey] = x.key[i];
							child.nkey++;

							for (j = 0; j < rbchild.nkey; j++)     //???rbchild???????????????child?????????  
							{
								child.key[child.nkey + j] = rbchild.key[j];
								child.Pointer[child.nkey + j] = rbchild.Pointer[j];
							}
							child.Pointer[child.nkey + j] = rbchild.Pointer[j];

							child.nkey += rbchild.nkey;

							setNodeEmpty(rbchildAdd);
							//??????rbchild????????????????????????,x,Pointer[i+1]  

							for (j = i; j < x.nkey - 1; j++)    //????????????????????????????????????????????????,????????????????????????????????????  
							{
								x.key[j] = x.key[j + 1];
								x.Pointer[j + 1] = x.Pointer[j + 2];
							}
							x.nkey--;

							writeBplusNode(current, x);
							writeBplusNode(x.Pointer[i], child);

						}

					}
				}
			}
			else  //?????????????????????????????????  
			{
				if (child.nkey > MAX_INT / 2)  //??????M  
				{

				}
				else        //??????????????????????????????  
				{
					if (i > 0) //????????????  
					{
						BplusNodeInt lbchild;
						readBplusNode(x.Pointer[i - 1], lbchild);

						if (lbchild.nkey > MAX_INT / 2)       //??????N  
						{
							for (j = child.nkey; j > 0; j--)
							{
								child.key[j] = child.key[j - 1];
								child.Pointer[j] = child.Pointer[j - 1];
							}
							child.key[0] = x.key[i - 1];
							child.Pointer[0] = lbchild.Pointer[lbchild.nkey - 1];
							child.nkey++;
							lbchild.nkey--;

							x.key[i - 1] = lbchild.key[lbchild.nkey - 1];

							writeBplusNode(x.Pointer[i - 1], lbchild);
							writeBplusNode(x.Pointer[i], child);
							writeBplusNode(current, x);

						}
						else        //??????O  
						{

							for (j = 0; j < child.nkey; j++)        //??????????????????????????????  
							{
								lbchild.key[lbchild.nkey + j] = child.key[j];
								lbchild.Pointer[lbchild.nkey + j] = child.Pointer[j];
							}
							lbchild.nkey += child.nkey;

							lbchild.Pointer[MAX_INT] = child.Pointer[MAX_INT];

							setNodeEmpty(childAdd);
							//??????child???????????????x.Pointer[i]  

							for (j = i - 1; j < x.nkey - 1; j++)
							{
								x.key[j] = x.key[j + 1];
								x.Pointer[j + 1] = x.Pointer[j + 2];
							}

							x.nkey--;

							writeBplusNode(x.Pointer[i - 1], lbchild);
							writeBplusNode(current, x);

							i--;

						}

					}
					else        //?????????????????????  
					{
						BplusNodeInt rbchild;
						ADDRESS rbchildAdd = x.Pointer[i + 1];
						readBplusNode(x.Pointer[i + 1], rbchild);

						if (rbchild.nkey > MAX_INT / 2)       //??????P  
						{
							x.key[i] = rbchild.key[0];
							child.key[child.nkey] = rbchild.key[0];
							child.Pointer[child.nkey] = rbchild.Pointer[0];
							child.nkey++;

							for (j = 0; j < rbchild.nkey - 1; j++)
							{
								rbchild.key[j] = rbchild.key[j + 1];
								rbchild.Pointer[j] = rbchild.Pointer[j + 1];
							}
							rbchild.nkey--;

							writeBplusNode(current, x);
							writeBplusNode(x.Pointer[i + 1], rbchild);
							writeBplusNode(x.Pointer[i], child);

						}
						else        //??????Q  
						{
							for (j = 0; j < rbchild.nkey; j++)
							{
								child.key[child.nkey + j] = rbchild.key[j];
								child.Pointer[child.nkey + j] = rbchild.Pointer[j];
							}
							child.nkey += rbchild.nkey;
							child.Pointer[MAX_INT] = rbchild.Pointer[MAX_INT];

							setNodeEmpty(rbchildAdd);
							//??????rbchild???????????????x.Pointer[i+1]  

							for (j = i; j < x.nkey - 1; j++)
							{
								x.key[j] = x.key[j + 1];
								x.Pointer[j + 1] = x.Pointer[j + 2];
							}
							x.nkey--;

							writeBplusNode(current, x);
							writeBplusNode(x.Pointer[i], child);


						}

					}

				}

			}

			deleteKey(x.Pointer[i], tar);
		}


	}
}

void IndexManager::deleteKey(ADDRESS current, TargetFloat& tar)
{
	int i, j;

	BplusNodeFloat x;
	readBplusNode(current, x);


	for (i = 0; i < x.nkey && tar.key > x.key[i]; i++);

	if (i < x.nkey && x.key[i] == tar.key)  //??????????????????????????????  
	{

		if (!x.isleaf)     //???????????????????????????  
		{
			BplusNodeFloat child;
			ADDRESS childAdd = x.Pointer[i];
			readBplusNode(x.Pointer[i], child);

			if (child.isleaf)     //????????????????????????  
			{
				if (child.nkey > MAX_FLOAT / 2)      //??????A  
				{
					x.key[i] = child.key[child.nkey - 2];
					child.nkey--;

					writeBplusNode(current, x);
					writeBplusNode(x.Pointer[i], child);

					return;
				}
				else    //?????????????????????????????????????????????  
				{
					if (i > 0)      //??????????????????  
					{
						BplusNodeFloat lbchild;
						readBplusNode(x.Pointer[i - 1], lbchild);

						if (lbchild.nkey > MAX_FLOAT / 2)        //??????B  
						{
							for (j = child.nkey; j > 0; j--)
							{
								child.key[j] = child.key[j - 1];
								child.Pointer[j] = child.Pointer[j - 1];
							}

							child.key[0] = x.key[i - 1];
							child.Pointer[0] = lbchild.Pointer[lbchild.nkey - 1];

							child.nkey++;

							lbchild.nkey--;

							x.key[i - 1] = lbchild.key[lbchild.nkey - 1];
							x.key[i] = child.key[child.nkey - 2];

							writeBplusNode(current, x);
							writeBplusNode(x.Pointer[i - 1], lbchild);
							writeBplusNode(x.Pointer[i], child);

						}
						else    //??????C  
						{
							for (j = 0; j < child.nkey; j++)
							{
								lbchild.key[lbchild.nkey + j] = child.key[j];
								lbchild.Pointer[lbchild.nkey + j] = child.Pointer[j];
							}
							lbchild.nkey += child.nkey;

							lbchild.Pointer[MAX_FLOAT] = child.Pointer[MAX_FLOAT];

							setNodeEmpty(childAdd);
							//??????child?????????????????????x.Pointer[i]  

							for (j = i - 1; j < x.nkey - 1; j++)
							{
								x.key[j] = x.key[j + 1];
								x.Pointer[j + 1] = x.Pointer[j + 2];
							}
							x.nkey--;

							x.key[i - 1] = lbchild.key[lbchild.nkey - 2];

							writeBplusNode(current, x);
							writeBplusNode(x.Pointer[i - 1], lbchild);

							i--;

						}


					}
					else      //?????????????????????  
					{
						BplusNodeFloat rbchild;
						ADDRESS rbchildAdd = x.Pointer[i + 1];
						readBplusNode(x.Pointer[i + 1], rbchild);

						if (rbchild.nkey > MAX_FLOAT / 2)        //??????D  
						{
							x.key[i] = rbchild.key[0];
							child.key[child.nkey] = rbchild.key[0];
							child.Pointer[child.nkey] = rbchild.Pointer[0];
							child.nkey++;

							for (j = 0; j < rbchild.nkey - 1; j++)
							{
								rbchild.key[j] = rbchild.key[j + 1];
								rbchild.Pointer[j] = rbchild.Pointer[j + 1];
							}

							rbchild.nkey--;

							writeBplusNode(current, x);
							writeBplusNode(x.Pointer[i], child);
							writeBplusNode(x.Pointer[i + 1], rbchild);

						}
						else    //??????E  
						{
							for (j = 0; j < rbchild.nkey; j++)
							{
								child.key[child.nkey + j] = rbchild.key[j];
								child.Pointer[child.nkey + j] = rbchild.Pointer[j];
							}
							child.nkey += rbchild.nkey;

							child.Pointer[MAX_FLOAT] = rbchild.Pointer[MAX_FLOAT];

							setNodeEmpty(rbchildAdd);
							//??????rbchild???????????????x.Pointer[i+1]  

							for (j = i; j < x.nkey - 1; j++)
							{
								x.key[j] = x.key[j + 1];
								x.Pointer[j + 1] = x.Pointer[j + 2];
							}
							x.nkey--;

							writeBplusNode(current, x);
							writeBplusNode(x.Pointer[i], child);

						}

					}

				}

			}
			else      //??????F  
			{

				//??????key???B+?????????????????????????????????,????????????????????????key?????????  

				TargetFloat ttar;
				ttar.key = tar.key;
				ttar.indexFileName = tar.indexFileName;
				ttar.type = SINGLE;
				ADDRESS resultAdd;
				vector<index_info> result;
				result = searchBplusTree(ttar, resultAdd);

				BplusNodeFloat last;

				readBplusNode(resultAdd, last);

				x.key[i] = last.key[last.nkey - 2];

				writeBplusNode(current, x);


				if (child.nkey > MAX_FLOAT / 2)        //??????H  
				{

				}
				else          //?????????????????????????????????????????????,???????????????????????????????????????????????????  
				{
					if (i > 0)  //x.key[i]????????????  
					{
						BplusNodeFloat lbchild;
						readBplusNode(x.Pointer[i - 1], lbchild);

						if (lbchild.nkey > MAX_FLOAT / 2)       //??????I  
						{
							for (j = child.nkey; j > 0; j--)
							{
								child.key[j] = child.key[j - 1];
								child.Pointer[j + 1] = child.Pointer[j];
							}
							child.Pointer[1] = child.Pointer[0];
							child.key[0] = x.key[i - 1];
							child.Pointer[0] = lbchild.Pointer[lbchild.nkey];

							child.nkey++;

							x.key[i - 1] = lbchild.key[lbchild.nkey - 1];
							lbchild.nkey--;

							writeBplusNode(current, x);
							writeBplusNode(x.Pointer[i - 1], lbchild);
							writeBplusNode(x.Pointer[i], child);
						}
						else        //??????J  
						{
							lbchild.key[lbchild.nkey] = x.key[i - 1];   //?????????????????????????????????????????????  
							lbchild.nkey++;

							for (j = 0; j < child.nkey; j++)      //???child???????????????lbchild???????????????,  
							{
								lbchild.key[lbchild.nkey + j] = child.key[j];
								lbchild.Pointer[lbchild.nkey + j] = child.Pointer[j];
							}
							lbchild.Pointer[lbchild.nkey + j] = child.Pointer[j];
							lbchild.nkey += child.nkey;        //?????????child?????????lbchild??????  

							setNodeEmpty(childAdd);
							//??????child?????????????????????,x.Pointer[i]  


							//???????????????????????????child??????????????????????????????lbchild?????????,?????????????????????,???????????????????????????????????????  
							for (j = i - 1; j < x.nkey - 1; j++)
							{
								x.key[j] = x.key[j + 1];
								x.Pointer[j + 1] = x.Pointer[j + 2];
							}
							x.nkey--;

							writeBplusNode(current, x);
							writeBplusNode(x.Pointer[i - 1], lbchild);

							i--;

						}

					}
					else        //??????x.key[i]???????????????  
					{
						BplusNodeFloat rbchild;
						ADDRESS rbchildAdd = x.Pointer[i + 1];
						readBplusNode(x.Pointer[i + 1], rbchild);

						if (rbchild.nkey > MAX_FLOAT / 2)     //??????K  
						{

							child.key[child.nkey] = x.key[i];
							child.nkey++;

							child.Pointer[child.nkey] = rbchild.Pointer[0];
							x.key[i] = rbchild.key[0];

							for (j = 0; j < rbchild.nkey - 1; j++)
							{
								rbchild.key[j] = rbchild.key[j + 1];
								rbchild.Pointer[j] = rbchild.Pointer[j + 1];
							}
							rbchild.Pointer[j] = rbchild.Pointer[j + 1];
							rbchild.nkey--;

							writeBplusNode(current, x);
							writeBplusNode(x.Pointer[i], child);
							writeBplusNode(x.Pointer[i + 1], rbchild);

						}
						else        //??????L  
						{
							child.key[child.nkey] = x.key[i];
							child.nkey++;

							for (j = 0; j < rbchild.nkey; j++)     //???rbchild???????????????child?????????  
							{
								child.key[child.nkey + j] = rbchild.key[j];
								child.Pointer[child.nkey + j] = rbchild.Pointer[j];
							}
							child.Pointer[child.nkey + j] = rbchild.Pointer[j];

							child.nkey = child.nkey + rbchild.nkey;

							setNodeEmpty(rbchildAdd);
							//??????rbchild????????????????????????,x,Pointer[i+1]  

							for (j = i; j < x.nkey - 1; j++)    //????????????????????????????????????????????????,????????????????????????????????????  
							{
								x.key[j] = x.key[j + 1];
								x.Pointer[j + 1] = x.Pointer[j + 2];
							}
							x.nkey--;

							writeBplusNode(current, x);
							writeBplusNode(x.Pointer[i], child);

						}

					}
				}

			}

			deleteKey(x.Pointer[i], tar);

		}
		else  //??????G  
		{
			for (j = i; j < x.nkey - 1; j++)
			{
				x.key[j] = x.key[j + 1];
				x.Pointer[j] = x.Pointer[j + 1];
			}
			x.nkey--;

			writeBplusNode(current, x);

			return;
		}

	}
	else        //?????????????????????????????????     
	{
		if (!x.isleaf)    //??????????????????,??????????????????????????????Pointer[i]??????????????????  
		{
			BplusNodeFloat child;
			ADDRESS childAdd = x.Pointer[i];
			readBplusNode(x.Pointer[i], child);

			if (!child.isleaf)      //?????????????????????????????????  
			{
				if (child.nkey > MAX_FLOAT / 2)        //??????H  
				{

				}
				else          //?????????????????????????????????????????????,???????????????????????????????????????????????????  
				{
					if (i > 0)  //x.key[i]????????????  
					{
						BplusNodeFloat lbchild;
						readBplusNode(x.Pointer[i - 1], lbchild);

						if (lbchild.nkey > MAX_FLOAT / 2)       //??????I  
						{
							for (j = child.nkey; j > 0; j--)
							{
								child.key[j] = child.key[j - 1];
								child.Pointer[j + 1] = child.Pointer[j];
							}
							child.Pointer[1] = child.Pointer[0];
							child.key[0] = x.key[i - 1];
							child.Pointer[0] = lbchild.Pointer[lbchild.nkey];

							child.nkey++;

							x.key[i - 1] = lbchild.key[lbchild.nkey - 1];
							lbchild.nkey--;

							writeBplusNode(current, x);
							writeBplusNode(x.Pointer[i - 1], lbchild);
							writeBplusNode(x.Pointer[i], child);
						}
						else        //??????J  
						{
							lbchild.key[lbchild.nkey] = x.key[i - 1];   //?????????????????????????????????????????????  
							lbchild.nkey++;

							for (j = 0; j < child.nkey; j++)      //???child???????????????lbchild???????????????,  
							{
								lbchild.key[lbchild.nkey + j] = child.key[j];
								lbchild.Pointer[lbchild.nkey + j] = child.Pointer[j];
							}
							lbchild.Pointer[lbchild.nkey + j] = child.Pointer[j];
							lbchild.nkey += child.nkey;        //?????????child?????????lbchild??????  


							setNodeEmpty(childAdd);
							//??????child?????????????????????,x.Pointer[i]  


							//???????????????????????????child??????????????????????????????lbchild?????????,?????????????????????,???????????????????????????????????????  
							for (j = i - 1; j < x.nkey - 1; j++)
							{
								x.key[j] = x.key[j + 1];
								x.Pointer[j + 1] = x.Pointer[j + 2];
							}
							x.nkey--;

							writeBplusNode(current, x);
							writeBplusNode(x.Pointer[i - 1], lbchild);

							i--;

						}

					}
					else        //??????x.key[i]???????????????  
					{
						BplusNodeFloat rbchild;
						ADDRESS rbchildAdd = x.Pointer[i + 1];
						readBplusNode(x.Pointer[i + 1], rbchild);

						if (rbchild.nkey > MAX_FLOAT / 2)     //??????K  
						{

							child.key[child.nkey] = x.key[i];
							child.nkey++;

							child.Pointer[child.nkey] = rbchild.Pointer[0];
							x.key[i] = rbchild.key[0];

							for (j = 0; j < rbchild.nkey - 1; j++)
							{
								rbchild.key[j] = rbchild.key[j + 1];
								rbchild.Pointer[j] = rbchild.Pointer[j + 1];
							}
							rbchild.Pointer[j] = rbchild.Pointer[j + 1];
							rbchild.nkey--;

							writeBplusNode(current, x);
							writeBplusNode(x.Pointer[i], child);
							writeBplusNode(x.Pointer[i + 1], rbchild);

						}
						else        //??????L  
						{
							child.key[child.nkey] = x.key[i];
							child.nkey++;

							for (j = 0; j < rbchild.nkey; j++)     //???rbchild???????????????child?????????  
							{
								child.key[child.nkey + j] = rbchild.key[j];
								child.Pointer[child.nkey + j] = rbchild.Pointer[j];
							}
							child.Pointer[child.nkey + j] = rbchild.Pointer[j];

							child.nkey += rbchild.nkey;

							setNodeEmpty(rbchildAdd);
							//??????rbchild????????????????????????,x,Pointer[i+1]  

							for (j = i; j < x.nkey - 1; j++)    //????????????????????????????????????????????????,????????????????????????????????????  
							{
								x.key[j] = x.key[j + 1];
								x.Pointer[j + 1] = x.Pointer[j + 2];
							}
							x.nkey--;

							writeBplusNode(current, x);
							writeBplusNode(x.Pointer[i], child);

						}

					}
				}
			}
			else  //?????????????????????????????????  
			{
				if (child.nkey > MAX_FLOAT / 2)  //??????M  
				{

				}
				else        //??????????????????????????????  
				{
					if (i > 0) //????????????  
					{
						BplusNodeFloat lbchild;
						readBplusNode(x.Pointer[i - 1], lbchild);

						if (lbchild.nkey > MAX_FLOAT / 2)       //??????N  
						{
							for (j = child.nkey; j > 0; j--)
							{
								child.key[j] = child.key[j - 1];
								child.Pointer[j] = child.Pointer[j - 1];
							}
							child.key[0] = x.key[i - 1];
							child.Pointer[0] = lbchild.Pointer[lbchild.nkey - 1];
							child.nkey++;
							lbchild.nkey--;

							x.key[i - 1] = lbchild.key[lbchild.nkey - 1];

							writeBplusNode(x.Pointer[i - 1], lbchild);
							writeBplusNode(x.Pointer[i], child);
							writeBplusNode(current, x);

						}
						else        //??????O  
						{

							for (j = 0; j < child.nkey; j++)        //??????????????????????????????  
							{
								lbchild.key[lbchild.nkey + j] = child.key[j];
								lbchild.Pointer[lbchild.nkey + j] = child.Pointer[j];
							}
							lbchild.nkey += child.nkey;

							lbchild.Pointer[MAX_FLOAT] = child.Pointer[MAX_FLOAT];

							setNodeEmpty(childAdd);
							//??????child???????????????x.Pointer[i]  

							for (j = i - 1; j < x.nkey - 1; j++)
							{
								x.key[j] = x.key[j + 1];
								x.Pointer[j + 1] = x.Pointer[j + 2];
							}

							x.nkey--;

							writeBplusNode(x.Pointer[i - 1], lbchild);
							writeBplusNode(current, x);

							i--;

						}

					}
					else        //?????????????????????  
					{
						BplusNodeFloat rbchild;
						ADDRESS rbchildAdd = x.Pointer[i + 1];
						readBplusNode(x.Pointer[i + 1], rbchild);

						if (rbchild.nkey > MAX_FLOAT / 2)       //??????P  
						{
							x.key[i] = rbchild.key[0];
							child.key[child.nkey] = rbchild.key[0];
							child.Pointer[child.nkey] = rbchild.Pointer[0];
							child.nkey++;

							for (j = 0; j < rbchild.nkey - 1; j++)
							{
								rbchild.key[j] = rbchild.key[j + 1];
								rbchild.Pointer[j] = rbchild.Pointer[j + 1];
							}
							rbchild.nkey--;

							writeBplusNode(current, x);
							writeBplusNode(x.Pointer[i + 1], rbchild);
							writeBplusNode(x.Pointer[i], child);

						}
						else        //??????Q  
						{
							for (j = 0; j < rbchild.nkey; j++)
							{
								child.key[child.nkey + j] = rbchild.key[j];
								child.Pointer[child.nkey + j] = rbchild.Pointer[j];
							}
							child.nkey += rbchild.nkey;
							child.Pointer[MAX_FLOAT] = rbchild.Pointer[MAX_FLOAT];

							setNodeEmpty(rbchildAdd);
							//??????rbchild???????????????x.Pointer[i+1]  

							for (j = i; j < x.nkey - 1; j++)
							{
								x.key[j] = x.key[j + 1];
								x.Pointer[j + 1] = x.Pointer[j + 2];
							}
							x.nkey--;

							writeBplusNode(current, x);
							writeBplusNode(x.Pointer[i], child);


						}

					}

				}

			}

			deleteKey(x.Pointer[i], tar);
		}


	}
}

void IndexManager::deleteKey(ADDRESS current, TargetChar& tar)
{
	BplusNodeChar x;
	readBplusNode(current, x);
	int i, j;

	for (i = 0; i < x.nkey && tar.key > toString(x.key[i]); i++);

	if (i < x.nkey && toString(x.key[i]) == tar.key)  //??????????????????????????????  
	{

		if (!x.isleaf)     //???????????????????????????  
		{
			BplusNodeChar child;
			ADDRESS childAdd = x.Pointer[i];
			readBplusNode(x.Pointer[i], child);

			if (child.isleaf)     //????????????????????????  
			{
				if (child.nkey > MAX_CHAR / 2)      //??????A  
				{
					// x.key[i] = child.key[child.nkey - 2];  
					memcpy(x.key[i], child.key[child.nkey - 2], 255);
					child.nkey--;

					writeBplusNode(current, x);
					writeBplusNode(x.Pointer[i], child);

					return;
				}
				else    //?????????????????????????????????????????????  
				{
					if (i > 0)      //??????????????????  
					{
						BplusNodeChar lbchild;
						readBplusNode(x.Pointer[i - 1], lbchild);

						if (lbchild.nkey > MAX_CHAR / 2)        //??????B  
						{
							for (j = child.nkey; j > 0; j--)
							{
								// child.key[j] = child.key[j-1]; 
								memcpy(child.key[j], child.key[j - 1], 255);
								child.Pointer[j] = child.Pointer[j - 1];
							}

							// child.key[0] = x.key[i-1];  
							memcpy(child.key[0], x.key[i - 1], 255);
							child.Pointer[0] = lbchild.Pointer[lbchild.nkey - 1];

							child.nkey++;

							lbchild.nkey--;

							// x.key[i-1] = lbchild.key[lbchild.nkey-1];  
							// x.key[i] = child.key[child.nkey-2];  
							memcpy(x.key[i - 1], lbchild.key[lbchild.nkey - 1], 255);
							memcpy(x.key[i], child.key[child.nkey - 2], 255);

							writeBplusNode(current, x);
							writeBplusNode(x.Pointer[i - 1], lbchild);
							writeBplusNode(x.Pointer[i], child);

						}
						else    //??????C  
						{
							for (j = 0; j < child.nkey; j++)
							{
								// lbchild.key[lbchild.nkey+j] = child.key[j];  
								memcpy(lbchild.key[lbchild.nkey + j], child.key[j], 255);
								lbchild.Pointer[lbchild.nkey + j] = child.Pointer[j];
							}
							lbchild.nkey += child.nkey;

							lbchild.Pointer[MAX_CHAR] = child.Pointer[MAX_CHAR];

							setNodeEmpty(childAdd);
							//??????child?????????????????????x.Pointer[i]  

							for (j = i - 1; j < x.nkey - 1; j++)
							{
								// x.key[j] = x.key[j+1]; 
								memcpy(x.key[j], x.key[j + 1], 255);
								x.Pointer[j + 1] = x.Pointer[j + 2];
							}
							x.nkey--;

							// x.key[i-1] = lbchild.key[lbchild.nkey-2];  
							memcpy(x.key[i - 1], lbchild.key[lbchild.nkey - 2], 255);

							writeBplusNode(current, x);
							writeBplusNode(x.Pointer[i - 1], lbchild);

							i--;

						}


					}
					else      //?????????????????????  
					{
						BplusNodeChar rbchild;
						ADDRESS rbchildAdd = x.Pointer[i + 1];
						readBplusNode(x.Pointer[i + 1], rbchild);

						if (rbchild.nkey > MAX_CHAR / 2)        //??????D  
						{
							// x.key[i] = rbchild.key[0]; 
							memcpy(x.key[i], rbchild.key[0], 255);
							// child.key[child.nkey] = rbchild.key[0];  
							memcpy(child.key[child.nkey], rbchild.key[0], 255);
							child.Pointer[child.nkey] = rbchild.Pointer[0];
							child.nkey++;

							for (j = 0; j < rbchild.nkey - 1; j++)
							{
								// rbchild.key[j] = rbchild.key[j+1];  
								rbchild.Pointer[j] = rbchild.Pointer[j + 1];
							}

							rbchild.nkey--;

							writeBplusNode(current, x);
							writeBplusNode(x.Pointer[i], child);
							writeBplusNode(x.Pointer[i + 1], rbchild);

						}
						else    //??????E  
						{
							for (j = 0; j < rbchild.nkey; j++)
							{
								memcpy(child.key[child.nkey + j], rbchild.key[j], 255);
								child.Pointer[child.nkey + j] = rbchild.Pointer[j];
							}
							child.nkey += rbchild.nkey;

							child.Pointer[MAX_CHAR] = rbchild.Pointer[MAX_CHAR];

							setNodeEmpty(rbchildAdd);
							//??????rbchild???????????????x.Pointer[i+1]  

							for (j = i; j < x.nkey - 1; j++)
							{
								// x.key[j] = x.key[j+1];  
								memcpy(x.key[j], x.key[j + 1], 255);
								x.Pointer[j + 1] = x.Pointer[j + 2];
							}
							x.nkey--;

							writeBplusNode(current, x);
							writeBplusNode(x.Pointer[i], child);

						}

					}

				}

			}
			else      //??????F  
			{

				//??????key???B+?????????????????????????????????,????????????????????????key?????????  

				TargetChar ttar;
				ttar.key = tar.key;
				ttar.indexFileName = tar.indexFileName;
				ttar.type = SINGLE;
				ADDRESS resultAdd;
				vector<index_info> result;
				result = searchBplusTree(ttar, resultAdd);

				BplusNodeChar last;

				readBplusNode(resultAdd, last);

				// x.key[i] = last.key[last.nkey-2];  
				memcpy(x.key[i], last.key[last.nkey - 2], 255);

				writeBplusNode(current, x);


				if (child.nkey > MAX_CHAR / 2)        //??????H  
				{

				}
				else          //?????????????????????????????????????????????,???????????????????????????????????????????????????  
				{
					if (i > 0)  //x.key[i]????????????  
					{
						BplusNodeChar lbchild;
						readBplusNode(x.Pointer[i - 1], lbchild);

						if (lbchild.nkey > MAX_CHAR / 2)       //??????I  
						{
							for (j = child.nkey; j > 0; j--)
							{
								// child.key[j] = child.key[j-1];  
								memcpy(child.key[j], child.key[j - 1], 255);
								child.Pointer[j + 1] = child.Pointer[j];
							}
							child.Pointer[1] = child.Pointer[0];
							// child.key[0] = x.key[i-1] ;  
							memcpy(child.key[0], x.key[i - 1], 255);
							child.Pointer[0] = lbchild.Pointer[lbchild.nkey];

							child.nkey++;

							memcpy(x.key[i - 1], lbchild.key[lbchild.nkey - 1], 255);
							lbchild.nkey--;

							writeBplusNode(current, x);
							writeBplusNode(x.Pointer[i - 1], lbchild);
							writeBplusNode(x.Pointer[i], child);
						}
						else        //??????J  
						{
							memcpy(lbchild.key[lbchild.nkey], x.key[i - 1], 255);   //?????????????????????????????????????????????
							lbchild.nkey++;

							for (j = 0; j < child.nkey; j++)      //???child???????????????lbchild???????????????,  
							{
								memcpy(lbchild.key[lbchild.nkey + j], child.key[j], 255);
								lbchild.Pointer[lbchild.nkey + j] = child.Pointer[j];
							}
							lbchild.Pointer[lbchild.nkey + j] = child.Pointer[j];
							lbchild.nkey += child.nkey;        //?????????child?????????lbchild??????  

							setNodeEmpty(childAdd);
							//??????child?????????????????????,x.Pointer[i]  


							//???????????????????????????child??????????????????????????????lbchild?????????,?????????????????????,???????????????????????????????????????  
							for (j = i - 1; j < x.nkey - 1; j++)
							{
								memcpy(x.key[j], x.key[j + 1], 255);
								x.Pointer[j + 1] = x.Pointer[j + 2];
							}
							x.nkey--;

							writeBplusNode(current, x);
							writeBplusNode(x.Pointer[i - 1], lbchild);

							i--;

						}

					}
					else        //??????x.key[i]???????????????  
					{
						BplusNodeChar rbchild;
						ADDRESS rbchildAdd = x.Pointer[i + 1];
						readBplusNode(x.Pointer[i + 1], rbchild);

						if (rbchild.nkey > MAX_CHAR / 2)     //??????K  
						{

							memcpy(child.key[child.nkey], x.key[i], 255);
							child.nkey++;

							child.Pointer[child.nkey] = rbchild.Pointer[0];
							memcpy(x.key[i], rbchild.key[0], 255);

							for (j = 0; j < rbchild.nkey - 1; j++)
							{
								memcpy(rbchild.key[j], rbchild.key[j + 1], 255);
								rbchild.Pointer[j] = rbchild.Pointer[j + 1];
							}
							rbchild.Pointer[j] = rbchild.Pointer[j + 1];
							rbchild.nkey--;

							writeBplusNode(current, x);
							writeBplusNode(x.Pointer[i], child);
							writeBplusNode(x.Pointer[i + 1], rbchild);

						}
						else        //??????L  
						{
							memcpy(child.key[child.nkey], x.key[i], 255);
							child.nkey++;

							for (j = 0; j < rbchild.nkey; j++)     //???rbchild???????????????child?????????  
							{
								memcpy(child.key[child.nkey + j], rbchild.key[j], 255);
								child.Pointer[child.nkey + j] = rbchild.Pointer[j];
							}
							child.Pointer[child.nkey + j] = rbchild.Pointer[j];

							child.nkey = child.nkey + rbchild.nkey;

							setNodeEmpty(rbchildAdd);
							//??????rbchild????????????????????????,x,Pointer[i+1]  

							for (j = i; j < x.nkey - 1; j++)    //????????????????????????????????????????????????,????????????????????????????????????  
							{
								memcpy(x.key[j], x.key[j + 1], 255);
								x.Pointer[j + 1] = x.Pointer[j + 2];
							}
							x.nkey--;

							writeBplusNode(current, x);
							writeBplusNode(x.Pointer[i], child);

						}

					}
				}

			}

			deleteKey(x.Pointer[i], tar);

		}
		else  //??????G  
		{
			for (j = i; j < x.nkey - 1; j++)
			{
				memcpy(x.key[j], x.key[j + 1], 255);
				x.Pointer[j] = x.Pointer[j + 1];
			}
			x.nkey--;

			writeBplusNode(current, x);

			return;
		}

	}
	else        //?????????????????????????????????     
	{
		if (!x.isleaf)    //??????????????????,??????????????????????????????Pointer[i]??????????????????  
		{
			BplusNodeChar child;
			ADDRESS childAdd = x.Pointer[i];
			readBplusNode(x.Pointer[i], child);

			if (!child.isleaf)      //?????????????????????????????????  
			{
				if (child.nkey > MAX_CHAR / 2)        //??????H  
				{

				}
				else          //?????????????????????????????????????????????,???????????????????????????????????????????????????  
				{
					if (i > 0)  //x.key[i]????????????  
					{
						BplusNodeChar lbchild;
						readBplusNode(x.Pointer[i - 1], lbchild);

						if (lbchild.nkey > MAX_CHAR / 2)       //??????I  
						{
							for (j = child.nkey; j > 0; j--)
							{
								memcpy(child.key[j], child.key[j - 1], 255);
								child.Pointer[j + 1] = child.Pointer[j];
							}
							child.Pointer[1] = child.Pointer[0];
							memcpy(child.key[0], x.key[i - 1], 255);
							child.Pointer[0] = lbchild.Pointer[lbchild.nkey];

							child.nkey++;

							memcpy(x.key[i - 1], lbchild.key[lbchild.nkey - 1], 255);
							lbchild.nkey--;

							writeBplusNode(current, x);
							writeBplusNode(x.Pointer[i - 1], lbchild);
							writeBplusNode(x.Pointer[i], child);
						}
						else        //??????J  
						{
							memcpy(lbchild.key[lbchild.nkey], x.key[i - 1], 255);   //?????????????????????????????????????????????  
							lbchild.nkey++;

							for (j = 0; j < child.nkey; j++)      //???child???????????????lbchild???????????????,  
							{
								memcpy(lbchild.key[lbchild.nkey + j], child.key[j], 255);
								lbchild.Pointer[lbchild.nkey + j] = child.Pointer[j];
							}
							lbchild.Pointer[lbchild.nkey + j] = child.Pointer[j];
							lbchild.nkey += child.nkey;        //?????????child?????????lbchild??????  


							setNodeEmpty(childAdd);
							//??????child?????????????????????,x.Pointer[i]  


							//???????????????????????????child??????????????????????????????lbchild?????????,?????????????????????,???????????????????????????????????????  
							for (j = i - 1; j < x.nkey - 1; j++)
							{
								memcpy(x.key[j], x.key[j + 1], 255);
								x.Pointer[j + 1] = x.Pointer[j + 2];
							}
							x.nkey--;

							writeBplusNode(current, x);
							writeBplusNode(x.Pointer[i - 1], lbchild);

							i--;

						}

					}
					else        //??????x.key[i]???????????????  
					{
						BplusNodeChar rbchild;
						ADDRESS rbchildAdd = x.Pointer[i + 1];
						readBplusNode(x.Pointer[i + 1], rbchild);

						if (rbchild.nkey > MAX_CHAR / 2)     //??????K  
						{

							memcpy(child.key[child.nkey], x.key[i], 255);
							child.nkey++;

							child.Pointer[child.nkey] = rbchild.Pointer[0];
							memcpy(x.key[i], rbchild.key[0], 255);

							for (j = 0; j < rbchild.nkey - 1; j++)
							{
								memcpy(rbchild.key[j], rbchild.key[j + 1], 255);
								rbchild.Pointer[j] = rbchild.Pointer[j + 1];
							}
							rbchild.Pointer[j] = rbchild.Pointer[j + 1];
							rbchild.nkey--;

							writeBplusNode(current, x);
							writeBplusNode(x.Pointer[i], child);
							writeBplusNode(x.Pointer[i + 1], rbchild);

						}
						else        //??????L  
						{
							memcpy(child.key[child.nkey], x.key[i], 255);
							child.nkey++;

							for (j = 0; j < rbchild.nkey; j++)     //???rbchild???????????????child?????????  
							{
								memcpy(child.key[child.nkey + j], rbchild.key[j], 255);
								child.Pointer[child.nkey + j] = rbchild.Pointer[j];
							}
							child.Pointer[child.nkey + j] = rbchild.Pointer[j];

							child.nkey += rbchild.nkey;

							setNodeEmpty(rbchildAdd);
							//??????rbchild????????????????????????,x,Pointer[i+1]  

							for (j = i; j < x.nkey - 1; j++)    //????????????????????????????????????????????????,????????????????????????????????????  
							{
								memcpy(x.key[j], x.key[j + 1], 255);
								x.Pointer[j + 1] = x.Pointer[j + 2];
							}
							x.nkey--;

							writeBplusNode(current, x);
							writeBplusNode(x.Pointer[i], child);

						}

					}
				}
			}
			else  //?????????????????????????????????  
			{
				if (child.nkey > MAX_CHAR / 2)  //??????M  
				{

				}
				else        //??????????????????????????????  
				{
					if (i > 0) //????????????  
					{
						BplusNodeChar lbchild;
						readBplusNode(x.Pointer[i - 1], lbchild);

						if (lbchild.nkey > MAX_CHAR / 2)       //??????N  
						{
							for (j = child.nkey; j > 0; j--)
							{
								memcpy(child.key[j], child.key[j - 1], 255);
								child.Pointer[j] = child.Pointer[j - 1];
							}
							memcpy(child.key[0], x.key[i - 1], 255);
							child.Pointer[0] = lbchild.Pointer[lbchild.nkey - 1];
							child.nkey++;
							lbchild.nkey--;

							memcpy(x.key[i - 1], lbchild.key[lbchild.nkey - 1], 255);

							writeBplusNode(x.Pointer[i - 1], lbchild);
							writeBplusNode(x.Pointer[i], child);
							writeBplusNode(current, x);

						}
						else        //??????O  
						{

							for (j = 0; j < child.nkey; j++)        //??????????????????????????????  
							{
								memcpy(lbchild.key[lbchild.nkey + j], child.key[j], 255);
								lbchild.Pointer[lbchild.nkey + j] = child.Pointer[j];
							}
							lbchild.nkey += child.nkey;

							lbchild.Pointer[MAX_CHAR] = child.Pointer[MAX_CHAR];

							setNodeEmpty(childAdd);
							//??????child???????????????x.Pointer[i]  

							for (j = i - 1; j < x.nkey - 1; j++)
							{
								memcpy(x.key[j], x.key[j + 1], 255);
								x.Pointer[j + 1] = x.Pointer[j + 2];
							}

							x.nkey--;

							writeBplusNode(x.Pointer[i - 1], lbchild);
							writeBplusNode(current, x);

							i--;

						}

					}
					else        //?????????????????????  
					{
						BplusNodeChar rbchild;
						ADDRESS rbchildAdd = x.Pointer[i + 1];
						readBplusNode(x.Pointer[i + 1], rbchild);

						if (rbchild.nkey > MAX_CHAR / 2)       //??????P  
						{
							memcpy(x.key[i], rbchild.key[0], 255);
							memcpy(child.key[child.nkey], rbchild.key[0], 255);
							child.Pointer[child.nkey] = rbchild.Pointer[0];
							child.nkey++;

							for (j = 0; j < rbchild.nkey - 1; j++)
							{
								memcpy(rbchild.key[j], rbchild.key[j + 1], 255);
								rbchild.Pointer[j] = rbchild.Pointer[j + 1];
							}
							rbchild.nkey--;

							writeBplusNode(current, x);
							writeBplusNode(x.Pointer[i + 1], rbchild);
							writeBplusNode(x.Pointer[i], child);

						}
						else        //??????Q  
						{
							for (j = 0; j < rbchild.nkey; j++)
							{
								memcpy(child.key[child.nkey + j], rbchild.key[j], 255);
								child.Pointer[child.nkey + j] = rbchild.Pointer[j];
							}
							child.nkey += rbchild.nkey;
							child.Pointer[MAX_CHAR] = rbchild.Pointer[MAX_CHAR];

							setNodeEmpty(rbchildAdd);
							//??????rbchild???????????????x.Pointer[i+1]  

							for (j = i; j < x.nkey - 1; j++)
							{
								memcpy(x.key[j], x.key[j + 1], 255);
								x.Pointer[j + 1] = x.Pointer[j + 2];
							}
							x.nkey--;

							writeBplusNode(current, x);
							writeBplusNode(x.Pointer[i], child);


						}

					}

				}

			}

			deleteKey(x.Pointer[i], tar);
		}


	}
}

void IndexManager::setNodeEmpty(ADDRESS add) {
	BufferNode& temp_bf = getBlock(INDEX, configFileName, 0);
	emptyNodeList overview;
	temp_bf.readBlock(overview, sizeof(emptyNodeList), 0);
	int page = (add - 1) / (BLOCK_SIZE / sizeof(int)-1);
	if (overview.isUsed[page] == 1)
	{
		overview.isUsed[page] = 0;
		temp_bf.writeBlock(overview, sizeof(emptyNodeList), 0);
	}

	BufferNode& temp_bf2 = getBlock(INDEX, configFileName, page + 1);
	emptyNodeList ENL;
	temp_bf2.readBlock(ENL, sizeof(emptyNodeList), 0);
	int block = add - 1 - page*(BLOCK_SIZE / sizeof(int)-1);
	ENL.isUsed[block] = 0;
	temp_bf2.writeBlock(ENL, sizeof(emptyNodeList), 0);
}
