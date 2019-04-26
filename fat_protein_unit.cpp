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

fat_protein_unit eval;

//read setting file
eval.set_file_settings();

eval.set_input();
eval.print_input();
////////////////////////////////////////////////////////////////
// calculations
////////////////////////////////////////////////////////////////

eval.calc_fast_ins();

eval.calc_correction();

//amount of calories from fat and proteins
eval.calc_fpf_calories();

eval.calc_slow_ins();

eval.tot_calories();

eval.calc_fpu();

eval.injection_time ();

eval.print_output();
}