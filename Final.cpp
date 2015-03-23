//Writen by Chet Michals, last edit October 6th 2014.
//Writen in Visual Studio 2012, makes use of C++11. 
//Makes mild use of Windows Systems calls

#include <iostream>
#include <vector>
#include <algorithm> 
#include <string>
using namespace std;

struct struct_ProcessInfo
{
	int processID;
	int burstNumber;
	int burstTime;
	int ioTime;
	int ioTotalTime;
	int waitTime;
	int executionTime;
	int turnaroundTime;
	int responseTime;
	int MLFQTime;
	int MLFQLevel;

	bool responceFlag;
	bool finishedBurst;
	
};

void setProcessTimes();
void InitilzeProcessInfo();

void PrintFrame(int inExecution);
void PrintFrame_MLFQ(int inExecution);
void PrintSummery();

void FCFS_NonModular();
void FCFS_Modular();
void SJF();
void MLFQ();

void MoveToExectuion_FCFS();
void MoveToExectuion_SJF();
void MoveToExectuion_MLFQ();
void CheckExecution_MLFQ();
void CheckExecution();
void UpdateIO(vector <int>& ioDequeueList);
void UpdateIO_MLFQ(vector <int>& ioDequeueList);

bool InitalSJFSort(int i, int j);

vector <vector<int>> ProcessTimes;
vector <struct_ProcessInfo> ProcessInfo;

vector <int> IOQueue;
vector <int> FinishedList;

vector <int> ReadyQueue;
vector <int> ReadyQueueLevel1;
vector <int> ReadyQueueLevel2;
vector <int> ReadyQueueLevel3;

int inExecution;
int CPUCycle;
int idleTime;

int main()
{
	string switchChar;
	do //Runs the main menu
		{
			cout << "Options: 1 FCFS, 2 SJF, 3 MLFQ, 4 Exit" << endl;

			//Gets the user inputs, trucates it to only the first character
			cin >> switchChar;
			switchChar = switchChar.substr(0,1);

			//Should likely be cleaner as a switch, I'm not changing it now. 
			if (switchChar == "1") //Print
			{
				FCFS_Modular();
				PrintSummery();
			}
			else if (switchChar == "4") //Exit
			{
				cout << "Now Exiting." << endl;
				break; //Gets out of while loop
			}
			else if (switchChar == "3") //Retrieve
			{
				MLFQ();
				PrintSummery();
			}
			else if (switchChar == "2") //Retrieve
			{
				SJF();
				PrintSummery();
			}
			else //Invalid input
			{
				cout << "Unknown Function \"" << switchChar << "\" Used, please use a valid function" << endl;
			}

		}
		while (switchChar!= "4");
	//system("PAUSE");
}

/* 
//This version only works with more robust C++11 support, VC2012 doesn't support vector inizilation though, while VC2013 does
void setProcessTimes()
{
	ProcessTimes.clear();

	vector <int> P1 = { 17, 24, 18, 73, 17, 31, 16, 27, 14, 33, 16, 43, 14, 64, 15, 19, 15 };
	vector <int> P2 = { 10, 31, 9, 35, 8, 42, 7, 43, 9, 47, 12, 43, 15, 51, 19 };
	vector <int> P3 = { 18, 51, 23, 53, 24, 61, 22, 31, 21, 43, 20, 31, 12 };
	vector <int> P4 = { 17, 42, 19, 55, 20, 54, 17, 52, 15, 67, 12, 72, 15, 66, 14 };
	vector <int> P5 = { 5, 61, 6, 82, 5, 71, 3, 61, 5, 62, 4, 51, 3, 77, 4, 61, 3, 42, 5 };
	vector <int> P6 = { 10, 35, 12, 41, 14, 33, 11, 32, 15, 41, 13, 29, 11 };
	vector <int> P7 = { 6, 18, 7, 21, 5, 19, 4, 16, 5, 29, 7, 21, 8, 22, 6, 24, 5 };
	vector <int> P8 = { 9, 52, 12, 42, 14, 31, 14, 21, 16, 43, 14, 31, 13, 32, 15 };
	vector <int> P9 = { 6, 35, 4, 41, 6, 33, 6, 32, 7, 41, 4, 29, 5, 16, 5, 22, 4 };

	ProcessTimes.push_back(P1);
	ProcessTimes.push_back(P2);
	ProcessTimes.push_back(P3);
	ProcessTimes.push_back(P4);
	ProcessTimes.push_back(P5);
	ProcessTimes.push_back(P6);
	ProcessTimes.push_back(P7);
	ProcessTimes.push_back(P8);
	ProcessTimes.push_back(P9);
}
*/

