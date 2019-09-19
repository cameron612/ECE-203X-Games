/*
 Student Name: Cameron Allotey
 Date: 4/4/2018

=======================
ECE 2035 Project 2-1:
=======================
This file provides definition for the structs and functions declared in the
header file. It also contains helper functions that are not accessible from
outside of the file.

FOR FULL CREDIT, BE SURE TO TRY MULTIPLE TEST CASES and DOCUMENT YOUR CODE.

===================================
Naming conventions in this file:
===================================
1. All struct names use camel case where the first letter is capitalized.
  e.g. "HashTable", or "HashTableEntry"

2. Variable names with a preceding underscore "_" will not be called directly.
  e.g. "_HashTable", "_HashTableEntry"

  Recall that in C, we have to type "struct" together with the name of the struct
  in order to initialize a new variable. To avoid this, in hash_table.h
  we use typedef to provide new "nicknames" for "struct _HashTable" and
  "struct _HashTableEntry". As a result, we can create new struct variables
  by just using:
    - "HashTable myNewTable;"
     or
    - "HashTableEntry myNewHashTableEntry;"

  The preceding underscore "_" simply provides a distinction between the names
  of the actual struct defition and the "nicknames" that we use to initialize
  new structs.
  [See Hidden Definitions section for more information.]

3. Functions, their local variables and arguments are named with camel case, where
  the first letter is lower-case.
  e.g. "createHashTable" is a function. One of its arguments is "numBuckets".
       It also has a local variable called "newTable".

4. The name of a struct member is divided by using underscores "_". This serves
  as a distinction between function local variables and struct members.
  e.g. "num_buckets" is a member of "HashTable".

*/

/****************************************************************************
* Include the Public Interface
*
* By including the public interface at the top of the file, the compiler can
* enforce that the function declarations in the the header are not in
* conflict with the definitions in the file. This is not a guarantee of
* correctness, but it is better than nothing!
***************************************************************************/
#include "hash_table.h"


/****************************************************************************
* Include other private dependencies
*
* These other modules are used in the implementation of the hash table module,
* but are not required by users of the hash table.
***************************************************************************/
#include <stdlib.h>   // For malloc and free
#include <stdio.h>    // For printf


/****************************************************************************
* Hidden Definitions
*
* These definitions are not available outside of this file. However, because
* the are forward declared in hash_table.h, the type names are
* available everywhere and user code can hold pointers to these structs.
***************************************************************************/
/**
 * This structure represents an a hash table.
 * Use "HashTable" instead when you are creating a new variable. [See top comments]
 */
struct _HashTable {
  /** The array of pointers to the head of a singly linked list, whose nodes
      are HashTableEntry objects */
  HashTableEntry** buckets;

  /** The hash function pointer */
  HashFunction hash;

  /** The number of buckets in the hash table */
  unsigned int num_buckets;
};

/**
 * This structure represents a hash table entry.
 * Use "HashTableEntry" instead when you are creating a new variable. [See top comments]
 */
struct _HashTableEntry {
  /** The key for the hash table entry */
  unsigned int key;

  /** The value associated with this hash table entry */
  void* value;

  /**
  * A pointer pointing to the next hash table entry
  * NULL means there is no next entry (i.e. this is the tail)
  */
  HashTableEntry* next;
};


/****************************************************************************
* Private Functions
*
* These functions are not available outside of this file, since they are not
* declared in hash_table.h.
***************************************************************************/
/**
* createHashTableEntry
*
* Helper function that creates a hash table entry by allocating memory for it on
* the heap. It initializes the entry with key and value, initialize pointer to
* the next entry as NULL, and return the pointer to this hash table entry.
*
* @param key The key corresponds to the hash table entry
* @param value The value stored in the hash table entry
* @return The pointer to the hash table entry
*/
static HashTableEntry* createHashTableEntry(unsigned int key, void* value) {

    HashTableEntry* this_HTEntry;
    this_HTEntry = (HashTableEntry*)malloc(sizeof(HashTableEntry));
    this_HTEntry -> key = key;
    this_HTEntry -> value = value;
    this_HTEntry -> next = NULL;
    if(this_HTEntry == NULL)
        return NULL;
    else
        return this_HTEntry;
}

/**
* findItem
*
* Helper function that checks whether there exists the hash table entry that
* contains a specific key.
*
* @param hashTable The pointer to the hash table.
* @param key The key corresponds to the hash table entry
* @return The pointer to the hash table entry, or NULL if key does not exist
*/
static HashTableEntry* findItem(HashTable* hashTable, unsigned int key) {
  HashTableEntry* this_HTE;
    HashFunction hash = hashTable -> hash;
    //pc.printf("finding item\r\n");
    unsigned int i = hash(key);
    //pc.printf("%d \r\n",i);
    this_HTE = hashTable -> buckets[i];
    //pc.printf("item found...%d\r\n", this_HTE);
    while(this_HTE != NULL){
        //pc.printf("while loop entered, current entry...%d\r\n",this_HTE);
        if(this_HTE -> key == key){
            //pc.printf("key matches, returning HTE\r\n");
            return this_HTE;
        }
        //pc.printf("item not found, going to next entry\r\n");
        this_HTE = this_HTE -> next;
        //pc.printf("next entry...%d \r\n",this_HTE);
    }
    //pc.printf("no item found, returning null \r\n");
    return this_HTE;

}

