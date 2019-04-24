#include <iostream>
#include <limits>
#include <iomanip>
#include <ctime>
#include <locale>
#include <fstream>
#include<cmath>

class fat_protein_unit
{
//TODO read from json/xml file
private:

std::array<unsigned int,24> sensitivity;   // mg/dl - per unit
std::array<double,24> ratio_ins_carb;      // gr/U - gr of sugar reduced by one unit of insulin
std::array<double,24> ratio_ins_fp_carb;   // ratio_ins_fp_carb = ratio_ins_carb / 2

std::array<double,10> data;
// double fpu;                     //0 unit of fat and proteins
// double carbohydrate;            //1 gr of carbohydrates
// double protein;                 //2 gr of proteins
// double fat;                     //3 gr of fat
// double fiber;                   //4 gr of fibers
// double fast_insulin;            //5 unit of insulin from carbohydrate
// double ins_correction;          //6
// double slow_insulin;            //7 unit of insulin from fat and protein
// double total_calories;          //8
// double fp_calories;             //9

unsigned int glycemia;
unsigned int time_of_inj;       // min - minutes of injection of slow insulin

bool  output_error;

time_t now;
tm *ltm;

std::locale loc;

public:

    fat_protein_unit()
    {
        ratio_ins_carb.fill(12.);
        ratio_ins_fp_carb.fill(24.);
        sensitivity.fill(100);
        data.fill(0.);

    glycemia = 100;
    time_of_inj = 0;
    output_error = true;

    now = time(0);
    ltm = localtime(&now);
    
     loc = std::locale (loc, "", std::locale::ctype);

     std::cin.imbue(loc);
     std::cout.imbue(loc);
    }

    ~fat_protein_unit(){};

    //read setting file
    //TODO read from json/xml file
    void set_file_settings(){
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
    }

