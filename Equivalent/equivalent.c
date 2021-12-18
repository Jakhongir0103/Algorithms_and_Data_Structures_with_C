#include <stdio.h>
#include <stdlib.h>
#define DEBUG 0

typedef struct node_s node_t;

struct node_s
{
    char key; 
/*I set the key to a single character as given in the example*/      
    int n_child;
    node_t **child;
};

node_t *addChild (node_t *, char, int, int *);
int equivalent(node_t *, node_t *);
void Print(node_t *);

int main()
{
    node_t *tree1;
    node_t *tree2;

    tree1 = NULL;
    tree2 = NULL;

    FILE *f1, *f2, *f3;
    f1 = fopen("equivalent1.txt", "r");
    f2 = fopen("equivalent2.txt", "r");
    f3 = fopen("equivalent3.txt", "r");

    if(f1 == NULL || f2 == NULL || f3 == NULL)
        exit(0);

    int number;
    char key;
    int inserted;

    while(fscanf(f1, "%c%d%*c", &key, &number) != EOF)
    {
        inserted = 0;
        tree1 = addChild(tree1, key, number, &inserted);
    }

    while(fscanf(f2, "%c%d%*c", &key, &number) != EOF)
    {
        inserted = 0;
        tree2 = addChild(tree2, key, number, &inserted);
    }

    fclose(f1);
    fclose(f2);
    fclose(f3);

    int equal;
    equal = equivalent(tree1, tree2);

    if(equal)
        printf("equal\n");
    else
        printf("not equal\n");

    return 0;
}

node_t *addChild (node_t *root, char key, int number, int *inserted)
{
    if(root == NULL)
    {
        *inserted = 1;
        root = calloc(1, sizeof(node_t));
        root->key = key;
        root->n_child = number;
        root->child = calloc(number, sizeof(node_t));
        if(root->child == NULL)
            exit(0);

        return root;
    }

    for(int i = 0; i < root->n_child; i++)
    {
        if(*inserted == 0)
            root->child[i] = addChild(root->child[i], key, number, inserted);
    }

    return root;
}

int equivalent(node_t *root1, node_t *root2)
{
    int equal;
    if(root1->n_child != root2->n_child)
        return 0;

    if(root1->key != root2->key)
        return 0;

    if(root1->n_child == 0 && root2->n_child == 0)
        return 1;

    for(int i = 0; i < root1->n_child; i++)
    {
        equal = 0;
        for(int j = 0; j < root2->n_child; j++)
            if(equal == 0)
            {
#if DEBUG
                printf("%c %c\n", root1->child[i]->key, root2->child[j]->key);
#endif
                equal = equivalent(root1->child[i], root2->child[j]);
            }
    }

    return equal;
}
