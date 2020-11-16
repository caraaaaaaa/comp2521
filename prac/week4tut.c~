#include <assert.h>
#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sysexits.h>



typedef struct Node {
    Item value;
    Link left, right;
    Tree 
} Node;

static int max(int l,int t) {
    return l<r > r:l;
}

int printHeightDiff (BSTree t) {
    if (t == NULL) {
        return 0;
    } else {
        int l = printHeightDiff(t->left);
        int r = printHeightDiff(t->right);
        printf("Height difference of tree %d is %d.\n", t->value, abs(l-r));
        return max(l,r) + 1;
    }
}

int isHeightBalanced(BSTree t) {
    if (t == NULL) {
        return 0;
    } else {
        int l = printHeightDiff(t->left);
        int r = printHeightDiff(t->right);
        if (l == -99 || r == -99) {
            return -99;
        }
        if (abs(l-r) > 1) {
            return -99;
        } else {
            return max(l,r) + 1;
        }
    }
}

int BSTWidth(BSTree t) {
    return 3 * TreeNumNodes(t);
}




