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
  int boosted;
  int age;
  int last_ran;
  int base_priority;
  bool started; 
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
/*
template< typename T >
typename std::vector<T>::iterator 
   insert_sorted( std::vector<T> & vec, T const& item )
{
    return vec.insert
        ( 
            std::upper_bound( vec.begin(), vec.end(), item ),
            item 
        );
}*/


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

bool lessPriority(process s){
 if (s.priority < 0 || s.priority > 99 || s.arrival < 0 || s.burst < 0){
    return true;
 }  else {
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

int start_time = 0;
int curr_pid;

int gant_time(int PID, int tick) {
  if (PID != curr_pid) {
    int t1 = start_time;
    start_time = tick;
    return t1 - tick;
  } else {
    return -1;
  }
}

void MFQS(string fileName){
  /************************** Get Input******************************************/
  //cout << string(5, '\n');
  std::cout << "MFQS Scheduler\n";
  std::cout << "Please enter the number of queues\n";
  int nqueues;
  cin >> nqueues;
  int base_time_quantum;
  std::cout << "Enter the base time quantum:\n";
  cin >> base_time_quantum; 
  int aging;
  std::cout << "Enter the aging amount:\n";
  cin >> aging; 

  /************************** Initialize ****************************************/
  //Setup Vector of processes 
  std::vector<process> schedule;
  Load(schedule, fileName);
  //Sort and remove junk
  schedule.erase(std::remove_if(schedule.begin(),schedule.end(),lessThanZero),schedule.end()); 
  sort(schedule.begin(), schedule.end()-1, sortByPriority);
  sort(schedule.begin(), schedule.end()-1, sortByArrival);

  float total_wait_time = 0;
  float total_turn_around_time = 0;
  float totalProcess = 0;
  int i;

  std::vector<process> queue[5];
  bool running = true;
  int tick = 0;

  /************************** Begin Simulation **********************************/
  //Read(schedule);
  schedule.erase(schedule.end()-1);
  cout << "\n-------------------------------\n";
  while (running){
    i = 0;
    //cout << "Tick: " << tick << "\n";
    while(!schedule.empty() && schedule[0].arrival <= tick) {
      schedule[0].rt = 0;
      schedule[0].last_ran = tick;
      totalProcess++;
      queue[0].push_back(schedule[0]);
      schedule.erase(schedule.begin());
    }
    while (i < nqueues-1 && queue[i].empty()) {
      i++;
    }
    //cout << "Running queue: " << i << " Process: " << queue[i][0].arrival << " With " << queue[i][0].burst << " remaining\n";

    if (!queue[i].empty()) {
      // If last queue, run FCFS
      //Manny - make the output pretty! [also time_quantum needs to double]
      int ptime = gant_time(queue[i][0].P_ID, tick);
      if (ptime >= 0 && tick > 0) {

        cout << "Pid\tstart\tstop\truntime\n";
        cout << curr_pid << "\t" << tick-ptime << "\t" << tick << "\t" << ptime << "\n";
      }
      if (i == nqueues-1) {
        while (queue[i][0].burst > 0) {
          tick++;
          queue[i][0].burst--;
          queue[i][0].last_ran = tick;
          queue[i][0].rt++;
        }
        //cout << "Process: " << queue[i][0].P_ID << "\twith burst " << queue[i][0].rt << "\tFinished at " << tick << "\tin queue " << i+1 << "\n";
        cout << "Finished\n";
        cout << "--------------------------------\n";
        queue[i].erase(queue[i].begin());
        int wait_time = tick - queue[i][0].arrival;
        total_wait_time += wait_time - queue[i][0].rt;;
        total_turn_around_time += wait_time;

      } else {
        bool finished = false;
        int time = 0;
        while (!finished && time < base_time_quantum) {
          queue[i][0].burst--;
          queue[i][0].last_ran = tick;
          queue[i][0].rt++;
          if (queue[i][0].burst <= 0) {
            //cout << "Process: " << queue[i][0].P_ID << "\twith burst " << queue[i][0].rt << "\tFinished at " << tick << "\tin queue " << i+1 << "\n";
            cout << "Finished\n";
          cout << "--------------------------------\n";
            int wait_time = tick - queue[i][0].arrival;
            total_wait_time += wait_time - queue[i][0].rt;;
            total_turn_around_time += wait_time;
            queue[i].erase(queue[i].begin());
            finished = true;
          } else {
            //cout << "Ran queue: " << i << " Process: " << queue[i][0].P_ID << " With " << queue[i][0].burst << " remaining\n";
          }
          tick++;
          time++;
        }
        if(!finished) {
          //send to lower queue
          //cout << "QUEUED\n";
          int next_queue = i+1;
          if (next_queue > nqueues-1) {
            next_queue = nqueues-1;
          }
          if (tick > 0) {
            cout << "Sent to queue " << next_queue << "\n";
          }
          cout << "--------------------------------\n";
          //cout << "Process:\t" << queue[i][0].P_ID << "\tPushed to\t" << next_queue << "\n";
          queue[next_queue].push_back(queue[i][0]);
          queue[i].erase(queue[i].begin());
        }
      }
    } else {
      tick++;
    }
    //
    // age
    int l = 2;
    while (l < nqueues-1 && !queue[l].empty()) {
      bool done_aging = false;
      int k = 0;
      while (!done_aging && k<queue[l].size()) {
        if (queue[l][k].last_ran-tick >= aging) {
          queue[l][k].last_ran = tick;
          queue[l-1].push_back(queue[l][k]);
          queue[l].erase(queue[l].begin()+k);
        } else {
          done_aging = true;
        }
        k++;
      }
      l++;
    }
    /*
    while (l < nqueues-1 && !queue[l].empty()) {
      for(int k=0; k<queue[l].size(); k++) {
        cout << i << " and " << l << "\n";
        //cout << "queue[" << l << "][" << k << "] == " << queue[l][k] << "\n";
        queue[l][k].age++;
        if (queue[l][k].age >= queue[l][k].aging) {
          int above_queue = l-1;
          if (above_queue > 0 ) {
            queue[above_queue].push_back(queue[l][k]);
            queue[l].erase(queue[l].begin()+k);
          }
          queue[l][k].age = 0;
        }
      }
      l++;
    }
*/
    // are we done?
    running = false;
    if (!schedule.empty()) {
      running = true;
    } else  {
      for(int k = 0; k < nqueues; k++) {
        if(!queue[k].empty()) {
          running = true;
        }
      }
    }
  }
  // print Gant into for final process
  cout << "Pid\tstart\tstop\nqueue\n";
  cout << curr_pid << "\t" << start_time << "\t" << tick << "\n" << i;

  float avwt = 0;
  float avtt = 0;
  avwt = total_wait_time/totalProcess;
  avtt = total_turn_around_time/totalProcess;
  cout << "\nNumber of processes: " << totalProcess << "\nAverage Waiting Time: " << avwt << "\nAverage Turnaround Time: " << avtt << "\n";
}

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

  std::vector<process> queue;
  int total_wait_time = 0;
  int total_turn_around_time = 0;
  int tick = 0;
  int running = 1;
  int completed_processes = 0;
#ifdef DEBUG
  //cout << "\nSchedule:\n";
  //Read(schedule);
  //cout << "\n";
#endif
  while(running) {
    // check for new processes
    while(!schedule.empty() && schedule[0].arrival <= tick) {
      schedule[0].rt = 0;
      queue.push_back(schedule[0]);
      schedule.erase(schedule.begin());
      sort(queue.begin(), queue.end()-1, sortByDeadline);
    }
    // do work
    if (!queue.empty()) {
      cout << "\n";
      Read(queue);
      cout << "\n";
      cout << "Time: " << tick << " Process: " << queue[0].P_ID << "\n";
      if (queue[0].burst + tick > queue[0].deadline) {
        if (hard_or_soft == 2) {
          cout << "\033[1;31mPROCESS " << queue[0].P_ID << " FAILED: Removing Process\033[0m\n";
#ifdef DEBUG
          cout << "\033[1;31mPID:\tARR\tR_BST:\tDL:\tCURR\033[0m\n";
          cout << "\033[1;31m" << queue[0].P_ID << "\t" <<queue[0].arrival << "\t" << queue[0].burst << "\t" << queue[0].deadline << "\t" << tick << "\033[0m\n";
#endif
          queue.erase(queue.begin());
          numProcess--;
        } else {
          cout << "\033[1;31mPROCESS " << queue[0].P_ID << " FAILED: ABORTING SCHEDULAR\033[0m\n";
          return;
        }
      } else {
        queue[0].burst--;
        queue[0].rt++;
        if (queue[0].burst <= 0) {
          int wait_time = tick+1 - queue[0].arrival;
          total_wait_time += wait_time - queue[0].rt;;
          total_turn_around_time += wait_time;
          completed_processes++;
#ifdef DEBUG
          cout << "Process " << queue[0].P_ID << " ended at " << tick+1 << "\n";
          cout << "cur: " << tick+1 << "\tarr: " << queue[0].arrival << "\trt: " << queue[0].rt << " total wait " << total_wait_time << "\n";
#endif
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
  avwt = total_wait_time/totalProcess;
  avtt = total_turn_around_time/totalProcess;
  cout << "\nNumber of processes: " << totalProcess << "\nSucessful Processes: " << completed_processes << "\nAverage Waiting Time: " << avwt << "\nAverage Turnaround Time: " << avtt << "\n";
}

void WHS(string fileName){
  std::cout << "WHS" << "\n";

  /************************** Get Input *****************************************/
  cout << "Please enter the time quantum" << "\n";
  int timeQuantum;
  cin >> timeQuantum;

  /*cout << "Please enter the aging amoung" << "\n";
  int aging;
  cin >> aging*/ 

  /************************** Initialize ****************************************/

  std::vector<process> schedule;
  Load(schedule, fileName);

  //sort arrival time
  sort(schedule.begin(), schedule.end()-1, sortByArrival);
  //Read(schedule);

  //Need to account for both soft and hard RT environments.

  //Remove all with burst, arrivals, and deadlines less than 0
  schedule.erase(std::remove_if(schedule.begin(),schedule.end(),lessPriority),schedule.end()); 
  
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
    schedule[l].started = false;
    schedule[l].aging =0;
    schedule[l].base_priority = schedule[l].priority;
  }

   /************************** Begin Simulation **********************************/
  std::vector<process> queue;
  
  int total_wait_time = 0;
  int total_turn_around_time = 0;
  int tick = 0;
  int running = 1;
  int completed_processes = 0;
  int aging = 100;
  int time = 0;
  int flagSet = 1;
  //Read(schedule);
  while (running){
     while(!schedule.empty() && schedule[0].arrival <= tick){
     //queue.push_back(schedule[0]);
      //schedule.erase(schedule.begin());
     if (schedule[0].i_o > 0){ //boost the priority 
        schedule[0].boosted = 1;
        //cout << "Process Boosted " << schedule[0].P_ID << " from " << schedule[0].priority << " to ";
        if (priorityCheck(schedule[0].priority) == true){ //low bound
           if (schedule[0].priority + schedule[0].i_o > 49){
              schedule[0].priority = 49;
           } else {
             schedule[0].priority = schedule[0].priority + schedule[0].i_o;
           }
        } else {
           if (schedule[0].priority + schedule[0].i_o > 99){
              schedule[0].priority = 99;            
           }else{
             schedule[0].priority = schedule[0].priority + schedule[0].i_o;
           }
        }
       //cout << schedule[0].priority << "\n";
     }
     queue.push_back(schedule[0]);
     schedule.erase(schedule.begin());
     //Read(queue);
   }

   //all processes have arrived, sort time now

   if(!queue.empty()){
     flagSet =0;
     std::sort(queue.begin(),queue.end(),sortByPriority);
      
      /*int ptime = gant_time(queue[0].P_ID, tick);
      cout << ptime << "\n";
      if (ptime >= 0 && tick > 0) {

        cout << "Pid\tstart\tstop\truntime\n";
        cout << curr_pid << "\t" << tick-ptime << "\t" << tick << "\t" << ptime << "\n";
      }*/
     //insertSort(queue);
     //Read(queue);
    //cout << "Currently working process with P_ID: " <<  queue[0].P_ID << " with remaining time " << queue[0].rt << "\n";

    //check when started:
    /*
    if (queue[0].started = false){
     queue[0].cameIn = tick;
     cout << tick << "\n";
     queue[0].started = true;
    }*/
 
    if(queue[0].rt<timeQuantum){
      
      while(queue[0].rt !=0){
      tick++;
       
      queue[0].rt--;
        //cout << "Process " << queue[0].P_ID << " rt = " << queue[0].rt << " tick = " << tick << "\n";
      }
    } else { //Process will not finished and will need to demote accordingly 
     
     for(int i=0; i<timeQuantum;i++){
         tick++;
         queue[0].rt--;
          
        //cout << "Process " << queue[0].P_ID << " rt = " << queue[0].rt << " tick = " << tick << "\n";
     }
     
     //Demote the process based on how long it took. 
   	if (queue[0].priority < (queue[0].priority - timeQuantum)){
          queue[0].priority = queue[0].priority - timeQuantum;
          //cout << " Priority for " << queue[0].P_ID << " changed to " << queue[0].priority << "\n";
     	} else {
          queue[0].priority = queue[0].base_priority; // change it back to its base priority. 
        }
        //Set the aging counter for the process; 
        queue[0].aging = tick + aging;
        queue[0].last_ran = tick;
     //send back to the queue
     queue.push_back(queue[0]);
     queue.erase(queue.begin());    
    } 
   
   if(queue[0].rt <= 0){
    cout << " P ==> " << queue[0].P_ID << " | start : " << " | end : " << tick << "\n"; 
    int wait_time = tick+1 - queue[0].arrival;
    total_wait_time += wait_time - queue[0].rt;;
    total_turn_around_time += wait_time;
    completed_processes++;
    queue.erase(queue.begin());
    numProcess--;
    
   }
  } 
  if (flagSet == 1){
   tick++;

  }
  //aging
  //
   
      bool done_aging = false;
      int k = 0;
      while (!done_aging && k<queue.size()) {
        if (queue[k].last_ran-tick >= aging) {
          queue[k].last_ran = tick;
          
          if (priorityCheck(queue[k].priority) == true){ //low bound
            if (queue[k].priority + 10 > 49){
               //cout << "Queu age " <<  queue[k].aging << " aging counter " << queue[k].age << " for PID " << queue[k].P_ID << "\n";  
               queue[k].priority = 49;
           } else {
             queue[k].priority = queue[k].priority + 10;
           }
         } else {
           if (queue[k].priority + 10 > 99){
              queue[k].priority = 99; 
           }else{
             queue[k].priority = queue[k].priority + 10;
           }
         }
       	} else {
          done_aging = true;
        }
        k++;
      }

  if (numProcess == 0){
   running = 0;
  }
}

  float avwt = 0;
  float avtt = 0;

  avwt = total_wait_time/totalProcess;
  avtt = total_turn_around_time/totalProcess;
  cout << "\nNumber of processes: " << totalProcess << "\nAverage Waiting Time: " << avwt << "\nAverage Turnaround Time: " << avtt << "\n";
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
