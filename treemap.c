#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "treemap.h"

typedef struct TreeNode TreeNode;


struct TreeNode {
    Pair* pair;
    TreeNode * left;
    TreeNode * right;
    TreeNode * parent;
};

struct TreeMap {
    TreeNode * root;
    TreeNode * current;
    int (*lower_than) (void* key1, void* key2);
};

int is_equal(TreeMap* tree, void* key1, void* key2){
    if(tree->lower_than(key1,key2)==0 &&  
        tree->lower_than(key2,key1)==0) return 1;
    else return 0;
}


TreeNode * createTreeNode(void* key, void * value) {
    TreeNode * new = (TreeNode *)malloc(sizeof(TreeNode));
    if (new == NULL) return NULL;
    new->pair = (Pair *)malloc(sizeof(Pair));
    new->pair->key = key;
    new->pair->value = value;
    new->parent = new->left = new->right = NULL;
    return new;
}

TreeMap * createTreeMap(int (*lower_than) (void* key1, void* key2)) {
  TreeMap * new = (TreeMap *)malloc(sizeof(TreeMap));
  if (new == NULL) return NULL;

  new->lower_than = lower_than;
  return new;

}


void insertTreeMap(TreeMap * tree, void* key, void * value) {
  TreeNode *new = createTreeNode(key, value);
  TreeNode *aux = tree->current;
  aux=tree->root;
  while(aux != NULL){
    if(tree->lower_than(key, aux->pair->key)==1){
      if(aux->left ==NULL){
        aux->left=new;
        new->parent=aux;
        }
      aux=aux->left;
      tree->current=aux;
    } 
    else if(tree->lower_than(aux->pair->key, key)==1){
      if(aux->right ==NULL){
        aux->right=new;
        new->parent=aux;
      } 
      aux=aux->right;
      tree->current=aux;
    }  
    else 
      return;
  }
  tree->current=new;
  
}

TreeNode * minimum(TreeNode * x){
  while(x->left != NULL){
    x=x->left;
  }
  return x;
}


void removeNode(TreeMap * tree, TreeNode* node) {
  //0 hijos
  if(node->left == NULL && node->right == NULL){
    if(node->parent==NULL){
      tree->root=NULL;
    }  
    if(node->parent->left==node){
      node->parent->left=NULL;
    }
    if(node->parent->right==node){
      node->parent->right=NULL;
    }  
    
  }
  //2 hijos  
  else if(node->right != NULL && node->left != NULL){
    TreeNode *aux=minimum(node->right);
    node->pair=aux->pair;
    removeNode(tree, aux);  
  }
  // 1 hijo  
  else{
    //izquierdo
    if(node->parent->left==node){
      if(node->right==NULL){
        node->parent->left=node->left;
        node->left->parent=node->parent;
      }
      if(node->left==NULL){
        node->parent->left=node->right;
        node->right->parent=node->parent;
      }
    }
    //derecho
    if(node->parent->right==node){
      if(node->right==NULL){
        node->parent->right=node->left;
        node->left->parent=node->parent;
      }
      if(node->left==NULL){
        node->parent->right=node->right;
        node->right->parent=node->parent;
      }
    }
    
    free(node);
  }
}

void eraseTreeMap(TreeMap * tree, void* key){
    if (tree == NULL || tree->root == NULL) return;

    if (searchTreeMap(tree, key) == NULL) return;
    TreeNode* node = tree->current;
    removeNode(tree, node);

}




Pair * searchTreeMap(TreeMap * tree, void* key) {
  TreeNode *aux = tree->root;
  while(aux != NULL){
    if(is_equal(tree, aux->pair->key, key)){
      tree->current = aux;
      return aux->pair;
    }
    if(tree->lower_than(aux->pair->key, key)){
      aux = aux->right;  
    }
    else{
      aux=aux->left;
    }
  }
  
  return NULL;
}


Pair * upperBound(TreeMap * tree, void* key) {
  TreeNode * ub_node=NULL;
  tree->current=tree->root;
  while(tree->current != NULL){
    if(tree->lower_than(key, tree->current->pair->key)==1){
      ub_node=tree->current;
      tree->current=tree->current->left;
      
      
    }
    else if(tree->lower_than(tree->current->pair->key, key)==1){
      tree->current=tree->current->right;  
    }
    else{
      ub_node=tree->current;
      break;
    }
  }
  if(ub_node == NULL){
    return NULL;
  } 
  return ub_node->pair;
}

Pair * firstTreeMap(TreeMap * tree) {
  TreeNode *aux=minimum(tree->root);
  return aux->pair;
}

Pair * nextTreeMap(TreeMap * tree) {
  TreeNode *aux=NULL;
  aux=tree->current->parent;
  if(tree->current->right!=NULL){
    aux=minimum(tree->current->right);
    return tree->current->pair;
  }
  while(aux!=NULL){
    if(tree->lower_than(tree->current->pair->key, aux->pair->key)==1){
      aux=aux->parent;
      tree->current=aux;
    }
    else{
      return aux->pair;
    }
   
  }
  return aux->pair;
}
//ya no me dio :( me rindo
