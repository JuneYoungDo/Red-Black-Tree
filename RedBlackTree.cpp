#include<iostream>
#include<string>
#include<vector>
#include<algorithm>
using namespace std;

#define endl '\n'

struct node {
	int key;
	string name;
	string phoneNum;
	int x, y;
	pair<string, int> p;		// ����� �����
	bool color;					// true = red , false = black
	node* par;
	node* child[2];				// [0] = �����ڽ�, [1] = �������ڽ�
	node()
	{
		key = -1;
		name = "";
		phoneNum = "";
		x = 0, y = 0;
		p.first = "";
		p.second = 0;
		color = true;			// ���ԵǴ� node�� ��� red
		par = NULL;
		child[0] = NULL;
		child[1] = NULL;
	}
};

// node���� key ���� �̿��� �� �Լ� (��������)
bool compare(node* a, node* b) {
	return a->key < b->key;
}

struct RedBlackTree {
	vector<node*> list;							// node�� ���翩�θ� Ȯ���ϱ� ���� ����
	node* root;									// root
	RedBlackTree()
	{
		root = NULL;
	}

	// key�� �̿��Ͽ� node�� ã�� �Լ�
	node* findNode(int x)
	{
		if (root == NULL)
			return NULL;

		node* tmp = root;
		while (tmp->key != -1)
		{
			int tKey = tmp->key;
			if (tKey == x)
				return tmp;

			if (tKey < x)
			{
				tmp = tmp->child[1];
			}
			else
				tmp = tmp->child[0];
		}

		return NULL;
	}

	// bst������� node�� ���� Ž��
	int findDepth(node* n)
	{
		int depth = 0;
		node* tmp = root;

		int nKey = n->key;

		while (tmp->key != -1)
		{
			if (tmp->key == nKey)
				break;
			int tKey = tmp->key;

			// ã���� �ϴ� node�� key ���� Ž���ϴ� node�� key ������ ������� �������� Ž��
			if (tKey > nKey)
			{
				tmp = tmp->child[0];
				depth++;
			}
			else
			{													// �� ū ��� ���������� Ž��
				tmp = tmp->child[1];
				depth++;
			}
		}

		return depth;
	}

	// root�� NULL�� ��� ���� node�� root�� ����
	void insertRoot(node* n)
	{
		list.push_back(n);
		root = n;

		// leaf ��带 �޾��ִ� ����
		node* newNode = new node();
		newNode->key = -1;
		newNode->color = false;
		newNode->par = n;
		n->child[0] = newNode;

		node* newNode1 = new node();
		newNode1->key = -1;
		newNode1->color = false;
		newNode1->par = n;
		n->child[1] = newNode1;

		// root �� color�� �׻� black
		root->color = false;
	}

