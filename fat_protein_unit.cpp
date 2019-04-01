#include <iostream>
#include <limits>
#include <iomanip>
#include <vector>
#include <ctime>
#include <locale>
//#include "fat_protein_unit.hpp"

int main()
{
/*FPU caluclation library

*/

//const double time_of_ins = 240.;      // min - minutes of duration of insulin in the blood
//TODO read from json/xml file
//TODO make it const
std::vector<int> sensitivity (24);      // mg/dl - per unit 
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
int glicemia = 100;
int time_of_inj = 0;              // min - minutes of injection of slow insulin
double total_calories = 0.;
double fp_calories = 0.;
bool  output_error = true;

for (int i = 0; i < 24; ++i)
{
    ratio_ins_carb[i]=12.;
    ratio_ins_fp_carb[i]=24.;
    sensitivity[i]=100;
}

time_t now = time(0);
tm *ltm = localtime(&now);
std::locale loc;
//TODO set the default to italian
loc = std::locale (loc, "", std::locale::ctype);; //("it_IT.UTF8");


std::cin.imbue(loc);
std::cout.imbue(loc);

//TODO remake with a templatized function
while (output_error==true)
{
    std::cout << " Quantity in grams of Carbohydrates: " << std::endl;
    if (!(std::cin >> carbohydrate) or carbohydrate < 0. )
    {
        output_error = true;
        std::cout << "Please enter positive numbers only." << std::endl;
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
    else
    {
        output_error = false;
    }
}

output_error = true;
while (output_error==true)
{
    std::cout << " Quantity in grams of Proteins: " << std::endl;
    if (!(std::cin >> protein) or protein < 0. )
    {
        output_error = true;
        std::cout << "Please enter positive numbers only." << std::endl;
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
    else
    {
        output_error = false;
    }
}

output_error = true;
while (output_error==true)
{
    std::cout << " Quantity in grams of Fat: " << std::endl;
    if (!(std::cin >> fat) or fat < 0. )
    {
        output_error = true;
        std::cout << "Please enter positive numbers only." << std::endl;
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
    else
    {
        output_error = false;
    }
}

output_error = true;
while (output_error==true)
{
    std::cout << " Quantity in grams of Soluble Fibers: (put 0 if you do not know)" << std::endl;
    if (!(std::cin >> fiber) or fiber < 0. )
    {
        output_error = true;
        std::cout << "Please enter positive numbers only." << std::endl;
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
    else
    {
        output_error = false;
    }
}

output_error = true;
while (output_error==true)
{
    std::cout << " Level of glicemia in mg/dl: " << std::endl;
    if (!(std::cin >> glicemia) or glicemia <= 10 or glicemia > 650)
    {
        output_error = true;
        std::cout << "Please enter integer positive numbers only." << std::endl;
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
    else
    {
        output_error = false;
    }
}

std::cout << " =========================================================== " << std::endl;
std::cout << " Input data:" << std::endl;
std::cout << " =========================================================== " << std::endl;
std::cout << " Carbohydrates:" << carbohydrate << " g" << std::endl;
std::cout << " Proteins:" << protein << " g" << std::endl;
std::cout << " Fat:" << fat << " g" << std::endl;
std::cout << " Fiber:" << fiber << " g" << std::endl;
std::cout << " Glicemia:" << glicemia << " mg/dl" << std::endl;
std::cout << " =========================================================== " << std::endl;
////////////////////////////////////////////////////////////////
// caculations
////////////////////////////////////////////////////////////////

fast_insulin = carbohydrate / ratio_ins_carb[ltm->tm_hour];

if ((glicemia - 140) > 0)
{
    ins_correction = (glicemia - 100) / sensitivity[ltm->tm_hour];
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