//Version that works in VS2012, makes use of the C++11 functions begin and end that are supported by Visusal Studio 2012
void setProcessTimes()
{
	ProcessTimes.clear();

	//Put numbers into arrays
	int a_P1[] = { 17, 24, 18, 73, 17, 31, 16, 27, 14, 33, 16, 43, 14, 64, 15, 19, 15 };
	int a_P2[] = { 10, 31, 9, 35, 8, 42, 7, 43, 9, 47, 12, 43, 15, 51, 19 };
	int a_P3[] = { 18, 51, 23, 53, 24, 61, 22, 31, 21, 43, 20, 31, 12 };
	int a_P4[] = { 17, 42, 19, 55, 20, 54, 17, 52, 15, 67, 12, 72, 15, 66, 14 };
	int a_P5[] = { 5, 61, 6, 82, 5, 71, 3, 61, 5, 62, 4, 51, 3, 77, 4, 61, 3, 42, 5 };
	int a_P6[] = { 10, 35, 12, 41, 14, 33, 11, 32, 15, 41, 13, 29, 11 };
	int a_P7[] = { 6, 18, 7, 21, 5, 19, 4, 16, 5, 29, 7, 21, 8, 22, 6, 24, 5 };
	int a_P8[] = { 9, 52, 12, 42, 14, 31, 14, 21, 16, 43, 14, 31, 13, 32, 15 };
	int a_P9[] = { 6, 35, 4, 41, 6, 33, 6, 32, 7, 41, 4, 29, 5, 16, 5, 22, 4 };

	//Convert Arrays into Vectors
	vector <int> P1(begin(a_P1), end(a_P1));
	vector <int> P2(begin(a_P2), end(a_P2));
	vector <int> P3(begin(a_P3), end(a_P3));
	vector <int> P4(begin(a_P4), end(a_P4));
	vector <int> P5(begin(a_P5), end(a_P5));
	vector <int> P6(begin(a_P6), end(a_P6));
	vector <int> P7(begin(a_P7), end(a_P7));
	vector <int> P8(begin(a_P8), end(a_P8));
	vector <int> P9(begin(a_P9), end(a_P9));
		
	//Put Vectors into Vector of Vectors
	ProcessTimes.push_back(P1);
	ProcessTimes.push_back(P2);
	ProcessTimes.push_back(P3);
	ProcessTimes.push_back(P4);
	ProcessTimes.push_back(P5);
	ProcessTimes.push_back(P6);
	ProcessTimes.push_back(P7);
	ProcessTimes.push_back(P8);
	ProcessTimes.push_back(P9);
}

//Intitilizes the global varrables back to their defalut states
void InitilzeProcessInfo()
{
	ProcessInfo.clear();
	ReadyQueue.clear();
	IOQueue.clear();
	FinishedList.clear();

	for (int i = 0; i < 9; i++)
	{
		struct_ProcessInfo newProcess;
		newProcess.processID = i;
		newProcess.burstNumber = 0;
		newProcess.executionTime = 0;
		newProcess.turnaroundTime = 0;
		newProcess.responseTime = 0;
		newProcess.responceFlag = 0;
		newProcess.ioTime = 0;
		newProcess.ioTotalTime = 0;
		newProcess.waitTime = 0;
		newProcess.MLFQTime = 0;
		newProcess.finishedBurst = true;
		newProcess.MLFQLevel = 1;

		ProcessInfo.push_back(newProcess);
		ReadyQueue.push_back(i);
	}
	inExecution = -1;
	CPUCycle = 0;
	idleTime = 0;

	ReadyQueueLevel1.clear();
	ReadyQueueLevel2.clear();
	ReadyQueueLevel3.clear();

	for (int i = 0; i < 9; i++) ReadyQueueLevel1.push_back(i); //Initilze the Level 1 Queue
}

