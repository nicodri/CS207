#include "bloom.h"

void set_bit(bloom_filter_t *B, index_t i){
    assert(i < B->size);

    index_t wsize = (sizeof(index_t) * 8);
    index_t position = i % wsize; 
    index_t index = i / wsize;

    B->table[index] |= (0x1UL << position); 
}

index_t get_bit(bloom_filter_t *B, index_t i){
    assert(i < B->size);

    index_t wsize = (sizeof(index_t) * 8);
    index_t masked =(B->table[i / wsize] >> (i % wsize)); // moving the bit to 1st position

    return masked & 0x1;
}

index_t hash1(bloom_filter_t *B, key_t k){
    // from https://gist.github.com/badboy/6267743

    index_t key = k;
    key = (~key) + (key << 21); // key = (key << 21) - key - 1;
    key = key ^ (key >> 24);
    key = (key + (key << 3)) + (key << 8); // key * 265
    key = key ^ (key >> 14);
    key = (key + (key << 2)) + (key << 4); // key * 21
    key = key ^ (key >> 28);
    key = key + (key << 31);
    return key;
}

index_t hash2(bloom_filter_t *B, key_t k){
    // suggested by Knuth in "The Art of Computer Programming"

    index_t h = (k * 11400714819323198549ul);

    return h;
}

void bloom_init(bloom_filter_t *B, index_t size_in_bits){
    if(B->table)free(B->table);

    B->size = size_in_bits;
    B->count = 0;
    B->table = (index_t*)malloc(size_in_bits / 8); // convert size from bits to bytes

    if(!B->table) {
        printf("memory error\n");
        return;
    }
    // set values to zero
    memset(B->table, 0, size_in_bits / 8);
}

void bloom_destroy(bloom_filter_t *B){
    if(B) {
        if(B->table)free(B->table);
        B->table = NULL;
        free(B);
    }
}

int bloom_check(bloom_filter_t *B, key_t k){
    for (index_t i = 0; i < N_HASHES; i++){
        index_t hash = (hash1(B, k) + i * hash2(B, k)) % B->size;

        if (!get_bit(B, hash)) return 0;
    }
    // possible case of a false positive
    return 1;
}
void bloom_add(bloom_filter_t *B, key_t k){
    for (index_t i = 0; i < N_HASHES; i++){
        index_t hash = (hash1(B, k) + i * hash2(B, k)) % B->size;

        // Check if bit is already set
        if(!get_bit(B, hash)) {
            set_bit(B, hash);
            B->count++;
        }
    }
}

index_t bloom_bits_count(bloom_filter_t *B){
    index_t count = 0;
    for (index_t i = 0UL; i < B->size; i++){
        if (get_bit(B, i)) count++;
    }
    return count;
}

int main(){

    // Testing the hash

    int test_keys[] = {0, 1, 2, 3, 13, 97};
    for (int i=0; i<6; i++){
        printf( "Key: %d , Hash1: %llu\n", test_keys[i], hash1(NULL, test_keys[i]));
        printf( "Key: %d , Hash2: %llu\n", test_keys[i], hash2(NULL, test_keys[i]));
    }

    // Testing the Bloom filter

    // Test 0
    bloom_filter_t *bloom = (bloom_filter_t*)malloc(sizeof(bloom_filter_t));
    bloom->table = NULL;
    bloom_init(bloom, 1000);

    for (key_t k=1UL; k < 71UL; k++){
        bloom_add(bloom, k);
    }    
    key_t k1 = 1UL;
    key_t k2 = 71UL;
    printf("Test of check: present if %d absent if %d \n", bloom_check(bloom, k1),
           bloom_check(bloom, k2));

    // 1st test
    // clear object
    bloom_init(bloom, 1000);

    for (key_t k=1UL; k < 71UL; k++){
        bloom_add(bloom, k);
    }

    printf("number of bits set %llu / %llu\n", bloom->count, bloom->size);

    // With thre requested function
    printf("number of bits set %llu / %llu\n", bloom_bits_count(bloom), bloom->size);

    // 2nd test
    // clear object
    bloom_init(bloom, 1000);

    index_t rand_max = 1000000;

    // Fill the array
    printf("RAND_MAX is %d\n", RAND_MAX);
    int test_occurences = 0;
    for(index_t i = 1UL; i <= 100UL; i++){
        key_t r = (key_t)rand() % rand_max;
        bloom_add(bloom, r);
        if (bloom_check(bloom, r)) test_occurences++;
    }
    printf("Test occurences : %d / 100 \n", test_occurences);

    // Counting presence of new random numbers
    int occurences = 0;
    for(index_t i = 1UL; i <= 100UL; i++){
        key_t r = (key_t)rand() % rand_max;
        if (bloom_check(bloom, r)) occurences++;
    }

    printf("occupancy: %d bits  false positives: %d\n", (int)bloom->count, occurences);


}