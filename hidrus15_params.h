/* File:   hidrus15_params.h
 * @author: Bhupendra Raut
 * @brief This class has functions to read and write various parameter files, required by the HiDRuS model.
 * Created on 22 November 2013, 12:09 PM
 */

#ifndef HIDRUS15_PARAMS_H
#define	HIDRUS15_PARAMS_H
#include <iostream>
#include <time.h>
#include <string>
#include <sstream> 
#include <vector>

//This is for the MapBase pMatch
#include "MapBaseControl.hh"

class hidrus_params {
private:
    char isHeaderStr;

    MapBaseControl *inMap; //MBC constructor object
    int mbSamples;
    int mbLines;
    float **pdat;
    int status;
    float datval;
    
    ///Generates random numbers between -0.5 to 0.5
    float randUnif();

    bool read_pMatchCentroidASCII(std::string fileName, std::vector<float> *pmatch);

    //!gives a random index for the vector which has zero value.
    int get_zeroValueIndex(std::vector<float> *pvect);
    
    //!gives a random index for the vector which has non-zero value.
    int get_PositiveValueIndex(std::vector<float> *pvect);
    
    //!This function adjust the vector mean to required mean by bootstraping.
    bool adjust_pVectMean(std::vector<float> *pmVect, float reqMean);
    
    //!This function adjust the vector length to required length by bootstraping.
    void adjust_pVectLength(std::vector<float> *pmVect, int reqLength);
    
    //! returns sum of all the values in a vector.
    float get_VectorSum(std::vector<float> *pvect);
    
    //
    void open_pMatchMB(string mbFileName);
    
    
   
    
public:
    //configuration parameters
    char prmFileName[256];
    char logFileName[256];
    char outFileName[256];
    char pMatchPathPrefix[256];
    bool prmHeader;
    int nLevels;
    int nLags;
    int lone;
    int ens_id;
    int tstep;

    int nlines;
    int nsamples;
    float psize_l;

    //NetCDF file parameters
    int crop_lat_S, crop_lat_N, crop_lon_E, crop_lon_W; ///crop limits for netcdf data
    char lat_name[256], lon_name[256], rec_name[256], var_name[256];
    char lat_units[256], lon_units[256], rec_units[256], var_units[256];
    char fillval_name[256], rec_epoch[256];
    int deflat;
    float fill_val;
    float radar_lat, radar_lon, cappi_ht;
    char description[256];

    hidrus_params();
    hidrus_params(const hidrus_params& orig);
    virtual ~hidrus_params();

    //reads configuration list of the simulate_list program
    void read_configlist(std::string inFileName);
    void write_parameters(std::string outFileName);
    bool read_simparams(FILE *prmFile);
    void revise_parameters();  //set parameter values to default if out of range.
    
    //! Uses PDF cluster centroid file to get pMatch array and scale it for the given fmean. 
    void make_pMatchTarget(std::string pMatchPath, float *pmatch);
    
    /*This function reads radar scan for the given pMatch time (the time of the past data)
     * and makes a pMatch target array as required by HiDRUS-2. It uses a lot of information from hidrus_params class.*/
    void get_pMatchTarget(int pmLength, float *pmatch);
    

    //simulation parameters to be read with read_simparams
    time_t mapTime ;
	int numberLevels ;
	int numberLags ;
	float lZero ;
	float lOne ;
	float fieldMean ;
	float fieldStd ;
	float rainMean ;
	float rainStd ;
	float cRainMean ;
	float rainFrac;
	float south ;
	float east ;
	float betaOne ;
	float betaTwo ;
	float hsOne ;
	float hsTwo ;
	float correls[64] ;
	float phi[64] ;
	float std[64] ;
        time_t pm;
};


#endif	/* HIDRUS15_PARAMS_H */

