#include <iostream>
#include <vector>
#include <set>
#include <chrono>
#include <windows.h>
#include <conio.h>
#include <iomanip>
#include <fstream>

using namespace std;

int m_count = 0;
int subm_crT_count = 0;
int size_count = 0;

bool fl_tree = FALSE;

void SetColor(int text, int bg) {
	HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hStdOut, (WORD)((bg << 4) | text));
}

int checkdigit() // �������� �� ���������� ����
{
	while (true)
	{
		int value;
		// ������ �����, ������� ����� ����������� � �������� �����
		cin >> value; // ����� �����
		if (cin.fail()) // ec�� ���������� ����������� �������� ���������,
		{
			cin.clear(); // �� ���������� cin � ������� ����� ������
			cin.ignore(32767, '\n'); // � ������� �� ������ �������� ����������� ����� 
			cout << "������������ �������� �����. ������� ����� ���������" << '\n';
		}
		else
		{
			cin.ignore(32767, '\n'); // ������� �� ������ �������� ����������� ����� 
			return value;
		}
	}
}

///////////////////////// PROTOTYPES //////////////////////////
struct Tree;
void menu(Tree*& root);
void submenu_create_tree(Tree*& root);
void menu_choice(Tree*& root);
void submenu_create_tree_choice(Tree*& root);
void conf_val(Tree*& root);
void subconf_crT_val(Tree*& root);

///////////////////////// PROTOTYPES //////////////////////////

struct Tree {
	int key;
	char height;
	Tree* right;
	Tree* left;
	Tree(int data) { key = data; height = 1; left = right = 0; }
};

char height(Tree* root) {
	if (root) return root->height;
	else return 0;
}

int BF(Tree* root) {
	return height(root->right) - height(root->left);
}

void OverHeight(Tree* root) {
	char hleft = height(root->left);
	char hright = height(root->right);
	root->height = (hleft > hright ? hleft : hright) + 1;
}

Tree* RightRotation(Tree* x) {
	Tree* y = x->left;
	x->left = y->right;
	y->right = x;
	OverHeight(x);
	OverHeight(y);
	return y;
}
Tree* LeftRotation(Tree* y) {
	Tree* x = y->right;
	y->right = x->left;
	x->left = y;
	OverHeight(y);
	OverHeight(x);
	return x;
}

Tree* Balance(Tree* root) {
	OverHeight(root);
	if (BF(root) == 2) {
		if (BF(root->right) < 0) root->right = RightRotation(root->right);
		return LeftRotation(root);
	}
	if (BF(root) == -2) {
		if (BF(root->left) > 0) root->left = LeftRotation(root->left);
		return RightRotation(root);
	}
	return root;
}

Tree* Insert(Tree* root, int data) {
	if (!root) return new Tree(data);
	if (data < root->key) root->left = Insert(root->left, data);
	else root->right = Insert(root->right, data);
	return Balance(root);
}

Tree* SearchMin(Tree* root) {
	if (root->left) return SearchMin(root->left);
	else return root;
}
Tree* DeleteMin(Tree* root) {
	if (root->left == 0) return root->right;
	root->left = DeleteMin(root->left);
	return Balance(root);
}

Tree* Delete(Tree* root, int data) {
	if (!root) return 0;
	if (data < root->key) root->left = Delete(root->left, data);
	else if (data > root->key) root->right = Delete(root->right, data);
	else {
		Tree* y = root->left;
		Tree* z = root->right;
		delete root;
		if (!z) return y;
		Tree* min = SearchMin(z);
		min->right = DeleteMin(z);
		min->left = y;
		return Balance(min);
	}
	return Balance(root);
}

Tree* getTreeByValue(Tree* root, int value) {
	while (root) {
		if (root->key > value) {
			root = root->left;
			continue;
		}
		else if (root->key < value) {
			root = root->right;
			continue;
		}
		else {
			return root;
		}
	}
	return NULL;
}

void sizeTree(Tree* root) {
	if (root) {
		size_count++;
		sizeTree(root->left);
		sizeTree(root->right);
	}
}

void widthValue(int step, string object) {
	for (int i = 0; i <= step - 1; i++) {
		cout << "     ";
	}
	if (object == "right")
		cout << ".--->";
	else if (object == "left")
		cout << "`--->";
	else
		cout << "  -->";
}