//First Draft of First Come First Serve, before breaking it down into smaller pieces to be reused. Has some logic errors and and band aid fixes to them
void FCFS_NonModular()
{
	//Put all Processes into ready queue
	setProcessTimes();
	InitilzeProcessInfo();
	vector <int> ioDequeueList;

	//while wait queue and ready queue are not empty
	while (ReadyQueue.size() != 0 || IOQueue.size() != 0 || inExecution >= 0)
	{
		//if execution is empty pop top process off ready queue, put it into execution box
		if (inExecution < 0 && ReadyQueue.size() != 0)
		{
			inExecution = ReadyQueue[0];
			ReadyQueue.erase(ReadyQueue.begin());//This acts as "pop top of queue", has O(N) run time instead of O(1) but its a list of 10 elements so why am I writing this
			ProcessInfo[inExecution].burstTime = ProcessTimes[inExecution][ProcessInfo[inExecution].burstNumber]; //Gets the CPU burst time

			//Check if this is the first CPU burst, if so set the responce time
			if (ProcessInfo[inExecution].responceFlag == false)
			{
				ProcessInfo[inExecution].responseTime = CPUCycle;
				ProcessInfo[inExecution].responceFlag = true;
			}
			PrintFrame(inExecution);
		}

		//Add a cycle to wait time
		for (int i = 0; i < (int)ReadyQueue.size(); i++)
		{
			ProcessInfo[ReadyQueue[i]].waitTime++;
		}

		//evalute if process in execution box should be moved to finished, IO, or ready
		if (inExecution >= 0)
		{
			ProcessInfo[inExecution].executionTime++;
			ProcessInfo[inExecution].burstTime--;
			if (ProcessInfo[inExecution].burstTime <= 0)
			{
				ProcessInfo[inExecution].burstNumber++; //Increase the counter for which burst/IO time is next

				if (ProcessInfo[inExecution].burstNumber == ProcessTimes[inExecution].size()) //Checks if all Burst and IOs have been done (Finished)
				{
					FinishedList.push_back(inExecution);
					ProcessInfo[inExecution].turnaroundTime = CPUCycle + 1;
				}
				else //If not, move to IO queue
				{
					IOQueue.push_back(inExecution);
					ProcessInfo[inExecution].ioTime = ProcessTimes[inExecution][ProcessInfo[inExecution].burstNumber] + 1; //There is some off by 1 error, going to see if this fixes it (I don't think it will)
				}
				inExecution = -1;

				//Print some stuff here
			}
		}
		//decrement waiting time in IO queue
		ioDequeueList.clear();
		sort(IOQueue.begin(), IOQueue.end());//This should make my output lines up with the example, not totally needed by will help find an error.
		for (int i = 0; i < (int)IOQueue.size(); i++)
		{
			ProcessInfo[IOQueue[i]].ioTime--;
			ProcessInfo[IOQueue[i]].ioTotalTime++;
			//check if anything in IO queue should move to Ready Queue.
			if (ProcessInfo[IOQueue[i]].ioTime <= 0)
			{
				ReadyQueue.push_back(ProcessInfo[IOQueue[i]].processID);
				ioDequeueList.push_back(i);
				ProcessInfo[IOQueue[i]].burstNumber++;
			}
		}

		//Remove finished IO queue entires from queue
		for (int i = ioDequeueList.size(); i > 0; i--)
		{
			IOQueue.erase(IOQueue.begin() + ioDequeueList.back());
			ioDequeueList.pop_back();
		}
		CPUCycle++; //advances one time unit. This is very important.
		//At this point it should hopefully go though every cycle in a FCFS queue, then we need to add the logic to keep track of metrics
	}
}

