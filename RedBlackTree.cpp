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
	pair<string, int> p;		// 병명과 진료비
	bool color;					// true = red , false = black
	node* par;
	node* child[2];				// [0] = 왼쪽자식, [1] = 오른쪽자식
	node()
	{
		key = -1;
		name = "";
		phoneNum = "";
		x = 0, y = 0;
		p.first = "";
		p.second = 0;
		color = true;			// 삽입되는 node는 모두 red
		par = NULL;
		child[0] = NULL;
		child[1] = NULL;
	}
};

// node간에 key 값을 이용한 비교 함수 (오름차순)
bool compare(node* a, node* b) {
	return a->key < b->key;
}

struct RedBlackTree {
	vector<node*> list;							// node의 존재여부를 확인하기 위한 벡터
	node* root;									// root
	RedBlackTree()
	{
		root = NULL;
	}

	// key를 이용하여 node를 찾는 함수
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

	// bst방식으로 node의 깊이 탐색
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

			// 찾고자 하는 node의 key 값이 탐색하는 node의 key 값보다 작은경우 왼쪽으로 탐색
			if (tKey > nKey)
			{
				tmp = tmp->child[0];
				depth++;
			}
			else
			{													// 더 큰 경우 오른쪽으로 탐색
				tmp = tmp->child[1];
				depth++;
			}
		}

		return depth;
	}

	// root가 NULL일 경우 최초 node를 root로 지정
	void insertRoot(node* n)
	{
		list.push_back(n);
		root = n;

		// leaf 노드를 달아주는 과정
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

		// root 의 color는 항상 black
		root->color = false;
	}

	// double red의 경우 restructuring 하는 과정
	void restructuring(node* z, node* v, node* gp)
	{
		// 비교함수를 통해 3개의 node를 정렬 시켜 중간 node를 찾음
		vector<node*> vec;
		vec.push_back(z);
		vec.push_back(v);
		vec.push_back(gp);
		sort(vec.begin(), vec.end(), compare);

		node* left = vec[0];
		node* mid = vec[1];
		node* right = vec[2];

		vec.clear();

		// 이하 구조 조건을 만족시켜주는 과정
		// z node의 grandParent node가 가장 작은 수일 경우
		if (left == gp)
		{
			// 가장 큰 수의 왼쪽 자식 node 가 mid 값의 node인 경우 ( CH6 강의노트 pg17 1번째 경우 ) 
			if (right->child[0] == mid)
			{
				// mid node가 부모로 올라감
				mid->par = left->par;

				// left의 부모가 NULL이 아닌경우 해당 과정을 통해 부모-자식 관계를 재정립
				if (left->par != NULL)
				{
					if (left->par->child[0] == left)
						left->par->child[0] = mid;
					else
						left->par->child[1] = mid;
				}

				// left node의 오른쪽 자식에 mid node의 왼쪽 자식을 옮겨줌
				left->child[1] = mid->child[0];
				left->child[1]->par = left;

				// right node의 왼쪽 자식에 mid node의 오른쪽 자식을 옮겨줌
				right->child[0] = mid->child[1];
				right->child[0]->par = right;

				// mid node의 자식으로 left, right node를 옮겨줌
				mid->child[0] = left;
				left->par = mid;

				mid->child[1] = right;
				right->par = mid;
			}
			// 가장 큰 수의 부모 node 가 mid 값의 node인 경우 ( CH6 강의노트 pg17 4번째 경우 ) 
			else if (right->par == mid)
			{
				// mid node가 부모로 올라감
				mid->par = left->par;

				// left의 부모가 NULL이 아닌경우 해당 과정을 통해 부모-자식 관계를 재정립
				if (left->par != NULL)
				{
					if (left->par->child[0] == left)
						left->par->child[0] = mid;
					else
						left->par->child[1] = mid;
				}

				// left node의 오른쪽 자식에 mid node의 왼쪽 자식을 옮겨줌
				left->child[1] = mid->child[0];
				left->child[1]->par = left;

				// mid node의 왼쪽 자식에 left node를 옮겨줌
				left->par = mid;
				mid->child[0] = left;
			}

			// left node가 tree에서 root node 였을 경우, mid node가 root가 됌
			if (root == left)
				root = mid;
		}
		// z node의 grandParent node가 가장 큰 수일 경우
		else if (right == gp)
		{
			// 중간 값 node의 parent node가 가장 작은 수의 node일 경우 ( CH6 강의노트 pg17 2번째 경우 )
			if (mid->par == left)
			{
				// mid node가 부모로 올라감
				mid->par = right->par;

				// right의 부모가 NULL이 아닌경우 해당 과정을 통해 부모-자식 관계를 재정립
				if (right->par != NULL)
				{
					if (right->par->child[0] == right)
						right->par->child[0] = mid;
					else
						right->par->child[1] = mid;
				}

				// left node의 오른쪽 자식에 mid node의 왼쪽 자식을 옮겨줌
				left->child[1] = mid->child[0];
				mid->child[0]->par = left;

				// right node의 왼쪽 자식에 mid node의 오른쪽 자식을 옮겨줌
				right->child[0] = mid->child[1];
				mid->child[1]->par = right;

				// mid node의 자식으로 left, right node를 옮겨줌
				mid->child[0] = left;
				left->par = mid;

				mid->child[1] = right;
				right->par = mid;
			}
			// 중간 값 node의 parent node가 가장 큰 수의 node일 경우 ( CH6 강의노트 pg17 3번째 경우 )
			else if (mid->par == right)
			{
				// mid node가 부모로 올라감
				mid->par = right->par;
				if (right->par != NULL)
				{
					if (right->par->child[0] == right)
						right->par->child[0] = mid;
					else
						right->par->child[1] = mid;
				}

				// right node의 왼쪽 자식에 mid node의 오른쪽 자식을 옮겨줌
				right->child[0] = mid->child[1];
				right->child[0]->par = right;

				// mid node의 오른쪽 자식에 right node를 옮겨줌
				mid->child[1] = right;
				right->par = mid;
			}

			// right node가 tree에서 root node 였을 경우, mid node가 root가 됌
			if (root == right)
				root = mid;
		}

		// color 조건을 만족시켜주는 과정
		mid->color = false;						// mid는 black
		left->color = true;						// left는 red
		right->color = true;					// right는 red

		root->color = false;					// root는 항상 black
	}

	// double red의 경우 recoloring 하는 과정
	void recoloring(node* z, node* v, node* gp, node* w)
	{
		// gp node는 grandParent node
		// v,gp,w node의 색상을 반전 -> 조부모인 gp node는 red, 
		//								조부모의 자식인 v와 w node는 black
		v->color = false;
		gp->color = true;
		w->color = false;

		root->color = false;			// root는 항상 black

		if (gp->par == NULL)			// gp node가 root 였을 경우 return
			return;
		else
			if (gp->par->color == true)		// gp node의 색이 red로 변환 되었기에, 
				doubleRed(gp);				// gp node의 parent node와 double red가 발생할 수 있음
	}

	// node 삽입 과정에서 doubleRed 발생 시 처리하는 과정 (z는 삽입된 node)
	void doubleRed(node* z)
	{
		node* v = z->par;				// z node의 부모 node
		node* gp = v->par;				// z node의 조부모 node
		node* w;						// z nonde의 삼촌 node
		if (v->par->child[0] == v)
			w = v->par->child[1];
		else
			w = v->par->child[0];

		if (w->color == false)			// w node가 black일 경우 -> restructuring
			restructuring(z, v, gp);
		if (w->color == true)			// w node가 red일 경우 -> recoloring
			recoloring(z, v, gp, w);
	}

	// root가 NULL이 아닌 경우에 node를 삽입하는 과정
	void insertNode(node* n)
	{
		list.push_back(n);

		node* tmp = root;
		int nKey = n->key;

		// Bst의 방식으로 node의 위치 탐색
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
		// leaf node를 달아주는 과정
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

		// 삽입된 node의 부모 node가 red인 경우 -> double red 발생
		if (n->par->color == true)
			doubleRed(n);

		// root node의 경우 항상 black
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

	// testcase 만큼 반복
	while (tc--)
	{
		// 명령어 입력
		char c;
		cin >> c;

		// node 삽입 명령어
		if (c == 'I')
		{
			int flag;
			int depth;
			int key, x, y, pay;
			string name, pn, sick;

			cin >> key >> name >> pn >> x >> y >> sick >> pay;

			// 해당 key를 가지는 node가 없을 경우
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

				// root node가 NULL인 경우 -> root node로 삽입
				if (rbt.root == NULL)
					rbt.insertRoot(newNode);
				// 그 외 경우 node의 위치를 찾아서 삽입
				else
					rbt.insertNode(newNode);

				// 삽입 성공한 경우 1 출력
				flag = 1;
			}
			else
				flag = 0;		// 이미 해당 key를 가지고 있는 node가 있을 경우 0 출력

			depth = rbt.findDepth(rbt.findNode(key));		// 해당 key를 가지는 node 의 depth 출력

			cout << depth << " " << flag << endl;
		}
		// node 탐색 명령
		if (c == 'F')
		{
			int key;
			cin >> key;

			// 해당 key 값을 가지는 node n을 탐색
			node* n = rbt.findNode(key);

			// node n이 없을 경우
			if (n == NULL)
				cout << "Not found" << endl;
			// node n이 있을 경우, 해당 node의 정보 출력
			else
				cout << rbt.findDepth(n) << " " << n->name << " " << n->phoneNum << " " << n->x << " " << n->y << endl;
		}
		// node에 병명과 비용 update 명령
		if (c == 'A')
		{
			int key, pay;
			string sick;
			cin >> key >> sick >> pay;

			// 해당 key 갑을 가지는 node n을 탐색
			node* n = rbt.findNode(key);

			// node n이 없을 경우
			if (n == NULL)
				cout << "Not found" << endl;
			// node n이 있을 경우, 해당 node의 병명과 비용 update
			else
			{
				n->p.first = sick;
				n->p.second = pay;
				// 해당 node 의 depth 출력
				cout << rbt.findDepth(n) << endl;
			}
		}
		// 해당 병명과 같은 병명을 가지는 node의 갯수 출력
		if (c == 'E')
		{
			string sick;
			cin >> sick;
			int cnt = 0;
			// 삽입된 모든 node를 확인하며 병명이 같을 경우 cnt++
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