void widthValue_file(int step, string object, ofstream& fout) {
	for (int i = 0; i <= step - 1; i++) {
		fout << "     ";
	}
	if (object == "right")
		fout << ".--->";
	else if (object == "left")
		fout << "`--->";
	else
		fout << "  -->";
}

void printColorData(int current_data, int data) {
	if (current_data == data) {
		SetColor(2, 0);
		cout << current_data << endl;
		SetColor(7, 0);
	}
	else
		cout << current_data << endl;
}

void printTree_value(Tree* root, int step, string object, int value) {
	if (root->right) {
		printTree_value(root->right, step + 1, "right", value);
		widthValue(step, object);
		printColorData(root->key, value);
	}
	if (!root->right && root->left) {
		widthValue(step, object);
		printColorData(root->key, value);
		printTree_value(root->left, step + 1, "left", value);
		return;
	}
	if (root->left) {
		printTree_value(root->left, step + 1, "left", value);
		return;
	}
	if (root->right && !root->left)
		return;
	widthValue(step, object);
	printColorData(root->key, value);

}

void printTree(Tree* root, int step, string object) {
	if (root->right) {
		printTree(root->right, step + 1, "right");
		widthValue(step, object);
		cout << root->key << endl;
	}
	if (!root->right && root->left) {
		widthValue(step, object);
		cout << root->key << endl;
		printTree(root->left, step + 1, "left");
		return;
	}
	if (root->left) {
		printTree(root->left, step + 1, "left");
		return;
	}
	if (root->right && !root->left)
		return;
	widthValue(step, object);
	cout << root->key << endl;

}

void printTree_file(Tree* root, int step, string object, ofstream& fout) {
	if (root->right) {
		printTree_file(root->right, step + 1, "right", fout);
		widthValue_file(step, object, fout);
		fout << root->key << endl;
	}
	if (!root->right && root->left) {
		widthValue_file(step, object, fout);
		fout << root->key << endl;
		printTree_file(root->left, step + 1, "left", fout);
		return;
	}
	if (root->left) {
		printTree_file(root->left, step + 1, "left", fout);
		return;
	}
	if (root->right && !root->left)
		return;
	widthValue_file(step, object, fout);
	fout << root->key << endl;
}

bool fill_set(set<int>& root_set, int value) {
	int temp, temp_value;
	temp = root_set.size();
	root_set.insert(value);
	if (temp != root_set.size())
		return TRUE;
	else
		return FALSE;
}

void fill_root(vector<int>& arr, Tree*& root) {
	for (int i = 0; i < arr.size(); i++) {
		root = Insert(root, arr[i]);
	}
}

void fill_set_via_Tree(Tree*& root, set<int>& arr) {
	if (root) {
		arr.insert(root->key);
		fill_set_via_Tree(root->right, arr);
		fill_set_via_Tree(root->left, arr);
	}
}

void fill_vec_via_Tree(Tree*& root, vector<int>& arr) {
	if (root) {
		arr.push_back(root->key);
		fill_vec_via_Tree(root->right, arr);
		fill_vec_via_Tree(root->left, arr);
	}
}

Tree* getTreeNoRepeatValues(Tree*& root) {
	Tree* tmp = NULL;
	set <int> TreeVal;
	vector <int> numbers_unic;
	vector<int> CopyTree;
	fill_vec_via_Tree(root, CopyTree);
	vector <int> RepeatValues;

	for (int i = 0; i < CopyTree.size(); i++) {
		int temp = TreeVal.size();
		int temp_value = CopyTree[i];
		TreeVal.insert(temp_value);
		if (temp != TreeVal.size())
			numbers_unic.push_back(temp_value);
		else
			RepeatValues.push_back(temp_value);
	}
	if (CopyTree.size() != numbers_unic.size()) {
		cout << "������ ����� ������������� ����. ��� ���� ���� �������" << endl;
		cout << "������������� ��������: "; for (const auto& token : RepeatValues) { cout << token << " "; } cout << endl;
		fill_root(numbers_unic, tmp);
		return tmp;
	}
	fill_root(numbers_unic, tmp);
	return tmp;
}

void individual_del(Tree*& root, vector<int>& neg_values) {
	if (root) {
		if (root->key < 0) {
			neg_values.push_back(root->key);
		}
		individual_del(root->right, neg_values);
		individual_del(root->left, neg_values);
	}
}

