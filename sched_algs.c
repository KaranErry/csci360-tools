/*
	(c) Karan Erry, 2017.
*/

#include <stdio.h>

void executeOneSecond(int process, int * queue, int * arrive, int * runTime, int * respTime, int * waitTime, int * sysclock);
void printCalculations(int * respTime, int * waitTime);
void contextSwitch(int conswitch);
int processComplete(int process, int * execute, int * runTime);
void initializeQueue(int * queue, int * arrive);
int pop(int * queue);
void push(int * queue, int num);
int corrProcess(int * array, int num);
int arrayContains(int * array, int num);
int queueEmpty(int * queue);
void initializeToZero(int * array);

int main () {
	int alg, quant, conswitch, i, arrive[5], execute[5], queue[5], runTime[5], respTime[5], waitTime[5], currproc;
	int hello = 0;
	int * sysClock = & hello;
	// Initalize all arrays to zero:
	initializeToZero(arrive);
	initializeToZero(execute);
	initializeToZero(queue);
	initializeToZero(runTime);
	initializeToZero(respTime);
	initializeToZero(waitTime);
	// Input:
	printf("Simplified program with 5 processes. Constraints:\n\t1. Do not have multiple process arrive at the exact same time,\n\t2. Make sure the CPU does not have to idle once started (i.e., at least one process must be waiting before one completes.)\n\tPROGRAM MAY BREAK IF CONSTRAINTS AREN'T ADHERED TO.\n\n");
	printf("Process 1 arrival time:\t\t");
	scanf("%d", &arrive[0]);
	printf("Process 1 execution time:\t");
	scanf("%d", &execute[0]);
	printf("Process 2 arrival time:\t\t");
	scanf("%d", &arrive[1]);
	printf("Process 2 execution time:\t");
	scanf("%d", &execute[1]);
	printf("Process 3 arrival time:\t\t");
	scanf("%d", &arrive[2]);
	printf("Process 3 execution time:\t");
	scanf("%d", &execute[2]);
	printf("Process 4 arrival time:\t\t");
	scanf("%d", &arrive[3]);
	printf("Process 4 execution time:\t");
	scanf("%d", &execute[3]);
	printf("Process 5 arrival time:\t\t");
	scanf("%d", &arrive[4]);
	printf("Process 5 execution time:\t");
	scanf("%d", &execute[4]);

	printf("\n  #\tArrival\tExecution\n");
	printf("  1\t%d\t%d\n", arrive[0], execute[0]);
	printf("  2\t%d\t%d\n", arrive[1], execute[1]);
	printf("  3\t%d\t%d\n", arrive[2], execute[2]);
	printf("  4\t%d\t%d\n", arrive[3], execute[3]);
	printf("  5\t%d\t%d\n\n", arrive[4], execute[4]);

	printf("Enter context switch time...  ");
	scanf("%d", &conswitch);

	printf("\nChoose scheduling algorithm...\n\n 1| FCFS\n 2| RR\n 3| NPSJF\n 4| SRTN\n\n");
	scanf("%d", &alg);

	initializeQueue(queue, arrive);

	switch (alg) {
		case 1:
			// FCFS -- strictly queue-based
			printf("\n\nCPU Execution Timeline for FCFS:\n\n");
			while (!queueEmpty(queue)) {
				currproc = pop(queue);
				while (!processComplete(currproc, execute, runTime))
					executeOneSecond(currproc, queue, arrive, runTime, respTime, waitTime, sysClock);
				if (!queueEmpty(queue))
					contextSwitch(conswitch);
				// Don't push the process back because it's complete
			}
			printf("\n\n");
			printCalculations(respTime, waitTime);
		break;
		case 2:
			// RR -- strictly queue-based
			printf("Enter RR quantum time...  ");
			scanf("%d", &quant);

			printf("\n\nCPU Execution Timeline for RR%d:\n\n", quant);
			while (!queueEmpty(queue)) {
				currproc = pop(queue);
				i = 1;
				while (i <= quant && !processComplete(currproc, execute, runTime)) {
					executeOneSecond(currproc, queue, arrive, runTime, respTime, waitTime, sysClock);
					i++;
				}
				if (!queueEmpty(queue))
					contextSwitch(conswitch);
				if (!processComplete(currproc, execute, runTime))
					push(queue, currproc);
			}
			printf("\n\n");
			printCalculations(respTime, waitTime);
		break;
		case 3:
			// NPSJF -- queue is filtered for conditions
		break;
		case 4:
			// SRTN -- queue is filtered for conditions
		break;
		default:
			printf("Invalid entry.");
		break;
	}



	return 1;
} 