//Final version of First Come First Serve
void FCFS_Modular()
{
	//Put all Processes into ready queue and inizile all the global varables
	setProcessTimes();
	InitilzeProcessInfo();
	vector <int> ioDequeueList;

	while (ReadyQueue.size() != 0 || IOQueue.size() != 0 || inExecution > 0)
	{		
		MoveToExectuion_FCFS(); //if execution is empty pop top process off ready queue, put it into execution box
		CPUCycle++; //advances one time unit. 
		for (int i = 0; i < (int)ReadyQueue.size(); i++)	ProcessInfo[ReadyQueue[i]].waitTime++; //Add a cycle to the wait time of all process in the ready queue
		if (inExecution < 0) //Used to print info when a process leaves the exectuion queue without something replacing it
		{
			if (inExecution == - 1)PrintFrame(inExecution);
			idleTime++;
			inExecution--;
		}
		UpdateIO(ioDequeueList); //Updates the IO process times, and removes items for the IOqueue when they finish. 
		CheckExecution(); //Evalute if the process in execution should be moved to finished or IO.
		sort(FinishedList.begin(),FinishedList.end()); //Sorts the list of finished processes to line up with the examples slightly better
	}
}

//Shortest Job First
void SJF()
{
	//Put all Processes into ready queue and inizile all the global varables
	setProcessTimes();
	InitilzeProcessInfo();
	vector <int> ioDequeueList;
	//sort (ReadyQueue.begin(),ReadyQueue.end(),InitalSJFSort);
	while (ReadyQueue.size() != 0 || IOQueue.size() != 0 || inExecution > 0)
	{		
		
		MoveToExectuion_SJF(); //if execution is empty pop top process off ready queue, put it into execution box
		CPUCycle++; //advances one time unit. 
		for (int i = 0; i < (int)ReadyQueue.size(); i++)	ProcessInfo[ReadyQueue[i]].waitTime++; //Add a cycle to the wait time of all process in the ready queue
		if (inExecution < 0) //Used to print info when a process leaves the exectuion queue without something replacing it
		{
			if (inExecution == - 1)PrintFrame(inExecution);
			idleTime++;
			inExecution--;
		}
		UpdateIO(ioDequeueList); //Updates the IO process times, and removes items for the IOqueue when they finish. 
		CheckExecution(); //Evalute if the process in execution should be moved to finished or IO.
		sort(FinishedList.begin(),FinishedList.end()); //Sorts the list of finished processes to line up with the examples slightly better
	}
}

//Multilevel Feedback Queue, Has 3 levels. First level has a Time Quotent of 6, the second a time quotent of 12, and the final is round robin
void MLFQ()
{
	//Put all Processes into ready queue and inizile all the global varables
	setProcessTimes();
	InitilzeProcessInfo();
	vector <int> ioDequeueList;

	while (ReadyQueueLevel1.size() != 0 || ReadyQueueLevel2.size() != 0 || ReadyQueueLevel3.size() != 0 || IOQueue.size() != 0 || inExecution > 0)
	{		
		MoveToExectuion_MLFQ(); //if execution is empty pop top process off ready queue, put it into execution box
		CPUCycle++; //advances one time unit. 
		for (int i = 0; i < (int)ReadyQueueLevel1.size(); i++)	ProcessInfo[ReadyQueueLevel1[i]].waitTime++;
		for (int i = 0; i < (int)ReadyQueueLevel2.size(); i++)	ProcessInfo[ReadyQueueLevel2[i]].waitTime++; 
		for (int i = 0; i < (int)ReadyQueueLevel3.size(); i++)	ProcessInfo[ReadyQueueLevel3[i]].waitTime++;
		if (inExecution < 0) //Used to print info when a process leaves the exectuion queue without something replacing it
		{
			if (inExecution == - 1)PrintFrame_MLFQ(inExecution);
			idleTime++;
			inExecution--;
		}
		UpdateIO_MLFQ(ioDequeueList); //Updates the IO process times, and removes items for the IOqueue when they finish. 
		CheckExecution_MLFQ(); //Evalute if the process in execution should be moved to finished or IO.
		sort(FinishedList.begin(),FinishedList.end()); //Sorts the list of finished processes to line up with the examples slightly better
	}
}

