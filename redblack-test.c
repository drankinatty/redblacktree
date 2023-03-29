/**
 *  THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 *  WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 *  MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 *  ANY DAMAGES, WHETHER SPECIAL, DIRECT, INDIRECT, CONSEQUENTIAL OR OTHERWISE
 *  OR ANY DAMAGES WHATSOEVER, WHETHER SOUNDING IN CONTRACT, NEGLIGENCE, TORT,
 *  OR OTHER ACTION ARISING OUT OF, OR IN CONNECTION WITH, ANY AND ALL USE OF
 *  THIS SOFTWARE BY ANY USER OF THIS SOFTWARE, OR ANYONE CLAIMING BY THROUGH
 *  OR UNDER AND PERSON OR ENTITY MAKING USE OF THIS SOFTWARE.
 *
 *  This Software is Licence Under the GNU Public Licenxe, GPLv2.
 *
 *  Copyright (c) 2015-2023 David C. Rankin,J.D.,P.E. <drankinatty@gmail.com>
 */

/* compile, gcc example

--
  gcc -Wall -Wextra -pedantic -Wshadow -Werror -std=c11 -Ofast redblack.c -o redblack-test redblack-test.c
--

Compile time defines that alter test behavior:

  DEBUG - causes the node and data pointers and node-color to be output

  EXTERNALSTRG - tests external data storage. An integer array is created
                 holding data for the tree. The tree assigns the address of
                 each integer to the node->data member

                 (default is for tree to allocate storage for node->data
                  of typesz bytes based on typesz parameter to rbinsert())

  e.g. add:  -DEXTERNALSTRG to compile string to activate

Program Use:

  ./redblack-test [no. of nodes (default 10)]

*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "redblack.h"

#define TSTNODES 10

#ifdef __STDC_VERSION__
#define SIZT "%zu"
#else
#define SIZT "%lu"
#endif

/** Random numbers within a range (correct way).
 *  This code provide a random number making attemps
 *  to correct bias injected by use of modulo. You must
 *  initilize the random
 *
 *  Assumes 0 <= range <= RAND_MAX
 *  Returns in the half-open interval [0, max]
 */
int randi (int max)
{
  unsigned
      /* max <= RAND_MAX < UINT_MAX, so this is okay. */
      num_bins = (unsigned) max + 1,
      num_rand = (unsigned) RAND_MAX + 1,
      bin_size = num_rand / num_bins,
      defect   = num_rand % num_bins;
  int x;

  /* carefully written not to overflow */
  while (num_rand - defect <= (unsigned)(x = rand()));

  /* truncated division is intentional */
  return x / bin_size;
}

/* simple free function to pass to rbdestroy to free tree data */
void idestroy (void *a)
{
  free (a);
}

/* comparison function for objects in redblack tree.
 * a compare function must be written to accompany each
 * object type held in the redblack tree.
 */
int icompare (const void *a, const void *b)
{
  const int *x = a,   /* cast to proper type (int*) here */
            *y = b;

  /* return result of conditional to avoid potential overflow */
  return (*x > *y) - (*x < *y);
}

/* simple print each key in list and cookie passed through rbapply.
 * cookie is just another void pointer that allows additional data
 * to be passed by rbapply for each node passed to the function
 * argument.
 */
int rbprint (void *data, void *c)
{
  printf (" node->key:  %3d    cookie: %d\n",
          *(int *)data, *(int *)c);

  return 0;
}

#ifdef DEBUG
/* debug output of rbprint data but includes pointer addresses and color
 * of each node
 */
const char *clr[] = { "red", "black" };

int rbtravnodes (void *node, void *c)
{
  printf (" node: %p    node->data: %p    data: %3d    cookie: %d    color: %s\n",
          node, (void*)((rbnode*)node)->data,
          *(int *)((rbnode*)node)->data, *(int *)c,
          clr[((rbnode*)node)->color]);

  return 0;
}
#endif

/* counts nodes in tree.
 * total count returned through cookie. if condition is simply a stub
 * to suppress warnings re: c/k set but unused..
 */
int rbsztree (void *data, void *c)
{
  *(int *)c += 1;

  (void)data;       /* suppress -Wunused warning */
  (void)c;

  return 0;
}

/*
 * simple use of random numbers to fill rbtree and exercise each funciton.
 */
