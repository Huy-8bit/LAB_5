#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
using namespace std;
struct info
{
    string keys;
    string values;
};
struct Node
{
    info data;
    Node *left;
    Node *right;
};

typedef Node *Tree;

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
void input(fstream &fsInFile)
{
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
                getline(ss, values, '\n');
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

Node *CreateNode(string keys, string value)
{
    Node *p = new Node;
    p->data.keys = keys;
    p->data.values = value;
    p->left = NULL;
    p->right = NULL;
    return p;
}

void CreateTree(Tree &root)
{
    root = NULL;
}

void DestroyTree(Tree &root)
{
    if (root)
    {
        DestroyTree(root->left);
        DestroyTree(root->right);
        delete root;
    }
}

void AddNode(Tree &root, Node *node)
{
    if (root)
    {
        // if (root->data.keys == node->data.keys)
        // {
        //     return;
        // }
        if (node->data.keys <= root->data.keys)
        {
            AddNode(root->left, node);
        }
        else
        {
            AddNode(root->right, node);
        }
    }
    else
    {
        root = node;
    }
}

Node *FindNode(Tree root, string keys)
{
    if (root)
    {
        if (root->data.keys == keys)
        {
            return root;
        }
        if (keys < root->data.keys)
        {
            return FindNode(root->left, keys);
        }
        return FindNode(root->right, keys);
    }
    return NULL;
}

void saveDataTree(Tree root, fstream &fs1, fstream &fs2)
{
    if (root == NULL)
    {
        return;
    }
    saveDataTree(root->left, fs1, fs2);

    fs1 << root->data.keys << endl;
    fs2 << root->data.values << endl;

    saveDataTree(root->right, fs1, fs2);
}
// void NLR(Tree root)
// {
//     if (root)
//     {
//         NLR(root->left);
//         NLR(root->right);
//     }
// }
// void LNR(Tree root)
// {
//     if (root)
//     {
//         LNR(root->left);
// LNR(root->right);
//     }
// }
// void LRN(Tree root)
// {
//     if (root)
//     {
//         LRN(root->left);
//         LRN(root->right);
//     }
// }

void FindAndReplace(Tree &p, Tree &tree)
{
    if (tree->left)
        FindAndReplace(p, tree->left);
    else
    {
        p->data.keys = tree->data.keys;
        p = tree;
        tree = tree->right;
    }
}

void DeleteNode(Tree &root, string x)
{
    if (root)
    {
        if (x > root->data.keys)
        {
            DeleteNode(root->right, x);
        }
        else if (x < root->data.keys)
        {
            DeleteNode(root->left, x);
        }
        else
        {
            Node *p = root;
            if (!root->left)
            {
                root = root->right;
            }
            else if (!root->right)
            {
                root = root->left;
            }
            else
            {
                FindAndReplace(p, root->right);
            }
            delete p;
        }
    }
    else
    {
        cout << endl
             << "Not found!\n";
    }
}

void processTree(Tree t, int seletc)
{
    string temps;
    if (seletc == 1)
    {
        cout << endl
             << "Input data to search: ";
        cin >> temps;
        Node *find = FindNode(t, temps);
        if (find == NULL)
        {
            cout << endl
                 << "Not find" << endl;
        }
        else
        {
            cout << endl
                 << find->data.keys << " : " << find->data.values << endl;
        }
    }
    else if (seletc == 2)
    {
        string keys;
        string value;
        cout << "Enter keys:";
        cin.ignore();
        cin >> keys;
        cout << endl
             << " Enter value: ";
        cin.ignore();
        getline(cin, value);
        AddNode(t, CreateNode(keys, value));
    }
    else if (seletc == 3)
    {
        cout << endl
             << "Enter keys delete ";
        cin >> temps;
        DeleteNode(t, temps);
    }
    else if (seletc == 4)
    {
        cout << endl
             << "Input data to Edit: ";
        cin >> temps;
        Node *find = FindNode(t, temps);
        if (find == NULL)
        {
            cout << endl
                 << "Not find" << endl;
        }
        else
        {
            cout << endl
                 << " prev data : " << find->data.keys << find->data.values << endl;
            string values;
            cout << "Enter new data ";
            cin.ignore();
            getline(cin, values);
            find->data.values = values;
        }
    }
}

void selectionTree(Tree t)
{
    fstream fsTest("keys.txt", ios::in);
    int size = sizeFileInput(fsTest);
    info *arr = new info[size];
    fsTest.close();
    fstream fsKeys("keys.txt", ios::in);
    fstream fsValue("values.txt", ios::in);
    int count = 0;

    while (!fsKeys.eof())
    {
        string keys;
        string values;
        getline(fsKeys, keys);
        getline(fsValue, values);
        arr[count].keys = keys;
        arr[count].values = values;
        count++;
    }
    for (int i = 0; i < count; i++)
    {
        AddNode(t, CreateNode(arr[i].keys, arr[i].values));
    }
    delete[] arr;
    cout << endl
         << count << endl;
    fsKeys.close();
    fsValue.close();

    int seletc = 1;

    while (seletc < 5)
    {
        cout << " AVL TREE " << endl;
        cout << "1. SEARCH" << endl;
        cout << "2. ADD DATA" << endl;
        cout << "3. DELETE DATA" << endl;
        cout << "4. EDIT DATA" << endl;
        cout << "5. EXIT " << endl;
        cout << " Choose your choice : ";
        int temp;
        cin >> temp;
        seletc = temp;
        processTree(t, seletc);
        system("pause");
    }
    cout << "Save File" << endl;
    ofstream ofs1("keys.txt", std::ofstream::out);
    ofs1.close();
    ofstream ofs2("values.txt", std::ofstream::out);
    ofs2.close();
    fstream fs1("keys.txt", ios::out);
    fstream fs2("values.txt", ios::out);
    saveDataTree(t, fs1, fs2);
    fs1.close();
    fs2.close();
}

int main()
{
    system("cls");
    Tree t;
    CreateTree(t);
    cout << "Run" << endl;
    fstream fsInFile("oxford English Dictionary.txt", ios::in);
    input(fsInFile);
    fsInFile.close();
    selectionTree(t);
    return 0;
}