/****************************************************************************
* Public Interface Functions
*
* These functions implement the public interface as specified in the header
* file, and make use of the private functions and hidden definitions in the
* above sections.
****************************************************************************/
// The createHashTable is provided for you as a starting point.
HashTable* createHashTable(HashFunction hashFunction, unsigned int numBuckets) {
  // The hash table has to contain at least one bucket. Exit gracefully if
  // this condition is not met.
  if (numBuckets==0) {
    printf("Hash table has to contain at least 1 bucket...\n");
    exit(1);
  }

  // Allocate memory for the new HashTable struct on heap.
  HashTable* newTable = (HashTable*)malloc(sizeof(HashTable));

  // Initialize the components of the new HashTable struct.
  newTable->hash = hashFunction;
  newTable->num_buckets = numBuckets;
  newTable->buckets = (HashTableEntry**)malloc(numBuckets*sizeof(HashTableEntry*));

  // As the new buckets are empty, init each bucket as NULL.
  unsigned int i;
  for (i=0; i<numBuckets; ++i) {
    newTable->buckets[i] = NULL;
  }

  // Return the new HashTable struct.
  return newTable;
}

void destroyHashTable(HashTable* hashTable) {
  unsigned int key;
    void* value;
    HashTableEntry* next;
    if(hashTable->buckets != NULL){
        //printf("there is a bucket...\n");
        for(int i = 0; i<(hashTable->num_buckets); i++){
            //printf("bucket %d ...\n",i);
            if(hashTable->buckets[i] != NULL){
                key = hashTable->buckets[i]->key;
                //printf("entry %d ...\n",i);
                value = hashTable->buckets[i]->value;
                //printf("%d...\n" ,value);
                next = hashTable->buckets[i]->next;
                //printf("next entry found ...\n");
                free(hashTable->buckets[i]->value);
                //printf("current entry deleted ...\n");
                hashTable->buckets[i] = next;
                //printf("head replaced ...\n");
                if(hashTable->buckets[i]){
                    hashTable->buckets[i]->next = next -> next;
                    //printf("nextvalue set...\n");
                }
                //printf("delete hashtable entry...\n");
            }
            //printf("delete hashtable bucket...\n");
            free(hashTable -> buckets[i]);
        }
        //printf("delete hashtable bucket field...\n");
        free(hashTable -> buckets); 
    }
    //printf("delete hashtable...\n");
    free(hashTable);
}

void* insertItem(HashTable* hashTable, unsigned int key, void* value) {
  //initialize necessary variables to insert (need entry, find entry, create entry, and hash)
    HashTableEntry* this_HashTableEntry;
    HashTableEntry* findItem(HashTable* hashTable, unsigned int key);
    HashTableEntry* createHashTableEntry(unsigned int key, void* value);
    HashFunction hash = hashTable -> hash;
    
    unsigned int i = hash(key);
    
    //perform linked list insert
    //pc.printf("insert called\r\n");
    this_HashTableEntry = findItem(hashTable,key);
   // pc.printf("item found or not found\r\n");
    if(this_HashTableEntry != NULL){
        //pc.printf("if statement entered\r\n");
        void* temp = this_HashTableEntry -> value;
        this_HashTableEntry -> value = value;
        //pc.printf("inserting entry overwrite...\r\n");
        return temp; 
    }
    else{
        this_HashTableEntry = createHashTableEntry(key,value);
        //pc.printf("inserting new entry...\r\n");
        this_HashTableEntry -> next = hashTable -> buckets[i];
        hashTable -> buckets[i] = this_HashTableEntry;
        //pc.printf("hashtable entry linked...\r\n");
        return NULL;
        
    }
}

void* getItem(HashTable* hashTable, unsigned int key) {
  HashTableEntry* addr;
    HashTableEntry* findItem(HashTable* hashTable, unsigned int key);
    //printf("start of get item...\n");
    addr = findItem(hashTable,key);
    //printf("item found...\n");
    if(addr != NULL){
        //printf("item value found...\n");
        return addr->value;
    }
    else
        //printf("item value not found...\n");
        return NULL;
}

void* removeItem(HashTable* hashTable, unsigned int key) {
  void* getItem(HashTable* hashTable, unsigned int key);
    HashTableEntry* this_HTE;
    HashTableEntry* trail;
    HashTableEntry* temp;
    HashFunction hash = hashTable -> hash;
    unsigned int i = hash(key);
    this_HTE = hashTable -> buckets[i];
    trail = NULL;
    void* val = getItem(hashTable,key);
    
    
    
    if(!this_HTE){
        printf("Entry Null...\n");
        return NULL;
    }
    else{   
        
        while((this_HTE -> next)&&(this_HTE-> key != key)){
            //printf("while loop remove...\n");
            trail = this_HTE;
            this_HTE = this_HTE -> next;
        }
        if(this_HTE-> key == key){
            
            temp = this_HTE -> next;
            //printf("if statement remove...\n");
            if(trail){
                //printf("2nd if statement remove...\n");
                trail -> next = temp;
            }
            else{
                //printf("else of 2nd if statement remove...\n");
                hashTable -> buckets[i] = temp;
            }
            free(this_HTE);
            //printf("entry freed...\n");
            /*if(trail){
                printf("trail not null...\n");
                return trail->value;
            }*/
            return val;
        }
        return NULL;
        }
        
        
    }

void deleteItem(HashTable* hashTable, unsigned int key) {
  void* removeItem(HashTable* hashTable, unsigned int key);
  void* val = removeItem(hashTable, key);
  free(val);
}