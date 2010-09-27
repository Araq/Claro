/* Claro Graphics - an abstraction layer for native UI libraries
 *
 * $Id$
 *
 * The contents of this file are subject to the Mozilla Public License
 * Version 1.1 (the "License"); you may not use this file except in
 * compliance with the License. You may obtain a copy of the License at
 * http://www.mozilla.org/MPL/
 *
 * Software distributed under the License is distributed on an "AS IS"
 * basis, WITHOUT WARRANTY OF ANY KIND, either express or implied. See the
 * License for the specific language governing rights and limitations
 * under the License.
 *
 * See the LICENSE file for more details.
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>


#include "base.h"


struct entry_
{
    void *k, *v;
    unsigned int h;
    struct entry_ *next;
};

struct hashtable_
{
	unsigned int tablelength;
    struct entry_ **table;
    unsigned int entrycount;
    unsigned int loadlimit;
    unsigned int primeindex;
    unsigned int (*hashfn) (void *k);
    int (*eqfn) (void *k1, void *k2);	
 	void (*key_freefn) (void*);
	void (*val_freefn) (void*);
};


static const unsigned int primes[] = {
53, 97, 193, 389,
769, 1543, 3079, 6151,
12289, 24593, 49157, 98317,
196613, 393241, 786433, 1572869,
3145739, 6291469, 12582917, 25165843,
50331653, 100663319, 201326611, 402653189,
805306457, 1610612741
};
const unsigned int prime_table_length = sizeof(primes)/sizeof(primes[0]);
const float max_load_factor = 0.65;

static unsigned int
hash(hashtable_t *h, void *k);

static inline unsigned int
indexFor(unsigned int tablelength, unsigned int hashvalue) {
    return (hashvalue % tablelength);
};

static unsigned int
hash(hashtable_t *h, void *k)
{
    /* Aim to protect against poor hash functions by adding logic here
     * - logic taken from java 1.4 hashtable source */
    unsigned int i = h->hashfn(k);
    i += ~(i << 9);
    i ^=  ((i >> 14) | (i << 18)); /* >>> */
    i +=  (i << 4);
    i ^=  ((i >> 10) | (i << 22)); /* >>> */
    return i;
}

#define freekey(X) free(X)

hashtable_t *
hashtable_create(unsigned int minsize,
                 unsigned int (*hashfunction) (void*),
                 int (*key_eq_fn) (void*,void*),
				 void (*key_free_fn) (void*),
				 void	 (*val_free_fn) (void*)	)
{
    struct hashtable_ *h;
    unsigned int pindex, size = primes[0];
    /* Check requested hashtable isn't too large */
    if (minsize > (1u << 30)) return NULL;
    /* Enforce size as prime */
    for (pindex=0; pindex < prime_table_length; pindex++) {
        if (primes[pindex] > minsize) { size = primes[pindex]; break; }
    }
    h = (struct hashtable_ *)malloc(sizeof(struct hashtable_));
    if (NULL == h) return NULL; /*oom*/
    h->table = (struct entry_ **)malloc(sizeof(struct entry_*) * size);
    if (NULL == h->table) { free(h); return NULL; } /*oom*/
    memset(h->table, 0, size * sizeof(struct entry_ *));
    h->tablelength  = size;
    h->primeindex   = pindex;
    h->entrycount   = 0;
    h->hashfn       = hashfunction;
    h->eqfn         = key_eq_fn;
    h->key_freefn   = key_free_fn;
    h->val_freefn   = val_free_fn;
    h->loadlimit    = (unsigned int) ceil(size * max_load_factor);
    return h;
}

static int
hashtable_expand(hashtable_t *h)
{
    /* Double the size of the table to accomodate more entries */
    struct entry_ **newtable;
    struct entry_ *e;
    struct entry_ **pE;
    unsigned int newsize, i, index;
    /* Check we're not hitting max capacity */
    if (h->primeindex == (prime_table_length - 1)) return 0;
    newsize = primes[++(h->primeindex)];

    newtable = (struct entry_ **)malloc(sizeof(struct entry_ *) * newsize);
    if (NULL != newtable)
    {
        memset(newtable, 0, newsize * sizeof(struct entry_ *));
        /* This algorithm is not 'stable'. ie. it reverses the list
         * when it transfers entries between the tables */
        for (i = 0; i < h->tablelength; i++) {
            while (NULL != (e = h->table[i])) {
                h->table[i] = e->next;
                index = indexFor(newsize,e->h);
                e->next = newtable[index];
                newtable[index] = e;
            }
        }
        free(h->table);
        h->table = newtable;
    }
    /* Plan B: realloc instead */
    else 
    {
        newtable = (struct entry_ **)
                   realloc(h->table, newsize * sizeof(struct entry_ *));
        if (NULL == newtable) { (h->primeindex)--; return 0; }
        h->table = newtable;
        memset(newtable[h->tablelength], 0, newsize - h->tablelength);
        for (i = 0; i < h->tablelength; i++) {
            for (pE = &(newtable[i]), e = *pE; e != NULL; e = *pE) {
                index = indexFor(newsize,e->h);
                if (index == i)
                {
                    pE = &(e->next);
                }
                else
                {
                    *pE = e->next;
                    e->next = newtable[index];
                    newtable[index] = e;
                }
            }
        }
    }
    h->tablelength = newsize;
    h->loadlimit   = (unsigned int) ceil(newsize * max_load_factor);
    return -1;
}

