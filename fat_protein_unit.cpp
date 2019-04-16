#include <iostream>
#include <limits>
#include <iomanip>
#include <vector>
#include <ctime>
#include <locale>
#include <fstream>
//#include <cstdio>

#include "fat_protein_unit.hpp"




bool policy_food(double &food_type){
bool result = false;
if (!(std::cin >> food_type) or food_type < 0. )
    result = true;

return result;
}


bool policy_glycemia(unsigned int &glycemia){
bool result = false;

if (!(std::cin >> glycemia) or glycemia <= 10 or glycemia > 650)
    result = true;

return result;
}


bool read_food_type(double &food_type)
{
    bool output_error;
    if ( policy_food(food_type) )
    {
        output_error = true;
        std::cerr << "Please enter positive numbers only." << std::endl;
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
    else
    {
        output_error = false;
    }
    return output_error;
}

bool read_glycemia(unsigned int &glycemia)
{
    bool output_error;
        if (policy_glycemia(glycemia))
    {
        output_error = true;
        std::cerr << "Please enter integer positive numbers only." << std::endl;
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
    else
    {
        output_error = false;
    }
    return output_error;
}

int main()//int argc, char *argv[] )
{
/*FPU caluclation library

*/
time_t now = time(0);
tm *ltm = localtime(&now);
std::locale loc;
//TODO set the default to italian
loc = std::locale (loc, "", std::locale::ctype);; //("it_IT.UTF8");


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
//TODO read from json/xml file
//TODO make it const

//read setting file
std::ifstream infile("settings.config", std::ifstream::in);

//check if there is the file
if ( !infile ) {
    std::cerr << "not found the settings file!" << std::endl;
    exit(127);
}
else{
    infile.imbue(loc);

    //check if the file is empty
    char c = infile.get();
    if (c != '/'){
        std::cerr << "the settings file is empty!" << std::endl;
        exit(127);
    }
    else{
        //load the data in the file
        while (infile.good()) {
        //std::cout << c << " pippo" << std::endl;
        c = infile.get();
        //check if number or not and store it

        }
    }
    infile.close();
}
std::cin.imbue(loc);
std::cout.imbue(loc);


std::vector<unsigned int> sensitivity (24);      // mg/dl - per unit
std::vector<double> ratio_ins_carb (24.);    // gr/U - gr of sugar reduced by one unit of insulin
std::vector<double> ratio_ins_fp_carb (24.);   // ratio_ins_fp_carb = ratio_ins_carb / 2

double fpu = 0.;                      // unit of fat and proteins 
double carbohydrate = 0.;             // gr of carbohydrates
double protein = 0.;                  // gr of proteins
double fat = 0.;                      // gr of fat
double fiber = 0.;                    // gr of fibers
double fast_insulin = 0.;             // unit of insulin from carbohydrate
double ins_correction = 0.;
double slow_insulin = 0.;             // unit of insulin from fat and protein
unsigned int glycemia = 100;
int time_of_inj = 0;              // min - minutes of injection of slow insulin
double total_calories = 0.;
double fp_calories = 0.;
bool  output_error = true;

for (unsigned int i = 0; i < 24; ++i)
{
    ratio_ins_carb[i]=12.;
    ratio_ins_fp_carb[i]=24.;
    sensitivity[i]=100;
}

//TODO remake with a templatized function
while (output_error==true)
{
    std::cout << " Quantity in grams of Carbohydrates: " << std::endl;
    output_error = read_food_type(carbohydrate);
}

output_error = true;
while (output_error==true)
{
    std::cout << " Quantity in grams of Proteins: " << std::endl;
    output_error = read_food_type(protein);
}

output_error = true;
while (output_error==true)
{
    std::cout << " Quantity in grams of Fat: " << std::endl;
    output_error = read_food_type(fat);
}

output_error = true;
while (output_error==true)
{
    std::cout << " Quantity in grams of Soluble Fibers: (put 0 if you do not know)" << std::endl;
    output_error = read_food_type(fiber);
}

output_error = true;
while (output_error==true)
{
    std::cout << " Level of glycemia in mg/dl: " << std::endl;
    output_error = read_glycemia(glycemia);
}

std::cout << " =========================================================== " << std::endl;
std::cout << " Input data:" << std::endl;
std::cout << " =========================================================== " << std::endl;
std::cout << " Carbohydrates:" << carbohydrate << " g" << std::endl;
std::cout << " Proteins:" << protein << " g" << std::endl;
std::cout << " Fat:" << fat << " g" << std::endl;
std::cout << " Fiber:" << fiber << " g" << std::endl;
std::cout << " Glicemia:" << glycemia << " mg/dl" << std::endl;
std::cout << " =========================================================== " << std::endl;
////////////////////////////////////////////////////////////////
// caculations
////////////////////////////////////////////////////////////////

fast_insulin = carbohydrate / ratio_ins_carb[ltm->tm_hour];

if ((glycemia - 140) > 0)
{
    ins_correction = (glycemia - 100) / sensitivity[ltm->tm_hour];
}

// table fo calories vs carbohydrates, fat and proteins
// carbohydrates: 1g = 4 kcal
// proteins: 1g = 4 kcal
// fat: 1g = 9 kcal
// fiber: 1g = 2 kcal only in case of soluble fiber

//amount of calories from fat and proteins
fp_calories = (4. * protein) + ( 9. * fat ) + (2. * fiber);

slow_insulin = (protein + (2.25 * fat) + (0.5 * fiber) ) / ratio_ins_fp_carb[ltm->tm_hour]; //(fp_calories / 4.) / ratio_ins_fp_carb

total_calories = fp_calories + (4. * carbohydrate);
fpu = fp_calories / 100.;  //TODO or divided by sensitivity ??

//TODO remake in a decent way
if ( fpu > 4. )
{
    time_of_inj = 8;
}
else
{
    if ( fpu > 3. )
    {
        time_of_inj = 5;
    }
    else
    {
        if ( fpu > 2. )
        {
            time_of_inj = 4;
        }
        else
        {
            if ( fpu > 1. )
            {
                time_of_inj = 3;
            }
            else
            {
                if ( fpu == 0. )
                {
                    time_of_inj = 0;
                }
                else
                {
                    time_of_inj = 2;
                }
            }
        }
    }
}

//TODO automaticcaly detect , or .


std::cout << " =========================================================== " << std::endl;
std::cout << " Bolus Insulin to inject now: " << std::setprecision(4) << std::setfill('0') << fast_insulin << " Units " << std::endl;
std::cout << " Correction Insulin to inject now: " << ins_correction << " Units " << std::endl;
std::cout << " =========================================================== " << std::endl;
std::cout << " Total Insulin to inject now: " << fast_insulin + ins_correction << " Units " << std::endl;   
std::cout << " =========================================================== " << std::endl;
std::cout << " Sensitivity used: " << sensitivity[ltm->tm_hour] << " mg/dl per Unit  " << std::endl;
std::cout << " Ratio Carbohydrate/Insulin used: " << ratio_ins_carb[ltm->tm_hour] << " grams per Unit  " << std::endl;
std::cout << " It is " << ltm->tm_hour << " o'clock" << std::endl;
std::cout << " =========================================================== " << std::endl;
std::cout << " FPU eaten: " << fpu << std::endl;
std::cout << " Total amount of energy eaten: " << total_calories << " kcal - " << 4.182 * total_calories << " kJoule " << std::endl;
std::cout << " =========================================================== " << std::endl;
std::cout << " Bolus Insulin to inject delayed: " << slow_insulin << " Units over " << time_of_inj  << " Hours" << std::endl;
std::cout << " =========================================================== " << std::endl;
std::cout << " Total Insulin to inject: " << fast_insulin + ins_correction  + slow_insulin << " Units " << std::endl;
std::cout << " =========================================================== " << std::endl;
}
