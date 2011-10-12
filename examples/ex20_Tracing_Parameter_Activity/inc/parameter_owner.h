#ifndef PARAMETER_OWNER_H
#define PARAMETER_OWNER_H

#include <cci.h>
#include "systemc.h"

class parameter_owner : public sc_module
{
	public	:

		SC_CTOR(parameter_owner)
		{	
			std::cout << "\nPrior to " << sc_time_stamp() << std::endl;				

			std::cout << "\n\t[OWNER C_TOR] : Defining new integer type cci-parameter with default value 10" << std::endl;
			int_param	=	new cci::cnf::cci_param<int>("int_param", 10);			

			SC_THREAD(run_owner);
		}


		~parameter_owner()
		{

		}

		void run_owner (void)
		{
			while(1)
			{
				std::cout << "\n@ " << sc_time_stamp() << std::endl;				
				*int_param = 15;
				
				wait(5.0, SC_NS);
				std::cout << "@ " << sc_time_stamp() << std::endl;

				std::cout << "\n\t[OWNER] : Defining new float type cci-parameter with no default value" << std::endl;
				float_param	=	new cci::cnf::cci_param<float>("float_param", 12.345);	

				wait(5.0, SC_NS);

				delete int_param;

				wait(10.0, SC_NS);

			}// End of WHILE

		}// End of SC_THREAD

		
	private	:

		cci::cnf::cci_param<int>*            int_param;
		cci::cnf::cci_param<float>*          float_param;

};// End of PARAMETER_OWNER SC_MODULE

#endif	// End of PARAMETER_OWNER_H
