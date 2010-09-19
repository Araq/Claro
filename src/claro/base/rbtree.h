#ifndef CLARO_RBTREE_H
#define CLARO_RBTREE_H

#include <assert.h>

/* Code taken from SGLIB library under the LGPL license. */

/* ------------------------------- binary tree traversal (level 0) -------------------- */


#define ADT___BIN_TREE_MAP_ON_ELEMENTS(type, tree, iteratedVariable, order, left, right, command) {\
/* this is non-recursive implementation of tree traversal */\
/* it maintains the path to the current node in the array '_path_' */\
/* the _path_[0] contains the root of the tree; */\
/* the _path_[_pathi_] contains the _current_element_ */\
/* the macro does not use the _current_element_ after execution of command */\
/* command can destroy it, it can free the element for example */\
type *_path_[ADT_MAX_TREE_DEEP];\
type *_right_[ADT_MAX_TREE_DEEP];\
char _pass_[ADT_MAX_TREE_DEEP];\
type *_cn_;\
int _pathi_;\
type *iteratedVariable;\
_cn_ = (tree);\
_pathi_ = 0;\
while (_cn_!=NULL) {\
    /* push down to leftmost innermost element */\
    while(_cn_!=NULL) {\
        _path_[_pathi_] = _cn_;\
        _right_[_pathi_] = _cn_->right;\
        _pass_[_pathi_] = 0;\
        _cn_ = _cn_->left;\
        if (order == 0) {\
            iteratedVariable = _path_[_pathi_];\
            {command;}\
        }\
        _pathi_ ++;\
        if (_pathi_ >= ADT_MAX_TREE_DEEP) assert(0 && "the binary_tree is too deep");\
    }\
do {\
    _pathi_ --;\
    if ((order==1 && _pass_[_pathi_] == 0)\
        || (order == 2 && (_pass_[_pathi_] == 1 || _right_[_pathi_]==NULL))) {\
        iteratedVariable = _path_[_pathi_];\
        {command;}\
    }\
    _pass_[_pathi_] ++;\
} while (_pathi_>0 && _right_[_pathi_]==NULL) ;\
    _cn_ = _right_[_pathi_];\
    _right_[_pathi_] = NULL;\
    _pathi_ ++;\
}\
}

#define ADT_BIN_TREE_MAP_ON_ELEMENTS(type, tree, _current_element_, left, right, command) {\
ADT___BIN_TREE_MAP_ON_ELEMENTS(type, tree, _current_element_, 1, left, right, command);\
}

#define ADT_BIN_TREE_MAP_ON_ELEMENTS_PREORDER(type, tree, _current_element_, left, right, command) {\
ADT___BIN_TREE_MAP_ON_ELEMENTS(type, tree, _current_element_, 0, left, right, command);\
}

#define ADT_BIN_TREE_MAP_ON_ELEMENTS_POSTORDER(type, tree, _current_element_, left, right, command) {\
ADT___BIN_TREE_MAP_ON_ELEMENTS(type, tree, _current_element_, 2, left, right, command);\
}

#define ADT___BIN_TREE_FIND_MEMBER(type, tree, elem, left, right, comparator, res) {\
type *_s_;\
int _c_;\
_s_ = (tree);\
while (_s_!=NULL) {\
    _c_ = comparator((elem), _s_);\
    if (_c_ < 0) _s_ = _s_->left;\
    else if (_c_ > 0) _s_ = _s_->right;\
    else break;\
}\
(res) = _s_;\
}


/* --------------------------------- red-black trees (level 1) -------------------------------- */

/*

This  implementation requires  pointers  to left  and  right sons  (no
parent  pointer  is needed)  and  one  bit  of additional  information
storing the color of  the node. The implementation follows discrepancy
fixing rules from:
http://www.cis.ohio-state.edu/~gurari/course/cis680/cis680Ch11.html

*/