void del_values(Tree*& root, vector<int>& neg_values) {
	while (neg_values.size()) {
		int temp = neg_values[neg_values.size() - 1];
		neg_values.pop_back();
		root = Delete(root, temp);
	}
}

void individual_ex(Tree*& root, int level) {
	if (root) {
		if (level % 2 != 0) { // �������� �� ����� � ���������� �� -2 �� 2
			int temp = (rand() % 5) - 2;
			cout << "������� ����� " << root->key << " ����� �������� �� " << temp << endl;
			root->key *= temp;
		}
		individual_ex(root->left, level + 1);
		individual_ex(root->right, level + 1);
	}
}

void individual(Tree*& root) {
	system("cls");
	chrono::steady_clock sc;
	vector<int> neg_values;

	cout << setw(59) << right << "�������������� �������" << endl;
	cout << setw(65) << right << "������� ��� ������������� �����." << endl;
	cout << setw(90) << right << "������ �������� ������� �������� �� ��������� �������� � ��������� �� - 2 �� 2. " << endl;
	SetColor(6, 0);
	cout << setw(65) << right << "������� ����� �������, ����� ������.";
	SetColor(7, 0);
	_getch();
	cout << endl;
	system("cls");

	cout << setw(30) << right << "��� ������:" << endl;
	printTree(root, 0, " "); cout << endl;
	auto start = sc.now();
	individual_del(root, neg_values);
	del_values(root, neg_values);
	individual_ex(root, 0);
	root = getTreeNoRepeatValues(root);
	cout << setw(30) << right << "���������� ��� ������:" << endl;
	printTree(root, 0, " "); cout << endl;
	auto end = sc.now();       // ������������� ����� ������� ������� �������
	auto time_span = static_cast<chrono::duration<double>>(end - start);   // ����������� �����, ����������� �� �������
	cout << "�������� ������ ��� ������ �����: " << time_span.count() << " ������" << endl;
	cout << "�������������� ������� ���������. ������� ����� �������" << endl;
	_getch();
	menu(root);
}

void delete_node(Tree*& root) {
	system("cls");

	int value;
	int prev_count;
	chrono::steady_clock sc;

	size_count = 0;
	sizeTree(root);
	prev_count = size_count;
	size_count = 0;

	cout << setw(30) << right << "��� ������:" << endl;
	printTree(root, 0, " ");
	cout << "������� �������, ������� ���������� �������: ";
	value = checkdigit();
	auto start = sc.now();
	root = Delete(root, value);
	sizeTree(root);
	if (size_count == prev_count) {
		cout << "�������� �� �������. ������� ����� �������, ����� ����������" << endl;
		_getch();
		menu(root);
	}
	auto end = sc.now();       // ������������� ����� ������� ������� �������
	auto time_span = static_cast<chrono::duration<double>>(end - start);   // ����������� �����, ����������� �� �������

	cout << "�������� �������� ���� �����: " << time_span.count() << " ������" << endl;
	cout << "���� ������� ������. ������� ����� �������" << endl;
	if (!root) {
		SetColor(4, 0);
		cout << "������ ���� ��������� �������." << endl;
		SetColor(7, 0);
		fl_tree = FALSE;
	}
	_getch();
	menu(root);
}

void insert_node(Tree*& root) {
	system("cls");

	int value;
	chrono::steady_clock sc;
	set<int> root_set;
	double time = 0;

	fill_set_via_Tree(root, root_set);
	if (root) {
		cout << setw(30) << right << "��� ������:" << endl;
		printTree(root, 0, " ");
	}
	cout << "������� �������� ��������: ";
	value = checkdigit();
	if (fill_set(root_set, value)) {
		auto start = sc.now();
		root = Insert(root, value);
		auto end = sc.now();
		auto time_span = static_cast<chrono::duration<double>>(end - start);
		time += time_span.count();
	}
	else {
		SetColor(4, 0);
		cout << "�������� �������� �� �����. ����� �������� ��� ���� � ��� ������" << endl;
		SetColor(7, 0);
		cout << "������� ����� �������, ����� ����������" << endl;
		_getch();
		menu(root);
	}

	cout << "�������� ������� ���� �����: " << time << " ������" << endl;
	cout << "���� ������� ��������. ������� ����� �������" << endl;
	_getch();
	fl_tree = TRUE;
	menu(root);
}

