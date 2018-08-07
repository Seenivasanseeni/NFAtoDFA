#include<stdio.h>
#include<string.h>
#define MAXSIZE 200

struct transition{
	int is;
	int fs;
	char input;
	struct transition * next;
};

struct transition *fun;
struct transition * temp;

void append(struct  transition * t){
	if(fun==NULL){
		fun=t;
		return;
	}
	temp=fun;
	while(temp->next!=NULL){
		temp=temp->next;
	}
	temp->next=t;

}

void makeTransitionFunction(){
	fun=NULL;
	int con;
	printf("\nEnter Whether to add entry to the transition function (1 for Y / 0 for N):");
	scanf("%d",&con);
	while(con==1){
		printf("\nEnter initial_state input nex_state:");
		temp=(struct transition *)malloc(sizeof(struct transition));
		scanf("%d %c %d",&(temp->is),&(temp->input),&(temp->fs));
		temp->next=NULL;
		append(temp);
		printf("\nEnter Whether to add entry to the transition function (1 for Y / 0 for N):");
		scanf("%d",&con);
	}
}

int fs[100];
int fsn;

void trans(int state,char c){
	temp=fun;
	fsn=0;
	for(int i=0;i<100;i++)
		fs[i]=-1;
	int index=0;
	while(temp!=NULL){
		if((temp->is==state && temp->input==c) || (temp->is==state && c=='0')){
			fs[index++]=temp->fs;
		}
		temp=temp->next;
	}
	fsn=index;
}


void printTransition(){
	printf("\nTransition Function===========================================");
	temp=fun;
	while(temp!=NULL){
		printf("\n%d %c -> %d",temp->is,temp->input,temp->fs);
		temp=temp->next;
	}
	printf("\n==============================================================");
}



struct nfa{
	char symbols[3];
	int num_states;
	int states[100];
	int is;
	int fs;
};

struct nfa nfa;


void getInput(){
	strcpy(nfa.symbols,"abe");
	printf("\nEnter no of states inital state and final state:");
	scanf("%d",&(nfa.num_states));
	scanf("%d %d",&(nfa.is),&(nfa.fs));
	printf("\nAll the states are " );
	for(int i=1;i<=nfa.num_states;i++)
		printf("%d ",i);
	}

typedef struct epclo{
	int closure[100];
	int n;
}epclo;


void printEpsilonClosure(epclo e){
	printf("\n");
	for(int i=0;i<e.n;i++)
		printf("%d ",e.closure[i]);
	printf("\n");
}


epclo addState(epclo e,int state){
	e.closure[e.n++]=state;
	return e;
}

int tinclosure(epclo e, int state){
	for(int i=0;i<e.n;i++)
		if(e.closure[i]==state)
			return 1;
	return 0;
}

epclo sort(epclo e){
    for(int i=0;i<e.n;i++){
        int currentmin=e.closure[i];
        int currentminIndex=i;
        for(int j=i+1;j<e.n;j++){
            if(e.closure[j]<currentmin) {
                currentmin = e.closure[j];
                currentminIndex= j;
            }
        }
        e.closure[currentminIndex]=e.closure[i];
        e.closure[i]=currentmin;
    }
    return e;
}

epclo epsilonClosure(int * T,int n){
	int stack[100];
	int top=-1;
	epclo e;
	e.n=0;
	for(int i=0;i<n;i++){
		stack[++top]=T[i];
		e=addState(e,T[i]);
	}
	while(top>-1){
		int s=stack[top--];
		trans(s,'e');
		for(int i=0;i<fsn;i++){
			int t=fs[i];
			if(! tinclosure(e,t)){
				e=addState(e,t);
				stack[++top]=t;
			}
		}

	}
	e=sort(e);
	return e;
}



typedef struct state{
	int n;
	int states[100];
	struct state * next;
	int mark;
}state;

state * statesHead=NULL;
state * stemp;


void printArray(int * states,int sn) {
    printf("{");
    for (int i = 0; i < sn; i++)
        printf("%d, ", states[i]);
    printf("}");
}

