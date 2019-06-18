

#include "Object.h"


class Instruction : public df::Object{
	private:
		bool hasSwapped;
		bool hasCreated;
	public:
		Instruction();
		int eventHandler(const df::Event *p_e);

};