void find_node(Tree*& root) {
	system("cls");

	int value;
	chrono::steady_clock sc;
	Tree* findTree = NULL;


	cout << setw(30) << right << "��� ������:" << endl;
	printTree(root, 0, " ");
	cout << "������� �������, ������� ���������� �����: ";
	value = checkdigit();
	auto start = sc.now();
	findTree = getTreeByValue(root, value);
	if (findTree == NULL) {
		cout << "�������� �� �������. ������� ����� �������, ����� ����������" << endl;
		_getch();
		menu(root);
	}
	else {
			cout << setw(30) << right << "��� ������ � �������� " << findTree->key << ":" << endl;
			printTree_value(root, 0, " ", findTree->key);
	}
	auto end = sc.now();       // ������������� ����� ������� ������� �������
	auto time_span = static_cast<chrono::duration<double>>(end - start);   // ����������� �����, ����������� �� �������

	cout << "�������� ������ ���� �����: " << time_span.count() << " ������" << endl;
	cout << "���� ������� ������. ������� ����� �������" << endl;
	_getch();
	menu(root);
}

void fill_root_random(Tree*& root) {
	system("cls"); // ������� �������
	root = NULL;
	int size = 0;
	int index = 0;
	chrono::steady_clock sc;
	set<int> root_set;

	cout << "������� ���������� ��������� � ������: ";
	size = checkdigit();
	if (size <= 0) {
		cout << "���������� ��������� ������ �� ����� ���� ������������� ��� �������. ������� ����� �������, ����� ����������.";
		_getch();
		menu(root);
	}
	auto start = sc.now();     // ������������� ������ ������� ������� �������
	while (index <= size - 1) {
		int temp = (rand() % 200) - 100;
		if (fill_set(root_set, temp)) {
			root = Insert(root, temp);
			index++;
		}
	}
	auto end = sc.now();       // ������������� ����� ������� ������� �������
	auto time_span = static_cast<chrono::duration<double>>(end - start);   // ����������� �����, ����������� �� �������
	cout << "�������� �������� ��� ������ �����: " << time_span.count() << " ������" << endl;

	cout << "��� ������ ������� �������. ������� ����� �������" << endl;
	_getch();
	fl_tree = TRUE;
	menu(root);
}

void fill_root_self(Tree*& root) {
	system("cls"); // ������� �������
	root = NULL;
	int temp;
	int index = 0;
	chrono::steady_clock sc;
	bool fl_stop = TRUE;
	set <int> root_set;
	double summ_time = 0;
	cout << "������� �������� ��������. ���� ������ ���������, �� ������� ����� ���������� ������" << endl;

	while (fl_stop) {
		cout << index << ": ";
		cin >> temp; // ����� �����

		if (cin.fail()) {
			cin.clear(); // �� ���������� cin � ������� ����� ������
			cin.ignore(32767, '\n'); // � ������� �� ������ �������� ����������� ����� 
			fl_stop = FALSE;
		}
		else {
			cin.ignore(32767, '\n');
			if (fill_set(root_set, temp)) {
				auto start = sc.now();
				root = Insert(root, temp);
				auto end = sc.now();
				auto time_span = static_cast<chrono::duration<double>>(end - start);
				summ_time += time_span.count();
				index++;
			}
			else {
				SetColor(4, 0);
				cout << "�������� �������� �� �����. ����� �������� ��� ���� � ��� ������" << endl;
				SetColor(7, 0);
			}
		}

	}
	cout << "�������� �������� ��� ������ �����: " << summ_time << " ������" << endl;

	cout << "��� ������ ������� �������. ������� ����� �������" << endl;
	_getch();
	fl_tree = TRUE;
	menu(root);
}

