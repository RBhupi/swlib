/* File:   hydrus_params.h
 * @author: Bhupendra Raut
 * @brief This class has functions to read and write various parameter files, required by the HiDRuS model.
 * Created on 22 November 2013, 12:09 PM
 */

#ifndef HYDRUS_PARAMS_H
#define	HYDRUS_PARAMS_H
#include <iostream>
#include <time.h>
#include <string>
#include <sstream> 

class hydrus_params {
public:
    //configuration parameters
    char biasFileName[256];
    char prmFileName[256];
    char logFileName[256];
    char outFileName[256];
    bool prmHeader;
    int nLevels;
    int nLags;
    int lone;
    int Ens_start, Ens_end;
    int tstep;
    float zra;
    float zrb;
    char sim_advection;
    char calc_beta;

    int nlines;
    int nsamples;
    float psize_l;
    
    //Simulation Paramaters
    time_t startTime;
    time_t endTime;
    float s_mean;
    float s_var;
    float s_azero;
    float s_h;
    float e_mean;
    float e_var;
    float e_azero;
    float e_h;
    float dbz_mean;
    float dbz_var;
    float dbz_azero;
    float dbz_h;
    //polynomial coefficients for std 
    //For Melbourne (a=5.0, b=0.167, c=0.14, d= -0.008)
    float std_a;
    float std_b;
    float std_c;
    float std_d;
    
    
    float tlen_a;
    float tlen_b;
    float tlen_c;
    float b1_a;
    float b1_b;
    float b1_c;
    float b2_a;
    float b2_b;
    float b2_c;
    
    //NetCDF file parameters
    int crop_lat_S, crop_lat_N, crop_lon_E, crop_lon_W; ///crop limits for netcdf data
    char lat_name[256], lon_name[256], rec_name[256], var_name[256];
    char lat_units[256], lon_units[256], rec_units[256], var_units[256];
    char fillval_name[256], rec_epoch[256];
    int deflat;
    float fill_val;
    float radar_lat, radar_lon, cappi_ht;
    
    hydrus_params();
    hydrus_params(const hydrus_params& orig);
    virtual ~hydrus_params();

    //reads configuration list of the simulate_list program
    void read_configlist(std::string inFileName);
    void write_parameters(std::string outFileName);
    bool read_simparams( FILE *prmFile);
private:
    char isHeaderStr;
};


//! This class supports reading of configuration file for mkCascSim program.
class cascade_config {
public:
    //cascade configuration parameters
    char pjnFileName[256];  //projection information MapBase style .pjn
    char biasFileName[256]; //bias mask 
    char prmDirName[256]; //input parameters   
    char outFilePrefix[256]; //output file Path and prefix.
    
    int Ens_start, Ens_end;
    int tstep;
    float zra;
    float zrb;
    
    int nLevels;
    int nLags;
    float lone;
    bool prmHeader;
    int nlines;
    int nsamples;
   
    float psize_l;
    
    float radar_lat, radar_lon, cappi_ht;
    
    //nc paramters
     int crop_lat_S, crop_lat_N, crop_lon_E, crop_lon_W; ///crop limits for netcdf data
    char lat_name[256], lon_name[256], rec_name[256], var_name[256];
    char lat_units[256], lon_units[256], rec_units[256], var_units[256];
    char fillval_name[256], rec_epoch[256];
    int deflat;
    float fill_val;
    
    cascade_config();
    cascade_config(const cascade_config& orig);
    virtual ~cascade_config();

    void read_configlist(char inFileName[]);
private:
    char isHeaderStr;
};

#endif	/* HYDRUS_PARAMS_H */

