#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define m (1<<11)

/* "Copyright 2020" */

struct Hashtable {
    /* array de maximul de zerouri consecutive*/
    unsigned int *buckets;
    /* Nr. de bucket-uri. */
    unsigned int hmax;
    /* (Pointer la) Functie pentru a calcula valoarea
    hash asociata cheilor. */
    unsigned int (*hash_function)(void*);
};

unsigned int hash_function_int(void *a) {
    unsigned int uint_a = *((unsigned int *)a);

    uint_a = ((uint_a >> 16u) ^ uint_a) * 0x45d9f3b;
    uint_a = ((uint_a >> 16u) ^ uint_a) * 0x45d9f3b;
    uint_a = (uint_a >> 16u) ^ uint_a;
    return uint_a;
}

void init_ht(struct Hashtable *ht, unsigned int (*hash_function)(void*)) {
    ht->buckets = (unsigned int*) calloc(m, sizeof(unsigned int));

    ht->hmax = m;
    ht->hash_function = hash_function;
}

int position_ht(int a) {
    int nr = 0;
    /*calculare pozitie bucket*/
    for (int i = 21; i <= 31; i++) {
        if ((a & (1 << i)) != 0)
            nr = nr + (1 << (i-21));
	}
	return nr;
}

unsigned int number_zeros(unsigned int a) {
    unsigned int count = 0;
    /* calculare numar de zerouri consecutive*/
    if ((a & (1 << (20))) == 0) {
        count = 1;
        for (int i = 19; i >= 0; i--) {
            if ((a & (1 << (i))) == 0) {
                count++;
            } else {
                break;
            }
        }
    }
    return count;
}

void put(struct Hashtable *ht, unsigned int key) {
    int index = position_ht((int)(key));
    unsigned int count_zeros = number_zeros(key);

    /* introducere in bucket numarul care are
    maximul de zerouri consecutive*/
    if (ht->buckets[index] < count_zeros + 1) {
        ht->buckets[index] = count_zeros + 1;
    }
}

void free_ht(struct Hashtable *ht) {
    free(ht->buckets);
    free(ht);
}

int main(int argc, char *argv[]) {
	struct Hashtable *ht;
	int i, n, a, E;
	float Z, s = 0, am;

	/*deschidere fisier*/
    FILE *f = fopen(argv[1], "rt");

    /*creare hashtable*/
	ht = malloc(sizeof(struct Hashtable));
	init_ht(ht, hash_function_int);

	/*introducere date in hashtable*/
	while (!feof(f)) {
		fscanf(f, "%d\n", &a);
		n =  hash_function_int(&a);
		put(ht, n);
	}

	for (i = 0; i < m; i++) {
		s = s + (1.0)/(1 << (ht->buckets[i]));
	}

	/*agregare valorile din toate bucketurile*/
	Z = 1/s;

	am = (0.7213) / (1 + (1.079)/m);
	/*numarul de elemente distincte intalnite*/
	E = (int)(am * m * m * Z);

	printf("%d\n", E);
	fclose(f);
	free_ht(ht);

	return 0;
}
