#include "cachelab.h"
#include <stdio.h>
#define INT_MAX 2147483647

struct cache_data {
    int E;
    int B;
    int S;
};

// C = E * B * S;
struct cache_set {
    unsigned long long tag[100];
    int valid[100];
    int cnt[100];
};

struct cache_res_cnt {
    int misses;
    int hits;
    int evictions;
};

int _stoi(const char *num) {
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

int mapping(char c) {
    if(c <= '9' && c >= '0') return c - '0';
    else if(c >= 'a' && c <= 'f') return c - 'a' + 10;
    else return -1;
}

struct cache_set my_cache_set[1000];
struct cache_data my_cache;
struct cache_res_cnt my_cache_res_cnt;

int cur_time = 0;

int cache_access(unsigned long long address) {
    int set_index = address / my_cache.B % my_cache.S;
    unsigned long long tag = address / my_cache.B / my_cache.S;
    
    cur_time++;

    for(int i = 0; i < my_cache.E; i++) {
        if(my_cache_set[set_index].valid[i] && tag == my_cache_set[set_index].tag[i]) {
            my_cache_set[set_index].cnt[i] = cur_time;
            return 1;
        }
    }
    
    int min_cnt_index = 0;
    int cur_cnt = INT_MAX;

    for(int i = 0; i < my_cache.E; i++) {
        if(my_cache_set[set_index].valid[i] == 0) {
            my_cache_set[set_index].valid[i] = 1;
            my_cache_set[set_index].tag[i] = tag;
            my_cache_set[set_index].cnt[i] = cur_time;
            return 2;
        }

        if(my_cache_set[set_index].cnt[i] < cur_cnt) {
            cur_cnt = my_cache_set[set_index].cnt[i];
            min_cnt_index = i;
        }
    }

    my_cache_set[set_index].tag[min_cnt_index] = tag;
    my_cache_set[set_index].cnt[min_cnt_index] = cur_time;
    my_cache_set[set_index].valid[min_cnt_index] = 1;

    return 3;
}

int main(int argc, char *argv[]) {
    my_cache.S = (1 << _stoi(argv[2]));
    my_cache.E = _stoi(argv[4]);
    my_cache.B = (1 << _stoi(argv[6]));

    FILE *fp = fopen(argv[8], "r");

    char line[100];
    while(fgets(line, sizeof(line), fp) != NULL) {
        int op = 0;
        unsigned long long address = 0;

        int i = 0;
        while(line[i] == ' ') i++;

        if(line[i] == 'L') op = 1;
        else if(line[i] == 'S') op = 2;
        else if(line[i] == 'M') op = 3;
        else if(line[i] == 'I') op = 0;
        i++;

        while(line[i] == ' ') i++;
        
        for(; line[i] != ','; i++) {
            address *= 16;
            address += mapping(line[i]);            
        }

        if(op == 0) continue;
        else if(op == 1 || op == 2) {
            int tmp = cache_access(address);

            if(tmp == 1) {
                my_cache_res_cnt.hits++;
            } else if(tmp == 2) {
                my_cache_res_cnt.misses++;
            } else {
                my_cache_res_cnt.misses++;
                my_cache_res_cnt.evictions++;
            }
        } else {
            int tmp1 = cache_access(address);
            if(tmp1 == 1) {
                my_cache_res_cnt.hits++;
            } else if(tmp1 == 2) {
                my_cache_res_cnt.misses++;
            } else {
                my_cache_res_cnt.misses++;
                my_cache_res_cnt.evictions++;
            }

            int tmp2 = cache_access(address);
            if(tmp2 == 1) {
                my_cache_res_cnt.hits++;
            } else if(tmp2 == 2) {
                my_cache_res_cnt.misses++;
            } else {
                my_cache_res_cnt.misses++;
                my_cache_res_cnt.evictions++;
            }
        }
    }

    printSummary(my_cache_res_cnt.hits, my_cache_res_cnt.misses, my_cache_res_cnt.evictions);
    return 0;
}
