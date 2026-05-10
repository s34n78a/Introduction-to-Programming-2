#include "function_present.h"
#include <iostream>
using namespace std;

KuoYangPresent::KuoYangPresent(int k){
    head = mid = tail = nullptr;
    this->sz = 0;
    this->k = k;
    this->now = 0;
    this->reverse = false;
}
void KuoYangPresent::Push(int x){
    Node *newNode = new Node();
  newNode->val = x;
  newNode->next = newNode->prev = nullptr;
  newNode->tag = now;
  
  sz++;
  if (sz == 1){
    head = mid = tail = newNode;
    return;
  }
  if(!reverse){
    tail->next = newNode;
    newNode->prev = tail;
    tail = newNode;
    if(sz % 2 != 0) mid = mid->next;
  }else{
    newNode->next = head;
    head->prev = newNode;
    head = newNode;
    if(sz % 2 != 0) mid = mid->prev;
  }
}

void KuoYangPresent::Pop() {
  if(sz == 0) return;
  Node *target = mid;
  Node *p = target->prev;
  Node *n = target->next;
  if(sz == 1){
    head = mid = tail = nullptr;
  }
  else{
    if(!reverse){
      mid = target->prev;
    }else{
      mid = target->next;
    }

    if(p) p->next = n; //for reverse case
    else head = n;

    if(n) n->prev = p;
    else tail = p;
  }
  delete target;
  sz--;
}
void KuoYangPresent::Reverse() {
    reverse = !reverse;
    if(sz % 2 == 0){
        if(reverse)
            mid = mid->next;
        else
            mid = mid->prev;
    }
}

void KuoYangPresent::ProgrammingTanoshi(){
  if(sz > 0) now++;
}
void KuoYangPresent::KuoYangTeTe() {
 Node* curr = head;
  while(curr != nullptr){
    if(curr->tag < now){
      curr->val %= k;
      curr->tag = now;
    }
    curr = curr->next;
  }
}
void KuoYangPresent::PrintList() {
  if(sz == 0) return;
  
  if(!reverse){
    Node* curr = head;
    while(curr != nullptr){
      cout << curr->val << " ";
      curr = curr->next;
    }
  }else{
    Node* curr = tail;
    while(curr != nullptr){
      cout << curr->val << " ";
      curr = curr->prev;
    }
  }
  cout << endl;
}