	// double red�� ��� restructuring �ϴ� ����
	void restructuring(node* z, node* v, node* gp)
	{
		// ���Լ��� ���� 3���� node�� ���� ���� �߰� node�� ã��
		vector<node*> vec;
		vec.push_back(z);
		vec.push_back(v);
		vec.push_back(gp);
		sort(vec.begin(), vec.end(), compare);

		node* left = vec[0];
		node* mid = vec[1];
		node* right = vec[2];

		vec.clear();

		// ���� ���� ������ ���������ִ� ����
		// z node�� grandParent node�� ���� ���� ���� ���
		if (left == gp)
		{
			// ���� ū ���� ���� �ڽ� node �� mid ���� node�� ��� ( CH6 ���ǳ�Ʈ pg17 1��° ��� ) 
			if (right->child[0] == mid)
			{
				// mid node�� �θ�� �ö�
				mid->par = left->par;

				// left�� �θ� NULL�� �ƴѰ�� �ش� ������ ���� �θ�-�ڽ� ���踦 ������
				if (left->par != NULL)
				{
					if (left->par->child[0] == left)
						left->par->child[0] = mid;
					else
						left->par->child[1] = mid;
				}

				// left node�� ������ �ڽĿ� mid node�� ���� �ڽ��� �Ű���
				left->child[1] = mid->child[0];
				left->child[1]->par = left;

				// right node�� ���� �ڽĿ� mid node�� ������ �ڽ��� �Ű���
				right->child[0] = mid->child[1];
				right->child[0]->par = right;

				// mid node�� �ڽ����� left, right node�� �Ű���
				mid->child[0] = left;
				left->par = mid;

				mid->child[1] = right;
				right->par = mid;
			}
			// ���� ū ���� �θ� node �� mid ���� node�� ��� ( CH6 ���ǳ�Ʈ pg17 4��° ��� ) 
			else if (right->par == mid)
			{
				// mid node�� �θ�� �ö�
				mid->par = left->par;

				// left�� �θ� NULL�� �ƴѰ�� �ش� ������ ���� �θ�-�ڽ� ���踦 ������
				if (left->par != NULL)
				{
					if (left->par->child[0] == left)
						left->par->child[0] = mid;
					else
						left->par->child[1] = mid;
				}

				// left node�� ������ �ڽĿ� mid node�� ���� �ڽ��� �Ű���
				left->child[1] = mid->child[0];
				left->child[1]->par = left;

				// mid node�� ���� �ڽĿ� left node�� �Ű���
				left->par = mid;
				mid->child[0] = left;
			}

			// left node�� tree���� root node ���� ���, mid node�� root�� ��
			if (root == left)
				root = mid;
		}
		// z node�� grandParent node�� ���� ū ���� ���
		else if (right == gp)
		{
			// �߰� �� node�� parent node�� ���� ���� ���� node�� ��� ( CH6 ���ǳ�Ʈ pg17 2��° ��� )
			if (mid->par == left)
			{
				// mid node�� �θ�� �ö�
				mid->par = right->par;

				// right�� �θ� NULL�� �ƴѰ�� �ش� ������ ���� �θ�-�ڽ� ���踦 ������
				if (right->par != NULL)
				{
					if (right->par->child[0] == right)
						right->par->child[0] = mid;
					else
						right->par->child[1] = mid;
				}

				// left node�� ������ �ڽĿ� mid node�� ���� �ڽ��� �Ű���
				left->child[1] = mid->child[0];
				mid->child[0]->par = left;

				// right node�� ���� �ڽĿ� mid node�� ������ �ڽ��� �Ű���
				right->child[0] = mid->child[1];
				mid->child[1]->par = right;

				// mid node�� �ڽ����� left, right node�� �Ű���
				mid->child[0] = left;
				left->par = mid;

				mid->child[1] = right;
				right->par = mid;
			}
			// �߰� �� node�� parent node�� ���� ū ���� node�� ��� ( CH6 ���ǳ�Ʈ pg17 3��° ��� )
			else if (mid->par == right)
			{
				// mid node�� �θ�� �ö�
				mid->par = right->par;
				if (right->par != NULL)
				{
					if (right->par->child[0] == right)
						right->par->child[0] = mid;
					else
						right->par->child[1] = mid;
				}

				// right node�� ���� �ڽĿ� mid node�� ������ �ڽ��� �Ű���
				right->child[0] = mid->child[1];
				right->child[0]->par = right;

				// mid node�� ������ �ڽĿ� right node�� �Ű���
				mid->child[1] = right;
				right->par = mid;
			}

			// right node�� tree���� root node ���� ���, mid node�� root�� ��
			if (root == right)
				root = mid;
		}

		// color ������ ���������ִ� ����
		mid->color = false;						// mid�� black
		left->color = true;						// left�� red
		right->color = true;					// right�� red

		root->color = false;					// root�� �׻� black
	}

	// double red�� ��� recoloring �ϴ� ����
	void recoloring(node* z, node* v, node* gp, node* w)
	{
		// gp node�� grandParent node
		// v,gp,w node�� ������ ���� -> ���θ��� gp node�� red, 
		//								���θ��� �ڽ��� v�� w node�� black
		v->color = false;
		gp->color = true;
		w->color = false;

		root->color = false;			// root�� �׻� black

		if (gp->par == NULL)			// gp node�� root ���� ��� return
			return;
		else
			if (gp->par->color == true)		// gp node�� ���� red�� ��ȯ �Ǿ��⿡, 
				doubleRed(gp);				// gp node�� parent node�� double red�� �߻��� �� ����
	}

	// node ���� �������� doubleRed �߻� �� ó���ϴ� ���� (z�� ���Ե� node)
	void doubleRed(node* z)
	{
		node* v = z->par;				// z node�� �θ� node
		node* gp = v->par;				// z node�� ���θ� node
		node* w;						// z nonde�� ���� node
		if (v->par->child[0] == v)
			w = v->par->child[1];
		else
			w = v->par->child[0];

		if (w->color == false)			// w node�� black�� ��� -> restructuring
			restructuring(z, v, gp);
		if (w->color == true)			// w node�� red�� ��� -> recoloring
			recoloring(z, v, gp, w);
	}

