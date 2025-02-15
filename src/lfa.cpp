#include<stdio.h>
#include<iostream>
#include<cstring>

using namespace std;

typedef struct node{
    int final;
    int id;
    struct node *neigh[26];

    struct node *prev[26];
} node_t;

int counter;
node good, bad;

void make_branch2(int len, char *word, node *root, int final) {
    node *crt = root;
    for(int i = 0; i < len; i++) {
        if(i == len - 1) {
            if (final == 1) crt->neigh[word[i] - 97] = &good;
            if (final == -1) crt->neigh[word[i] - 97] = &bad;
            return;
        }

        if(crt->neigh[word[i] - 97] == NULL) {
            crt->neigh[word[i] - 97] = (node *)calloc(1, sizeof(node_t));
            crt->neigh[word[i] - 97]->prev[word[i] - 97] = crt;
        }
        crt = crt->neigh[word[i] - 97];
    }
}

// simplify the graph
int compute2(node *root) {
    int n = 0;
    int sum = 0;
    int to_del = 0;

    if(root->final != 0) return root->final;

    for(int i = 0; i < 26; i++) {
        if(root->neigh[i] != NULL) {
            n++;
            sum += compute2(root->neigh[i]);
        }
    }

    if (n == sum) {
        for(int i = 0; i < 26; i++) {
            if(root->prev[i] != NULL) {
                root->prev[i]->neigh[i] = &good;
            }
        }
        free(root);
        return 1;
    } else if (n == (-1) * sum) {
        for(int i = 0; i < 26; i++) {
            if(root->prev[i] != NULL) {
                root->prev[i]->neigh[i] = &bad;
            }
        }
        free(root);
        return -1;
    }
    return 0;
}

// count nodes and tag each one
void count_and_tag2(node *root) {
    root->id = counter++;

    for(int i = 0; i < 26; i++)
        if(root->neigh[i] != NULL && root->neigh[i]->id == 0)
            count_and_tag2(root->neigh[i]);
}

void make_matrix(int **mat, node *root) {
    for(int i = 0; i < 26; i++) {
        if(root->neigh[i] == NULL)
            mat[root->id][i] = root->id;
        else mat[root->id][i] = root->neigh[i]->id;
    }

    for(int i = 0; i < 26; i++)
        if(root->neigh[i] != NULL)
            make_matrix(mat, root->neigh[i]);
}

void destroy_tree(node *root) {
    for(int i = 0; i < 26; i++) {
        if(root->neigh[i] != NULL && root->neigh[i]->final == 0) {
            destroy_tree(root->neigh[i]);
            free(root->neigh[i]);
        }
    }
}

int main() {
    int num_a, num_f, len;
    char word[101];
    FILE *input = fopen("input.txt", "r");
    FILE *output = fopen("output.txt", "w");
    node s;
    s.final = 0;
    memset(s.neigh, NULL, 26 * sizeof(node *));
    good.final = 1;
    bad.final = -1;

    fscanf(input, "%d %d %d", &num_a, &num_f, &len);
    // accepted words
    for(int i = 0; i < num_a; i++) {
        fscanf(input, "%s", word);
        make_branch2(len, word, &s, 1);
    }

    // refused words
    for(int i = 0; i < num_f; i++) {
        fscanf(input, "%s", word);
        make_branch2(len, word, &s, -1);
    }

    compute2(&s);
    count_and_tag2(&s);

    int **adj = (int **)malloc(counter * sizeof(int*));
    for(int i = 0; i < counter; i++) {
        adj[i] = (int *)calloc(26, sizeof(int));
    }
    make_matrix(adj, &s);


    fprintf(output, "dfa\n");
    fprintf(output, "%d %d %d\n", counter, 1, 0);
    fprintf(output, "%d\n", good.id);
    for(int i = 0; i < counter; i++) {
        for(int j = 0; j < 26; j++) {
            fprintf(output, "%d ", adj[i][j]);
        }
        fprintf(output, "\n");
    }

    for(int i = 0; i < counter; i++) {
        free(adj[i]);
    }
    free(adj);
    destroy_tree(&s);

    return 0;
}
