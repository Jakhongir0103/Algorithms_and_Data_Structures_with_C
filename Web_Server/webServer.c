#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX 100
#define CHECK 1

typedef struct info_s info_t;
typedef struct IP_s IP_t;
typedef struct name_s name_t;
typedef struct wrapper_s wrapper_t;

struct info_s
{
    char *IP;
    char *data;
    char *type;
    char *resources;
    int accept;

    struct info_s *next;
};

struct IP_s
{
    char *IP;
    char *name;

    IP_t *left;
    IP_t *right;
};

struct name_s
{
    char *name;

    info_t *info;
    name_t *left;
    name_t *right;
};

struct wrapper_s
{
    name_t *byName;
    IP_t *byIP;
};

wrapper_t *read(char *);
name_t *insert_all(name_t *, char *, char *, char *, char *, char *, int);
IP_t *insert_by_IP(IP_t *, char *, char *);
void printByName(name_t *, char *);
void print_node(info_t *);
void printByIP(IP_t *, name_t *, char *);
FILE *file_open_check(char *);
char *allocate(void);
void printError(void);

void main()
{
    wrapper_t *wrap;
    wrap = malloc(sizeof(wrap));
    if (wrap == NULL)
        printError();

    wrap = read("file.txt");
#if CHECK
    printByName(wrap->byName, "badguy");
    printByIP(wrap->byIP, wrap->byName, "178.1.192.33");
#endif
    return;
}

wrapper_t *read(char *file)
{
    wrapper_t *wrap;
    wrap = malloc(sizeof(wrap));
    if (wrap == NULL)
        printError();

    name_t *root1;
    IP_t *root2;
    root1 = NULL;
    root2 = NULL;

    FILE *f;
    f = file_open_check(file); //to open and check the file

    char *IP, *name, *data, *type, *resources;
    int accept;
    IP = allocate();
    name = allocate();
    data = allocate();
    type = allocate();
    resources = allocate();

    while (fscanf(f, "%s%s%s%s%s%d", IP, name, data, type, resources, &accept) != EOF)
    {
        root1 = insert_all(root1, IP, name, data, type, resources, accept);
        root2 = insert_by_IP(root2, IP, name);
    }

    wrap->byName = root1;
    wrap->byIP = root2;

    free(IP);
    free(name);
    free(data);
    free(type);
    free(resources);
    fclose(f);

    return wrap;
}

name_t *insert_all(name_t *root, char *IP, char *name, char *data, char *type, char *resources, int accept)
{
    if (root == NULL)
    {
        name_t *node;
        node = calloc(1, sizeof(name_t));
        if (node == NULL)
            printError();

        node->name = strdup(name);
        node->info = calloc(1, sizeof(info_t));
        if (node->info == NULL)
            printError();

        node->info->IP = strdup(IP);
        node->info->data = strdup(data);
        node->info->type = strdup(type);
        node->info->resources = strdup(resources);
        node->info->accept = accept;

        return node;
    }

    if (!strcmp(root->name, name))
    {
        info_t *nodeIP;
        nodeIP = calloc(1, sizeof(info_t));
        if (nodeIP == NULL)
            printError();

        nodeIP->IP = strdup(IP);
        nodeIP->data = strdup(data);
        nodeIP->type = strdup(type);
        nodeIP->resources = strdup(resources);
        nodeIP->accept = accept;

        nodeIP->next = root->info;
        root->info = nodeIP;
    }

    if (strcmp(root->name, name))
        root->left = insert_all(root->left, IP, name, data, type, resources, accept);
    if (strcmp(name, root->name))
        root->right = insert_all(root->right, IP, name, data, type, resources, accept);

    return root;
}

IP_t *insert_by_IP(IP_t *root, char *IP, char *name)
{
    if (root == NULL)
    {
        IP_t *node;
        node = calloc(1, sizeof(IP_t));
        if (node == NULL)
            printError();

        node->IP = strdup(IP);
        node->name = strdup(name);

        return node;
    }

    if (strcmp(root->IP, IP))
        root->left = insert_by_IP(root->left, IP, name);
    if (strcmp(IP, root->IP))
        root->right = insert_by_IP(root->right, IP, name);

    return root;
}

void printByName(name_t *root, char *name)
{
    if (root == NULL)
        return;

    if (strcmp(root->name, name))
        printByName(root->left, name);
    else if (strcmp(name, root->name))
        printByName(root->right, name);
    else
    {
        printf("%s: \n", root->name);
        print_node(root->info);
    }

    return;
}

void print_node(info_t *root)
{
    info_t *head;
    head = root;

    while (head != NULL)
    {
        printf("%s %s %s %s %d\n", head->IP, head->data, head->type, head->resources, head->accept);
        head = head->next;
    }

    return;
}

void printByIP(IP_t *root, name_t *name, char *IP)
{
    if (root == NULL)
        return;

    if (strcmp(root->IP, IP))
        printByIP(root->left, name, IP);
    else if (strcmp(IP, root->IP))
        printByIP(root->right, name, IP);
    else
        printByName(name, root->name);

    return;
}

FILE *file_open_check(char *file)
{
    FILE *f;
    f = fopen(file, "r");

    if (f == NULL)
    {
        printf("error with the file\n");
        exit(0);
    }

    return f;
}

char *allocate(void)
{
    char *string;
    string = malloc(MAX * sizeof(char));
    if (string == NULL)
        printError();

    return string;
}

void printError(void)
{
    printf("error with the allocation\n");
    exit(0);
}
