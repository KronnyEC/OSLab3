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
  int numProcess = schedule.size() -1;
  cout << "Number of Processes " << numProcess << "\n"; 
  //cout << "TimeQuantum : " << timeQuantum << "\n";
  remainingProcess = numProcess;
  int timeRan = 0;
  int t =0;

   //cout << "Total number of processes scheduled " << schedule.end->P_ID;
  // cout << "Total Time = " << totalTime << "\n";
  int elapsedTime = 0; 
  flag=1;
  i =0;

  //Process the following
   
  //cout << "Pid:\tBst:\tArr:\tPri:\tDline:\tI/O:\t\n";
  for(int i=0; i < schedule.size()-1; i++){
    std::cout << schedule[i].P_ID << "\n";

  }
    //TODO: Incorporate Aging Time, calculate waiting time and turnaround time, verify what happens if a process cannot finish? Doesn't work if arrival is not 0 
    cout << totalTime << "\n";    
    for(time=0;time<totalTime && que!=0;){ 
     for(i=0;i<numProcess;i++)
     {
       
      if(schedule[i].arrival <= time && schedule[i].finished==0){
        //cout<<"PID:"<<schedule[i].P_ID<< " -> " << time ;
        if(flag !=0){
          schedule[i].cameIn = time;
          //cout << "PID: " << schedule[i].P_ID << " START: " << schedule[i].cameIn << "\n"; 
        }
          
      
        if(schedule[i].rt<timeQuantum){
           dec=schedule[i].rt;
          // cout << "dec " << schedule[i].rt << "\n";     
        } else {dec=timeQuantum;}

        schedule[i].rt=schedule[i].rt-dec;
        //cout << "Remaining " << schedule[i].rt << "\n";
        if(schedule[i].rt==0) { 
             schedule[i].finished=1;
                 
             //schedule[i].end = time;
              //cout << "PID: " << schedule[i].P_ID << " END: " << schedule[i].end << "\n"; 
             //cout << schedule[i].end << "\n";    
        }
         time=time+dec;
           
         if(schedule[i].finished==1){

           schedule[i].end = time;


         }       
         //cout << "PID: " << schedule[i].P_ID << " END: " << time << "\n"; 
         //cout << "Time " << time << "\n";
         //cout << "Que is " << que << "\n";
       } 
        
     }
     
     //que decreases after first iteration
     que--;
     flag=0;
     //cout << "Que now is " << que << "\n";    
     //Time quantum doubles
     timeQuantum = timeQuantum*2;
     //cout << "Time Quantum " << timeQuantum << "\n";     

     //FCFS 
     if(que==1){
     //cout << "Time at FCFS que is " << time << "\n";
      
      for(i=0;i<numProcess;i++){
       if(schedule[i].arrival <= time && schedule[i].finished==0){
         if(schedule[i].rt<timeQuantum){
           dec=schedule[i].rt;
           //cout << "Process that needs to finish is " << schedule[i].P_ID << "with process time of " << schedule[i].rt <<  "\n";
         } else {dec=timeQuantum;}
          
         //schedule[i].rt=schedule[i].rt-dec;
         time=time+dec;
         
         schedule[i].end = time;
         //cout << "Time " << time << "\n";
         }
         
          
        }
       }
      }
  float avwt = 0;
  float avtt = 0;
  for(int i=0; i<numProcess;i++){
     
     cout << "PID: " << schedule[i].P_ID << " START: " << schedule[i].cameIn << " END: " << schedule[i].end << "\n"; 
     schedule[i].wt = schedule[i].cameIn;
     schedule[i].tt = schedule[i].wt + schedule[i].burst; 
     avwt += schedule[i].wt;
     avtt += schedule[i].tt;     
  }
  
  avwt = avwt/numProcess;
  avtt = avtt/numProcess;
  cout << "Average Waiting Time " << avwt << " Average Turnaround Time " << avtt << "\n";
    
}

//TODO:Algorithm is very slow need to recalculate
void RTS(string fileName){
  std::cout << "RTS" << "\n";
  cout << "---------------" << "\n";
   int input;
  
  //Allow for Hard or Soft RTS
  cout << "Please enter a number Hard or Soft RTS" << "\n";
  cout << "1. Hard RTS" << "\n";
  cout << "2. Soft RTS" << "\n";
  cin >> input;
  
  
  std::vector<process> schedule;
  Load(schedule, fileName);
 
  //sort arrival time
  sort(schedule.begin(), schedule.end()-1, sortByArrivalDeadline);
  Read(schedule);

  //TODO:Sorts based on Deadline 
  //sort by deadline
  //sort(schedule.begin(), schedule.end()-1, sortByDeadline);
  //Read(schedule); 
  
  
 
   //Need to account for both soft and hard RT environments.
  
  //Remove all with burst, arrivals, and deadlines less than 0
  schedule.erase(std::remove_if(schedule.begin(),schedule.end(),lessThanZeroDeadline),schedule.end()); 
    
  int numProcess = schedule.size()-1;
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

  
  //we can check preempty if it will fail if the arrival time is greater than the deadline time
  if (input == 1){
   for(int i=0; i<numProcess;i++){
    if(schedule[i].arrival > schedule[i].deadline){
     cout << "Process " << schedule[i].P_ID << " will not make the deadline. It will arrive at " << schedule[i].arrival << "but has a deadline of " << schedule[i].deadline << "\n"; 
     break;
    } 

   }   

  }


  /*
  for(int i=0; i<numProcess;i++){
    for(int j=i;j<numProcess;j++){
      cout << "Process " << schedule[j].P_ID << "\n";
      if(schedule[j+1].arrival == schedule[j].arrival){
        temp=schedule[j+1].deadline;
        schedule[j+1].deadline=schedule[j].deadline;
        schedule[j].deadline=temp;
        
        temp=schedule[j+1].P_ID;
        schedule[j+1].P_ID = schedule[j].P_ID; 
        schedule[j].P_ID = temp;

      }

    }


  }*/
  
  float avwt = 0;
  float avtt = 0;
  /*
  for(int i=0; i<numProcess;i++){
   cout << "Process Order" << schedule[i].P_ID << "\n";   
  }*/
  
  avwt = avwt/numProcess;
  avtt = avtt/numProcess;
  cout << "Average Waiting Time " << avwt << " Average Turnaround Time " << avtt << "\n";

}
void WHS(){
  std::cout << "WHS";

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
    WHS();

  } else {
    cout << "Please enter a valid number";
  }

}