void fill_root_from_file(Tree*& root) {
	system("cls"); // ������� �������
	root = NULL;
	ifstream fin;
	chrono::steady_clock sc;
	set <int> root_set;
	vector<int> RepeatValues;

	fin.open("file_root.txt");
	auto start = sc.now();
	if (fin.is_open()) {
		int temp = 0;
		while (!fin.eof()) {
			fin >> temp;
			if (fin.fail()) {
				fin.clear();
				fin.ignore(32767, '\n');
				cout << "���� �������� ������������ �������� ������. ���������� ������� ��������� � ������ � ��������� ���� ������ ��������� (����� ��������� ���������� ������, ����������� ������ ���� �� ������)." << endl;
				cout << "������: 50 -3 45 14 0 4 10 1" << endl;
				cout << "�������, ����� ������� ���������" << endl;
				_getch();
				exit(3);
			}
			else {
				if (fill_set(root_set, temp)) {
					root = Insert(root, temp);
				}
				else
					RepeatValues.push_back(temp);
			}
		}
	}
	else {
		cout << "���� ������� �� �������! �������� ��������� � �������� � �������� ����� ��������� ���� file_root.txt ��� ���������� ������ ����� ����." << endl;
		cout << "�������, ����� ������� ���������" << endl;
		_getch();
		exit(3);
	}

	auto end = sc.now();       // ������������� ����� ������� ������� �������
	auto time_span = static_cast<chrono::duration<double>>(end - start);   // ����������� �����, ����������� �� �������
	fin.close();
	if (RepeatValues.size() != 0) {
		cout << "������������� ��������: "; for (const auto& token : RepeatValues) { cout << token << " "; } cout << endl;
	}
	cout << "�������� �������� ������ �����: " << time_span.count() << " ������" << endl;

	cout << "��� ������ ������� �������. ������� ����� �������" << endl;
	_getch();
	fl_tree = TRUE;
	menu(root);
}

void correctBalance(Tree*& root) {
	system("cls");
	chrono::steady_clock sc;
	cout << setw(30) << right << "��� ������:" << endl;
	printTree(root, 0, " ");
	cout << endl;
	auto start = sc.now();
	root = Balance(root);
	auto end = sc.now();       // ������������� ����� ������� ������� �������
	auto time_span = static_cast<chrono::duration<double>>(end - start);   // ����������� �����, ����������� �� �������
	SetColor(2, 0);
	cout << "������ ��������������." << endl;
	SetColor(7, 0);
	cout << "�������� �������� �� ������������������ �����: " << time_span.count() << " ������" << endl;

	cout << "������� ����� �������" << endl;
	_getch();
	menu(root);
}

///////////////// MENU ////////////////////

void print_alarm(Tree*& root) {
	SetColor(4, 0);
	cout << endl << "������ ���������� ������� ������. ������� ����� �������";
	SetColor(7, 0);
	_getch();
	submenu_create_tree(root);
}

void subconf_crT_val(Tree*& root) {
	switch (subm_crT_count) {
	case 0:
		fill_root_random(root);
		break;
	case 1:
		fill_root_self(root);
		break;
	case 2:
		fill_root_from_file(root);
		break;
	case 3:
		menu(root);
		break;
	default:
		break;
	}
}

void conf_val(Tree*& root) {
	switch (m_count) {
	case 0:
		submenu_create_tree(root);
		break;
	case 1:
		insert_node(root);
		break;
	case 2:
		if (fl_tree)
			delete_node(root);
		else
			print_alarm(root);
		break;
	case 3:
		if (fl_tree)
			find_node(root);
		else
			print_alarm(root);
		break;
	case 4:
		if (fl_tree) {
			ofstream fout;
			fout.open("AVL_tree.txt", ios::trunc);
			system("cls");
			cout << setw(30) << right << "��� ������:" << endl;
			fout << setw(30) << right << "��� ������:" << endl;
			printTree(root, 0, " ");
			printTree_file(root, 0, " ", fout);
			cout << "������ ���� ��������� � ��������� ����� AVL_tree.txt" << endl;
			cout << "������� ����� �������, ����� ��������� � ����.";
			_getch();
			fout.close();
		}
		else
			print_alarm(root);
		break;
	case 5:
		if (fl_tree)
			correctBalance(root);
		else
			print_alarm(root);
		break;
	case 6:
		if (fl_tree)
			individual(root);
		else
			print_alarm(root);
		break;
	case 7:
		SetColor(0, 0);
		exit(0);
		break;
	default:
		break;
	}
	menu(root);
}