//Print the info the current state of the CPU simulation when called
void PrintFrame(int inExecution)
{
	cout << endl << "Current Time: " << CPUCycle << endl;
	if (inExecution >= 0)	cout << "Now Running: P" << 1 + inExecution << "\tBurst Time: "<< ProcessTimes[inExecution][ProcessInfo[inExecution].burstNumber] << endl;
	else cout << "Now Running: [idle]" << endl;
	cout << "-------------" << endl;
	cout << "Ready Queue:  Process    Burst" << endl;
	if (ReadyQueue.size() != 0)
	{
		for (int i = 0; i < (int)ReadyQueue.size(); i++)
		{
			cout << "\t\tP" << ProcessInfo[ReadyQueue[i]].processID + 1 << "\t" << ProcessTimes[ReadyQueue[i]][ProcessInfo[ReadyQueue[i]].burstNumber] << endl;
		}
	}
	else
	{
		cout << "[empty]" << endl;
	}
	cout << "-------------" << endl;
	cout << "Now in I/O:   Process    Remaining I/O time" << endl;

	if (IOQueue.size() != 0)
	{
		for (int i = 0; i < (int)IOQueue.size(); i++)
		{
			cout << "\t\tP" << ProcessInfo[IOQueue[i]].processID + 1 << "\t" << ProcessInfo[IOQueue[i]].ioTime << endl;
		}
	}
	else
	{
		cout << "[empty]" << endl;
	}

	if (FinishedList.size() != 0)
	{
		cout << "-------------" << endl;
		cout << "Completed: ";
		for (int i = 0; i < (int)FinishedList.size(); i++)
		{
			cout << "P" << 1 + FinishedList[i] << "\t";
		}
	}

	cout << endl << endl;
	//system("PAUSE");
}

//Prints the info of the CPU simulation when called
void PrintFrame_MLFQ(int inExecution)
{
	cout << endl << "Current Time: " << CPUCycle << endl;
	if (inExecution >= 0) cout << "Now Running: P" << 1 + inExecution << "\tBurst Time: "<< ProcessInfo[inExecution].burstTime << endl;
	else cout << "Now Running: [idle]" << endl;
	cout << "-------------" << endl;
	cout << "Ready Queue:  Process    Burst" << endl;
	if (ReadyQueueLevel1.size() != 0 || ReadyQueueLevel2.size() != 0 || ReadyQueueLevel3.size() != 0)
	{
		cout << "Level 1: " << endl;
		for (int i = 0; i < (int)ReadyQueueLevel1.size(); i++)
		{
			cout << "\t\tP" << ProcessInfo[ReadyQueueLevel1[i]].processID + 1 << "\t" << ProcessTimes[ReadyQueueLevel1[i]][ProcessInfo[ReadyQueueLevel1[i]].burstNumber] << endl;
		}
		cout << "Level 2: " << endl;
		for (int i = 0; i < (int)ReadyQueueLevel2.size(); i++)
		{
			cout << "\t\tP" << ProcessInfo[ReadyQueueLevel2[i]].processID + 1 << "\t";
			if (ProcessInfo[ReadyQueueLevel2[i]].burstTime != 0)
			{
				cout << ProcessInfo[ReadyQueueLevel2[i]].burstTime;
			}
			else
			{
				cout << ProcessTimes[ReadyQueueLevel2[i]][ProcessInfo[ReadyQueueLevel2[i]].burstNumber];
			}
			cout << endl;
		}
		cout << "Level 3: " << endl;
		for (int i = 0; i < (int)ReadyQueueLevel3.size(); i++)
		{
			cout << "\t\tP" << ProcessInfo[ReadyQueueLevel3[i]].processID + 1 << "\t";
			if (ProcessInfo[ReadyQueueLevel3[i]].burstTime != 0)
			{
				cout << ProcessInfo[ReadyQueueLevel3[i]].burstTime;
			}
			else
			{
				cout << ProcessTimes[ReadyQueueLevel3[i]][ProcessInfo[ReadyQueueLevel3[i]].burstNumber];
			}
			cout << endl;
		}
	}
	else
	{
		cout << "[empty]" << endl;
	}
	cout << "-------------" << endl;
	cout << "Now in I/O:   Process    Remaining I/O time" << endl;

	if (IOQueue.size() != 0)
	{
		for (int i = 0; i < (int)IOQueue.size(); i++)
		{
			cout << "\t\tP" << ProcessInfo[IOQueue[i]].processID + 1 << "\t" << ProcessInfo[IOQueue[i]].ioTime << endl;
		}
	}
	else
	{
		cout << "[empty]" << endl;
	}

	if (FinishedList.size() != 0)
	{
		cout << "-------------" << endl;
		cout << "Completed: ";
		for (int i = 0; i < (int)FinishedList.size(); i++)
		{
			cout << "P" << 1 + FinishedList[i] << "\t";
		}
	}

	cout << endl << endl;
	//system("PAUSE");
}