#define ADT___RBTREE_FIX_INSERTION_DISCREPANCY(type, tree, leftt, rightt, bits, RED, BLACK) {\
  type *t, *tl, *a, *b, *c, *ar, *bl, *br, *cl, *cr;\
  t = *tree;\
  tl = t->leftt;\
  if (t->rightt!=NULL && ADT___GET_VALUE(t->rightt->bits)==RED) {\
    if (ADT___GET_VALUE(tl->bits)==RED) {\
      if ((tl->leftt!=NULL && ADT___GET_VALUE(tl->leftt->bits)==RED) \
          || (tl->rightt!=NULL && ADT___GET_VALUE(tl->rightt->bits)==RED)) {\
        ADT___SET_VALUE(t->leftt->bits,BLACK);\
        ADT___SET_VALUE(t->rightt->bits,BLACK);\
        ADT___SET_VALUE(t->bits,RED);\
      }\
    }\
  } else {\
    if (ADT___GET_VALUE(tl->bits)==RED) {\
      if (tl->leftt!=NULL && ADT___GET_VALUE(tl->leftt->bits)==RED) {\
        a = t; b = tl; c = tl->leftt;\
        br = b->rightt;\
        a->leftt = br;\
        b->leftt = c; b->rightt = a;\
        ADT___SET_VALUE(a->bits,RED);\
        ADT___SET_VALUE(b->bits,BLACK);\
        *tree = b;\
      } else if (tl->rightt!=NULL && ADT___GET_VALUE(tl->rightt->bits)==RED) {\
        a = t; b = tl; ar=a->rightt;\
        bl=b->leftt; c=b->rightt;\
        cl=c->leftt; cr=c->rightt;\
        b->rightt = cl;\
        a->leftt = cr;\
        c->leftt = b;\
        c->rightt = a;\
        ADT___SET_VALUE(c->bits,BLACK);\
        ADT___SET_VALUE(a->bits,RED);\
        *tree = c;\
      }\
    }\
  }\
}

#define ADT___RBTREE_FIX_DELETION_DISCREPANCY(type, tree, leftt, rightt, bits, RED, BLACK, res) {\
  type  *t, *a, *b, *c, *d, *ar, *bl, *br, *cl, *cr, *dl, *dr;\
  t = a = *tree;\
  assert(t!=NULL);\
  ar = a->rightt;\
  b = t->leftt;\
  if (b==NULL) {\
    assert(ADT___GET_VALUE(t->bits)==RED);\
    ADT___SET_VALUE(t->bits,BLACK);\
    res = 0;\
  } else {\
    bl = b->leftt;\
    br = b->rightt;\
    if (ADT___GET_VALUE(b->bits)==RED) {\
      if (br==NULL) {\
        *tree = b;\
        ADT___SET_VALUE(b->bits,BLACK);\
        b->rightt = a;\
        a->leftt = br;\
        res = 0;\
      } else {\
        c = br;\
        assert(c!=NULL && ADT___GET_VALUE(c->bits)==BLACK);\
        cl = c->leftt;\
        cr = c->rightt;\
        if ((cl==NULL||ADT___GET_VALUE(cl->bits)==BLACK) && (cr==NULL||ADT___GET_VALUE(cr->bits)==BLACK)) {\
          *tree = b;\
          b->rightt = a;\
          ADT___SET_VALUE(b->bits,BLACK);\
          a->leftt = c;\
          ADT___SET_VALUE(c->bits,RED);\
          res = 0;\
        } else if (cl!=NULL && ADT___GET_VALUE(cl->bits)==RED) {\
          if (cr!=NULL && ADT___GET_VALUE(cr->bits)==RED) {\
            d = cr;\
            dl = d->leftt;\
            dr = d->rightt;\
            *tree = d;\
            ADT___SET_VALUE(d->bits,BLACK);\
            d->leftt = b;\
            c->rightt = dl;\
            d->rightt = a;\
            a->leftt = dr;\
            res = 0;\
          } else {\
            *tree = c;\
            c->leftt = b;\
            c->rightt = a;\
            b->leftt = bl;\
            b->rightt = cl;\
            a->leftt = cr;\
            ADT___SET_VALUE(cl->bits,BLACK);\
            res = 0;\
          }\
        } else if (cr!=NULL && ADT___GET_VALUE(cr->bits)==RED) {\
          assert(cl==NULL || ADT___GET_VALUE(cl->bits)==BLACK);\
          d = cr;\
          dl = d->leftt;\
          dr = d->rightt;\
          *tree = d;\
          ADT___SET_VALUE(d->bits,BLACK);\
          d->leftt = b;\
          c->rightt = dl;\
          d->rightt = a;\
          a->leftt = dr;\
          res = 0;\
        } else {\
          assert(0);\
          res = 0;\
        }\
      }\
    } else {\
      if ((bl==NULL || ADT___GET_VALUE(bl->bits)==BLACK) && (br==NULL || ADT___GET_VALUE(br->bits)==BLACK)) {\
        res = (ADT___GET_VALUE(a->bits)==BLACK);\
        ADT___SET_VALUE(a->bits,BLACK);\
        ADT___SET_VALUE(b->bits,RED);\
      } else if (bl!=NULL && ADT___GET_VALUE(bl->bits)==RED) {\
        if (br==NULL || ADT___GET_VALUE(br->bits)==BLACK) {\
          *tree = b;\
          ADT___SET_VALUE(b->bits,ADT___GET_VALUE(a->bits));\
          ADT___SET_VALUE(a->bits,BLACK);\
          b->rightt = a;\
          a->leftt = br;\
          ADT___SET_VALUE(bl->bits,BLACK);\
          res = 0;\
        } else {\
          assert(bl!=NULL);\
          assert(br!=NULL);\
          assert(ADT___GET_VALUE(bl->bits)==RED);\
          assert(ADT___GET_VALUE(br->bits)==RED);\
          c = br;\
          cl = c->leftt;\
          cr = c->rightt;\
          *tree = c;\
          ADT___SET_VALUE(c->bits,ADT___GET_VALUE(a->bits));\
          ADT___SET_VALUE(a->bits,BLACK);\
          c->leftt = b;\
          c->rightt = a;\
          b->rightt = cl;\
          a->leftt = cr;\
          res = 0;\
        }\
      } else {\
        assert(br!=NULL && ADT___GET_VALUE(br->bits)==RED);\
        c = br;\
        cl = c->leftt;\
        cr = c->rightt;\
        *tree = c;\
        ADT___SET_VALUE(c->bits,ADT___GET_VALUE(a->bits));\
        ADT___SET_VALUE(a->bits,BLACK);\
        c->leftt = b;\
        c->rightt = a;\
        b->rightt = cl;\
        a->leftt = cr;\
        res = 0;\
      }\
    }\
  }\
}


