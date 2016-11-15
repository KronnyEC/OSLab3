#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>
using namespace std;

struct scheduler {
 int P_ID;
 int burst;
 int arrival;
 int priority;
 int deadline;
 int i_o; 
 int rt;
 int wt;
 int finished;
 int time;
};



std::istream& operator>>(std::istream& is, scheduler& s)
{
 is >> s.P_ID >> s.burst >> s.arrival >> s.priority >> s.deadline >> s.i_o;
 return is;

}

std::ostream& operator<<(std::ostream& os, scheduler& s)
{
 os << "P_ID: " << s.P_ID << "\nBurst: " << s.burst << "\nArrival: " << s.arrival << "\nPriority: " << s.priority << "\nDeadline :" << s.deadline << "\nI_O: " << s.i_o << "\n";

return os;


}


void Load(std::vector<scheduler>& r, std::string fileName)
{
	std::ifstream ifs(fileName.c_str());
	//if there are headers dump the firstline
        string dumpline;
        getline(ifs, dumpline);

        for (std::string line; getline(ifs, line);){
        
			scheduler temp;
			ifs >> temp;
			r.push_back(temp);
        }

}


void Read(std::vector<scheduler>& r){

	for(unsigned int i=0; i < r.size(); i++){
		//std::cout << r[i] << "\n";
	
	}

}

bool sortByArrival(scheduler lhs, scheduler rhs){return lhs.arrival < rhs.arrival; }

bool sortByDeadline(scheduler lhs, scheduler rhs){return lhs.deadline < rhs.deadline; }

bool lessThanZero(scheduler s){
	if (s.arrival<0){
         return true;
        } else {
         return false;
        }
}

bool lessThanZeroBurst(scheduler s){
	if(s.burst<0){
	  return true;
	}else {
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
 
 
 
 int totalTime = 0; //calculate the totalTime based on all burstTimes  

  
 std::vector<scheduler> schedule;
 Load(schedule, fileName);
 Read(schedule);
 
 int i =0; 
 vector<scheduler>::iterator it;
 for(it=schedule.begin(); it <schedule.end(); it++){
  totalTime += schedule[i].burst;	
  i++;
 }
 
 //Sort by Arrival Time
 sort(schedule.begin(), schedule.end(), sortByArrival);
 for (scheduler &n : schedule){
   if (n.arrival < 0){
    cout << n.arrival << "\n";
   }
 }
 
 cout << "Number of Processes Before Removal " << schedule.size() << "\n"; 
 //Remove all Arrival Times that are less than 0
 schedule.erase(std::remove_if(schedule.begin(),schedule.end(),lessThanZero),schedule.end()); 
 

 //set values at wt,rt, and finished to 0
 for(int l=0; l < schedule.size(); l++){
    schedule[l].wt=0;
    
    schedule[l].rt=0;
    
    schedule[l].finished=0;
    schedule[l].time = schedule[l].burst;   
 }

 int time, flag, dec, j, count = 0;
 int remainingTime = 0; 
 int remainingProcess = 0;
 int numProcess = schedule.size();
 cout << "Number of Processes " << numProcess << "\n"; 
 cout << "TotalTime : " << totalTime << "\n"; 
 cout << "TimeQuantum : " << timeQuantum << "\n";
 remainingProcess = numProcess;
 int timeRan = 0;
 int t =0;
 
// cout << "Total number of processes scheduled " << schedule.end().P_ID;
// cout << "Total Time = " << totalTime << "\n";



 while(numProcess!=0){
  cout << "Here";  
  
  if (schedule[count].time > timeQuantum){
   schedule[count].rt = schedule[count].burst - timeQuantum;
   schedule[count].time = schedule[count].burst - timeQuantum; 
 
  } else {
  
   schedule[count].finished = 1;
   numProcess-=1;
  
  }
  
  
   count++;
   cout << "Count is : " << count << "\n";

}

/*  
 for(time=0,count=0;remainingProcess!=0;){
    // cout << "Time is \n" << count;
   if(schedule[count].rt <= timeQuantum && schedule[count].rt>0){
     time+=schedule[count].rt;
     schedule[count].rt=0;
     flag = 1; 
   } else if(schedule[count].rt > 0) { 
     schedule[count].rt-=timeQuantum;
     //cout << schedule[count].rt << "\n";     
     time+= timeQuantum;
     //cout << time << "\n";
   }

   if (schedule[count].rt==0 && flag==1){
     remainingProcess--;
     cout << "Finished";
     cout << "Process: " << schedule[count].P_ID << "\n";
     cout << "Wait Time : " << time - schedule[count].arrival << "\n";
     cout << "---------------------" << "\n"; 
     flag=0; 
   }
   
   if(count==numProcess-1){
     count=0;
     cout << numProcess-1 << "\n";
   } else if(schedule[count+1].arrival<=time) {
     count++;
     //cout << "Made it here : " << schedule[count+1].arrival << "\n"; 
     break;
   }
    
     
    cout << "Made it here : " << schedule[count+1].arrival << "\n"; 
    cout << "Time is : " << time << "\n";
    cout << "Count is : " << count << "\n";
    cout << "RemaingTime is : " << remainingProcess << "\n";
    cout << "Schedule at : " << schedule[count].arrival << "\n";
    cout << "Process at : " << schedule[count].P_ID << "\n";    
   
    t++;

    if (t ==10){
     break;
    }     
    
 }
*/
 
}

void RTS(string Filename){
 std::cout << "RTS";

 


}

void WHS(){
 std::cout << "WHS";

}

int main(){

 ifstream inputFile;
 string fileName;
 int i;
 
 //User Interface for schedulers 
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
