#ifndef GRITVM_H
#define GRITVM_H

#include <string>
#include <iterator>
#include <vector>
#include <list>

#include "GritVMBase.hpp"

using namespace std;

class GritVM : public GritVMInterface
{
	public:
		//default state
		GritVM()
    		{
			accumulator = 0;        		
			machineStatus = WAITING;		
    		}

		STATUS load(const std::string filename, const std::vector<long> &initialMemory);
		STATUS run();
		std::vector<long> getDataMem();
		STATUS reset();
	
	private:
		vector<long> dataMem;
		list<Instruction> instructMem;
		list<Instruction>::iterator currentInstruct;
		STATUS machineStatus;
		string line;
		long accumulator;		
};

#endif 


