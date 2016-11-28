#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>
using namespace std;

struct process {
  int P_ID;
  int burst;
  int arrival;
  int priority;
  int deadline;
  int i_o; 
  int rt;
  int wt;
  int tt;
  int finished;
  int time;
  int cameIn;
  int end;
  int aging;
};

std::istream& operator>>(std::istream& is, process& s)
{
  is >> s.P_ID >> s.burst >> s.arrival >> s.priority >> s.deadline >> s.i_o;
  return is;

}

std::ostream& operator<<(std::ostream& os, process& s)
{
  //os << "P_ID: " << s.P_ID << "\nBurst: " << s.burst << "\nArrival: " << s.arrival << "\nPriority: " << s.priority << "\nDeadline :" << s.deadline << "\nI_O: " << s.i_o << "\n";
  os << s.P_ID << "\t" << s.burst << "\t" << s.arrival << "\t" << s.priority << "\t" << s.deadline << "\t" << s.i_o;

  return os;
}


void Load(std::vector<process>& r, std::string fileName)
{
  std::ifstream ifs(fileName.c_str());
  /*
  //if there are headers dump the firstline
  string dumpline;
  getline(ifs, dumpline);
  cout << dumpline << "\n";
  */

  //I think the problem is here but do not understand how this works.
  //TEMP: Ignoring the header fixes the problem
  //TODO: Fix this
  for (std::string line; getline(ifs, line);){
    process temp;
    ifs >> temp;
    r.push_back(temp);
  }
}


void Read(std::vector<process>& r){
  cout << "Pid:\tBst:\tArr:\tPri:\tDline:\tI/O:\t\n";
  for(unsigned int i=0; i < r.size()-1; i++){
    std::cout << r[i] << "\n";
  }
}

bool sortByArrivalDeadline(process const &lhs, process const &rhs){
  if(lhs.deadline < rhs.deadline){
    return true;
  } else if (rhs.deadline < lhs.deadline){
    return false;

  } else { //deadline equals
    if (lhs.arrival < rhs.arrival){
      return true;
    } else if (rhs.arrival < lhs.arrival){
      return false;
    } else { //Arrival and Deadline Equal sort on priority
      return lhs.priority < rhs.priority;

    }
  }

}

bool priorityCheck(int x){
  if ( x >= 0 && x <=49){
     return true;   
  } else {
     return false;
  }


}

bool sortByBurst(process lhs, process rhs){return lhs.burst < rhs.burst; }

bool sortByPriority(process lhs, process rhs){return lhs.priority > rhs.priority; }

bool sortByArrival(process lhs, process rhs){return lhs.arrival < rhs.arrival; }

bool sortByDeadline(process lhs, process rhs){return lhs.deadline < rhs.deadline; }

bool lessThanZero(process s){
  if (s.arrival <0 || s.burst <0){
    return true;
  } else {
    return false;
  }
}

bool lessThanZeroDeadline(process s){
  if (s.arrival <0 || s.burst < 0 || s.deadline <0){
    return true;
  } else {
    return false;
  }


}

