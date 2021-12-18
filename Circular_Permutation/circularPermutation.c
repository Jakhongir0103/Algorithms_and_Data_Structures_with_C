#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define DEBUG 0

typedef struct array_s
{
    char name;
    int *edge;
} array_t;

typedef struct graph_s
{
    array_t *arr;
    int n;
} graph_t;

typedef struct fullSol_s
{
    int **fullSol;
    struct fullSol_s *next;
} fullSol_t;

void circularPermutation(char *);
fullSol_t *permutation(array_t *, int, char *, char *, int *, int, fullSol_t *);
int getNum(char c, char *str, int n);
fullSol_t *insert(fullSol_t *, array_t *, int);
void clear(array_t *, int);
fullSol_t *initialize(int);

int main()
{
    char *str = "ABCD";
    circularPermutation(str);
}

void circularPermutation(char *str)
{
    graph_t *graph;
    graph = malloc(strlen(str) * sizeof(graph_t));
    if (graph == NULL)
    {
        printf("error with the allocation\n");
        exit(0);
    }
    graph->n = strlen(str);

    graph->arr = malloc(graph->n * sizeof(array_t));
    if (graph->arr == NULL)
    {
        printf("error with the allocation\n");
        exit(0);
    }

    for (int i = 0; i < graph->n; i++)
    {
        graph->arr[i].edge = malloc(graph->n * sizeof(int));
        if (graph->arr[i].edge == NULL)
        {
            printf("error with the allocation\n");
            exit(0);
        }
    }

    /*building a matrix of size strlen(str) and intializing all edges to 0*/
    for (int i = 0; i < graph->n; i++)
    {
        graph->arr[i].name = str[i];
        for (int j = 0; j < graph->n; j++)
            graph->arr[i].edge[j] = 0;
    }

    char *sol;
    sol = malloc(graph->n * sizeof(char));
    if (sol == NULL)
    {
        printf("error with the allocation\n");
        exit(0);
    }

    int *mark;
    mark = malloc(graph->n * sizeof(int));
    if (mark == NULL)
    {
        printf("error with the allocation\n");
        exit(0);
    }

    for (int i = 0; i < graph->n; i++)
    {
        mark[i] = 0;
    }

    fullSol_t *fullSol;
    fullSol = NULL;
    fullSol_t *head;

    fullSol = permutation(graph->arr, graph->n, sol, str, mark, 0, fullSol);

    /*to print out the solution*/

#if DEBUG
    printf("The Circular Permutation result:\n");
#endif

    for (head = fullSol; head != NULL; head = head->next)
    {
        for (int i = 0, next = 0, itr = 0; itr < graph->n; i = next, itr++)
        {
            for (int j = 0; j < graph->n; j++)
            {
                if (head->fullSol[i][j] == 1)
                {
                    printf("%c ", graph->arr[next].name);
                    next = j;
                }
            }
        }
        printf("\n");
#if DEBUG
        for (int i = 0; i < graph->n; i++)
        {
            for (int j = 0; j < graph->n; j++)
            {
                printf("%d ", head->fullSol[i][j]);
            }
            printf("\n");
        }
        printf("\n");
#endif
    }

    /*Free up the allocated spaces*/
    for (int i = 0; i < graph->n; i++)
    {
        free(graph->arr[i].edge);
    }
    free(graph->arr);
    free(graph);
    free(sol);
    free(mark);

    for (head = fullSol; head != NULL;)
    {
        fullSol_t *tmp;
        tmp = head->next;
        free(head);
        head = tmp;
    }
    fullSol = head;
    free(head);
    free(fullSol);

    return;
}

fullSol_t *permutation(array_t *v, int n, char *sol, char *str, int *mark, int pos, fullSol_t *fullSol)
{
    if (pos >= n)
    {
#if DEBUG
        for (int i = 0; i < n; i++)
        {
            printf("%c ", sol[i]);
        }
        printf("\n");
#endif
        for (int i = 0; i < n; i++)
        {
            for (int j = 0; j < n; j++)
            {
                if (v[j].name == sol[i])
                {
                    if (i != n - 1)
                        v[j].edge[getNum(sol[i + 1], str, n)] = 1;
                    else
                        v[j].edge[getNum(sol[0], str, n)] = 1;
                }
            }
        }
#if DEBUG
        for (int i = 0; i < n; i++)
        {
            for (int j = 0; j < n; j++)
            {
                printf("%d ", v[i].edge[j]);
            }
            printf("\n");
        }
        printf("\n");
#endif
        fullSol = insert(fullSol, v, n);
        clear(v, n);

        return fullSol;
    }

    for (int i = 0; i < n; i++)
    {
        if (mark[i] == 0)
        {
            mark[i] = 1;
            sol[pos] = str[i];
            fullSol = permutation(v, n, sol, str, mark, pos + 1, fullSol);
            mark[i] = 0;
        }
    }

    return fullSol;
}

int getNum(char c, char *str, int n)
{
    for (int i = 0; i < n; i++)
    {
        if (str[i] == c)
        {
            return i;
        }
    }
}

fullSol_t *insert(fullSol_t *fullSol, array_t *v, int n)
{
    int same = 1, ok = 1;
    fullSol_t *head;
    head = fullSol;

    while (fullSol != NULL)
    {
        same = 1;
        for (int i = 0; i < n; i++)
        {
            for (int j = 0; j < n; j++)
            {
                if (v[i].edge[j] != fullSol->fullSol[i][j])
                {
                    same = 0;
                }
            }
        }

        if (same)
            ok = 0;

        fullSol = fullSol->next;
    }

    if (ok)
    {
        fullSol_t *root;
        root = initialize(n);
        for (int i = 0; i < n; i++)
        {
            for (int j = 0; j < n; j++)
            {
                root->fullSol[i][j] = v[i].edge[j];
            }
        }

        root->next = head;
        head = root;
    }

    return head;
}

void clear(array_t *v, int n)
{
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
            v[i].edge[j] = 0;
    }
}

fullSol_t *initialize(int n)
{
    fullSol_t *head;

    head = malloc(sizeof(fullSol_t));
    if (head == NULL)
    {
        printf("error with the allocation\n");
        exit(0);
    }

    head->fullSol = calloc(n, sizeof(int *));
    if (head->fullSol == NULL)
    {
        printf("error with the allocation\n");
        exit(0);
    }
    for (int i = 0; i < n; i++)
    {
        head->fullSol[i] = calloc(n, sizeof(int));
        if (head->fullSol[i] == NULL)
        {
            printf("error with the allocation\n");
            exit(0);
        }
    }

    return head;
}
