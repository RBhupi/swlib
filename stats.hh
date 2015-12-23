/*!
 * File:   stats.hh
 * Author: Bhupendra Raut
 *
 * Created on 15 August 2015, 9:31 AM
 */

#ifndef STATS_HH
#define	STATS_HH
#include<vector>
#include<string>
#include <stdio.h>
#include<cstdlib>
#include <iostream>
#include <cstring>
#include <errno.h>  
#include <math.h>       
#include "hidrus15_params.h"

class stats {
public:
    stats();

    //! Create two ascii files to write daily and tstep statistics.
    stats(std::string ofile);
    virtual ~stats();

    /*This function will take in the inParms object and forecast field of dbz 
     * and rain-rate to compute the stats and write it into the file */
    bool write_stats(hidrus_params &inParams_passed, 
                    std::vector<float> *grid_dbz, std::vector<float> *grid_rain);
    
private:
    std::string ofName;
    FILE *tstatf;
    std::vector<float> grid_rain;
    float rFrac;
    
    void write_header_statf();

    /*!This function will take inParams object and write 
    all the input paramters to the output stats file for the timestep.*/
    void write_input_toStatf(hidrus_params &inParams_passed);

    /*! Takes in a vector and write its mean and std to the file.*/
    void write_output_meanStd(std::vector<float> *grid);
    
    /*! Takes in a vector of rain-rate and write conditional mean and rainFrac to the file.*/
    void write_output_cMeanRfrac(std::vector<float> *grid);

    /*!computes vector mean*/
    float vectorMean(std::vector<float> *inVector);
    
    /*!computes vector standard deviation of vector*/
    float vectorStDev(std::vector<float> *inVector);
    
    /*!computes vector conditional mean for >=0.1 and also update rFrac*/
    float vector_cMean(std::vector<float> *inVector);
    
     /*! Raining fraction of the field for rainrate  >=0.1*/
    float vector_rFrac();

};

#endif	/* STATS_HH */