void MFQS(string fileName){
  //Clear Screen

  /************************** Get Input******************************************/
  cout << string(5, '\n');
  std::cout << "MFQS Scheduler\n";
  std::cout << "Please enter the number of ques\n";

  int que;
  cin >> que;

  int timeQuantum;
  std::cout << "Enter the time quantum:\n";
  cin >> timeQuantum; 

  int aging;
  std::cout << "Enter the aging amount :\n";
  cin >> aging; 

  //Setup Vector of processes 
  std::vector<process> schedule;
  Load(schedule, fileName);

  int i =0; 


  /************************** Initialize ****************************************/
  cout << "---------\n";
  //Sort
  sort(schedule.begin(), schedule.end()-1, sortByPriority);
  sort(schedule.begin(), schedule.end()-1, sortByArrival);
  // Read(schedule);


  //cout << "Number of Processes Before Removal " << schedule.size() << "\n"; 
  //Remove all Arrival and Burst Times that are less than 0
  schedule.erase(std::remove_if(schedule.begin(),schedule.end(),lessThanZero),schedule.end()); 
  cout << "Number of Process After Removal " << schedule.size() << "\n";

  //Setup the total amount of time based valid Burst and Arrived Times 
  int totalTime = 0; 

  //set values at wt,rt, and finished to 0
  for(int l=0; l < schedule.size(); l++){
    schedule[l].wt=0;
    totalTime+=schedule[l].burst;
    schedule[l].rt=schedule[l].burst;
    schedule[l].finished=0;
  }

  int time, flag, dec, j, count = 0;
  int remainingTime = 0; 
  int remainingProcess = 0;
  int totalProcess = 0;
  int numProcess = schedule.size() -1;
  cout << "Number of Processes " << numProcess << "\n"; 
  //cout << "TimeQuantum : " << timeQuantum << "\n";
  remainingProcess = numProcess;
  totalProcess = numProcess;
  int timeRan = 0;
  int t =0;

  //cout << "Total number of processes scheduled " << schedule.end->P_ID;
  // cout << "Total Time = " << totalTime << "\n";
  int elapsedTime = 0; 
  flag=1;
  i =0;

  /************************** Begin Simulation **********************************/
  //Process the following

  //cout << "Pid:\tBst:\tArr:\tPri:\tDline:\tI/O:\t\n";
  /*
     for(int i=0; i < schedule.size()-1; i++){
     std::cout << schedule[i].P_ID << "\n";

     }
     */
  //TODO: Incorporate Aging Time, calculate waiting time and turnaround time, verify what happens if a process cannot finish? Doesn't work if arrival is not 0 
  //cout << totalTime << "\n";
      
  float avwt = 0;
  float avtt = 0;
  std::vector<process> queue;
  int tick = 0;
  int clockTime = 0;
  int running =1;
  int p = 0;
  int in = 0;
  Read(schedule);
  while (running){
  
   if(!schedule.empty() && schedule[0].arrival <= tick){
     queue.push_back(schedule[0]);
     
     schedule.erase(schedule.begin());
    
     //sort(queue.begin(), queue.end()-1, sortByArrival);
     //Read(queue);
   }	
   if(!queue.empty() && que != 1){
   
    if(queue[0].rt<timeQuantum){
      
      queue[0].rt--;
   } else { //Process will not finished and will need to demote accordingly 
     queue[0].rt--; 
     clockTime++;
     cout << "time " << clockTime << "\n";
     if (clockTime == timeQuantum){ 
      //Demote the process based on how long it took. 
        //Set the aging counter for the process; 
        queue[0].aging = tick+1 + aging;
     //send back to the queue
     p++;
     clockTime =0;
     cout << "p " << p << "\n";
     queue.push_back(queue[0]);
    } 
   }
   if(queue[0].rt <= 0){
    cout << "Ended at " << tick+1 << ": process " << queue[0].P_ID << "\n"; 
    avtt += (tick+1 - in); 
    
    queue.erase(queue.begin());
    numProcess--;
    p++;
    cout << "p " << p << "\n";
   }
  }
  if (remainingProcess == p){
   que--;
   remainingProcess = numProcess;
   timeQuantum = timeQuantum * 2;
   p = 0;
   cout << "RemainingProcess " << remainingProcess << " TimeQuantum : " << timeQuantum << " que : " << que << "\n";
  }
 
  if (que ==1){
    queue[0].rt--;
    if(queue[0].rt<0){
    
    cout << "Ended at " << tick+1 << ": process " << queue[0].P_ID << "\n"; 
     queue.erase(queue.begin());
     numProcess--;
    } 

  }
 
  if (numProcess == 0){
   running = 0;
  }
  tick++;
}
 
  /*
  for(int i=0; i<numProcess;i++){

    cout << "PID: " << schedule[i].P_ID << " START: " << schedule[i].cameIn << " END: " << schedule[i].end << "\n"; 
    schedule[i].wt = schedule[i].cameIn;
    schedule[i].tt = schedule[i].wt + schedule[i].burst; 
    avwt += schedule[i].wt;
    avtt += schedule[i].tt;     
  }*/

  avwt = avwt/numProcess;
  avtt = avtt/numProcess;
  cout << "Average Waiting Time " << avwt << " Average Turnaround Time " << avtt << "\n";

}

