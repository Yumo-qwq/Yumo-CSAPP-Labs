#include "cachelab.h"
#include <stdio.h>

struct cache_data {
    int E;
    int B;
    int S;
};

// C = E * B * S;
struct cache_set {
    int valid;
    int tag;
    int cnt;
};

int _stoi(const char num[]) {
    int i = 0;
    int res = 0;
    while(!(num[i] <= '9' && num[i] >= '0')) i++;
    
    while(num[i] <= '9' && num[i] >= '0' && num[i] != '\0') {
        res *= 10;
        res += (num[i] - '0');
        i++;
    }
    return res;
}

struct cache_set my_cache_set[1000];
struct cache_data my_cache;

int main(int argc, char *argv[]) {
    my_cache.S = (1 << _stoi(argv[2]));
    my_cache.E = _stoi(argv[4]);
    my_cache.B = (1 << _stoi(argv[6]));

    FILE *fp = fopen(argv[8], "r");

    char line[100];
    while (fgets(line, sizeof(line), fp) != NULL) {
        printf("%s", line);
    }
        
    // printf("%d %d %d\n", my_cache.S, my_cache.E, my_cache.B);
    
    // for(int i = 0; i < 9; i++) {
    //     printf("%s\n", argv[i]);
    // }
    
    printSummary(0, 0, 0);
    return 0;
}