void submenu_create_tree_choice(Tree*& root) {
	int k1;
	k1 = _getch(); // �������� ������ ������� ��� ������ �����
	if (k1 == 0xE0) { // ���� �������
		switch (k1) {
		case 0x48: // ������� �����
			subm_crT_count--;
			if (subm_crT_count < 0) subm_crT_count = 0;
			submenu_create_tree(root);
			break;

		case 0x50: // ������� ����
			subm_crT_count++;
			if (subm_crT_count > 3) subm_crT_count = 3;
			submenu_create_tree(root);
			break;
		case 0xD: // �����������
			subconf_crT_val(root);
			break;
		default:
			submenu_create_tree_choice(root);
		}
	}
	switch (k1) {
	case 0x48: // ������� �����
		subm_crT_count--;
		if (subm_crT_count < 0) subm_crT_count = 0;
		submenu_create_tree(root);
		break;

	case 0x50: // ������� ����
		subm_crT_count++;
		if (subm_crT_count > 3) subm_crT_count = 3;
		submenu_create_tree(root);
		break;
	case 0xD: // �����������
		subconf_crT_val(root);
		break;
	default:
		submenu_create_tree_choice(root);
	}
}

void menu_choice(Tree*& root) {
	int k1;
	k1 = _getch(); // �������� ������ ������� ��� ������ �����
	if (k1 == 0xE0) { // ���� �������
		switch (k1) {
		case 0x48: // ������� �����
			m_count--;
			if (m_count < 0) m_count = 0;
			menu(root);
			break;

		case 0x50: // ������� ����
			m_count++;
			if (m_count > 7) m_count = 7;
			menu(root);
			break;
		case 0xD: // �����������
			conf_val(root);
			break;
		default:
			menu_choice(root);
		}
	}
	switch (k1) {
	case 0x48: // ������� �����
		m_count--;
		if (m_count < 0) m_count = 0;
		menu(root);
		break;

	case 0x50: // ������� ����
		m_count++;
		if (m_count > 7) m_count = 7;
		menu(root);
		break;
	case 0xD: // �����������
		conf_val(root);
		break;
	default:
		menu_choice(root);
	}
}

void submenu_create_tree(Tree*& root) {
	system("cls"); // ������� �������

	if (subm_crT_count == 0) {
		cout << right << "__�������� ������__" << endl << endl;
		SetColor(3, 0);
		cout << "����� ������� ������� ��� ������?" << endl << endl;
		SetColor(7, 0);
		SetColor(5, 0);
		cout << "�������������, � �������� ������������" << endl;
		SetColor(7, 0);
		cout << "�������, �������� ��������" << endl;
		cout << "���������� � ������� ���������� �����" << endl;
		cout << "�����" << endl;
		submenu_create_tree_choice(root);
	}
	if (subm_crT_count == 1) {
		cout << right << "__�������� ������__" << endl << endl;
		SetColor(3, 0);
		cout << "����� ������� ������� ��� ������?" << endl << endl;
		SetColor(7, 0);
		cout << "�������������, � �������� ������������" << endl;
		SetColor(5, 0);
		cout << "�������, �������� ��������" << endl;
		SetColor(7, 0);
		cout << "���������� � ������� ���������� �����" << endl;
		cout << "�����" << endl;
		submenu_create_tree_choice(root);
	}
	if (subm_crT_count == 2) {
		cout << right << "__�������� ������__" << endl << endl;
		SetColor(3, 0);
		cout << "����� ������� ������� ��� ������?" << endl << endl;
		SetColor(7, 0);
		cout << "�������������, � �������� ������������" << endl;
		cout << "�������, �������� ��������" << endl;
		SetColor(5, 0);
		cout << "���������� � ������� ���������� �����" << endl;
		SetColor(7, 0);
		cout << "�����" << endl;
		submenu_create_tree_choice(root);
	}
	if (subm_crT_count == 3) {
		cout << right << "__�������� ������__" << endl << endl;
		SetColor(3, 0);
		cout << "����� ������� ������� ��� ������?" << endl << endl;
		SetColor(7, 0);
		cout << "�������������, � �������� ������������" << endl;
		cout << "�������, �������� ��������" << endl;
		cout << "���������� � ������� ���������� �����" << endl;
		SetColor(5, 0);
		cout << "�����" << endl;
		SetColor(7, 0);
		submenu_create_tree_choice(root);
	}
}