//TODO:Algorithm is very slow need to recalculate
void RTS(string fileName){
  /************************** Get Input *****************************************/
  std::cout << "RTS" << "\n";
  cout << "---------------" << "\n";
  int hard_or_soft;

  //Allow for Hard or Soft RTS
  cout << "Please enter a number Hard or Soft RTS" << "\n";
  cout << "1. Hard RTS" << "\n";
  cout << "2. Soft RTS" << "\n";
  cin >> hard_or_soft;


  /************************** Initialize ****************************************/
  std::vector<process> schedule;
  Load(schedule, fileName);

  //sort arrival time
  sort(schedule.begin(), schedule.end()-1, sortByArrivalDeadline);
  sort(schedule.begin(), schedule.end()-1, sortByArrival);
  //Read(schedule);

  //Need to account for both soft and hard RT environments.

  //Remove all with burst, arrivals, and deadlines less than 0
  schedule.erase(std::remove_if(schedule.begin(),schedule.end(),lessThanZeroDeadline),schedule.end()); 

  int numProcess = schedule.size()-1;
  int totalProcess = numProcess;
  cout << "Number of processes " << numProcess << "\n";
  //Setup the total amount of time based valid Burst and Arrived Times 
  int totalTime = 0; 
  int temp;
  //set values at wt,rt, and finished to 0
  for(int l=0; l < schedule.size(); l++){
    schedule[l].wt=0;
    totalTime+=schedule[l].burst;
    schedule[l].rt=schedule[l].burst;
    schedule[l].finished=0;
  }


  /************************** Begin Simulation **********************************/
  //we can check preempty if it will fail if the arrival time is greater than the deadline time
  if (hard_or_soft == 1){
    for(int i=0; i<numProcess;i++){
      ; if(schedule[i].arrival > schedule[i].deadline){
        cout << "Process " << schedule[i].P_ID << " will not make the deadline. It will arrive at " << schedule[i].arrival << "but has a deadline of " << schedule[i].deadline << "\n"; 
        return;
      } 
    }   
  }

  //manny
  std::vector<process> queue;
  int tick = 0;
  int running = 1;
  cout << "First item arrives at: " << schedule[0].arrival << "\n\n";
  Read(schedule);
  while(running) {
    // check for new processes
    if (!schedule.empty() && schedule[0].arrival <= tick) {
      queue.push_back(schedule[0]);
      schedule.erase(schedule.begin());
      sort(queue.begin(), queue.end()-1, sortByDeadline);
    }
    // do work
    if (!queue.empty()) {
      if (queue[0].burst + tick > queue[0].deadline) {
        if (hard_or_soft == 2) {
          cout << "\033[1;31mFAILED: PID:\tR_BST:\tDL:\tCURR\033[0m\n";
          cout << "\033[1;31m\t" << queue[0].P_ID << "\t" << queue[0].burst << "\t" << queue[0].deadline << "\t" << tick << "\033[0m\n";
          queue.erase(queue.begin());
          numProcess--;
        } else {
          cout << "\033[1;31mFAILED: ABORTING SCHEDULAR\033[0m\n";
          return;
        }
      } else {
        queue[0].burst--;
        if (queue[0].burst <= 0) {
          cout << "Ended at " << tick+1 << ": process " << queue[0].P_ID << "\n";
          queue.erase(queue.begin());
          numProcess--;
        }
      }
    }
    if (numProcess == 0) {
      running = 0;
    }
    tick++;
  }
  float avwt = 0;
  float avtt = 0;

  avwt = avwt/totalProcess;
  avtt = avtt/totalProcess;
  cout << "Average Waiting Time " << avwt << " Average Turnaround Time " << avtt << "\n";
}

