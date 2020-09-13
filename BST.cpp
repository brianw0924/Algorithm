#include <iostream>
#include <string.h>
#include <vector>
#include <queue>
#include <set>
#include <map>
#include <stack>
#include <algorithm>
#include <math.h>
#include <ctype.h>

using namespace std;
struct node
{
    int data, l; // l : number of leftsubtree nodes, including itself
    node *left;
    node *right;

    node(int d)
    {
        data = d;
        l = 1;
        left = NULL;
        right = NULL;
    }

    bool node_Search(node *root, int target)
    {
        if (root)
        {
            if (target == root->data)
            {
                return true;
            }
            else if (target > root->data)
            {
                return node_Search(root->right, target);
            }
            else
            {
                return node_Search(root->left, target);
            }
        }
        return false;
    }

    node *node_findMax(node *root)
    {
        if (root == NULL)
            return root;

        node *temp = root;
        while (temp->right)
        {
            temp = temp->right;
        }
        return temp;
    }

    node *node_findMin(node *root)
    {
        if (root == NULL)
            return root;

        node *temp = root;
        while (temp->left)
        {
            temp = temp->left;
        }
        return temp;
    }

    node *node_Insert(node *root, int value)
    {
        if (root == NULL)
            return new node(value);

        if (value > root->data)
            root->right = node_Insert(root->right, value);
        else
        {
            root->l++;
            root->left = node_Insert(root->left, value);
        }

        return root;
    }

    node *node_Delete(node *root, int target)
    {
        if (root)
        {
            if (target > root->data)
            {
                root->right = node_Delete(root->right, target);
            }
            else if (target < root->data)
                root->left = node_Delete(root->left, target);
            else
            { // found target
                if (root->left && root->right)
                {
                    node *temp = node_findMax(root->left);
                    root->data = temp->data;
                    root->left = node_Delete(root->left, temp->data);
                }
                else
                {
                    node *temp = root;
                    if (root->right)
                        root = root->right;
                    else
                        root = root->left;
                    free(temp);
                }
            }
        }
        return root;
    }

    void node_Inorder(node *root)
    {
        if (root)
        {
            node_Inorder(root->left);
            printf("%d ", root->data);
            node_Inorder(root->right);
        }
    }

    void node_Preorder(node *root)
    {
        if (root)
        {
            printf("%d ", root->data);
            node_Preorder(root->left);
            node_Preorder(root->right);
        }
    }

    void node_Postorder(node *root)
    {
        if (root)
        {
            node_Postorder(root->left);
            node_Postorder(root->right);
            printf("%d ", root->data);
        }
    }

    node *node_find_kth_small(node *root, int k)
    {
        if (root == NULL)
            return root;

        if (k == root->l)
            return root;
        else if (k > root->l)
            return node_find_kth_small(root->right, k - root->l);
        else // k < root->l
            return node_find_kth_small(root->left, k);
    }
};

class BST
{
public:
    node *root;

    BST()
    {
        root = NULL;
    };

    bool search(int target)
    {
        return root->node_Search(root, target);
    }

    node *findMax()
    {
        return root->node_findMax(root);
    }

    node *findMin()
    {
        return root->node_findMin(root);
    }

    void Insert(int value)
    {
        root = root->node_Insert(root, value);
    }

    void Delete(int target)
    {
        root = root->node_Delete(root, target);
    }

    void Inorder(){
        root->node_Inorder(root);
        printf("\n");
    }

    void Preorder(){
        root->node_Preorder(root);
        printf("\n");
    }

    void Postorder(){
        root->node_Postorder(root);
        printf("\n");
    }

    node *find_kth_small(int k)
    {
        return root->node_find_kth_small(root, k);
    }
};

int main()
{   
    return 0;
}