#include <stdio.h>

/*
Author: Anurag Dubey
Class: Advanced Embedded Architecture
Topic: An Example priority scheduler for 3 tasks
*/

#define RUNNING 2
#define BLOCKED 1
#define WAITING 0
#define ANY -1

#define TIMER_INIT 1
#define TIMER_EXPIRE 0

#define EV_TIMEOUT 11
#define EV_BLOCKED 12
#define EV_RESOURCE_AVAILABLE 13

#define TIMER_X 21
#define TIMER_Y 22
#define TIMER_Z 23

#define RESOURCE_A 31
#define RESOURCE_B 32
#define RESOURCE_C 33

#define PR_HIGH 100
#define PR_MED 200
#define PR_LOW 300

typedef struct {
    int id;
    int state;
    int event;
    int timer;
    int resource;
    int priority;
} task;

task task1 = {
    .id = 1,
    .state = ANY,
    .event = ANY,
    .timer = TIMER_X,
    .resource = RESOURCE_A,
    .priority = PR_HIGH
};
task task2 = {
    .id = 2,
    .state = ANY,
    .event = ANY,
    .timer = TIMER_Y,
    .resource = RESOURCE_B,
    .priority = PR_MED
};
task task3 = {
    .id = 3,
    .state = ANY,
    .event = ANY,
    .timer = TIMER_Z,
    .resource = RESOURCE_C,
    .priority = PR_LOW
};

task default_task = {
    .id = 0,
    .state = ANY,
    .event = ANY,
    .timer = ANY,
    .resource = ANY,
    .priority = ANY
};

// Scheduler is called whenever any event happens related to currently running task 
// or if the timer runs out

task* scheduler(task *running) {
 if (running->event == EV_TIMEOUT) 
 {
     running->state = WAITING;
 }
  else
      if (running->event == EV_BLOCKED)
      {
         running->state = BLOCKED;
      }
      else
          if (running->event == ANY)
          {
              printf("\n No event occured..");
              return running;
          }
  // Find next higherst priority waiting task
  // We could use priority queue here but for the sake of simplicity
  // we can hardcode the priorities
  if (task1.state == WAITING || task1.state == BLOCKED && task1.event == EV_RESOURCE_AVAILABLE) 
  {
       task1.state = RUNNING;
       task1.event = ANY;
       return &task1;
  }
  else

  if (task2.state == WAITING || task2.state == BLOCKED && task2.event == EV_RESOURCE_AVAILABLE) 
  {
       task2.state = RUNNING;
       task2.event = ANY;
       return &task2;
  }
  else

  if (task3.state == WAITING || task3.state == BLOCKED && task3.event == EV_RESOURCE_AVAILABLE) 
  {
       task3.state = RUNNING;
       task3.event = ANY;
       return &task3;
  }
  else
      return &default_task;
}

void getstate(task pr){
    if (pr.state == RUNNING) printf("\nState: RUNNING");
    if (pr.state == BLOCKED) printf("\nState: BLOCKED");
    if (pr.state == WAITING) printf("\nState: WAITING");
}
void getevent(task pr){
    if (pr.event == EV_TIMEOUT) printf("\nEvent: TIMEOUT");
    if (pr.event == EV_BLOCKED) printf("\nEvent: BLOCKED");
    if (pr.event == EV_RESOURCE_AVAILABLE) printf("\nEvent: RESOURCE_AVAILABLE");
}

void printstate(){
    printf("\nCurrent State: ");
    printf("\ntask1:");
    getstate(task1);
    getevent(task1);
    printf("\ntask2:");
    getstate(task2);
    getevent(task2);
    printf("\ntask3:");
    getstate(task3);
    getevent(task3);
}

int main(){
    // Initialize all tasks
    task1.state = RUNNING;
    task2.state = WAITING;
    task3.state = WAITING;
    task1.event = EV_BLOCKED;
    task *currentTask;
    currentTask = scheduler(&task1);
    printf("\n task1 is blocked for resources..");
    printstate();
    currentTask->event = EV_TIMEOUT;
    task1.event = EV_RESOURCE_AVAILABLE;
    currentTask = scheduler(currentTask);
    printf("\n Current task times out and task1's resource is available");
    printstate();
    currentTask->event = EV_TIMEOUT;
    currentTask = scheduler(currentTask);
    printf("\n Current task times out again");
    printstate();
    currentTask = scheduler(currentTask);
    currentTask->event = EV_BLOCKED;
    task2.event = EV_RESOURCE_AVAILABLE;
    currentTask = scheduler(currentTask);
    printf("\n Current task Blocked for Resource and task2 has resources again");
    printstate();
    currentTask->event = EV_BLOCKED;
    task1.event = EV_RESOURCE_AVAILABLE;
    currentTask = scheduler(currentTask);
    printf("\n Current task blocked and task1 and task3 both have resources");
    printstate();
    currentTask->event = EV_BLOCKED;
    currentTask = scheduler(currentTask);
    printf("\n Current task blocked");
    printstate();
    printf("\n Exiting Simulation..");
    return 0;
}
