#include "rbtree.h"

#include <stdlib.h>

static int inorder(node_t *p, key_t *arr, const rbtree *t, int i, size_t n);
void left_rotate(rbtree *t, node_t *tmp);
void right_rotate(rbtree *t, node_t *tmp);
void rb_transplant(rbtree *t, node_t *u, node_t *v);

//RB Tree 구조체 생성
rbtree *new_rbtree(void) {
  rbtree *p = (rbtree *)calloc(1, sizeof(rbtree));
  node_t *NIL = (node_t *)calloc(1, sizeof(node_t));
  NIL->color = RBTREE_BLACK;
  p->root = NIL;
  p->nil = NIL;
  return p;
}

//RB Tree 구조체가 차지했던 메모리 반환
void delete_rbtree(rbtree *t) {
  node_t *tmp = NULL;
  rbtree *n = NULL;
  if (t->root != t->nil){
    tmp = t->root;
    if (tmp->left != t->nil){
      n->root = tmp->left;
      delete_rbtree(n);
    }
    if (tmp->right != t->nil) {
      n->root = tmp->right;
      delete_rbtree(n);
    }
  }
  free(t);
}

//RB Tree에 key 삽입하기
node_t *rbtree_insert(rbtree *t, const key_t key) {
  // tmp : 현재 삽입된 노드, y : tmp의 부모가 될 노드
  node_t *x = t->root;
  node_t *y = t->nil;
  node_t *tmp = (node_t *)malloc(sizeof(node_t));
  
  //삽입할 위치 찾기--------------------------------------
  while(x != t->nil) {
    y = x;
    if(x->key > key) {
      x = x->left;
    }
    else {
      x = x->right;
    }
  }

  //tmp 기본 정보 초기화----------------------------------
  tmp->key = key;
  tmp->color = RBTREE_RED;
  tmp->parent = y;
  tmp->left = t->nil;
  tmp->right = t->nil;
  
  //부모의 어느 위치에 저장되어야 하는지 확인-------------------
  if (y == t->nil){ //root일 경우
    t->root = tmp;
  }
  else if (y->key > key) { //왼쪽 자식일 경우
    y->left = tmp;
  }
  else { //오른쪽 자식일 경우
    y->right = tmp;
  }

  //속성 유지하기 RB-Insert-Fixup-------------------------
  //tmp : 현재 삽입된 노드 = 무조건 빨강색이다.
  while (tmp->parent->color == RBTREE_RED) {
    //tmp의 부모가 왼쪽 자식일 경우
    if (tmp->parent == tmp->parent->parent->left) {
      // 부모의 형제
      y = tmp->parent->parent->right;
      //case1 : 부모와 부모의 형제 모두 red일 경우 
      if (y->color == RBTREE_RED) {
        tmp->parent->color = RBTREE_BLACK;
        y->color = RBTREE_BLACK;
        tmp->parent->parent->color = RBTREE_RED;
        tmp = tmp->parent->parent;
      }
      //부모와 부모의 형제의 색이 다를 경우
      else {
        //case2 : 부모 red, 부모의 형제 black
        //현재 노드 tmp가 오른쪽 노드일 경우
        if (tmp == tmp->parent->right) {
          tmp = tmp->parent;
          //왼쪽 회전
          left_rotate(t, tmp);
        }
        tmp->parent->color = RBTREE_BLACK;
        tmp->parent->parent->color = RBTREE_RED;
        tmp = tmp->parent->parent;
        //오른쪽 회전
        right_rotate(t, tmp);
      }
    }
    //부모가 오른쪽 노드일 경우
    else {
      // 부모의 형제
      y = tmp->parent->parent->left; 
      //case1 : 부모와 부모의 형제 모두 red일 경우 
      if (y->color == RBTREE_RED) {
        tmp->parent->color = RBTREE_BLACK;
        y->color = RBTREE_BLACK;
        tmp->parent->parent->color = RBTREE_RED;
        tmp = tmp->parent->parent;
      }
      //부모와 부모의 형제의 색이 다를 경우
      else {
        //case2 : 부모의 형제 black, 부모 red
        //현재 노드 tmp가 오른쪽 노드일 경우
        if (tmp == tmp->parent->left) {
          tmp = tmp->parent;
          //오른쪽 회전
          right_rotate(t, tmp);
        }
        tmp->parent->color = RBTREE_BLACK;
        tmp->parent->parent->color = RBTREE_RED;
        tmp = tmp->parent->parent;
        //왼쪽 회전
        left_rotate(t, tmp);
      }
    }
  }
  t->root->color = RBTREE_BLACK; //root는 항상 검정색이어야 한다.
  return t->root;
}

//왼쪽 회전
void left_rotate(rbtree *t, node_t *tmp) {
  node_t *x = tmp->right;
  tmp->right = x->left;
  if (x->left != t->nil) {
    x->left->parent = tmp;
  }
  x->parent = tmp->parent;
  if (tmp->parent == t->nil) {
    t->root = x;
  }
  else if (tmp == tmp->parent->left) {
    tmp->parent->left = x;
  }
  else {
    tmp->parent->right = x;
  }
  x->left = tmp;
  tmp->parent = x;
}

//오른쪽 회전
void right_rotate(rbtree *t, node_t *tmp) {
  node_t *x = tmp->left;
  tmp->left = x->right;
  if (x->right != t->nil) {
    x->right->parent = tmp;
  }
  x->parent = tmp->parent;
  if(tmp->parent == t->nil) {
    t->root = x;
  }
  else if (tmp == tmp->parent->right) {
    tmp->parent->right = x;
  }
  else {
    tmp->parent->left = x;
  }
  x->right = tmp;
  tmp->parent = x;
}

