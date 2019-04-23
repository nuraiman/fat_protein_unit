#include "fat_protein_unit.hpp"

int main()//int argc, char *argv[] )
{
/*FPU calculation library
*/

//TODO set the default to italian
//loc = std::locale (loc, "", std::locale::ctype);; //("it_IT.UTF8");

/*if( argc == 2 ){
    printf("The argument supplied is %s\n", argv[1]);
}
else if( argc > 2 ) {
    printf("Too many arguments supplied.\n");
}
else {
printf("One argument expected.\n");
}*/

//const double time_of_ins = 240.;      // min - minutes of duration of insulin in the blood

fat_protein_unit nur;

//read setting file
nur.set_file_settings();

nur.set_input();
nur.get_input();
////////////////////////////////////////////////////////////////
// calculations
////////////////////////////////////////////////////////////////

nur.calc_fast_ins();

nur.calc_correction();

//amount of calories from fat and proteins
nur.calc_fpf_calories();

nur.calc_slow_ins();

nur.tot_calories();

nur.calc_fpu();

nur.injection_time ();

nur.get_output();
}