//Prints a final summery of the simulation. 
void PrintSummery()
{
	float waitTime = 0;
	float turnaroundTime = 0;
	float responaceTime = 0;

	cout << endl << "Finished" << endl << endl << "Total Time: " << CPUCycle << endl;
	cout << "CPU Utilization: " << 100 - (100 * (float)idleTime / (float)CPUCycle) <<"%" << endl << endl;
	cout << "Waiting Times: " << endl << "P1\t P2\t P3\t P4\t P5\t P6\t P7\t P8\t P9" << endl;
	for (int i = 0; i < (int)ProcessInfo.size(); i++)
	{
		cout << ProcessInfo[i].waitTime << "\t";
		waitTime += ProcessInfo[i].waitTime;
	}
	cout << endl;
	cout << "Average Wait Time: " << waitTime/9 << endl << endl;
	cout << "Turnaround Times: " << endl << "P1\t P2\t P3\t P4\t P5\t P6\t P7\t P8\t P9" << endl << endl;
	for (int i = 0; i < (int)ProcessInfo.size(); i++)
	{
		cout << ProcessInfo[i].turnaroundTime << "\t";
		turnaroundTime += ProcessInfo[i].turnaroundTime;
	}
	cout << endl;
	cout << "Average Turnaround Time: " << turnaroundTime/9 << endl << endl;
	cout << "Response Times: " << endl << "P1\t P2\t P3\t P4\t P5\t P6\t P7\t P8\t P9" << endl;
	for (int i = 0; i < (int)ProcessInfo.size(); i++)
	{
		cout << ProcessInfo[i].responseTime << "\t";
		responaceTime+=ProcessInfo[i].responseTime;
	}
	cout << endl << "Average Responce Time: " << responaceTime/9 << endl << endl;
	cout << endl;
}

//Checks the anything is in the Execution area, if not tries to move the top of the ready queue to it
void MoveToExectuion_FCFS()
{

	if (inExecution < 0 && ReadyQueue.size() != 0)
		{
			inExecution = ReadyQueue[0];
			ReadyQueue.erase(ReadyQueue.begin());//This acts as "pop top of queue", has O(N) run time instead of O(1) but its a list of 10 elements so why am I writing this
			ProcessInfo[inExecution].burstTime = ProcessTimes[inExecution][ProcessInfo[inExecution].burstNumber]; //Gets the CPU burst time

			//Check if this is the first CPU burst, if so set the responce time
			if (ProcessInfo[inExecution].responceFlag == false)
			{
				ProcessInfo[inExecution].responseTime = CPUCycle;
				ProcessInfo[inExecution].responceFlag = true;
			}
			PrintFrame(inExecution);
		}
}

//Checks the anything is in the Execution area, if not tries to move the shortest job to it
void MoveToExectuion_SJF()
{
	if (inExecution < 0 && ReadyQueue.size() != 0)
		{
			//sort(ReadyQueue.begin(), ReadyQueue.end());//Sort the Ready Queue so to emulate the "Higher ID goes first in a tie" reasining without needing to check for ID
			
			int shortestJob = ReadyQueue[0];
			int deleteSpot = 0;
			for (int i = 1; i < (int)ReadyQueue.size(); i++)
			{
				//If the process at spot i in the Ready Queue has a shorter run time then the current "Shortest" process, make the process at i the new shortest process
				if (ProcessTimes[ReadyQueue[i]][ProcessInfo[ReadyQueue[i]].burstNumber] < ProcessTimes[shortestJob][ProcessInfo[shortestJob].burstNumber])
				{
					shortestJob = ReadyQueue[i];
					deleteSpot = i;
				}
			}
			
			inExecution = shortestJob;
			ReadyQueue.erase(ReadyQueue.begin() + deleteSpot);//This will remove the job that goes into execution from the ready queue
			ProcessInfo[inExecution].burstTime = ProcessTimes[inExecution][ProcessInfo[inExecution].burstNumber]; //Gets the CPU burst time

			//Check if this is the first CPU burst, if so set the responce time
			if (ProcessInfo[inExecution].responceFlag == false)
			{
				ProcessInfo[inExecution].responseTime = CPUCycle;
				ProcessInfo[inExecution].responceFlag = true;
			}
			PrintFrame(inExecution);
		}
}