//RB Tree에 key와 같은 값을 가진 값이 있는지 탐색
node_t *rbtree_find(const rbtree *t, const key_t key) {
  // TODO: implement find
  node_t *tmp = t->root;
  while(tmp != t->nil) {
    if (tmp->key == key) { //찾은 경우
      return tmp;
    }
    else if (tmp->key > key) { //현재 값보다 작은 경우
      tmp = tmp->left;
    }
    else { //현재 값보다 큰 경우
      tmp = tmp->right;
    }
  }
  if (tmp == t->nil) {
    return NULL;
  }
  return t->root;
}

//RB Tree에서 최솟값 반환
node_t *rbtree_min(const rbtree *t) {
  node_t *x = t->root;
  while(x->left != t->nil) {
    x = x->left;
  }
  return x;
}

//RB Tree에서 최댓값 반환
node_t *rbtree_max(const rbtree *t) {
  node_t *x = t->root;
  while(x->right != t->nil) {
    x = x->right;
  }
  return x;
}

//RB Tree에서 지정된 노드p를 제거
int rbtree_erase(rbtree *t, node_t *p) {
  //p가 없는 노드이면 삭제 작업 안함
  if (p == NULL) {
    return 0;
  }

  // y : 삭제할 노드, x : y의 원래의 위치로 이동할 노드
  node_t *y = p;
  color_t y_original_color = y->color;
  node_t *x;

  //p가 오른쪽 자식만 가질 경우
  if (p->left == t->nil) {
    x = p->right;
    rb_transplant(t, p, p->right);
  } //p가 왼쪽 자식만 가질 경우
  else if (p->right == t->nil) {
    x = p->left;
    rb_transplant(t, p, p->left);
  } //양쪽 자식 모두 가질 경우
  else {
    rbtree *tmp = NULL;
    tmp->root = p->right;
    tmp->nil = t->nil;
    //오른쪽 서브트리에서 가장 작은 수 반환
    //successor 를 찾는다.
    y = rbtree_min(tmp);
    y_original_color = y->color;
    x = y->right;
    if (y == p->right) {
      x->parent = y;
    }
    else {
      rb_transplant(t, y, y->right);
      y->right = p->right;
      y->right->parent = y;
    }
    rb_transplant(t, p, y);
    y->left = p->left;
    y->left->parent = y;
    y->color = p->color;
  }

  //RB-DELETE-Fixup------------------------------
  if (y_original_color == RBTREE_BLACK) {
    node_t *w = NULL;
    while (x != t->root && x->color == RBTREE_BLACK){
      if(x == x->parent->left) {
        w = x->parent->right;
        if(w->color == RBTREE_RED) {
          w->color = RBTREE_BLACK;
          x->parent->color = RBTREE_RED;
          left_rotate(t, x->parent);
          w = x->parent->right;
        }
        if(w->left->color == RBTREE_BLACK && w->right->color == RBTREE_BLACK) {
          w->color = RBTREE_RED;
          x = x->parent;
        }
        else {
          if (w->right->color == RBTREE_BLACK) {
            w->left->color = RBTREE_BLACK;
            w->color = RBTREE_RED;
            right_rotate(t, w);
            w = x->parent->right;
          }
          w->color = x->parent->color;
          x->parent->color = RBTREE_BLACK;
          w->right->color = RBTREE_BLACK;
          left_rotate(t, x->parent);
          x = t->root;
        }
      }
      else {
        w = x->parent->left;
        if(w->color == RBTREE_RED) {
          w->color = RBTREE_BLACK;
          x->parent->color = RBTREE_RED;
          right_rotate(t, x->parent);
          w = x->parent->left;
        }
        if(w->right->color == RBTREE_BLACK && w->left->color == RBTREE_BLACK) {
          w->color = RBTREE_RED;
          x = x->parent;
        }
        else {
          if (w->left->color == RBTREE_BLACK) {
            w->right->color = RBTREE_BLACK;
            w->color = RBTREE_RED;
            left_rotate(t, w);
            w = x->parent->left;
          }
          w->color = x->parent->color;
          x->parent->color = RBTREE_BLACK;
          w->left->color = RBTREE_BLACK;
          right_rotate(t, x->parent);
          x = t->root;
        }
      }
    }
    x->color = RBTREE_BLACK;
  }
  free(p);
  p = NULL;
  return 0;
}

//u : 삭제할 노드, v : u의 자식 노드 - u의 부모 노드와 연결되어야 한다.
void rb_transplant(rbtree *t, node_t *u, node_t *v) {
  if (u->parent == t->nil) {
    t->root = v;
  }
  else if (u == u->parent->left) {
    u->parent->left = v;
  }
  else {
    u->parent->right = v;
  }
  v->parent = u->parent;
}

//중위 순회
int inorder(node_t *p, key_t *arr, const rbtree *t, int i, size_t n) {
    if (p == t->nil) {
      return i;
    }
    i = inorder(p->left, arr, t, i, n);
    if (i < n) {
      arr[i] = p->key;
      i++;
    }
    i = inorder(p->right, arr, t, i, n);
    return i;
}

int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n) {
  // TODO: implement to_array
  node_t *p = t->root;
  inorder(p, arr, t, 0, n);
  return 0;
}