unsigned int
hashtable_count(hashtable_t *h)
{
    return h->entrycount;
}

static int
hashtable_change(hashtable_t *h, void *k, void *v)
{
    struct entry_ *e;
    unsigned int hashvalue, index;
    hashvalue = hash(h,k);
    index = indexFor(h->tablelength,hashvalue);
    e = h->table[index];
    while (NULL != e)
    {
        /* Check hash value to short circuit heavier comparison */
        if ((hashvalue == e->h) && (h->eqfn(k, e->k)))
        {
            if(h->val_freefn != NULL)
				h->val_freefn(e->v);
            e->v = v;
            return -1;
        }
        e = e->next;
    }
    return 0;
}

int
hashtable_insert(hashtable_t *h, void *k, void *v,int replace)
{    
	/* This method allows duplicate keys - but they shouldn't be used */
    unsigned int index;
    unsigned int hashvalue;
	struct entry_ *e;

	if(replace)
	{
		hashvalue = hash(h,k);
    index = indexFor(h->tablelength,hashvalue);
    e = h->table[index];
    while (NULL != e)
		{
        	/* Check hash value to short circuit heavier comparison */
        	if ((hashvalue == e->h) && (h->eqfn(k, e->k)))
			{	
		//	printf("%p: %p -> %p\n",e->k, e->v, v);
				return hashtable_change(h, k, v);
        	}
			e = e->next;
    	}
	}

    if (++(h->entrycount) > h->loadlimit)
    {
        /* Ignore the return value. If expand fails, we should
         * still try cramming just this value into the existing table
         * -- we may not have memory for a larger table, but one more
         * element may be ok. Next time we insert, we'll try expanding again.*/
        hashtable_expand(h);
    }
    e = (struct entry_ *)malloc(sizeof(struct entry_));
    if (NULL == e) { --(h->entrycount); return 0; } /*oom*/
    e->h = hash(h,k);
    index = indexFor(h->tablelength,e->h);
    e->k = k;
    e->v = v;
    e->next = h->table[index];
    h->table[index] = e;
    return -1;
}

void *
hashtable_search(hashtable_t *h, void *k)
{
    struct entry_ *e;
    unsigned int hashvalue, index;
    hashvalue = hash(h,k);
    index = indexFor(h->tablelength,hashvalue);
    e = h->table[index];
    while (NULL != e)
    {
        /* Check hash value to short circuit heavier comparison */
        if ((hashvalue == e->h) && (h->eqfn(k, e->k))) return e->v;
        e = e->next;
    }
    return NULL;
}

void * 
hashtable_remove(hashtable_t *h, void *k)
{
    /* TODO: consider compacting the table when the load factor drops enough,
     *       or provide a 'compact' method. */

    struct entry_ *e;
    struct entry_ **pE;
    void *v;
    unsigned int hashvalue, index;

    hashvalue = hash(h,k);
    index = indexFor(h->tablelength,hash(h,k));
    pE = &(h->table[index]);
    e = *pE;
    while (NULL != e)
    {
        /* Check hash value to short circuit heavier comparison */
        if ((hashvalue == e->h) && (h->eqfn(k, e->k)))
        {
            *pE = e->next;
            h->entrycount--;
            v = e->v;
			if(h->key_freefn != NULL)
            	h->key_freefn(e->k);
			if(h->val_freefn != NULL)
				h->val_freefn(e->v); 
            free(e);
            return v;
        }
        pE = &(e->next);
        e = e->next;
    }
    return NULL;
}

void
hashtable_destroy(hashtable_t *h)
{
    unsigned int i;
    struct entry_ *e, *f;
    struct entry_ **table = h->table;
    for (i = 0; i < h->tablelength; i++)
    {
    	e = table[i];
        while (e != NULL)
        { 
			f = e; 
			e = e->next;
			if(h->key_freefn != NULL)
				h->key_freefn(f->k);
			if(h->val_freefn != NULL)
				h->val_freefn(f->v); 
			free(f); 
		}
    }
    free(h->table);
    free(h);
}