void executeOneSecond(int process, int * queue, int * arrive, int * runTime, int * respTime, int * waitTime, int * sysClock) {
	/*
	1. 	Executes specified process for one second by:
			a. Printing the process number to the schedule table on the screen;
			b. Incrementing the system clock;
			c. Incrementing the runTime of the process;
			d. Incrementing the waitTime of the other processes.
	2. 	Pushes newly-arrived processes onto the queue.
	*/
	printf("%d ", process);
	runTime[process-1]++;
	(*sysClock)++;
	int i = 0, currElem;
	while (i < 5 && queue[i] != 0) {
		currElem = queue[i];
		if (runTime[currElem-1] == 0)	// If the process has not begun executing...
			respTime[currElem-1]++;	// ...then increment response time
		waitTime[currElem-1]++;	// Increment wait time
		i++;
	}
	if (arrayContains(arrive, *sysClock))
		push(queue, corrProcess(arrive, *sysClock));
}

void printCalculations(int * respTime, int * waitTime) {
	printf("Calculations:\n\n");
	printf("\t#\tResponse Time\tWait Time\n");
	printf("\t1\t%d\t\t%d\n", respTime[0], waitTime[0]);
	printf("\t2\t%d\t\t%d\n", respTime[1], waitTime[1]);
	printf("\t3\t%d\t\t%d\n", respTime[2], waitTime[2]);
	printf("\t4\t%d\t\t%d\n", respTime[3], waitTime[3]);
	printf("\t5\t%d\t\t%d\n", respTime[4], waitTime[4]);
	printf("-----------------------------------------\n");
	int totalRespTime = (respTime[0]+respTime[1]+respTime[2]+respTime[3]+respTime[4]);
	int totalWaitTime = (waitTime[0]+waitTime[1]+waitTime[2]+waitTime[3]+waitTime[4]);
	printf("   Totals\t%d\t\t%d\n", totalRespTime, totalWaitTime);
	printf("-----------------------------------------\n");
	float avgRespTime = totalRespTime / 5.0;
	float avgWaitTime = totalWaitTime / 5.0;
	printf(" Averages\t%.2f\t\t%.2f\n\n", avgRespTime, avgWaitTime);
}

void contextSwitch(int conswitch) {
	// Performs a context switch by printing "X" to the
	// schedule table on the screen.
	int i;
	for (i = 1; i <= conswitch; i++)
		printf(" X ");
	printf(" ");
}

int processComplete(int process, int * execute, int * runTime) {
	return execute[process-1] == runTime[process-1];
}

int getNextProcess(int conditionCode, int * arrive) {
	// Condition codes:
	//		-1 = FCFS
	//		-2 = NPSJF
	//		-3 = SRTN
	//		>0 = RR with <condition-code> quant time

}

void initializeQueue(int * queue, int * arrive) {
	// Initializes queue with the first process to arrive
	int i, smallest = -1;
	for (i = 0; i < 5; i++) {
		if (smallest == -1 || arrive[i] < smallest)
			smallest = arrive[i];
	}
	push(queue, corrProcess(arrive, smallest));
}

int pop(int * queue) {
	// Pop queue and return num
	int popped = queue[0], i;
	for (i = 1; i < 5; i++)
		queue[i-1] = queue[i];
	queue[4] = 0;
	return popped;
}

void push(int * queue, int num) {
	// Push num onto queue
	int i;
	for (i = 0; i < 5; i++) {
		if (queue[i] == 0) {
			queue[i] = num;
			break;
		}
	}
}

int corrProcess(int * array, int num) {
	// Returns which process corresponds to element 'num' in array 'array'
	// (essentially index-of, plus 1)
	int i;
	for (i = 0; i < 5; i++){
		if (array[i] == num)
			return (i+1);
	}
}

int arrayContains(int * array, int num) {
	return array[0]==num || array[1]==num || array[2]==num || array[3]==num || array[4]==num;
}

int queueEmpty(int * queue) {
	return queue[0]==0 && queue[1]==0 && queue[2]==0 && queue[3]==0 && queue[4]==0;
}

void initializeToZero(int * array) {
	int i;
	for (i = 0; i < 5; i++)
		array[i] = 0;
}