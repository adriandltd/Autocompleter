#include "autocompleter.h"

bool Autocompleter::startsWith(string mainStr, string toMatch)
{
	if (mainStr.find(toMatch) == 0)
		return true;
	else
		return false;
}

Autocompleter::Autocompleter()
{

}

void Autocompleter::insert(string x, int freq)
{
	Entry temp;
	temp.s = x;
	temp.freq = freq;
	insert_recurse(temp, root);
}

int Autocompleter::size()
{
	return size_recurse(root);
}

void Autocompleter::completions(string x, vector<string>& T)
{
	T.clear();
	vector<Entry> V;
	completions_recurse(x, root, V);

	//sort vector by freq and push to top3 vector
	vector<string> TOP3;
	TOP3.clear();
	for (int j = 0; j < 3; j++)
	{
		int max = 0;
		int index = 0;	//index of top entry
		for (int i = 0; i < V.size(); i++)
		{
			if (V.at(i).freq > max)
			{
				max = V.at(i).freq;
				index = i;
			}
		}
		if (max > 0)
		{
			TOP3.push_back(V.at(index).s);
			V.erase(V.begin() + index);
		}
	}

	T = TOP3;
}

int Autocompleter::size_recurse(Node * root)
{
	if (root == nullptr)
		return 0;
	return size_recurse(root->left) + size_recurse(root->right) + 1;
}

void Autocompleter::completions_recurse(string x, Node * root, vector<Entry>& C)
{
	if (root == nullptr)
		return;
	if ((startsWith(root->e.s, x)))
		C.push_back(root->e);
	completions_recurse(x, root->left, C);
	completions_recurse(x, root->right, C);
}

void Autocompleter::insert_recurse(Entry e, Node * root)
{
	if (root == nullptr)
		root = new Node(e);
	else if (e.s < root->e.s)
	{
		insert_recurse(e, root->left);
		if (height(root->left) - height(root->right) == 2)
			if (e.s < root->left->e.s)
				right_rotate(root);
			else
				doubleWithLeft(root);
	}
	else if (e.s > root->e.s)
	{
		insert_recurse(e, root->right);
		if (height(root->right) - height(root->left) == 2)
			if (root->right->e.s < e.s)
				left_rotate(root);
			else
				doubleWithRight(root);
	}
	else
		;  // Duplicate; do nothing
	root->height = max(height(root->left), height(root->right)) + 1;
}

void Autocompleter::right_rotate(Node * root)
{
	Node *node = root->right;
	root->right = node->left;
	node->left = root;
	root->height = max(height(root->left), height(root->right)) + 1;
	node->height = max(height(node->right), root->height) + 1;
	root = node;
}

void Autocompleter::left_rotate(Node * root)
{
	Node *node = root->left;
	root->left = node->right;
	node->right = root;
	root->height = max(height(root->left), height(root->right)) + 1;
	node->height = max(height(node->left), root->height) + 1;
	root = node;
}

void Autocompleter::doubleWithLeft(Node * root)
{
	left_rotate(root->left);
	right_rotate(root);
}

void Autocompleter::doubleWithRight(Node * root)
{
	left_rotate(root->right);
	right_rotate(root);
}