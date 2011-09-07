#ifndef PARAMETER_OWNER_H
#define PARAMETER_OWNER_H

#include <cci.h>

class parameter_owner : public sc_module
{
	public	:

		SC_CTOR(parameter_owner)
		{
			int_param	=	new cci::cnf::cci_param<int>("int_param");			

			SC_THREAD(run_owner);
		}

		void run_owner (void)
		{
			while(1)
			{
				wait(5.0, SC_NS);
				
				*int_param = 15;
				
				wait(10.0, SC_NS);
			}
		}
		
	private	:

		cci::cnf::cci_param<int>*            int_param;

};

#endif	// End of PARAMETER_OWNER_H
