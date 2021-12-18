#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX 100
#define DEBUG 0

struct comp_s
{
    char name[MAX];
    float dist;
};

float minDist(char *);
float *allocate(int);
struct comp_s insert(char *, int);
float **allocateDist(int, int);
float makePositive(float);
float findMinimum(float **, int, int);

int main()
{
    int sum = minDist("minDist.txt");
    printf("the sum is %d\n", sum);

    return 0;
}

float minDist(char *name)
{
    FILE *f;
    f = fopen(name, "r");
    if (f == NULL)
        exit(0);

    int n, m;
    fscanf(f, "%d%d", &n, &m);

    struct comp_s *comp;
    comp = calloc(n, sizeof(struct comp_s));
    if (comp == NULL)
        exit(0);

    float *gnd;
    gnd = allocate(m); /*allocate m times elements and insert -1*/

    char str[MAX];
    float dist;

    while (fscanf(f, "%s%f", str, &dist) != EOF)
    {
        if (!strcmp(str, "gnd"))
        {
            for (int i = 0; i < m; i++)
            {
                if (gnd[i] == -1)
                {
                    gnd[i] = dist;
                    break;
                }
            }
        }

        else
        {
            for (int i = 0; i < n; i++)
            {
                if (!strcmp(comp[i].name, ""))
                {
                    comp[i] = insert(str, dist);
                    break;
                }
            }
        }
    }

    float **dist_diff;
    dist_diff = allocateDist(n, m); /*allocate m times rows and n times columns*/

    for (int j = 0; j < m; j++)
    {
        for (int i = 0; i < n; i++)
        {
            dist_diff[j][i] = makePositive(gnd[j] - comp[i].dist); /*turnes the negative into positive*/
        }
    }

#if DEBUG
    printf("dist_diff  is equal to:\n");
    for(int i = 0; i < m; i++)
    {
        for(int j = 0; j < n; j++)
        {
            printf("%.2f ", dist_diff[i][j]);
        }
        printf("\n");
}
#endif

    float sum = 0;
    for (int i = 0; i < n; i++)
    {
        sum = sum + findMinimum(dist_diff, m, i); /*finds the minimum*/
    }
    
    free(comp);
    free(gnd);
    for (int i = 0; i < m; i++)
    {
        free(dist_diff[i]);
    }
    free(dist_diff);
    
    return sum;
}

float *allocate(int m)
{
    float *gnd;
    gnd = malloc(m * sizeof(float));
    if(gnd == NULL)
        exit(0);

    for(int i = 0; i < m; i++)
        gnd[i] = -1;

    return gnd;
}

struct comp_s insert(char *str, int dist)
{
    struct comp_s comp;
    strcpy(comp.name, str);
    comp.dist = dist;

    return comp;
}

float **allocateDist(int n, int m)
{
    float **diff;
    diff = malloc(m * sizeof(float *));
    if(diff == NULL)
        exit(0);

    for(int i = 0; i < m; i++)
    {
        diff[i] = malloc(n * sizeof(float));
        if(diff[i] == NULL)
            exit(0);
    }

    return diff;
}

float makePositive(float number)
{
    if(number < 0)
        return number * (-1);
    else
        return number;
}

float findMinimum(float **arr, int m, int c)
{
    float min = arr[0][c];
    for(int i = 0; i < m; i++)
    {
        if(arr[i][c] < min)
            min = arr[i][c];
    }

    return min;
}