//Check if the exectuion area is free, if so grab a task based off of the queue prorities
void MoveToExectuion_MLFQ()
{
	if (inExecution < 0 && (ReadyQueueLevel1.size() != 0 || ReadyQueueLevel2.size() != 0 || ReadyQueueLevel3.size() != 0))
		{
			if (ReadyQueueLevel1.size() != 0)
			{
				inExecution = ReadyQueueLevel1[0];
				ReadyQueueLevel1.erase(ReadyQueueLevel1.begin()); //This acts as "pop top of queue", has O(N) run time instead of O(1) but its a list of 10 elements so why am I writing this
				ProcessInfo[inExecution].MLFQTime = 6;
				ProcessInfo[inExecution].MLFQLevel = 1;
			}
			else if (ReadyQueueLevel2.size() != 0)
			{
				inExecution = ReadyQueueLevel2[0];
				ReadyQueueLevel2.erase(ReadyQueueLevel2.begin());
				ProcessInfo[inExecution].MLFQTime = 12;
				ProcessInfo[inExecution].MLFQLevel = 2;
			}
			else
			{
				inExecution = ReadyQueueLevel3[0];
				ReadyQueueLevel3.erase(ReadyQueueLevel3.begin());
				ProcessInfo[inExecution].MLFQTime = 1024; //This should be "Infinite Time", but we are just going to use a sizeable burst instead
				ProcessInfo[inExecution].MLFQLevel = 3;
			}
			if (ProcessInfo[inExecution].finishedBurst == true)
			{
				ProcessInfo[inExecution].burstTime = ProcessTimes[inExecution][ProcessInfo[inExecution].burstNumber]; //Gets the CPU burst time
				ProcessInfo[inExecution].finishedBurst = false;
			}

			//Check if this is the first CPU burst, if so set the responce time
			if (ProcessInfo[inExecution].responceFlag == false)
			{
				ProcessInfo[inExecution].responseTime = CPUCycle;
				ProcessInfo[inExecution].responceFlag = true;
			}
			PrintFrame_MLFQ(inExecution);
		}
}

//Evalute if the process in execution should be moved to finished or IO. This version is used in FCFS and SJF.
void CheckExecution_MLFQ()
{
		if (inExecution >= 0)
		{
			ProcessInfo[inExecution].executionTime++;
			ProcessInfo[inExecution].burstTime--;
			ProcessInfo[inExecution].MLFQTime--;
			if (ProcessInfo[inExecution].burstTime <= 0)
			{
				ProcessInfo[inExecution].burstNumber++; //Increase the counter for which burst/IO time is next
				ProcessInfo[inExecution].responceFlag = true;

				if (ProcessInfo[inExecution].burstNumber == ProcessTimes[inExecution].size()) //Checks if all Burst and IOs have been done (Finished)
				{
					FinishedList.push_back(inExecution);
					ProcessInfo[inExecution].turnaroundTime = CPUCycle;
				}
				else //If not, move to IO queue
				{
					IOQueue.push_back(inExecution);
					ProcessInfo[inExecution].ioTime = ProcessTimes[inExecution][ProcessInfo[inExecution].burstNumber];
					ProcessInfo[inExecution].finishedBurst = true;
				}
				inExecution = -1;

				//Print some stuff here
			}
			else if (ProcessInfo[inExecution].MLFQTime <= 0) //If the Time Quotent is up, bump down in proirty
			{
				if (ProcessInfo[inExecution].MLFQLevel == 1)
				{
					ReadyQueueLevel2.push_back(inExecution);
				}
				else
				{
					ReadyQueueLevel3.push_back(inExecution);
				}
				inExecution = -1;
			}
			else if (ProcessInfo[inExecution].MLFQLevel == 2 && ReadyQueueLevel1.size() != 0)
			{
				ReadyQueueLevel2.push_back(inExecution);
				inExecution = -1;
			}
			else if (ProcessInfo[inExecution].MLFQLevel == 3 && (ReadyQueueLevel1.size() != 0 || ReadyQueueLevel2.size() != 0))
			{
				ReadyQueueLevel3.push_back(inExecution);
				inExecution = -1;
			}
		}
}

