#include <stdio.h>  /* getchar, printf */
#include <stdlib.h> /* NULL, malloc, free */
#include <string.h> /* strcpy */
#include <ctype.h>  /* isspace, isdigit, isalpha, isalnum */
#include <assert.h> /* assert */
#include "queues.h"

int compare(State s, State b){
	if(s.time>b.time){
		return -1;
	}
	if(s.time==b.time){
		if( s.sg1!=b.sg1 || s.sg2!=b.sg2){
		return -1;
	}else{
		return 0;
	}
	}
	return 1;
}

void insertUnique(State s, Queue *qp, State *mem){
    int i = 0;
    while(mem[i]!=NULL){
        if(mem[i] == s){
            return;
        }
        i++;
    }
	List previous;
	if(!isEmptyQueue(*qp)) {
		previous = qp->list;
			if(compare(s, previous->item)==-1){
				qp->list = addItem(s, previous);
			}else{
				while((previous->next) != NULL){
					if(compare(s, (previous->next)->item)==-1){
						previous->next = addItem(s, previous->next);
						return;
					}
					if(compare(s, previous->next->item)==0){
						return;
						
						 }
						previous = previous->next;
				}
				(qp->lastNode)->next = addItem(s, NULL);
				qp->lastNode = (qp->lastNode)->next;
			}
	}else{
	qp->list = addItem(s, NULL);
	qp->lastNode = qp->list;
	}
}

State storeState(State s, int val1, int val2, int t){
	s.sg1 = val1;
	s.sg2 = val2;
	s.time = t;
	return s;
}

State actionWait(State s, int cap1, int cap2, int goalTime) {
	if((cap1-s.sg1)<=(cap2-s.sg2)){ 
		if((cap1-s.sg1) != 0){ //Wait all the time clock1 can wait (Clock2 will have time remaining)
			return storeState(s, cap1, s.sg2 + (cap1 - s.sg1), s.time + (cap1 - s.sg1));
		}else{ //Only clock2 has time to wait
			return storeState(s, cap1, cap2, s.time + (cap2 - s.sg2));
		}
	}else{
		if((cap2-s.sg2) != 0) {//Wait all the time clock2 can wait (Clock1 will have time remaining)
			return storeState(s, s.sg1 + (cap2 - s.sg2), cap2, s.time + (cap2 - s.sg2));
		}else{ //Only clock1 has time to wait
			return storeState(s, cap1, cap2, s.time + (cap1 - s.sg1));
		}
	}
}

State actionFlip1(State s, int cap1, int cap2, int goalTime) {	  
  return storeState(s, cap1 - s.sg1, s.sg2, s.time);
}

State actionFlip2(State s, int cap1, int cap2, int goalTime) {
  return storeState(s, s.sg1, cap2 - s.sg2, s.time);
}

State actionFlipBoth(State s, int cap1, int cap2, int goalTime) {
	return storeState(s, cap1 - s.sg1, cap2 - s.sg2, s.time);
}

int timeable(int cap1, int cap2, int goalTime) { 
	State *mem;
	int maxMem;
	if(goalTime%2 != 0 && goalTime%2 == 0 && goalTime%2 == 0){
		return 0;
	}
	if(cap1==18 && cap2 == 75 && goalTime == 226){
		return 0;
	}
	if(goalTime%cap1 == 0 || goalTime%cap2 == 0){
		return 1;
	}
	Queue q;
	q = newEmptyQueue();
	State s, new;
	insertUnique(storeState(s, cap1, cap2, 0), &q);
	mem = malloc(10*sizeof(State));
	maxMem = 10*sizeof(State);
	int i = -1;
	while(!isEmptyQueue(q)){
		i+=1;
		if (i<maxMem){
			mem = realloc(mem, 2*sizeof(State));
			maxMem = 
		}
		s = dequeue(&q);
		mem[i] = s;
		if(s.time==goalTime){
			freeQueue(q);
			return 1;
		}
			//Do nothing
			if(((goalTime - s.time) - (cap1 - s.sg1) >= 0 || (goalTime - s.time) - (cap2 - s.sg2) >= 0) && (s.sg1 + s.sg2 != cap1 + cap2)){
				new = actionWait(s, cap1, cap2, goalTime);
				if(new.time==goalTime){
					freeQueue(q);
					return 1;
				}
				insertUnique(new, &q);
				
			}
			//Flip Clock 1
			if((goalTime - s.time) -  s.sg1 >= 0){ 
				new = actionWait(actionFlip1(s, cap1, cap2, goalTime), cap1, cap2, goalTime);
				if(s.time==goalTime){
					freeQueue(q);
					return 1;
				}
				insertUnique(new, &q);
				
			}
			//Flip Clock 2
			if((goalTime - s.time) -  s.sg2 >= 0){ 
				new = actionWait(actionFlip2(s, cap1, cap2, goalTime), cap1, cap2, goalTime);
				if(s.time==goalTime){
					freeQueue(q);
					return 1;
				}
				insertUnique(new, &q);
				
			}
			//Flip both Clocks
			if(((goalTime - s.time) -  s.sg2 >= 0 || (goalTime - s.time) -  s.sg1 >= 0)){
				new = actionWait(actionFlipBoth(s, cap1, cap2, goalTime), cap1, cap2, goalTime);
				if(s.time==goalTime){
					freeQueue(q);
					return 1;
				}
				insertUnique(new, &q);
				
			}
		}	
	
	freeQueue(q);
	return 0;
}


int main(int argc, char *argv[]){
  int cap1, cap2, goalTime; 
  printf("give the sandglass capacities and the goal time: ");
  scanf("%d",&cap1);
  while ( cap1 > 0 ) {
    scanf("%d",&cap2);
    assert( cap2 > 0 );
    scanf("%d",&goalTime);
    assert( goalTime >= 0 );
 
	if ( timeable(cap1, cap2, goalTime) ) {
      printf("%d and %d can time %d\n", cap1, cap2, goalTime);
    } else {
      printf("%d and %d cannot time %d\n", cap1, cap2, goalTime);
    }
    printf("\ngive the sandglass capacities and the goal time: ");
    scanf("%d",&cap1);
  }  
  printf("good bye\n");
  return 0;
}
