#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <stdint.h>

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
//pthread_cond_t ingreso = PTHREAD_COND_INITIALIZER;
//pthread_cond_t creo = PTHREAD_COND_INITIALIZER;

typedef struct node{
  int data;
  struct node* next;
} node;

void print_list(node* head)
{
    node* cursor = head;
    while(cursor != NULL)
    {
        printf("%d ", cursor->data);
        cursor = cursor->next;
    }
}

node* create(int data, node* next)
{
    node* new_node = (node*)malloc(sizeof(node));
    if(new_node == NULL)
    {
        printf("Error creating a new node.\n");
        exit(0);
    }
    new_node->data = data;
    new_node->next = next;

    return new_node;
}

node* append(node* head, int data)
{
    /* go to the last node */
    node *cursor = head;
    while(cursor->next != NULL)
        cursor = cursor->next;
 
    /* create a new node */
    node* new_node =  create(data,NULL);
    cursor->next = new_node;
    
    return head;
}

node* remove_front(node* head)
{
    if(head == NULL)
        return NULL;
    node *front = head;
    head = head->next;
    front->next = NULL;
    /* is this the last node in the list */
    if(front == head)
        head = NULL;
    free(front);
    
    return head;
}

node* remove_back(node* head)
{
    if(head == NULL)
        return NULL;
 
    node *cursor = head;
    node *back = NULL;
    while(cursor->next != NULL)
    {
        back = cursor;
        cursor = cursor->next;
    }
    if(back != NULL)
        back->next = NULL;
 
    /* if this is the last node in the list*/
    if(cursor == head)
        head = NULL;
 
    free(cursor);
    
    return head;
}

//ref:https://stackoverflow.com/questions/39815094/linked-list-in-thread
//https://repl.it/@gfhuertac/lock-producerconsumer


#define MAX 50
node* head = NULL;

void* list(void *arg){
    int loops = (int) arg;
    //p1:MAX p2:2*MAX

    //printf("\nNumero: %i\n", loops);

    for(int i = 0; i < loops; i++){
      //se bloquea al agregar o crear una lista
    	pthread_mutex_lock(&lock);
      if (head == NULL){
        head = create(0,head);
        //printf("\nHe creado una lista!\n");
      }
      else{
        append(head,i);
        //printf("\nHe creado un nodo %i!\n", i);
      }
      pthread_mutex_unlock(&lock);
      //luego se desbloquea 
    }
  }
void* list2(void *arg){
    int loops = (int) arg;
    //p1:MAX p2:2*MAX
    // printf("\nNumero: %i\n", loops);

    for(int i = MAX; i < loops; i++){
      //se bloquea al agregar o crear una lista
    	pthread_mutex_lock(&lock);
      if (head == NULL){
        head = create(0,head);
        //printf("\nHe creado una lista!\n");
      }
      else{
        append(head,i);
        //printf("\nHe creado un nodo %i!\n", i);
        
      }
      pthread_mutex_unlock(&lock);
      //luego se desbloquea 
    }
  }

int main(void) {
  // inserte código para agregar 100 nodos
  
  //primero secuencialmente 
  /*
  for (int i = 100; i >= 1; i--){
    head = create(i, head);
  }
  */
	
  // luego en paralelo
  //https://repl.it/@gfhuertac/lock-producerconsumer
  pthread_t p1; 
  pthread_t p2;

  pthread_create(&p1, NULL, list, (void*)MAX);
  pthread_create(&p2, NULL, list2, (void*)(MAX*2));

  pthread_join(p1, NULL);
  pthread_join(p2, NULL);

  print_list(head);
  printf("\nListo!\n");
  return 0;
}