//Evalutes if a process in exectuion should be moved to finished, IO, or to a lower queued. Used by MLFQ.
void CheckExecution()
{
	if (inExecution >= 0)
		{
			ProcessInfo[inExecution].executionTime++;
			ProcessInfo[inExecution].burstTime--;
			if (ProcessInfo[inExecution].burstTime <= 0)
			{
				ProcessInfo[inExecution].burstNumber++; //Increase the counter for which burst/IO time is next

				if (ProcessInfo[inExecution].burstNumber == ProcessTimes[inExecution].size()) //Checks if all Burst and IOs have been done (Finished)
				{
					FinishedList.push_back(inExecution);
					ProcessInfo[inExecution].turnaroundTime = CPUCycle;
				}
				else //If not, move to IO queue
				{
					IOQueue.push_back(inExecution);
					ProcessInfo[inExecution].ioTime = ProcessTimes[inExecution][ProcessInfo[inExecution].burstNumber]; //There is some off by 1 error, going to see if this fixes it (I don't think it will)
				}
				inExecution = -1;

				//Print some stuff here
			}
		}
}

//Updates the IO queue, moving process that have finished their IO bursts back to the ready queues
void UpdateIO(vector <int>& ioDequeueList)
{
	//decrement waiting time in IO queue
	ioDequeueList.clear();
	sort(IOQueue.begin(), IOQueue.end());//This ensure the output lines up with the example
	for (int i = 0; i < (int)IOQueue.size(); i++)
	{
		ProcessInfo[IOQueue[i]].ioTime--;
		ProcessInfo[IOQueue[i]].ioTotalTime++;
		//check if anything in IO queue should move to Ready Queue.
		if (ProcessInfo[IOQueue[i]].ioTime <= 0)
		{
			ReadyQueue.push_back(ProcessInfo[IOQueue[i]].processID);
			ioDequeueList.push_back(i);
			ProcessInfo[IOQueue[i]].burstNumber++;
		}
	}

	//Remove finished IO queue entires from queue
	for (int i = ioDequeueList.size(); i > 0; i--)
	{
		IOQueue.erase(IOQueue.begin() + ioDequeueList.back());
		ioDequeueList.pop_back();
	}
}

//Updates the IO queue, moving process that have finished their IO bursts back to their propper ready queues
void UpdateIO_MLFQ(vector <int>& ioDequeueList)
{
	//decrement waiting time in IO queue
	ioDequeueList.clear();
	sort(IOQueue.begin(), IOQueue.end());//This ensure the output lines up with the example
	for (int i = 0; i < (int)IOQueue.size(); i++)
	{
		ProcessInfo[IOQueue[i]].ioTime--;
		ProcessInfo[IOQueue[i]].ioTotalTime++;
		//check if anything in IO queue should move to Ready Queue.
		if (ProcessInfo[IOQueue[i]].ioTime <= 0)
		{
			//determine which queue the task goes back too
			if (ProcessInfo[IOQueue[i]].MLFQLevel == 1)
			{
				ReadyQueueLevel1.push_back(ProcessInfo[IOQueue[i]].processID);
			}
			else if (ProcessInfo[IOQueue[i]].MLFQLevel == 2)
			{
				ReadyQueueLevel2.push_back(ProcessInfo[IOQueue[i]].processID);
			}
			else
			{
				ReadyQueueLevel3.push_back(ProcessInfo[IOQueue[i]].processID);
			}

			ioDequeueList.push_back(i);
			ProcessInfo[IOQueue[i]].burstNumber++;
		}
	}

	//Remove finished IO queue entires from queue
	for (int i = ioDequeueList.size(); i > 0; i--)
	{
		IOQueue.erase(IOQueue.begin() + ioDequeueList.back());
		ioDequeueList.pop_back();
	}
}

//This is used to sort the process at the very start of SJF
bool InitalSJFSort(int i, int j)
{
	return (ProcessTimes[i][0] < ProcessTimes[j][0]);
}