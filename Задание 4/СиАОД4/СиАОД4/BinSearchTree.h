#pragma once
#include <iostream>

using namespace std;

struct node
{
	int key = 0;
	int numRecordInBinFile = 0;
	node* left = nullptr;
	node* right = nullptr;
};

typedef node* Tbin;

void Insert(Tbin& T, int key, int numRecord)
{
	if (T == NULL)
	{
		T = new node;
		T->key = key;
		T->numRecordInBinFile = numRecord;
	}
	else
	{
		if (key < T->key)
			Insert(T->left, key, numRecord);
		else
			if (key > T->key)
				Insert(T->right, key, numRecord);
	}
}
// ����� ������ � ������ T � ������ key
//���������� ��������� �� ��������� ���� ��� NULL
Tbin Search(Tbin T, int key)
{
	if (T == NULL)
		return NULL;
	else
	{
		if (T->key == key)
			return T;
		else
		{
			if (key < T->key)
				return Search(T->left, key);
			else
				return Search(T->right, key);
		}
	}
}
void printT(Tbin T, int l, int k)
{
	if (T != NULL)
	{
		printT(T->right, l + 1, k);
		for (int i = 1; i <= l * k; i++) {
			cout << ' ';
		}
		cout << T->key << "\n";
		printT(T->left, l + 1, k);
	}
}

void del(Tbin& r, Tbin& q) {
	if (r->right)
		del(r->right, q);
	else {
		q->key = r->key;
		q = r;
		r = r->left; //����� �.�., ���������� ����,
		 //������� ����� ������, ��� �������� �����
		// ���� �.�. ������� ���
	}
}


int delTree(int x, Tbin& r) {
	Tbin q;
	if (r == NULL)return 1;
	if (r)
	{
		if (x < r->key)
			delTree(x, r->left);
		else
		{
			if (x > r->key)
				delTree(x, r->right);
			else // delete - ����� ���� ��� ��������
			{
				q = r;
				if (q->right == NULL) //2.1 � 2.2
					r = q->left; //��� 2.1 r=NULL ��� 2.2 r � ������
					// �� ����� �.�.
				else
					if (q->left == NULL) //��� 2.3
						r = q->right;
					else
						//����� ����������� ���� ��� 2.4
						del(q->left, q);
			}
			//delete q;
			return 0;
		}
	}
}


//���������� ������ ������� �� �������
void Create_Tree(Tbin& T, int x[], int n) {
	for (int i = 0; i < n; i++) {
		Insert(T, x[i], i);
	}
}