void WHS(string fileName){
  std::cout << "WHS" << "\n";

  /************************** Get Input *****************************************/
  cout << "Please enter the time quantum" << "\n";
  int timeQuantum;
  cin >> timeQuantum;


  /************************** Initialize ****************************************/

  std::vector<process> schedule;
  Load(schedule, fileName);

  //sort arrival time
  sort(schedule.begin(), schedule.end()-1, sortByArrival);
  //Read(schedule);

  //Need to account for both soft and hard RT environments.

  //Remove all with burst, arrivals, and deadlines less than 0
  schedule.erase(std::remove_if(schedule.begin(),schedule.end(),lessThanZeroDeadline),schedule.end()); 

  int numProcess = schedule.size()-1;
  int totalProcess = numProcess;
  cout << "Number of processes " << numProcess << "\n";
  //Setup the total amount of time based valid Burst and Arrived Times 
  int totalTime = 0; 
  //set values at wt,rt, and finished to 0
  for(int l=0; l < schedule.size(); l++){
    schedule[l].wt=0;
    totalTime+=schedule[l].burst;
    schedule[l].rt=schedule[l].burst;
    schedule[l].finished=0;
    schedule[l].aging =0;
  }

   /************************** Begin Simulation **********************************/
  std::vector<process> queue;
  int tick = 0;
  int aging = 100;
  int running =1;
  int time = 0;
  Read(schedule);
  while (running){
  
   if(!schedule.empty() && schedule[0].arrival <= tick){
     queue.push_back(schedule[0]);
     schedule.erase(schedule.begin());
    
     //check to make sure that priority stays within bounds of its oringal priority 0-49 and 50 to 99  
     if(tick != 0 && queue[0].aging == tick){
        
        //aging has expired boost its priority
        if (priorityCheck(queue[0].priority) == true){ //low bound
           if (queue[0].priority + 10 > 49){
              queue[0].priority = 49;
           } else {
             queue[0].priority = queue[0].priority + 10;
           }
        } else {
           if (queue[0].priority + 10 > 99){
              queue[0].priority = 99;            
           }else{
              queue[0].priority = queue[0].priority + 10; 
           }
        }

     }
     
     if (queue[0].i_o > 0){ //boost the priority 
       
        if (priorityCheck(queue[0].priority) == true){ //low bound
           if (queue[0].priority + queue[0].i_o > 49){
              queue[0].priority = 49;
           } else {
             queue[0].priority = queue[0].priority + queue[0].i_o;
           }
        } else {
           if (queue[0].priority + queue[0].i_o > 99){
              queue[0].priority = 99;            
           }else{
             queue[0].priority = queue[0].priority + queue[0].i_o;
           }
        }
     }
     sort(queue.begin(), queue.end()-1, sortByPriority);
     //Read(queue);
   }	
   if(!queue.empty()){
   
    if(queue[0].rt<timeQuantum){
      
      queue[0].rt--;
   } else { //Process will not finished and will need to demote accordingly 
     queue[0].rt--; 
     time++;
     if (time == timeQuantum){ 
      //Demote the process based on how long it took. 
     	if (queue[0].priority > queue[0].priority - time){
          queue[0].priority = queue[0].priority - time;
     	}
        //Set the aging counter for the process; 
        queue[0].aging = tick+1 + aging;
        time =0;   
     //send back to the queue
     queue.push_back(queue[0]);
    } 
   }
   if(queue[0].rt <= 0){
    cout << "Ended at " << tick << ": process " << queue[0].P_ID << "\n"; 
    queue.erase(queue.begin());
    numProcess--;
   }
  } 
  if (numProcess == 0){
   running = 0;
  }
  tick++;
}

  float avwt = 0;
  float avtt = 0;

  avwt = avwt/totalProcess;
  avtt = avtt/totalProcess;
  cout << "Average Waiting Time " << avwt << " Average Turnaround Time " << avtt << "\n";

}

int main(){

  ifstream inputFile;
  string fileName;
  int i;

  //User Interface for processs 
  cout << "Please enter the number to implement a scheduling algorithm \n";
  cout << "1: Multi-level Feedback Queue Scheduler (MFQS\n";
  cout << "2: Real-Time Scheduler (RTS)\n";
  cout << "3: Windows Hybrid Scheduler (WHS)\n";
  cin >> i;

  cout << "Enter the file to test for scheduling \n";
  cin >> fileName;

  if (i == 1 ){


    MFQS(fileName);
  } else if (i == 2){

    RTS(fileName);
  } else if (i == 3) { 
    WHS(fileName);

  } else {
    cout << "Please enter a valid number";
  }

}
