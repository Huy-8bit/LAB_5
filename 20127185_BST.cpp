#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <cstdlib>
#include <ctime>
using namespace std;
struct info
{
    string keys;
    string values;
};
int sizeFileInput(fstream &fsInFile)
{
    int size = 0;
    while (!fsInFile.eof())
    {
        string temp;
        getline(fsInFile, temp);
        size++;
    }
    return size;
}
void inputFile()
{
    fstream fsInFile("Oxford English info.txt", ios::in);
    fstream fsTest("keys.txt", ios::in);
    int size = sizeFileInput(fsTest);
    fsTest.close();
    if (size < 10)
    {
        fstream fs("keys.txt", ios::out);
        fstream fs1("values.txt", ios::out);

        while (!fsInFile.eof())
        {
            string line;
            getline(fsInFile, line);
            if (line.size() > 3)
            {
                string keys;
                string values;
                stringstream ss(line);
                getline(ss, keys, ' ');
                getline(ss, values, ' ');
                fs << keys << endl;
                fs1 << values << endl;
            }
        }
        fs.clear();
        fs1.clear();
        fs.close();
        fs1.close();
    }
}
struct Node
{
    info Data;
    Node *left;
    Node *right;
};
typedef Node *Tree;

Node *createNode(info D)
{
    Node *p = new Node;
    p->Data = D;
    p->left = NULL;
    p->right = NULL;
    return p;
}

void addNode(Tree &t, Node *node)
{
    if (t != NULL)
    {
        if (t->Data.keys == node->Data.keys)
        {
            return;
        }
        if (t->Data.keys > node->Data.keys)
        {
            addNode(t->left, node);
        }
        else
        {
            addNode(t->right, node);
        }
    }
    else
    {
        t = node;
    }
}

Node *minValueNode(Node *node)
{
    Node *p = node;
    while (p && p->left != NULL)
    {
        p = p->left;
    }
    return p;
}
Node *deleteNode(Node *node, string s)
{
    if (node == NULL)
        return node;

    if (s < node->Data.keys)
        node->left = deleteNode(node->left, s);
    else if (s > node->Data.keys)
        node->right = deleteNode(node->right, s);
    else
    {
        if (node->left == NULL && node->right == NULL)
        {
            delete node;
            return NULL;
        }
        else if (node->left == NULL)
        {
            Node *p = node->right;
            delete node;
            return p;
        }
        else if (node->right == NULL)
        {
            Node *p = node->left;
            delete node;
            return p;
        }

        Node *p = minValueNode(node->right);
        node->Data = p->Data;
        node->right = deleteNode(p, node->Data.keys);
    }

    return node;
}

void processingUsage(info *D, int &size)
{
    for (int i = 0; i < size; ++i)
    {
        if (D[i].keys == "Usage")
        {
            if (D[i].values[0] != 'n' || D[i].values[1] != '.')
            {
                D[i - 1].values += " " + D[i].keys + ": " + D[i].values;
                D[i].keys = D[size - 1].keys;
                D[i].values = D[size - 1].values;
                --size;
            }
        }
    }
}
void delRepeat(info *D, int &size)
{
    for (int i = 0; i < size; ++i)
    {
        for (int j = i + 1; j < size; ++j)
        {
            if (D[j].values == D[i].values)
            {
                D[j].keys = D[size - 1].keys;
                D[j].values = D[size - 1].values;
                --size;
            }
        }
    }
}
void load(Tree &t)
{
    fstream f;
    string s;
    int size(0);

    f.open("Oxford English Dictionary.txt", ios::in);
    while (!f.eof())
    {
        getline(f, s);
        if (s.size() > 3)
            ++size;
    }
    f.clear();
    f.seekg(0L, ios::beg);

    info *D = new info[size];
    for (int i = 0; i < size; ++i)
    {
        do
        {
            getline(f, s);
        } while (s.size() < 3);

        stringstream ss(s);
        string s1;

        getline(ss, D[i].keys, ' ');
        getline(ss, s1, ' ');
        if (s1.size() != 0)
        {
            D[i].keys += " " + s1;
            getline(ss, s1, ' ');
            if (s1.size() != 0)
            {
                D[i].keys += " " + s1;
                getline(ss, s1, ' ');
            }
        }
        getline(ss, D[i].values, '\n');
    }
    f.close();
    delRepeat(D, size);
    processingUsage(D, size);

    for (int i = 0; i < size; ++i)
    {
        int temp = rand() % size + 0;
        swap(D[i], D[temp]);
    }

    for (int i = 0; i < size; ++i)
        addNode(t, createNode(D[i]));

    delete[] D;
}