	// root�� NULL�� �ƴ� ��쿡 node�� �����ϴ� ����
	void insertNode(node* n)
	{
		list.push_back(n);

		node* tmp = root;
		int nKey = n->key;

		// Bst�� ������� node�� ��ġ Ž��
		while (tmp->key != -1)
		{
			int tKey = tmp->key;

			if (nKey > tKey)
			{
				tmp = tmp->child[1];
				if (tmp->key == -1)
				{
					tmp->par->child[1] = n;
					n->par = tmp->par;
					break;
				}
			}
			else
			{
				tmp = tmp->child[0];
				if (tmp->key == -1)
				{
					tmp->par->child[0] = n;
					n->par = tmp->par;
					break;
				}
			}
		}
		// leaf node�� �޾��ִ� ����
		node* newNode = new node();
		newNode->key = -1;
		newNode->color = false;
		newNode->par = n;
		n->child[0] = newNode;

		node* newNode1 = new node();
		newNode1->key = -1;
		newNode1->color = false;
		newNode1->par = n;
		n->child[1] = newNode1;

		// ���Ե� node�� �θ� node�� red�� ��� -> double red �߻�
		if (n->par->color == true)
			doubleRed(n);

		// root node�� ��� �׻� black
		root->color = false;
	}

};

int main()
{
	ios::sync_with_stdio(false);
	cin.tie(NULL);
	cout.tie(NULL);

	int tc;
	cin >> tc;

	RedBlackTree rbt;

	// testcase ��ŭ �ݺ�
	while (tc--)
	{
		// ��ɾ� �Է�
		char c;
		cin >> c;

		// node ���� ��ɾ�
		if (c == 'I')
		{
			int flag;
			int depth;
			int key, x, y, pay;
			string name, pn, sick;

			cin >> key >> name >> pn >> x >> y >> sick >> pay;

			// �ش� key�� ������ node�� ���� ���
			if (rbt.findNode(key) == NULL)
			{
				node* newNode = new node();
				newNode->key = key;
				newNode->name = name;
				newNode->phoneNum = pn;
				newNode->x = x;
				newNode->y = y;
				newNode->p.first = sick;
				newNode->p.second = pay;

				// root node�� NULL�� ��� -> root node�� ����
				if (rbt.root == NULL)
					rbt.insertRoot(newNode);
				// �� �� ��� node�� ��ġ�� ã�Ƽ� ����
				else
					rbt.insertNode(newNode);

				// ���� ������ ��� 1 ���
				flag = 1;
			}
			else
				flag = 0;		// �̹� �ش� key�� ������ �ִ� node�� ���� ��� 0 ���

			depth = rbt.findDepth(rbt.findNode(key));		// �ش� key�� ������ node �� depth ���

			cout << depth << " " << flag << endl;
		}
		// node Ž�� ���
		if (c == 'F')
		{
			int key;
			cin >> key;

			// �ش� key ���� ������ node n�� Ž��
			node* n = rbt.findNode(key);

			// node n�� ���� ���
			if (n == NULL)
				cout << "Not found" << endl;
			// node n�� ���� ���, �ش� node�� ���� ���
			else
				cout << rbt.findDepth(n) << " " << n->name << " " << n->phoneNum << " " << n->x << " " << n->y << endl;
		}
		// node�� ����� ��� update ���
		if (c == 'A')
		{
			int key, pay;
			string sick;
			cin >> key >> sick >> pay;

			// �ش� key ���� ������ node n�� Ž��
			node* n = rbt.findNode(key);

			// node n�� ���� ���
			if (n == NULL)
				cout << "Not found" << endl;
			// node n�� ���� ���, �ش� node�� ����� ��� update
			else
			{
				n->p.first = sick;
				n->p.second = pay;
				// �ش� node �� depth ���
				cout << rbt.findDepth(n) << endl;
			}
		}
		// �ش� ����� ���� ������ ������ node�� ���� ���
		if (c == 'E')
		{
			string sick;
			cin >> sick;
			int cnt = 0;
			// ���Ե� ��� node�� Ȯ���ϸ� ������ ���� ��� cnt++
			for (int i = 0; i < rbt.list.size(); i++)
			{
				if (rbt.list[i]->p.first == sick)
					cnt++;
			}
			cout << cnt << endl;
		}
	}

	return 0;
}