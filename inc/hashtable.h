#define _XOPEN_SOURCE 500 /* Enable certain library functions (strdup) on linux.  See feature_test_macros(7) */

struct entry_s {
	char *key;
	char *value;
	struct entry_s *next;
};

typedef struct entry_s entry_t;

struct hashtable_s {
	int size;
	struct entry_s **table;	
};

typedef struct hashtable_s hashtable_t;


/* Create a new hashtable. */
extern hashtable_t *ht_create( int size );
/* Hash a string for a particular hash table. */
extern int ht_hash( hashtable_t *hashtable, char *key );
/* Create a key-value pair. */
extern entry_t *ht_newpair( char *key, char *value );
/* Insert a key-value pair into a hash table. */
extern void ht_set( hashtable_t *hashtable, char *key, char *value );
/* Retrieve a key-value pair from a hash table. */
extern char *ht_get( hashtable_t *hashtable, char *key );