Node *find(Tree t, string s)
{
    if (t)
    {
        if (t->Data.keys == s)
            return t;
        if (t->Data.keys > s)
            return find(t->left, s);
        else
            return find(t->right, s);
    }
    else
        return NULL;
}
void search(Tree t)
{
    Node *result;
    string key;

    cout << "Enter a word to translate meaning: ";
    getline(cin, key);
    if (key[0] >= 'a' && key[0] <= 'z')
        key[0] -= 32;

    result = find(t, key);
    if (result == NULL)
        cout << "Can't find the worData." << endl;
    else
        cout << result->Data.keys + ": " + result->Data.values + "" << endl;
}

void addNewWord(Tree &t)
{
    info Data;

    cout << "Enter the word for adding: ";
    getline(cin, Data.keys);
    if (Data.keys[0] >= 'a' && Data.keys[0] <= 'z')
        Data.keys[0] -= 32;
    cout << "Enter the meaning of word: ";
    getline(cin, Data.values);
    addNode(t, createNode(Data));
    cout << "Successfully addeData..." << endl;
}

void editMeaning(Tree t)
{
    string key;
    Node *result;

    cout << "Enter a word to edit meaning: ";
    getline(cin, key);
    if (key[0] >= 'a' && key[0] <= 'z')
        key[0] -= 32;

    result = find(t, key);
    if (result == NULL)
        cout << "Can't find the worData." << endl;
    else
    {
        cout << result->Data.keys + ": " + result->Data.values << endl;
        cout << "Enter new meaning for editing: ";
        getline(cin, key);
        result->Data.values = key;
        cout << result->Data.keys + ": " + result->Data.values << endl;
    }
}

void delAWord(Tree &t)
{
    string key;
    Node *result;

    cout << "Enter a word to delete: ";
    getline(cin, key);
    if (key[0] >= 'a' && key[0] <= 'z')
        key[0] -= 32;

    result = deleteNode(t, key);
    if (result == NULL)
        cout << "Can't find the worData." << endl;
    else
        cout << "Successfully deleteData..." << endl;
}

void outputTree(Tree t, fstream &fs1, fstream &fs2)
{
    if (t == NULL)
        return;

    outputTree(t->left, fs1, fs2);
    fs1 << t->Data.keys << endl;
    fs2 << t->Data.keys + "  " + t->Data.values + "" << endl;
    outputTree(t->right, fs1, fs2);
}
// void save(Tree t)
// {
//     fstream f;
//     string fn;
//     cout << "Enter file name to save: ";
//     getline(cin, fn);
//     f.open(fn + ".txt", ios::out);
//     outputTree(t, f);
//     f.close();
// }

void menu(Tree t)
{
    load(t);
    bool f = true;

    while (f == true)
    {
        cout << "1. Search" << endl;
        cout << "2. Add" << endl;
        cout << "3. Edit meaning" << endl;
        cout << "4. Delete" << endl;
        cout << "0. Exit" << endl;
        cout << "Enter your choice: ";

        int temp;
        cin >> temp;
        cin.ignore();
        cout << endl;

        if (temp = 0)
        {
            f = false;
        }

        else if (temp = 1)
        {
            search(t);
        }

        else if (temp = 2)
        {
            addNewWord(t);
        }

        else if (temp = 3)
        {
            editMeaning(t);
        }

        else if (temp = 4)
        {
            delAWord(t);
        }
        system("pause");
    }

    ofstream ofs1("keys.txt", std::ofstream::out);
    ofs1.close();
    ofstream ofs2("values.txt", std::ofstream::out);
    ofs2.close();
    fstream fs1("keys.txt", ios::out, ios::app);
    fstream fs2("values.txt", ios::out, ios::app);
    outputTree(t, fs1, fs2);
    fs1.close();
    fs2.close();
}
int main()
{
    //inputFile();
    srand(time(NULL));
    Tree t = NULL;
    menu(t);

    return 0;
}