void menu(Tree*& root) {
	system("cls"); // ������� �������

	if (m_count == 0) {
		cout << setw(15) << right << "__����__" << endl;
		SetColor(5, 0);
		cout << "������� ��� ������" << endl;
		SetColor(7, 0);
		cout << "�������� ������� � ��� ������" << endl;
		cout << "������� ������� ��� ������" << endl;
		cout << "�������� ������� ��� ������" << endl;
		cout << "������� ��� ������" << endl;
		cout << "�������� �� ������������������" << endl;
		cout << "������� �� ��������" << endl;
		cout << "�����" << endl;
		menu_choice(root);
	}
	if (m_count == 1) {
		cout << setw(15) << right << "__����__" << endl;
		cout << "������� ��� ������" << endl;
		SetColor(5, 0);
		cout << "�������� ������� � ��� ������" << endl;
		SetColor(7, 0);
		cout << "������� ������� ��� ������" << endl;
		cout << "�������� ������� ��� ������" << endl;
		cout << "������� ��� ������" << endl;
		cout << "�������� �� ������������������" << endl;
		cout << "������� �� ��������" << endl;
		cout << "�����" << endl;
		menu_choice(root);
	}
	if (m_count == 2) {
		cout << setw(15) << right << "__����__" << endl;
		cout << "������� ��� ������" << endl;
		cout << "�������� ������� � ��� ������" << endl;
		SetColor(5, 0);
		cout << "������� ������� ��� ������" << endl;
		SetColor(7, 0);
		cout << "�������� ������� ��� ������" << endl;
		cout << "������� ��� ������" << endl;
		cout << "�������� �� ������������������" << endl;
		cout << "������� �� ��������" << endl;
		cout << "�����" << endl;
		menu_choice(root);
	}
	if (m_count == 3) {
		cout << setw(15) << right << "__����__" << endl;
		cout << "������� ��� ������" << endl;
		cout << "�������� ������� � ��� ������" << endl;
		cout << "������� ������� ��� ������" << endl;
		SetColor(5, 0);
		cout << "�������� ������� ��� ������" << endl;
		SetColor(7, 0);
		cout << "������� ��� ������" << endl;
		cout << "�������� �� ������������������" << endl;
		cout << "������� �� ��������" << endl;
		cout << "�����" << endl;
		menu_choice(root);
	}
	if (m_count == 4) {
		cout << setw(15) << right << "__����__" << endl;
		cout << "������� ��� ������" << endl;
		cout << "�������� ������� � ��� ������" << endl;
		cout << "������� ������� ��� ������" << endl;
		cout << "�������� ������� ��� ������" << endl;
		SetColor(5, 0);
		cout << "������� ��� ������" << endl;
		SetColor(7, 0);
		cout << "�������� �� ������������������" << endl;
		cout << "������� �� ��������" << endl;
		cout << "�����" << endl;
		menu_choice(root);
	}
	if (m_count == 5) {
		cout << setw(15) << right << "__����__" << endl;
		cout << "������� ��� ������" << endl;
		cout << "�������� ������� � ��� ������" << endl;
		cout << "������� ������� ��� ������" << endl;
		cout << "�������� ������� ��� ������" << endl;
		cout << "������� ��� ������" << endl;
		SetColor(5, 0);
		cout << "�������� �� ������������������" << endl;
		SetColor(7, 0);
		cout << "������� �� ��������" << endl;
		cout << "�����" << endl;
		menu_choice(root);
	}
	if (m_count == 6) {
		cout << setw(15) << right << "__����__" << endl;
		cout << "������� ��� ������" << endl;
		cout << "�������� ������� � ��� ������" << endl;
		cout << "������� ������� ��� ������" << endl;
		cout << "�������� ������� ��� ������" << endl;
		cout << "������� ��� ������" << endl;
		cout << "�������� �� ������������������" << endl;
		SetColor(5, 0);
		cout << "������� �� ��������" << endl;
		SetColor(7, 0);
		cout << "�����" << endl;
		menu_choice(root);
	}
	if (m_count == 7) {
		cout << setw(15) << right << "__����__" << endl;
		cout << "������� ��� ������" << endl;
		cout << "�������� ������� � ��� ������" << endl;
		cout << "������� ������� ��� ������" << endl;
		cout << "�������� ������� ��� ������" << endl;
		cout << "������� ��� ������" << endl;
		cout << "�������� �� ������������������" << endl;
		cout << "������� �� ��������" << endl;
		SetColor(5, 0);
		cout << "�����" << endl;
		SetColor(7, 0);
		menu_choice(root);
	}
}

///////////////// MENU ////////////////////


int main() {
	setlocale(0, "");
	srand(time(NULL));

	Tree* root = NULL;
	menu(root);

	system("pause");
	return 0;
}
