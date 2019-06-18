#include "header.h"

//Runs a packet transmission simulation
//Written by Artian Kica and Ezra Davis
int main(int argc, char* argv[])
{

	srand(time(NULL));

	//Reading in input:
	if(argc != 5)
	{
		std::cout << "Usage:\n" << argv[0] << " [sources] [receivers] [mules] [dimension]" <<std::endl;
		return 1;
	}
	int sources = std::atoi(argv[1]);
	int receivers = std::atoi(argv[2]);
	int mules = std::atoi(argv[3]);
	int dimension = std::atoi(argv[4]);

	if(DEBUG_MESG) std::cout << "Inputs received:\n" << sources << " " << receivers << " " << mules << " " << dimension << std:: endl;

	//Some input validation
	if(mules % 4 != 0  ||  mules > dimension * dimension)
	{
		std::cout << "Illegal number of Mules." << std::endl;
		return 1;

	}
	if(receivers > dimension)
	{
		std::cout << "There are more Receivers than there is space to place them" << std::endl;
		return 1;
	}
	if(sources > dimension)
	{
		std::cout << "There are more Sources than there is space to place them" << std::endl;
		return 1;
	}
	if(receivers < 0 || mules < 0 || dimension < 0 || sources < 0)
	{
		std::cout << "Negative input values are illegal" << std::endl;
		return 1;
	}
	//End input validation


	std::cout << "Sources are numbered 1 to " << sources\
	 << "\nMules are " << sources + 1 << " to " << sources + mules\
	 << "\n And Receivers are " << sources + mules + 1 << " to " << sources + mules + receivers << std::endl;
/*

*/
	Node::initializeNodes(sources, receivers, mules, dimension);



		//Running simulation:
	int time = 0;
	while (!Node::allDone())
	{
		if(DEBUG_MESG || VERBOSE)
			std::cout << "Time: " << time << std::endl;
		
		Node::nodeStep(time);
		Node::packetActions(time); 
		
		//Keeps packets from waiting one tick in the queue before sending if the queue is empty
		//(Useful for Mule nodes only)
		Node::nodeStep(time); 
		
		if(time % 10 == 0 && time != 0)
		{
			Node::moveStep();
		}
		if(time % 100 == 0)
		{
			std::cout << "Field at time " << time << ":\t\t(After Mules have moved)" << std::endl;
			Node::field.printField(std::cout);
		}


		time++;
	}



		//Reporting on final state:
	if(DEBUG_MESG) std::cout << "the eventlist is " << Node::eventList.length() << " packets long\n";
	//if(DEBUG_MESG) std::cout << "The receiver's queue is " << r1->queueLength()<< " packets long" << std::endl;
	
	std::cout << "\nSimulation results:\n" << std::endl;

	Node::printResults();

	std::cout << "Time of the end of simulation in sims: " << time << std::endl;

	std::cout << "All units of time are in 'sims' (100 millisecond increments)" << std::endl;

	std::cout << std::endl;
}