//possoble bug when states in an entry are duplicated
//removed by returing sorted states

void addStateDFA(int *s,int sn){
	stemp=(state *)malloc(sizeof(state));
	for(int i=0;i<sn;i++){
		stemp->states[i]=s[i];
	}
	stemp->n=sn;
	stemp->mark=0;

	stemp->next=NULL;
	if(statesHead==NULL){
		statesHead=stemp;
	}
	else{
		state * t=statesHead;
		while(t->next!=NULL)
			t=t->next;
		t->next=stemp;
	}
	return;
}

void printDFAStates(){
    stemp=statesHead;
    while (stemp!=NULL){
        printf("\nState=================\n");
        printArray(stemp->states,stemp->n);
        printf("\n");
        stemp=stemp->next;
    }
}


int unmarkedState(){
	stemp=statesHead;
	while (stemp!=NULL) {
		if(stemp->mark==0)
			return 1; //unmarkedState
		stemp=stemp->next;
	}
	return 0;
}

int initialState(){
    return  nfa.is;
}

void getUnmarkedState(){
    stemp=statesHead;
    fsn=-1;
    while (stemp!=NULL){
        if(stemp->mark==0){
            stemp->mark=1;
            for(int i=0;i<stemp->n;i++){
                fs[i]=stemp->states[i];
            }
            fsn=stemp->n;
            return;
        }
        stemp=stemp->next;
    }
}

int equal(epclo e, int * states,int sn){
    if(e.n!=sn)
        return 0;
    int matchCount=0;
    for(int i=0;i<e.n;i++){
        if(e.closure[i]!=states[i])
            return 0;
    }
    return 1;
}

int isThereSameState(epclo e){
    stemp=statesHead;
    while (stemp!=NULL){
        if(equal(e,stemp->states,stemp->n))
            return 1;
        stemp=stemp->next;
    }
    return 0;

}


void printDFATransition(int * T,int Tn,char c,int * nextStates,int nsn){
    printf("\nTRANSITION\n");
    printArray(T,Tn);
    printf(" ==%c==> ",c);
    printArray(nextStates,nsn);
    printf("\n");
}


void NFAtoDFA() {
    int x = initialState();
    //find epsilon clousre
    int T[100] = {x};
    int Tn = 1;
    epclo e = epsilonClosure(T, Tn);
    //printf("Epsilon closure for initial states");
    //printEpsilonClosure(e);

    for (int i = 0; i < e.n; i++)
        T[i] = e.closure[i];
    addStateDFA(T, e.n);
    //printf("DFA Transition\n");
    //printDFAStates();
    while (unmarkedState()) {
        getUnmarkedState();
        //printf("Unmarked State \n");
        for (int i = 0; i < fsn; i++) {
            T[i] = fs[i];
           // printf("%d ", T[i]);
        }
        //printf("\n");
        Tn = fsn;
        int nextStates[200];
        int nsn = 0;
        for (char c = 'a'; c <= 'b'; c++) {
            nsn = 0;
            for (int i = 0; i < Tn; i++) {
                trans(T[i], c);
                for (int k = 0; k < fsn; k++) {
                    nextStates[nsn++] = fs[k];
                }
                //printArray(nextStates, nsn);
              //  printf("\n");
            }
      //      printArray(T, Tn);
            //printf(" On %c transitions into \n", c);
            //printArray(nextStates, nsn);
            //printf("\n");
            epclo e = epsilonClosure(nextStates, nsn);

            if (e.n == 0) {
                //printf("Empty Transition\n");
                continue;
            }
            int res = isThereSameState(e);
            if (res == 0) {
               // printf("No entry\n");
                printDFATransition(T, Tn, c, e.closure, e.n);
                printf("\n");
                addStateDFA(e.closure, e.n);
            } else {
            //    printf("Avoiding Entry\n");
            }
        }
    }

}

void main(){
	getInput();
	makeTransitionFunction();
	printTransition();
    NFAtoDFA();
    printf("\n");
}
