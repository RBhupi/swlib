/*
 * File:   stats.cpp
 * Author: bhupendra
 *
 * Created on 15 August 2015, 9:31 AM
 */

#include "stats.hh"

stats::stats() {
}

stats::stats(std::string ofile) {
    std::string tfName = ofile + "_tStat.asc";

    std::cout << "Opening stats output file "<<tfName<<std::endl;
    tstatf = fopen(tfName.c_str(), "w");

    if (tstatf == NULL) {
        printf("\nError: %d (%s)\n", errno, strerror(errno));
        exit(errno);
    }
   
    
    stats::rFrac=0.0;
    write_header_statf();
}

stats::~stats() {
    fclose(tstatf);
}

void stats::write_header_statf() {
    
    std::string headt[16]={"time", "pm", "in_dbz_fmean", "in_dbz_fstd", 
                "in_rain_fmean", "in_rain_fstd", "in_rain_cmean", "in_rFrac",
                "in_east", "in_south",
                "out_dbz_fmean", "out_dbz_fstd", "out_rain_fmean", "out_rain_fstd", 
                "out_rain_cmean", "out_rFrac"};

    int nparams = sizeof (headt) / sizeof (headt[0]);

    for (int i = 0; i < nparams; i++) {
        fprintf(stats::tstatf, "%s\t", headt[i].c_str());
    }
    fprintf(stats::tstatf, "\b\n");
    return;
};

bool stats::write_stats(hidrus_params &inParams_passed, std::vector<float> *grid_dbz, 
                                                        std::vector<float> *grid_rain) {

    stats::write_input_toStatf(inParams_passed);
    stats::write_output_meanStd(grid_dbz);
    stats::write_output_meanStd(grid_rain);
    stats::write_output_cMeanRfrac(grid_rain);
    fprintf(stats::tstatf, "\b\n");

    return true;
};

void stats::write_input_toStatf(hidrus_params &inParams_passed) {
    fprintf(stats::tstatf, "%10ld\t", inParams_passed.mapTime);
    fprintf(stats::tstatf, "%2d\t", inParams_passed.pm);
    fprintf(stats::tstatf, "%4.2f\t", inParams_passed.fieldMean);
    fprintf(stats::tstatf, "%4.2f\t", inParams_passed.fieldStd);
    fprintf(stats::tstatf, "%4.2f\t", inParams_passed.rainMean);
    fprintf(stats::tstatf, "%4.2f\t", inParams_passed.rainStd);
    fprintf(stats::tstatf, "%4.2f\t", inParams_passed.cRainMean);
    fprintf(stats::tstatf, "%3.2f\t", inParams_passed.rainFrac);
    fprintf(stats::tstatf, "%4.1f\t", inParams_passed.east);
    fprintf(stats::tstatf, "%4.1f\t", inParams_passed.south);
    return;
};

void stats::write_output_meanStd(std::vector <float> *grid) {
    fprintf(stats::tstatf, "%4.2f\t", stats::vectorMean(grid));
    fprintf(stats::tstatf, "%4.2f\t", stats::vectorStDev(grid));
};

void stats::write_output_cMeanRfrac(std::vector<float> *grid) {
    fprintf(stats::tstatf, "%4.2f\t", stats::vector_cMean(grid));
    fprintf(stats::tstatf, "%3.2f\t", stats::vector_rFrac());
};

float stats::vectorMean(std::vector<float> *inVector) {
    // referencing vector pointer here.
    std::vector<float>& inVectRef = *inVector;

    int size = inVectRef.size();
    float sum_of_vector = 0.0;

    float vMean;

    for (int i = 0; i < size; i++) {
        if (inVectRef[i] >= 0.0) {
            sum_of_vector += inVectRef[i];
        }
    }
    vMean = sum_of_vector / size;
    return (vMean);
};

float stats::vectorStDev(std::vector<float> *inVector) {
    // referencing vector pointer here.
    std::vector<float>& inVectRef = *inVector;

    int n = 0;
    float mean = 0.0;
    float M2 = 0;
    float delta = 0.0;
    for (int i = 0; i < inVectRef.size(); i++) {
        if (inVectRef[i] < 0.0) continue;
        n++;
        delta = inVectRef[i] - mean;
        mean = mean + delta / n;
        M2 = M2 + delta * (inVectRef[i] - mean);
        delta = 0.0;
    }
    if (n < 2) return (0);

    float stdev = sqrt(M2 / (n - 1));
    return (stdev);
};

float stats::vector_cMean(std::vector<float> *inVector) {
    // referencing vector pointer here.
    std::vector<float>& inVectRef = *inVector;

    int size = inVectRef.size();
    float sum_of_vector = 0.0;
    int rainyPoints = 0; //
    float minRain = 0.1;
    float vMean;

    for (int i = 0; i < size; i++) {
        if (inVectRef[i] >= minRain) {
            sum_of_vector += inVectRef[i];
            rainyPoints++;
        }
    }

    //rFrac is computed and updated here.
    stats::rFrac =  float(rainyPoints)/float(size);

    vMean = sum_of_vector / rainyPoints;
    return (vMean);
};

float stats::vector_rFrac() {
    return (stats::rFrac);
};