#define ADT_DEFINE_RBTREE_FUNCTIONS_GENERAL(type, left, right, bits, comparator, RED, BLACK) \
static void adt___##type##_fix_left_insertion_discrepancy(type **tree) {\
  ADT___RBTREE_FIX_INSERTION_DISCREPANCY(type, tree, left, right, bits, RED, BLACK);\
}\
\
static void adt___##type##_fix_right_insertion_discrepancy(type **tree) {\
  ADT___RBTREE_FIX_INSERTION_DISCREPANCY(type, tree, right, left, bits, RED, BLACK);\
}\
\
static int adt___##type##_fix_left_deletion_discrepancy(type **tree) {\
  int       res;\
  ADT___RBTREE_FIX_DELETION_DISCREPANCY(type, tree, right, left, bits, RED, BLACK, res);\
  return(res);\
}\
\
static int adt___##type##_fix_right_deletion_discrepancy(type **tree) {\
  int       res;\
  ADT___RBTREE_FIX_DELETION_DISCREPANCY(type, tree, left, right, bits, RED, BLACK, res);\
  return(res);\
}\
\
static void adt___##type##_add_recursive(type **tree, type *elem) {\
  int cmp;\
  type *t;\
  t = *tree;\
  if (t == NULL) {\
    ADT___SET_VALUE(elem->bits,RED);\
    *tree =elem;\
  } else {\
    cmp = comparator(elem, t);\
    if (cmp < 0 || (cmp==0 && elem<t)) {\
      adt___##type##_add_recursive(&t->left, elem);\
      if (ADT___GET_VALUE(t->bits)==BLACK) adt___##type##_fix_left_insertion_discrepancy(tree);\
    } else {\
      adt___##type##_add_recursive(&t->right, elem);\
      if (ADT___GET_VALUE(t->bits)==BLACK) adt___##type##_fix_right_insertion_discrepancy(tree);\
    }\
  }\
}\
\
static int adt___##type##_delete_rightmost_leaf(type **tree, type **theLeaf) {\
  type  *t;\
  int       res, deepDecreased;\
  t = *tree;\
  res = 0;\
  assert(t!=NULL);\
  if (t->right == NULL) {\
    *theLeaf = t;\
    if (t->left!=NULL) {\
      if (ADT___GET_VALUE(t->bits)==BLACK && ADT___GET_VALUE(t->left->bits)==BLACK) res = 1;\
      ADT___SET_VALUE(t->left->bits,BLACK);\
      *tree = t->left;\
    } else {\
      *tree = NULL;\
      res = (ADT___GET_VALUE(t->bits)==BLACK);\
    }\
  } else {\
    deepDecreased = adt___##type##_delete_rightmost_leaf(&t->right, theLeaf);\
    if (deepDecreased) res = adt___##type##_fix_right_deletion_discrepancy(tree);\
  }\
  return(res);\
}\
\
int adt___##type##_delete_recursive(type **tree, type *elem) {\
  type  *t, *theLeaf;\
  int       cmp, res, deepDecreased;\
  t = *tree;\
  res = 0;\
  if (t==NULL) {\
    assert(0 && "The element to delete not found in the tree,  use 'delete_if_member'"!=NULL);\
  } else {\
    cmp = comparator(elem, t);\
    if (cmp < 0 || (cmp==0 && elem<t)) {\
      deepDecreased = adt___##type##_delete_recursive(&t->left, elem);\
      if (deepDecreased) {\
        res = adt___##type##_fix_left_deletion_discrepancy(tree);\
      }\
    } else if (cmp > 0  || (cmp==0 && elem>t)) {\
      deepDecreased = adt___##type##_delete_recursive(&t->right, elem);\
      if (deepDecreased) {\
        res = adt___##type##_fix_right_deletion_discrepancy(tree);\
      }\
    } else {\
      assert(elem==t && "Deleting an element which is non member of the tree, use 'delete_if_member'"!=NULL);\
      if (t->left == NULL) {\
        if (t->right == NULL) {\
          /* a leaf, delete, it; */\
          *tree = NULL;\
          res = (ADT___GET_VALUE(t->bits)==BLACK);\
        } else {\
          if (ADT___GET_VALUE(t->bits)==0 && ADT___GET_VALUE(t->right->bits)==0) res = 1;\
          ADT___SET_VALUE(t->right->bits,BLACK);\
          *tree = t->right;\
        }\
      } else {\
        /* propagate deletion until righmost leaf of left subtree */\
        deepDecreased = adt___##type##_delete_rightmost_leaf(&t->left, &theLeaf);\
        theLeaf->left = t->left;\
        theLeaf->right = t->right;\
        ADT___SET_VALUE(theLeaf->bits,ADT___GET_VALUE(t->bits));\
        *tree = theLeaf;\
        if (deepDecreased) res = adt___##type##_fix_left_deletion_discrepancy(tree);\
      }\
    }\
  }\
  return(res);\
}\
\
void adt_##type##_add(type **tree, type *elem) {\
  elem->left = elem->right = NULL;\
  adt___##type##_add_recursive(tree, elem);\
  ADT___SET_VALUE((*tree)->bits,BLACK);\
}\
\
void adt_##type##_delete(type **tree, type *elem) {\
  adt___##type##_delete_recursive(tree, elem);\
  if (*tree!=NULL) ADT___SET_VALUE((*tree)->bits,BLACK);\
}\
\
type *adt_##type##_find_member(type *t, type *elem) {\
  type *res;\
  ADT___BIN_TREE_FIND_MEMBER(type, t, elem, left, right, comparator, res);\
  return(res);\
}\
\
int adt_##type##_is_member(type *t, type *elem) {\
  int       cmp;\
  while (t!=NULL) {\
    cmp = comparator(elem, t);\
    if (cmp < 0 || (cmp==0 && elem<t)) {\
      t = t->left;\
    } else if (cmp > 0 || (cmp==0 && elem>t)) {\
      t = t->right;\
    } else {\
      assert(t == elem);\
      return(1);\
    }\
  }\
  return(0);\
}\
\
int adt_##type##_delete_if_member(type **tree, type *elem, type **memb) {\
  if ((*memb=adt_##type##_find_member(*tree, elem))!=NULL) {\
    adt_##type##_delete(tree, *memb);\
    return(1);\
  } else {\
    return(0);\
  }\
}\
int adt_##type##_add_if_not_member(type **tree, type *elem, type **memb) {\
  if ((*memb=adt_##type##_find_member(*tree, elem))==NULL) {\
    adt_##type##_add(tree, elem);\
    return(1);\
  } else {\
    return(0);\
  }\
}\
int adt_##type##_len(type *t) {\
    int   n;\
    n = 0;\
    ADT_BIN_TREE_MAP_ON_ELEMENTS(type, t, e, left, right, n++);\
    return(n);\
}\
\
void adt__##type##_it_compute_current_elem(struct adt_##type##_iterator *it) {\
    int   i,j;\
    type  *s, *eqt;\
    int   (*subcomparator)(type *, type *);\
    eqt = it->equalto;\
    subcomparator = it->subcomparator;\
    it->currentelem = NULL;\
    while(it->pathi > 0 && it->currentelem==NULL) {\
        i = it->pathi-1;\
        if (i >= 0) {\
            if (it->pass[i] >= 2) {\
                /* goto up */\
                it->pathi --;\
            } else {\
              if (it->pass[i] == 0) {\
                  /* goto left */\
                s = it->path[i]->left;\
              } else {\
                /* goto right */\
                s = it->path[i]->right;\
              }\
              if (eqt != NULL) {\
                if (subcomparator == NULL) {\
                  ADT___BIN_TREE_FIND_MEMBER(type, s, eqt, left, right, comparator, s);\
                } else {\
                  ADT___BIN_TREE_FIND_MEMBER(type, s, eqt, left, right, subcomparator, s);\
                }\
              }\
              if  (s != NULL) {\
                j = i+1;\
                it->path[j] = s;\
                it->pass[j] = 0;\
                it->pathi ++;\
              }\
              it->pass[i] ++;\
            }\
        }\
        if (it->pathi>0 && it->order == it->pass[it->pathi-1]) {\
            it->currentelem = it->path[it->pathi-1];\
        }\
    }\
}\
type *adt__##type##_it_init(struct adt_##type##_iterator *it, type *tree, int order, int (*subcomparator)(type *, type *), type *equalto) {\
    type *t;\
    assert(it!=NULL);\
    it->order = order;\
    it->equalto = equalto;\
    it->subcomparator = subcomparator;\
    if (equalto == NULL) {  \
        t = tree;\
    } else {\
        if (subcomparator == NULL) {\
          ADT___BIN_TREE_FIND_MEMBER(type, tree, equalto, left, right, comparator, t);\
        } else {\
          ADT___BIN_TREE_FIND_MEMBER(type, tree, equalto, left, right, subcomparator, t);\
        }\
    }\
    if (t == NULL) {\
        it->pathi = 0;\
        it->currentelem = NULL;\
    } else {\
        it->pathi = 1;\
        it->pass[0] = 0;\
        it->path[0] = t;\
        if (order == 0) {\
            it->currentelem = t;\
        } else {\
            adt__##type##_it_compute_current_elem(it);\
        }\
    }\
    return(it->currentelem);\
}\
type *adt_##type##_it_init(struct adt_##type##_iterator *it, type *tree) {\
  return(adt__##type##_it_init(it, tree, 2, NULL, NULL));\
}\
type *adt_##type##_it_init_preorder(struct adt_##type##_iterator *it, type *tree) {\
  return(adt__##type##_it_init(it, tree, 0, NULL, NULL));\
}\
type *adt_##type##_it_init_inorder(struct adt_##type##_iterator *it, type *tree) {\
  return(adt__##type##_it_init(it, tree, 1, NULL, NULL));\
}\
type *adt_##type##_it_init_postorder(struct adt_##type##_iterator *it, type *tree) {\
  return(adt__##type##_it_init(it, tree, 2, NULL, NULL));\
}\
type *adt_##type##_it_init_on_equal(struct adt_##type##_iterator *it, type *tree, int (*subcomparator)(type *, type *), type *equalto) {\
  return(adt__##type##_it_init(it, tree, 1, subcomparator, equalto));\
}\
type *adt_##type##_it_current(struct adt_##type##_iterator *it) {\
  return(it->currentelem);\
}\
type *adt_##type##_it_next(struct adt_##type##_iterator *it) {\
  adt__##type##_it_compute_current_elem(it);\
  return(it->currentelem);\
}\
\
static void adt___##type##_consistency_check_recursive(type *t, int *pathdeep, int cdeep) {\
  if (t==NULL) {\
    if (*pathdeep < 0) *pathdeep = cdeep;\
    else assert(*pathdeep == cdeep);\
  } else {\
    if (t->left!=NULL) assert(comparator(t->left, t) <= 0);\
    if (t->right!=NULL) assert(comparator(t, t->right) <= 0);\
    if (ADT___GET_VALUE(t->bits) == RED) {\
      assert(t->left == NULL || ADT___GET_VALUE(t->left->bits)==BLACK);\
      assert(t->right == NULL || ADT___GET_VALUE(t->right->bits)==BLACK);\
      adt___##type##_consistency_check_recursive(t->left, pathdeep, cdeep);\
      adt___##type##_consistency_check_recursive(t->right, pathdeep, cdeep);\
    } else {\
      adt___##type##_consistency_check_recursive(t->left, pathdeep, cdeep+1);\
      adt___##type##_consistency_check_recursive(t->right, pathdeep, cdeep+1);\
    }\
  }\
}\
\
void adt___##type##_consistency_check(type *t) {\
  int pathDeep;\
  assert(t==NULL || ADT___GET_VALUE(t->bits) == BLACK);\
  pathDeep = -1;\
  adt___##type##_consistency_check_recursive(t, &pathDeep, 0);\
}


