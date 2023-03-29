/**
 *  Further adaption of the redblack tree code with the etimology shown
 *  below from MIT as provided opensource.apple.com as part of the sudo-73
 *  utility code.
 *
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

/*
 * Copyright (c) 2004, 2007 Todd C. Miller <Todd.Miller@courtesan.com>
 *
 * Permission to use, copy, modify, and distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#ifndef _REDBLACK_H
#define _REDBLACK_H

enum rbcolor {
  red,
  black
};

enum rbtraversal {
  preorder,
  inorder,
  postorder
};

typedef struct rbnode {
  struct rbnode *left,
                *right,
                *parent;
  void *data;
  enum rbcolor color;
} rbnode;

typedef struct rbtree {
  int (*compar)(const void *, const void *);
  struct rbnode root,
                nil,
                err;
} rbtree;

#define rbapply(t, f, c, o) rbapply_node((t), (t)->root.left, (f), (c), (o))
#define rbisempty(t)        ((t)->root.left == &(t)->nil && (t)->root.right == &(t)->nil)
#define rbfirst(t)          ((t)->root.left)
#define rbroot(t)           (&(t)->root)
#define rbnil(t)            (&(t)->nil)
#define rberr(t)            (&(t)->err)

int rbapply_node            (rbtree *, rbnode *,
                            int (*)(void *, void *), void *, enum rbtraversal);
rbtree *rbcreate            (int (*)(const void *, const void *));
rbnode *rbinsert            (rbtree *, void *, size_t);

rbnode *rbfind              (rbtree *, void *);
rbnode *rbmin               (rbtree *);
rbnode *rbmax               (rbtree *);
rbnode *rbsuccessor         (rbtree *, rbnode *);
rbnode *rbprior             (rbtree *, rbnode *);
int rbtraverse              (rbtree *, rbnode *,
                            int (*)(void *, void *), void *, enum rbtraversal);
rbnode *rbreplace           (rbtree *, rbnode *, rbnode *);

void rbdestroy              (rbtree *, void (*)(void *));
void *rbdelete              (rbtree *, rbnode *);


#endif /* _REDBLACK_H */
