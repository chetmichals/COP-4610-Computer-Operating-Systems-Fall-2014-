#include <iostream>
#include <vector>
#include <algorithm> 
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
	bool responceFlag;
};
void setProcessTimes();
void InitilzeProcessInfo();
void FCFS();
void PrintFrame(int inExecution);
void PrintSummery();

vector <vector<int>> ProcessTimes;
vector <struct_ProcessInfo> ProcessInfo;
vector <int> ReadyQueue;
vector <int> IOQueue;
vector <int> FinishedList;

int inExecution;
int CPUCycle;

int main()
{
	cout << "Hello World! ";
	cout << "I'm a C++ program";
	FCFS();
	PrintSummery();
	system("PAUSE");
}

//Inializees the ProcessTimes Vector. I feel this is a crummy way but it does the job.
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

		ProcessInfo.push_back(newProcess);
		ReadyQueue.push_back(i);
	}
	inExecution = -1;
	CPUCycle = 0;
}


//There seems to be a systamtic off by 1 error in the IO wait time. I placed a bunch of band aids, but not totally sure the root cause
void FCFS()
{
	//Put all Processes into ready queue
	setProcessTimes();
	InitilzeProcessInfo();
	vector <int> ioDequeueList;

	//while wait queue and ready queue are not empty
	while (ReadyQueue.size() != 0 || IOQueue.size() != 0 || inExecution > 0)
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
		for (int i = 0; i < ReadyQueue.size(); i++)
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

void PrintFrame(int inExecution)
{
	cout << endl << "Current Time: " << CPUCycle << endl;
	cout << "Now Running: P" << 1 + inExecution << endl;
	cout << "-------------" << endl;
	cout << "Ready Queue:  Process    Burst" << endl;
	for (int i = 0; i < ReadyQueue.size(); i++)
	{
		cout << "\t\tP" << ProcessInfo[ReadyQueue[i]].processID + 1 << "\t" << ProcessTimes[ReadyQueue[i]][ProcessInfo[ReadyQueue[i]].burstNumber] << endl;
	}
	cout << "-------------" << endl;
	cout << "Now in I/O:   Process    Remaining I/O time" << endl;
	for (int i = 0; i < IOQueue.size(); i++)
	{
		cout << "\t\tP" << ProcessInfo[IOQueue[i]].processID + 1 << "\t" << ProcessInfo[IOQueue[i]].ioTime << endl;
	}
	cout << "-------------" << endl;
	cout << "Completed: ";
	for (int i = 0; i < FinishedList.size(); i++)
	{
		cout << "P" << 1 + FinishedList[i] << "\t";
	}

	cout << endl << endl;

}

void PrintSummery()
{
	cout << endl << "Finished" << endl << endl << "Total Time: " << CPUCycle << endl;
	cout << "Waiting Times: P1\t P2\t P3\t P4\t P5\t P6\t P7\t P8" << endl << "\t\t";
	for (int i = 0; i < ProcessInfo.size(); i++)
	{
		cout << ProcessInfo[i].waitTime << "\t";
	}

	cout << "Turnaround Times: P1\t P2\t P3\t P4\t P5\t P6\t P7\t P8" << endl << "\t\t";
	for (int i = 0; i < ProcessInfo.size(); i++)
	{
		cout << ProcessInfo[i].turnaroundTime << "\t";
	}
	cout << "Response Times: P1\t P2\t P3\t P4\t P5\t P6\t P7\t P8" << endl << "\t\t";
	for (int i = 0; i < ProcessInfo.size(); i++)
	{
		cout << ProcessInfo[i].responseTime << "\t";
	}
}