#define ADT_DEFINE_RBTREE_PROTOTYPES(type, left, right, colorbit, comparator) \
 struct adt_##type##_iterator {\
    type *currentelem;\
    char pass[ADT_MAX_TREE_DEEP];\
    type *path[ADT_MAX_TREE_DEEP];\
    short int pathi;\
    short int order;\
    type *equalto;\
    int (*subcomparator)(type *, type *);\
 };\
 extern void adt___##type##_consistency_check(type *t); \
 extern void adt_##type##_add(type **tree, type *elem); \
 extern int adt_##type##_add_if_not_member(type **tree, type *elem, type **memb); \
 extern void adt_##type##_delete(type **tree, type *elem); \
 extern int adt_##type##_delete_if_member(type **tree, type *elem, type **memb); \
 extern int adt_##type##_is_member(type *t, type *elem); \
 extern type *adt_##type##_find_member(type *t, type *elem); \
 extern int adt_##type##_len(type *t); \
 extern type *adt_##type##_it_init(struct adt_##type##_iterator *it, type *tree); \
 extern type *adt_##type##_it_init_preorder(struct adt_##type##_iterator *it, type *tree); \
 extern type *adt_##type##_it_init_inorder(struct adt_##type##_iterator *it, type *tree); \
 extern type *adt_##type##_it_init_postorder(struct adt_##type##_iterator *it, type *tree); \
 extern type *adt_##type##_it_init_on_equal(struct adt_##type##_iterator *it, type *tree, int (*subcomparator)(type *, type *), type *equalto); \
 extern type *adt_##type##_it_current(struct adt_##type##_iterator *it); \
 extern type *adt_##type##_it_next(struct adt_##type##_iterator *it); \