int main (int argc, char **argv)
{
  int count = 0,
      target = 0,
      target2 = 0,
#ifdef EXTERNALSTRG
     *ivalues = NULL, /* pointer to allocate to hold values */
#endif
      cookie = 14;    /* arbitrary value for cookie */

  struct rbnode *newNode = NULL,
                *tmp = NULL;
  struct rbtree *tree = NULL;

  /* use number of nodes specified as argument, or TSTNODES */
  size_t  nnodes = argc > 1 ? (size_t)atoi (argv[1]) : TSTNODES,
          it = 0;

  if (nnodes < 1) {   /* if number of nodes = 0, bail */
    fputs ("error: number of nodes requested < 1.\n", stderr);
    return 1;
  }

  srand (time (NULL));    /* seed random number generator */

#ifdef EXTERNALSTRG
  /* allocate/validate storage for nnodes int values */
  if (!(ivalues = malloc (nnodes * sizeof *ivalues))) {
    perror ("malloc-ivalues");
    return 1;
  }
#endif

  /* create tree, initialize empty tree and return address */
  if (!(tree = rbcreate (icompare))) {
    return 1;
  }

  /* generate random values and insert in tree */
  printf ("\n Adding " SIZT " random ints in the range 0 - " SIZT ":\n",
          nnodes, nnodes * 10);
  for (it = 0; it < nnodes; it++)
  {
    int ival = randi (nnodes * 10);

    if (it == nnodes / 2) {
      target = ival;          /* random target value */
    }

    if (it == 3 * nnodes / 4) {
      target2 = ival;
    }
#ifdef EXTERNALSTRG
    ivalues[it] = ival;
#endif

    /* insert in tree - nonzero return indicates value already exists */
#ifndef EXTERNALSTRG
    if ((tmp = rbinsert (tree, &ival, sizeof ival))) {
#else
    if ((tmp = rbinsert (tree, ivalues + it, 0))) {
#endif
      if (tmp != rberr(tree)) {
        fprintf (stderr, "  duplicate: %3d\n", ival);
      }
      else {
        fprintf (stderr, "  error: insert of %d failed.\n", ival);
        exit (EXIT_FAILURE);
      }
    }
  }

  /* print */
  printf ("\n printing tree:\n\n");

  /* with cookie arbitrarily set at &ivalues[nnodes/2] */
#ifdef DEBUG
  rbtraverse (tree, rbfirst (tree), rbtravnodes, &cookie, inorder);
#else
  rbapply_node (tree, rbfirst (tree), rbprint, &cookie, inorder);
#endif
  /*
    * ( see rbapply macro to eliminate 2nd argument, e.g. )
    * rbapply (tree, rbprint, ivalues + nnodes / 2, inorder);
    */

  /* population count */
  rbapply (tree, rbsztree, &count, inorder);
  printf ("\n tree population      : %d\n", count);
  count = 0;

  /* find minimum/maximum integer elements in tree */
  printf (" minimum data in tree : %d\n maximum data in tree : %d\n\n",
          *(int *)(rbmin (tree))-> data, *(int *)(rbmax (tree))-> data);

  /* search tree for node with matching key value */
  printf (" searching for %d in tree ", target);
  if ((tmp = rbfind (tree, &target)))
    printf ("=> found key %d in tree.\n\n", *(int *)tmp->data);
  else
    puts ("=> NOT found in tree.\n\n");

  /* find previous node */
  if ((newNode = rbfind (tree, &target))) {
    newNode = rbprior (tree, newNode);
    if (rbnil(tree) == newNode) {
      puts ("  no prior node for node (it is a minimum)");
    }
    else {
      printf ("  prior node has key %d\n", *(int *)newNode->data);
    }
  }
  else {
    puts ("  data not in tree");
  }

  /* find successor (next) node */
  if ((newNode = rbfind (tree, &target))) {
    newNode = rbsuccessor (tree, newNode);
    if (rbnil(tree) == newNode) {
      puts ("  no successor for node (it is a maximum)");
    }
    else {
      printf ("  successor has key %d\n", *(int *)newNode->data);
    }
  }
  else {
    puts ("  data not in tree");
  }

  /* delete node with key equal target) */
  printf ("\n deleting node '%d' in tree:\n", target);
  if ((newNode = rbfind (tree, &target))) {
#ifndef EXTERNALSTRG
    free (rbdelete (tree, newNode));
#else
    rbdelete (tree, newNode);
#endif
  }
  else {
    puts ("key not found in tree, no action taken");
  }

  printf (" deleting node '%d' in tree:\n", target2);
  if ((newNode = rbfind (tree, &target2))) {
#ifndef EXTERNALSTRG
    /* for tree allocated storage for data, pass the return of
     * rbdelete() to free()
     */
    free (rbdelete (tree, newNode));
#else
    /* with external storage, the user is responsible for freeing data */
    rbdelete (tree, newNode);
#endif
  }
  else {
    puts ("key not found in tree, no action taken");
  }

  /* population count */
  rbapply (tree, rbsztree, &count, inorder);
  printf ("\n tree population: %d\n\n", count);
  count = 0;

  /* print each node key/data value in tree */
  puts (" printing tree:\n");
#ifdef DEBUG
  rbtraverse (tree, rbfirst (tree), rbtravnodes, &cookie, inorder);
#else
  rbapply (tree, rbprint, &cookie, inorder);
#endif

  /* destroy (free) tree memory */
  puts ("\n destroying tree:\n");
#ifndef EXTERNALSTRG
  rbdestroy (tree, idestroy);
#else
  rbdestroy (tree, NULL);

  /* free ivalues int storage */
  free (ivalues);
#endif

  return 0;
}

