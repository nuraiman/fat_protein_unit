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

    inline double& get_fpu(){
        return data.front();
    }
    inline double&  get_carbohydrate(){
        return data[1];
    }

    inline double& get_proteins(){
        return data[2];
    }

    inline double& get_fat(){
        return data[3];
    }

    inline double& get_fibers(){
        return data[4];
    }

    inline double& get_fast_ins(){
        return data[5];
    }

    inline double& get_ins_corr(){
        return data[6];
    }

    inline double& get_slow_ins(){
        return data[7];
    }

    inline double& get_tot_cal(){
        return data[8];
    }

    inline double& get_fp_cal(){
        return data.back();
    }

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
    output_error = this->read_food_type(this->get_carbohydrate()); //carbohydrate
}

output_error = true;
while (output_error==true)
{
    std::cout << " Quantity in grams of Proteins: " << std::endl;
    output_error = this->read_food_type(this->get_proteins()); //protein
}

output_error = true;
while (output_error==true)
{
    std::cout << " Quantity in grams of Fat: " << std::endl;
    output_error = this->read_food_type(this->get_fat()); //fat
}

output_error = true;
while (output_error==true)
{
    std::cout << " Quantity in grams of Soluble Fibers: (put 0 if you do not know)" << std::endl;
    output_error = this->read_food_type(this->get_fibers()); //fiber
}

output_error = true;
while (output_error==true)
{
    std::cout << " Level of glycemia in mg/dl: " << std::endl;
    output_error = this->read_glycemia(glycemia); 
}
}


void print_input(){
    std::cout << " =========================================================== " << std::endl;
    std::cout << " Input data:" << std::endl;
    std::cout << " =========================================================== " << std::endl;
    std::cout << " Carbohydrates:" << this->get_carbohydrate() << " g" << std::endl; //carbohydrate
    std::cout << " Proteins:" << this->get_proteins() << " g" << std::endl; //protein
    std::cout << " Fat:" << this->get_fat() << " g" << std::endl; //fat
    std::cout << " Fiber:" << this->get_fibers() << " g" << std::endl; //fiber
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
    if ( this->policy_food(food_type) )
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
        if (this->policy_glycemia(glycemia))
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
    this->calc_fast_ins( this->get_fast_ins(), this->get_carbohydrate(), ratio_ins_carb[ltm->tm_hour]);//fast_insulin,carbohydrate
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
    this->calc_correction( this->get_ins_corr(), glycemia, sensitivity[ltm->tm_hour]); //ins_correction
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
    this->fpf_calories (this->get_fp_cal(), this->get_proteins(), this->get_fat(), this->get_fibers()); //fp_calories, protein,fat,fiber
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
    this->calc_slow_ins(this->get_slow_ins(), this->get_proteins(), this->get_fat(), this->get_fibers(), ratio_ins_fp_carb[ltm->tm_hour]); //slow_insulin, protein,fat,fiber
}

void calc_slow_ins(double &slow_insulin, double protein, double fat, double fiber, double ratio_ins_fp_carb) {
slow_insulin = (protein + (2.25 * fat) + (0.5 * fiber) ) / ratio_ins_fp_carb; //(fp_calories / 4.) / ratio_ins_fp_carb
}

void calc_fpu(){
    this->calc_fpu(this->get_fpu(), this->get_fp_cal()); //fpu,fp_calories
}

//TODO or divided by sensitivity ??
void calc_fpu(double &fpu, double &fp_calories){
    fpu = fp_calories / 100.; 
}

void tot_calories(){
    this->tot_calories(this->get_tot_cal(), this->get_fp_cal(), this->get_carbohydrate());//total_calories, fp_calories,carbohydrate
}

void tot_calories(double &total_calories, double fp_calories, double carbohydrate){
    total_calories = fp_calories + (4. * carbohydrate);
}

void injection_time (){
    this->injection_time (time_of_inj, get_fpu());//fpu
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
void  print_output(){
std::cout << " =========================================================== " << std::endl;
std::cout << " Bolus Insulin to inject now: " << std::setprecision(4) << std::setfill('0') << this->get_fast_ins() << " Units " << std::endl; //fast_insulin
std::cout << " Correction Insulin to inject now: " << this->get_ins_corr() << " Units " << std::endl; //ins_correction
std::cout << " =========================================================== " << std::endl;
std::cout << " Total Insulin to inject now: " << this->get_fast_ins() + this->get_ins_corr() << " Units " << std::endl; //fast_insulin, ins_correction
std::cout << " =========================================================== " << std::endl;
std::cout << " Sensitivity used: " << this->sensitivity[ltm->tm_hour] << " mg/dl per Unit  " << std::endl;
std::cout << " Ratio Carbohydrate/Insulin used: " << ratio_ins_carb[ltm->tm_hour] << " grams per Unit  " << std::endl;
std::cout << " It is " << ltm->tm_hour << " o'clock" << std::endl;
std::cout << " =========================================================== " << std::endl;
std::cout << " FPU eaten: " << this->get_fpu() << std::endl; //fpu
std::cout << " Total amount of energy eaten: " << this->get_tot_cal() << " kcal - " << 4.182 * this->get_tot_cal() << " kJoule " << std::endl; //total_calories
std::cout << " =========================================================== " << std::endl;
std::cout << " Bolus Insulin to inject delayed: " << this->get_slow_ins() << " Units over " << time_of_inj  << " Hours" << std::endl; //slow_insulin
std::cout << " =========================================================== " << std::endl;
std::cout << " Total Insulin to inject: " << this->get_fast_ins() + this->get_ins_corr()  + this->get_slow_ins() << " Units " << std::endl; //fast_insulin, ins_correction,slow_insulin
std::cout << " =========================================================== " << std::endl;
}  
};