   void set_input(){
    //TODO remake with a templatized function
    while (output_error==true)
{
    std::cout << " Quantity in grams of Carbohydrates: " << std::endl;
    output_error = read_food_type(data[1]); //carbohydrate
}

output_error = true;
while (output_error==true)
{
    std::cout << " Quantity in grams of Proteins: " << std::endl;
    output_error = read_food_type(data[2]); //protein
}

output_error = true;
while (output_error==true)
{
    std::cout << " Quantity in grams of Fat: " << std::endl;
    output_error = read_food_type(data[3]); //fat
}

output_error = true;
while (output_error==true)
{
    std::cout << " Quantity in grams of Soluble Fibers: (put 0 if you do not know)" << std::endl;
    output_error = read_food_type(data[4]); //fiber
}

output_error = true;
while (output_error==true)
{
    std::cout << " Level of glycemia in mg/dl: " << std::endl;
    output_error = read_glycemia(glycemia); 
}
}


void get_input(){
    std::cout << " =========================================================== " << std::endl;
    std::cout << " Input data:" << std::endl;
    std::cout << " =========================================================== " << std::endl;
    std::cout << " Carbohydrates:" << data[1] << " g" << std::endl; //carbohydrate
    std::cout << " Proteins:" << data[2] << " g" << std::endl; //protein
    std::cout << " Fat:" << data[3] << " g" << std::endl; //fat
    std::cout << " Fiber:" << data[4] << " g" << std::endl; //fiber
    std::cout << " Glicemia:" << glycemia << " mg/dl" << std::endl;
    std::cout << " =========================================================== " << std::endl;
    }

bool policy_food(double &food_type){
bool result = false;
if (!(std::cin >> food_type) or food_type < 0. )
    result = true;

return result;
}


bool policy_glycemia( unsigned int &glycemia){
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

void calc_fast_ins(){
    calc_fast_ins( data[5], data[1], ratio_ins_carb[ltm->tm_hour]);//fast_insulin,carbohydrate
}

void calc_fast_ins(double &fast_insulin, double carbohydrate, double curr_ratio_ins_carb)
{
    fast_insulin = carbohydrate / curr_ratio_ins_carb;

    if (std::isnan(fast_insulin))
    {
        std::cerr << "Error: current ratio insulin-carbohydrates is zero!!" << std::endl;
        exit(127);
    }
}


void calc_correction(){
    calc_correction( data[6], glycemia, sensitivity[ltm->tm_hour]); //ins_correction
}

void calc_correction( double &ins_correction, unsigned int glycemia, unsigned int curr_sensitivity)
{
    if ((glycemia - 140) > 0)
    {
        ins_correction = (glycemia - 100) / curr_sensitivity;
        if (std::isnan(ins_correction))
        {
            std::cerr << "Error: current sensitivity is zero!!" << std::endl;
            exit(127);
        }
    }
    else ins_correction = 0.;
}

void calc_fpf_calories (){
    fpf_calories (data.back(), data[2], data[3], data[4]); //fp_calories, protein,fat,fiber
}

// table of calories vs carbohydrates, fat and proteins
// carbohydrates: 1g = 4 kcal
// proteins: 1g = 4 kcal
// fat: 1g = 9 kcal
// fiber: 1g = 2 kcal only in case of soluble fiber

void fpf_calories ( double &fp_calories, double protein, double fat, double fiber){
    fp_calories = (4. * protein) + ( 9. * fat ) + (2. * fiber);
}

void calc_slow_ins() {
    calc_slow_ins(data[7], data[2], data[3], data[4], ratio_ins_fp_carb[ltm->tm_hour]); //slow_insulin, protein,fat,fiber
}

void calc_slow_ins(double &slow_insulin, double protein, double fat, double fiber, double ratio_ins_fp_carb) {
slow_insulin = (protein + (2.25 * fat) + (0.5 * fiber) ) / ratio_ins_fp_carb; //(fp_calories / 4.) / ratio_ins_fp_carb
}

void calc_fpu(){
    calc_fpu(data.front(), data.back()); //fpu,fp_calories
}

//TODO or divided by sensitivity ??
void calc_fpu(double &fpu, double fp_calories){
    fpu = fp_calories / 100.; 
}

void tot_calories(){
    tot_calories(data[8], data.back(), data[1]);//total_calories, fp_calories,carbohydrate
}

void tot_calories(double &total_calories, double fp_calories, double carbohydrate){
    total_calories = fp_calories + (4. * carbohydrate);
}

void injection_time (){
    injection_time (time_of_inj, data.front());//fpu
}

//TODO remake in a decent way
void injection_time (unsigned int &time_of_inj, double fpu)
{
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
}

//TODO automatically detect , or .
void  get_output(){
std::cout << " =========================================================== " << std::endl;
std::cout << " Bolus Insulin to inject now: " << std::setprecision(4) << std::setfill('0') << data[5] << " Units " << std::endl; //fast_insulin
std::cout << " Correction Insulin to inject now: " << data[6] << " Units " << std::endl; //ins_correction
std::cout << " =========================================================== " << std::endl;
std::cout << " Total Insulin to inject now: " << data[5] + data[6] << " Units " << std::endl; //fast_insulin, ins_correction
std::cout << " =========================================================== " << std::endl;
std::cout << " Sensitivity used: " << sensitivity[ltm->tm_hour] << " mg/dl per Unit  " << std::endl;
std::cout << " Ratio Carbohydrate/Insulin used: " << ratio_ins_carb[ltm->tm_hour] << " grams per Unit  " << std::endl;
std::cout << " It is " << ltm->tm_hour << " o'clock" << std::endl;
std::cout << " =========================================================== " << std::endl;
std::cout << " FPU eaten: " << data.front() << std::endl; //fpu
std::cout << " Total amount of energy eaten: " << data[8] << " kcal - " << 4.182 * data[8] << " kJoule " << std::endl; //total_calories
std::cout << " =========================================================== " << std::endl;
std::cout << " Bolus Insulin to inject delayed: " << data[7] << " Units over " << time_of_inj  << " Hours" << std::endl; //slow_insulin
std::cout << " =========================================================== " << std::endl;
std::cout << " Total Insulin to inject: " << data[5] + data[6]  + data[7] << " Units " << std::endl; //fast_insulin, ins_correction,slow_insulin
std::cout << " =========================================================== " << std::endl;
}  
};