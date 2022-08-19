#include <stdio.h>
#define SIZE 60

struct tree {
	tree* parent;
	tree* brothers;
	tree* sons;
	int label;
};

void create_tree(tree& t) {
	t.parent = t.brothers = t.sons = NULL;
	t.label = 0;
}
void add_brother(tree*& t, int label) {
	tree* temp;
	temp = new tree;
	temp->parent = t->parent;
	temp->label = label;
	temp->brothers = temp->sons = NULL;
	t->brothers = temp;
	t = temp;
}
void add_son(tree*& t, int label) {
	tree* temp;
	temp = new tree;
	temp->parent = *&t;
	if (t->sons != NULL) {
		while (t->sons->brothers != NULL) {
			t->sons->brothers = t->sons->brothers->brothers;
		}
		t->sons->brothers = temp;
	}
	else {
		t->sons = temp;
	}
	temp->label = label;
	temp->brothers = temp->sons = NULL;
	t = temp;
}
void back_to_parent(tree*& t) {
	t = t->parent;
}
void decode_trees(int& lab, tree*& d, char* option) {
	int i = 0, number = 0;
	int q = 1;
	while (q) {
		switch (option[i]) {
		case '(':
			if (option[i + 1] >= '1' && option[i + 1] <= '9') {
				number = int(option[i + 1]) - 48;
				if (option[i + 2] == '0') {
					number = 10;
				}
				add_son(d, number);
			}
			else {
				number = lab;
				lab--;
				add_son(d, number);
			}
			break;
		case ',':
			if (option[i + 1] >= '1' && option[i + 1] <= '9') {
				number = int(option[i + 1]) - 48;
				if (option[i + 2] == '0') {
					number = 10;
				}
				add_brother(d, number);
			}
			else {
				number = lab;
				lab--;
				add_brother(d, number);
			}
			break;
		case ')':
			back_to_parent(d);
			break;
		case ';':
			q--;
			break;
		}
		i++;
	}
}
void add_to_table(int** table, int temp, int leaf, int axis) {
	if (axis == 2) {
		table[leaf - 1][10 - temp] = 1;
	}
	else {
		table[10 - temp][leaf - 1] = 1;
	}
}
void pre_order(tree*& trees, int**& table, int temp, bool labeled_yet, int ax) {
	if (temp == trees->label) {
		labeled_yet = true;
	}
	if (trees != NULL) {
		if (labeled_yet) {
			if (trees->label > 0) {
				add_to_table(table, temp, trees->label, ax);
			}
		}
		if (trees->sons != NULL) {
			trees = trees->sons;
			pre_order(trees, table, temp, labeled_yet, ax);
		}
		else if (trees->brothers != NULL) {
			trees = trees->brothers;
			pre_order(trees, table, temp, labeled_yet, ax);
		}
		else if (trees->parent != NULL) {
			while (trees->parent != NULL) {
				if (trees->brothers == NULL) {
					trees = trees->parent;
					if (trees->label == temp) {
						labeled_yet = false;
					}
				}
				else {
					trees = trees->brothers;
					pre_order(trees, table, temp, labeled_yet, ax);
					break;
				}
			}
		}
	}
}
bool pre_order2(tree*& trees, int temp, bool labeled_yet) {
	if (temp == trees->label) {
		labeled_yet = true;
		return true;
	}
	if (trees != NULL) {
		if (trees->sons != NULL) {
			trees = trees->sons;
			return pre_order2(trees, temp, labeled_yet);
		}
		else if (trees->brothers != NULL) {
			trees = trees->brothers;
			return pre_order2(trees, temp, labeled_yet);
		}
		else if (trees->parent != NULL) {
			while (trees->parent != NULL) {
				if (trees->brothers == NULL) {
					trees = trees->parent;
				}
				else {
					trees = trees->brothers;
					return pre_order2(trees, temp, labeled_yet);
					break;
				}
			}
		}
	}
	return false;
}
void create(int** tables, int index1, int index2) {
	for (int i = 0; i < index1; i++) {
		tables[i] = new int[index2];
	}
}
void delete_table(int** tables, int index) {
	for (int i = 0; i < index; i++) {
		delete[] tables[i];
	}
	delete[] tables;
}
void showtable(int** tables, int sizex, int sizey) {
	for (int i = 0; i < sizex; i++) {
		for (int j = 0; j < sizey; j++) {
			printf("%i|", tables[i][j]);
		}
		printf("\n");
	}
	printf("\n");
}
void swap(int* table, int i1, int i2) {
	int temp = table[i1];
	table[i1] = table[i2];
	table[i2] = temp;
}
void check_permutations(int* tab, int sizeX, int**& tables1, int s, int**& tables2, int index1, int index2, int way) {  //heap's algorithm
	int sum = 0;
	if (sizeX == 1) {
		if (way == 1) {
			for (int j = 0; j < s; j++) {
				sum += tables1[j][tab[j]];
			}
		}
		else {
			for (int j = 0; j < s; j++) {
				sum += tables1[tab[j]][j];
			}
		}
	}
	if (sum > tables2[index1][index2]) {
		tables2[index1][index2] = sum;
	}
	for (int i = 0; i < sizeX; i++) {
		check_permutations(tab, sizeX - 1, tables1, s, tables2, index1, index2, way);

		if (sizeX % 2 == 1) {
			swap(tab, 0, sizeX - 1);
		}
		else {
			swap(tab, i, sizeX - 1);
		}
	}
}
void create_perm(int size, int* table) {
	for (int i = 0; i < size; i++) {
		table[i] = i;
	}
}
void vertexSons(int**& tables, int indexX, int indexY, int label, int version) {
	if (version == 1) {
		if (tables[indexX][label] > tables[indexX][indexY]) {
			tables[indexX][indexY] = tables[indexX][label];
		}
	}
	else {
		if (tables[label][indexY] > tables[indexX][indexY]) {
			tables[indexX][indexY] = tables[label][indexY];
		}
	}
}
void algorithm(tree*& trees1, tree*& trees2, int** tables, int indexX, int indexY) {
	tree* poin1, * poin2;
	poin1 = new tree;
	poin2 = new tree;
	int n1 = 0, n2 = 0, way = 0, ii = 0, jj = 0, temp;
	int label1, label2, t;
	pre_order2(trees2, indexX, false);
	if (pre_order2(trees1, indexY, false)) {
		poin1 = trees1->sons;
		if (poin1->label > 0) {
			temp = poin1->label - 1;
		}
		else {
			temp = 10 - (poin1->label);
		}
		vertexSons(tables, 10 - indexY, 10 - indexX, temp, 2);
		n1++;
		poin1 = poin1->brothers;
		while (poin1 != NULL) {
			if (poin1->label > 0) {
				temp = poin1->label - 1;
			}
			else {
				temp = 10 - (poin1->label);
			}
			vertexSons(tables, 10 - indexY, 10 - indexX, temp, 2);
			n1++;
			poin1 = poin1->brothers;
		}
	}
	if (pre_order2(trees2, indexX, false)) {
		poin1 = trees2->sons;
		if (poin1->label > 0) {
			temp = poin1->label - 1;
		}
		else {
			temp = 10 - (poin1->label);
		}
		vertexSons(tables, 10 - indexY, 10 - indexX, temp, 1);
		n2++;
		poin1 = poin1->brothers;
		while (poin1 != NULL) {
			if (poin1->label > 0) {
				temp = poin1->label - 1;
			}
			else {
				temp = 10 - (poin1->label);
			}
			vertexSons(tables, 10 - indexY, 10 - indexX, temp, 1);
			n2++;
			poin1 = poin1->brothers;
		}
	}
	int** permutations = new int* [n1];
	create(permutations, n1, n2);
	poin1 = trees2->sons;
	poin2 = trees1;
	trees1 = trees1->sons;
	while (trees1 != NULL) {
		poin1 = trees2->sons;
		label1 = trees1->label;
		if (label1 <= 0) {
			label1 = 10 - label1;
		}
		else {
			label1--;
		}
		jj = 0;
		while (poin1 != NULL) {
			label2 = poin1->label;
			if (label2 <= 0) {
				label2 = 10 - label2;
			}
			else {
				label2--;
			}
			if (ii < n1 && jj < n2) {
				permutations[ii][jj] = tables[label1][label2];
			}
			jj++;
			poin1 = poin1->brothers;
		}
		ii++;
		trees1 = trees1->brothers;
	}
	if (n1 > n2) {
		t = n1;
		way = 2;
	}
	else {
		t = n2;
		way = 1;
	}
	int* version = new int[t];
	create_perm(t, version);
	if (way == 1) {
		check_permutations(version, t, permutations, n1, tables, 10 - indexY, 10 - indexX, way);
	}
	else {
		check_permutations(version, t, permutations, n2, tables, 10 - indexY, 10 - indexX, way);
	}
	delete_table(permutations, n1);
	delete[] version;
}
void search_theBiggest(int** tables, int iMax, int jMax) {
	int greatest = 0;
	for (int i = 10; i < iMax; i++) {
		for (int j = 10; j < jMax; j++) {
			if (tables[i][j] > greatest) {
				greatest = tables[i][j];
			}
		}
	}
	greatest = 10 - greatest;
	printf("%i\n", greatest);
}
void create_table(tree* trees, int index1, int index2, int* label) {
	int i1 = 11 - label[index1];
	int i2 = 11 - label[index2];
	int** table = new int* [i1];
	create(table, i1, i2);
	for (int i = 0; i < i1; i++) {
		for (int j = 0; j < i2; j++) {
			if (((i == 10) ^ (j == 10)) && (i <= 10 && j <= 10)) {
				table[i][j] = 1;
			}
			else if (i == j && j <= 9) {
				table[i][j] = 1;
			}
			else {
				table[i][j] = 0;
			}
		}
	}
	tree* point1, * point2;
	point1 = new tree;
	point2 = new tree;
	for (int ii = -1; ii >= label[index1]; ii--) {
		point1 = &trees[index1];
		pre_order(point1, table, ii, false, 1);
	}
	for (int ii = -1; ii >= label[index2]; ii--) {
		point1 = &trees[index2];
		pre_order(point1, table, ii, false, 2);
	}
	for (int ii = label[index1]; ii <= 0; ii++) {
		for (int jj = label[index2]; jj <= 0; jj++) {
			point1 = &trees[index1];
			point2 = &trees[index2];
			algorithm(point1, point2, table, jj, ii);
		}
	}
	search_theBiggest(table, 10 - label[index1], 10 - label[index2]);
	//showtable(table, i1, i2);
	delete_table(table, i1);
}
int main() {
	int n;
	char structure[SIZE];
	scanf_s("%i\n", &n);
	int p = 0;
	int lab = -1;
	tree* trees = new tree[n];
	int* labels = new int[n];
	tree* d;
	d = new tree;
	while (p < n) {
		create_tree(trees[p]);
		d = &trees[p];
		gets_s(structure);
		decode_trees(lab, d, structure);
		labels[p] = (lab + 1);
		lab = -1;
		p++;
	}
	for (int i = 0; i < n; i++) {
		for (int j = i + 1; j < n; j++) {
			create_table(trees, i, j, labels);
		}
	}
	delete[] labels;
	delete[] trees;
	return 0;
}

