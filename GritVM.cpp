#include <list>
#include <string>
#include <vector>
#include <fstream>
#include <iostream>

#include "GritVM.hpp"

using namespace std;

//Load function
STATUS GritVM::load(const std::string filename, const std::vector<long> &initialMemory)
{
	if(machineStatus != WAITING)
	{
		return machineStatus;
	}

	//file opening
	ifstream inFile;
	inFile.open(filename);

	if (!inFile.good())
	{
		throw("Can't open the file!"); //if file is not opening
		
	}
	else
	{
		machineStatus = READY;
					
		while (getline(inFile, line)) // read line in the file
		{
			if(line.empty() || line[0] == '#') 
			{
				continue; //if the line is blank or # is the first character of the line, then skip the line
			}
		
			Instruction instruct = GVMHelper::parseInstruction(line);
		
			if(instruct.operation == UNKNOWN_INSTRUCTION) 
			{
				return machineStatus = ERRORED;
			}

			instructMem.push_back(GVMHelper::parseInstruction(line)); // push the instruction into instructMem
		}
	}

	//copy initialMemory to dataMem
	dataMem = initialMemory;
	
	if(instructMem.size() != 0)
	{
		machineStatus = READY;
	}
	else
	{
		machineStatus = WAITING;	
	}
	
	return machineStatus;		
}

//Run function
STATUS GritVM::run()
{
	switch(machineStatus)
	{
		case READY: 
			machineStatus = RUNNING; 
			break;

		case WAITING:  
			break;

		case RUNNING:  
			break;

		case HALTED:  
			GritVM::reset(); 
			break;
				
		case ERRORED: 
			GritVM::reset(); 
			break;
		
		case UNKNOWN:  
			GritVM::reset(); 
			break;		
	}
 
	currentInstruct = instructMem.begin(); //start currentInstruct with first index of instructMem

	while(machineStatus == RUNNING) 
	{
		if(currentInstruct == instructMem.end()) //means null instructMem
		{
			machineStatus = HALTED;
		}
		
		switch(currentInstruct -> operation)
		{		
			case CLEAR:
				accumulator = 0;
				break;

			case AT:
				accumulator = dataMem.at(currentInstruct -> argument); 
				break;

			case SET: 
				dataMem.at(currentInstruct -> argument) = accumulator;	
				break;

			case INSERT:
				dataMem.insert(dataMem.begin() + (currentInstruct -> argument), accumulator);				
				break;

			case ERASE:		
				dataMem.erase(dataMem.begin() + currentInstruct -> argument);	
				break;

			case ADDCONST:
				accumulator += currentInstruct -> argument; 
				break;

			case SUBCONST:
				accumulator -= currentInstruct -> argument; 
				break;

			case MULCONST:
				accumulator *= currentInstruct -> argument; 
				break;	

			case DIVCONST:
				accumulator /= currentInstruct -> argument; 
				break;

			case ADDMEM:
				accumulator += dataMem.at(currentInstruct -> argument); 
				break;

			case SUBMEM:
				accumulator -= dataMem.at(currentInstruct -> argument); 
				break;

			case MULMEM:
				accumulator *= dataMem.at(currentInstruct -> argument);
				break;

			case DIVMEM:
				accumulator /= dataMem.at(currentInstruct -> argument); 
				break;

			case JUMPREL:
				if(currentInstruct -> argument == 0)
				{
					continue;
				}

				if(currentInstruct -> argument < 0)
				{
					for(int i = currentInstruct -> argument; i <= 0; ++i)
					{
						currentInstruct--;
					}	
				}
				else
				{					
					for(int i = currentInstruct -> argument; i >= 0; --i)
					{
						currentInstruct++;
					}
				}
				break;
				
			case JUMPZERO:

				if(accumulator == 0)
				{		
					if(currentInstruct -> argument < 0)
					{
						for(int i = currentInstruct -> argument; i < 0; ++i)
						{
							currentInstruct--;
						}
						currentInstruct--;
					}
					else
					{
						for(int i = currentInstruct -> argument; i > 0; --i)
						{
							currentInstruct++;
						}
						currentInstruct--;
					}
				}
				break;
			
			case JUMPNZERO:
				if(accumulator != 0)
				{
					if(currentInstruct -> argument < 0)
					{
						for(int i = currentInstruct -> argument; i < 0; ++i)
						{
							currentInstruct--;
						}
						currentInstruct--;
					}
					else
					{
						for(int i = currentInstruct -> argument; i > 0; --i)
						{
							currentInstruct++;
						}
						currentInstruct--;
					}
				}
				break;
				
			case NOOP: break;
				
			case HALT:
				machineStatus = HALTED;
				break;
			
			case OUTPUT: break;
				
			case CHECKMEM:
				if(int(currentInstruct -> argument) <= int(dataMem.size()))
				{
					break;
				}
				else
				{
					machineStatus = ERRORED;
				}
				break; 	

			case UNKNOWN_INSTRUCTION:
				machineStatus = ERRORED;
				break;

			default: 
				machineStatus = HALTED;
				break;	
		}

		currentInstruct++;	
			
		if(currentInstruct == instructMem.end())
		{
			machineStatus = HALTED;
		}	
	}

	return machineStatus;
}



//getDataMem function
std::vector<long> GritVM::getDataMem()
{
    return dataMem;
}

STATUS GritVM::reset()
{
    accumulator = 0; //set accumulator to be 0
    machineStatus = WAITING; //set machine status to be WAITING
    dataMem.clear(); //clear data memory
    instructMem.clear(); //clear instruction memory

    return machineStatus;
}