#define ADT_DEFINE_RBTREE_FUNCTIONS(type, left, right, colorbit, comparator) \
  ADT_DEFINE_RBTREE_FUNCTIONS_GENERAL(type, left, right, colorbit, comparator, 1, 0)

 /* ---------------------------------------------------------------------------- */
 /* ---------------------------------------------------------------------------- */
 /* -                          SUPPLEMENTARY DEFINITIONS                       - */
 /* ---------------------------------------------------------------------------- */
 /* ---------------------------------------------------------------------------- */


#define ADT___GET_VALUE(x) (x)
#define ADT___SET_VALUE(x, value) {(x) = (value);}
#define ADT_ARRAY_ELEMENTS_EXCHANGER(type, a, i, j) {type _sgl_aee_tmp_; _sgl_aee_tmp_=(a)[(i)]; (a)[(i)]=(a)[(j)]; (a)[(j)]= _sgl_aee_tmp_;}


#define ADT_NUMERIC_COMPARATOR(x, y) ((int)((x) - (y)))
#define ADT_REVERSE_NUMERIC_COMPARATOR(x, y) ((int)((y) - (x)))

#ifndef ADT_MAX_TREE_DEEP
#define ADT_MAX_TREE_DEEP 128
#endif

#ifndef ADT_HASH_TAB_SHIFT_CONSTANT
#define ADT_HASH_TAB_SHIFT_CONSTANT 16381   /* should be a prime */
 /* #define ADT_HASH_TAB_SHIFT_CONSTANT 536870912  for large tables */
#endif

#endif /* rbtree_h */
 
