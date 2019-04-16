#include <iostream>
#include <limits>
#include <iomanip>
#include <vector>
#include <ctime>

bool policy_food(double food_type);
bool policy_glycemia(unsigned int glycemia);

bool read_food_type(double food_type);

bool read_glycemia(unsigned int glycemia);

/*class fat_protein_unit
{
//TODO replace sensitivity and ratio_ins_carb with a vector of 24 elements (every 30 min)
//TODO calculate insulin based of the current time with the current sensitivity and ratio_ins_carb
//const double time_of_ins = 240.;      // min - minutes of duration of insulin in the blood
//TODO read from json/xml file
//TODO make it const
private:
std::vector<double> sensitivity (24);      // mg/dl - per unit 
std::vector<double> ratio_ins_carb (24);    // gr/U - gr of sugar reduced by one unit of insulin
std::vector<double> ratio_ins_fp_carb (24);   // ratio_ins_fp_carb = ratio_ins_carb / 2

double fpu = 0.;                      // unit of fat and proteins 
double carbohydrate = 0.;             // gr of carbohydrates
double protein = 0.;                  // gr of proteins
double fat = 0.;                      // gr of fat
//double fiber = 0.;                    // gr of fibers
double fast_insulin = 0.;             // unit of insulin from carbohydrate
double ins_correction = 0.;
double slow_insulin = 0.;             // unit of insulin from fat and protein
double glicemia = 100.;
double time_of_inj = 0.;              // min - minutes of injection of slow insulin
double total_calories = 0.;
double fp_calories = 0.;
bool  output_error = true;
public:
    fat_protein_unit()
    {
    	for (int i = 0; i < 24; ++i)
    		{
    			ratio_ins_carb[i]=12.;
    			ratio_ins_fp_carb[i]=24.;
    			sensitivity[i]=100.;
    		}
    }
    ~fat_protein_unit();
    input_file();
    output(){
    	std::cout << " =========================================================== " << std::endl;
    	std::cout << " Bolus Insulin to inject now: " << std::setprecision(3) << std::setfill('0') << fast_insulin << " Units " << std::endl;
    	std::cout << " Correction Insulin to inject now: " << ins_correction << " Units " << std::endl; 
    	std::cout << " =========================================================== " << std::endl;
std::cout << " Total Insulin to inject now: " << fast_insulin + ins_correction << " Units " << std::endl;   
std::cout << " =========================================================== " << std::endl;
std::cout << " Sensitivity used: " << sensitivity[ltm->tm_hour] << " mg/dl per Unit  " << std::endl;
std::cout << " Ratio Carbohydrate/Insulin used: " << ratio_ins_carb[ltm->tm_hour] << " grams per Unit  " << std::endl;
std::cout << " It is " << ltm->tm_hour << " Hour" << std::endl;
std::cout << " =========================================================== " << std::endl;
std::cout << " FPU eaten: " << fpu << std::endl;
std::cout << " Total amount of calories eaten: " << total_calories << " Kcal " << std::endl;
std::cout << " =========================================================== " << std::endl;
std::cout << " Bolus Insulin to inject delayed: " << slow_insulin << " Units over " << time_of_inj  << " hours" << std::endl;
std::cout << " =========================================================== " << std::endl;
std::cout << " Total Insulin to inject: " << fast_insulin + ins_correction  + slow_insulin << " Units " << std::endl;
std::cout << " =========================================================== " << std::endl;
    }

    
};*/