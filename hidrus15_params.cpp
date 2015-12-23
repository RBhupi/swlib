/*
 * File:   hidrus_params.cpp
 * Author: bhupendra
 *
 * Created on 22 November 2013, 12:09 PM
 */

#include "hidrus15_params.h"
#include "btime_mo.h"
#include <errno.h>
#include <string>
#include <vector>

using namespace std;

hidrus_params::hidrus_params () { }

hidrus_params::hidrus_params (const hidrus_params& orig) { }

hidrus_params::~hidrus_params () { }

void
hidrus_params::read_configlist (string inFileName)
{
    
FILE *paramFile = fopen (inFileName.c_str (), "r"); //open the file
   if(paramFile==NULL) //if file not exist
      {
          //printf("Error in opening file %c", inFileName.c_str ());
          printf("ERROR %d: can't open file -> %s \n%s\n", errno, inFileName.c_str (), strerror(errno));
          exit(errno);
      }
   char line[1024] = {0};
   int offset = 0;
   char field[1024] = {0};
   char value[1024] = {0};
   ens_id=1;
   while (fgets (line, 1024, paramFile) != NULL)
      {
         if (line[0] == '#') continue; //skip all the lines with #
         for (int ia = 0; ia < 1024; ia++) field[ia] = 0;
         for (int ia = 0; ia < 1024; ia++) value[ia] = 0;

         for (offset = 0; offset < 1024; offset++)
            {
               if (line[offset] == '=') break;
               field[offset] = line[offset];
            }
         offset++;

         for (int ia = 0; offset < 1024; offset++, ia++) {
            if (line[offset] == ';') break;
            if (line[offset] == 0) break;
            value[ia] = line[offset];
        }
         
         if (strncmp(field, "prmFileName", 11) == 0)
            sscanf(value, "%s", prmFileName);

        if (strncmp(field, "pMatchPath", 10) == 0)
            sscanf(value, "%s", pMatchPathPrefix);

        if (strncmp(field, "outFilePathPrefix", 11) == 0)
            sscanf(value, "%s", outFileName);

         if (strncmp (field, "logFileName", 11) == 0)
            sscanf (value, "%s", logFileName);

         if (strncmp (field, "nLevels", 7) == 0)
            sscanf (value, "%d", &nLevels);

         if (strncmp (field, "nLags", 5) == 0)
            sscanf (value, "%d", &nLags);
         
         if (strncmp (field, "lOne", 4) == 0)
            sscanf (value, "%d", &lone);

         if (strncmp (field, "Ens_IDNumber", 12) == 0)
            sscanf (value, "%d", &ens_id);

         if (strncmp (field, "TimeStep_minute", 15) == 0)
            sscanf (value, "%d", &tstep);

         if (strncmp (field, "nlines", 6) == 0)
            sscanf (value, "%d", &nlines);
         
         if (strncmp (field, "nsamples", 8) == 0)
            sscanf (value, "%d", &nsamples);
         
         if (strncmp (field, "psize", 5) == 0)
            sscanf (value, "%f", &psize_l);
         
         //Crop setting
         if (strncmp (field, "crop_lat_N", 10) == 0)
            sscanf (value, "%d", &crop_lat_N);

         if (strncmp (field, "crop_lat_S", 10) == 0)
            sscanf (value, "%d", &crop_lat_S);

         if (strncmp (field, "crop_lon_E", 10) == 0)
            sscanf (value, "%d", &crop_lon_E);

         if (strncmp (field, "crop_lon_W", 10) == 0)
            sscanf (value, "%d", &crop_lon_W);

         //Netcdf variable names and units
         if (strncmp (field, "LAT_NAME", 8) == 0)
            sscanf (value, "%s", lat_name);

         if (strncmp (field, "LON_NAME", 8) == 0)
            sscanf (value, "%s", lon_name);

         if (strncmp (field, "REC_NAME", 8) == 0)
            sscanf (value, "%s", rec_name);

         if (strncmp (field, "VAR_NAME", 8) == 0)
            sscanf (value, "%s", var_name);

         if (strncmp (field, "LAT_UNITS", 9) == 0)
            sscanf (value, "%s", lat_units);

         if (strncmp (field, "LON_UNITS", 9) == 0)
            sscanf (value, "%s", lon_units);

         if (strncmp (field, "REC_UNITS", 9) == 0)
            sscanf (value, "%s", rec_units);

         if (strncmp (field, "REC_EPOCH", 9) == 0)
            sscanf (value, "%s", rec_epoch);

         if (strncmp (field, "VAR_UNITS", 9) == 0)
            sscanf (value, "%s", var_units);

         if (strncmp (field, "FILL_VAL", 8) == 0)
            sscanf (value, "%f", &fill_val);

         if (strncmp (field, "FILL_NAME", 8) == 0)
            sscanf (value, "%s", fillval_name);

         if (strncmp (field, "DEFLAT", 6) == 0)
            sscanf (value, "%d", &deflat);
         
         //Radar and domain information
         if (strncmp (field, "Radar_Lat", 9) == 0)
            sscanf (value, "%f", &radar_lat);

         if (strncmp (field, "Radar_Lon", 9) == 0)
            sscanf (value, "%f", &radar_lon);

         if (strncmp (field, "CAPPI_Height", 12) == 0)
            sscanf (value, "%f", &cappi_ht);
         
        if (strncmp (field, "description", 11) == 0)
            sscanf (value, "%s", &description);
        
      }

   stringstream ss;
   ss << rec_units << " since " << rec_epoch << " 00:00:00 UTC";
   string str_ss = ss.str ();
   strcpy (rec_units, str_ss.c_str ());
   prmHeader = true;

   cout << "parameters file name " << prmFileName << endl;
   cout << "log file name " << logFileName << endl;
   cout << "nLevels " << nLevels << endl;
   cout << "nLags " << nLags << endl;
   cout << "Time Step " << tstep << " minutes" << endl;
   cout << "Ensemble member " << ens_id << endl;
   fclose (paramFile);
   
   hidrus_params::open_pMatchMB(hidrus_params::pMatchPathPrefix);
}

bool
hidrus_params::read_simparams (FILE *paramFile)
{
    char line[1024] = {0};
    int offset = 0;
    int nbytes = 0; //no of bytes consumed by sscanf
    
    if (fgets(line, 1024, paramFile) == NULL) {
        return false;
    }

    bool status = isalnum(line[0]);
    if (status) {
        sscanf(line, "%10ld%n", &mapTime, &nbytes);
        offset +=nbytes ;
        
        sscanf(line + offset, "%10ld%n", &pm, &nbytes);
        offset += nbytes;
  
        sscanf(line + offset, "%8d%n", &numberLevels, &nbytes);
        offset += nbytes;
        
        sscanf(line + offset, "%8d%n", &numberLags, &nbytes);
        offset += nbytes;
        
        sscanf(line + offset, "%8f%n", &lZero, &nbytes);
        offset += nbytes;
        
        sscanf(line + offset, "%8f%n", &lOne, &nbytes);
        offset += nbytes;
        
        sscanf(line + offset, "%8f%n", &fieldMean, &nbytes);
        offset += nbytes;
        sscanf(line + offset, "%8f%n", &fieldStd, &nbytes);
        offset += nbytes;
        sscanf(line + offset, "%8f%n", &rainMean, &nbytes);
        offset += nbytes;
        sscanf(line + offset, "%8f%n", &cRainMean, &nbytes);
        offset += nbytes;
        sscanf(line + offset, "%8f%n", &rainStd, &nbytes);
        offset += nbytes;
        sscanf(line + offset, "%8f%n", &rainFrac, &nbytes);
        offset += nbytes;
        sscanf(line + offset, "%8f%n", &south, &nbytes);
        offset += nbytes;
        sscanf(line + offset, "%8f%n", &east, &nbytes);
        offset += nbytes;
        sscanf(line + offset, "%8f%n", &betaOne, &nbytes);
        offset += nbytes;
        sscanf(line + offset, "%8f%n", &betaTwo, &nbytes);
        offset += nbytes;
        sscanf(line + offset, "%8f%n", &hsOne, &nbytes);
        offset += nbytes;
        sscanf(line + offset, "%8f%n", &hsTwo, &nbytes);
        offset += nbytes;

        for (int ia = 0; ia < numberLevels; ia++) {
            sscanf(line + offset, " %9f%n", std + ia, &nbytes);
            offset += nbytes;
        }

        for (int ia = 0; ia < numberLevels; ia++) {
            sscanf(line + offset, " %9f%n", correls + ia, &nbytes);
            offset += nbytes;
        }

        for (int ia = 0; ia < (numberLags + 1) * numberLevels; ia++) {
            sscanf(line + offset, " %9f%n", phi + ia, &nbytes);
            offset += nbytes;
        }
    }
    return status;
};



// change bad parameter values to acceptable range.
void
hidrus_params::revise_parameters() {
    if(betaOne>betaTwo)//swap the values
    {
        float temp_beta2=betaTwo;
        betaTwo=betaOne;
        betaOne=temp_beta2;
    }
   //set the scaling parameters to reasonable values.
    if (betaOne < 2.1) betaOne = 2.1;
    if (betaOne > 2.5) betaOne = 2.5;
    if (betaTwo < 2.5) betaTwo = 2.5;
    if (betaTwo > 2.95) betaTwo = 2.95;
    
   //Also compute Hs paramters using Beta.
    hsOne = (betaOne-2)/2 ;
    hsTwo = (betaTwo-2)/2 ;
}




/*! function that takes in pMatch ASCII file generated by the R script 
 * containing centroids of the PDF clusters  and makes HiDRUS style pMatch 
 * vector with the correct mean.*/
void hidrus_params::make_pMatchTarget(string pMatchPath, float *pmatch)
{
    std::vector<float> pmVect;
    
    read_pMatchCentroidASCII(pMatchPath, &pmVect);
    adjust_pVectMean(&pmVect, fieldMean);
    
    for(int i=0; i < pmVect.size()-1; i++){
        pmatch[i]=pmVect[i];
    }
};


/*! function reads in radar scan for the time of pMatch
 * and makes HiDRUS style pMatch vector with the correct mean.*/
void hidrus_params::get_pMatchTarget(int pmLength, float *pmatch)
{  
    status = hidrus_params::inMap->ReadFromDatabase(hidrus_params::pm);
    if (status < 0){
        std::cerr<< "Failed to read pMatch target from MapBase :("<<endl;
        exit(status);
    } 
    
    vector<float> pmVect;
    pmVect.clear();
    //copy the data into output map array
            for (int iLine = 0; iLine < nlines; iLine++) {
                for (int iSample = 0; iSample < nsamples; iSample++) {
                    datval = pdat[iLine][iSample];
          
                    if (datval < 0) 
                        continue;
                    else if (datval < 15)
                        pmVect.push_back(0);
                    else
                        pmVect.push_back(datval);
                }
            }
    hidrus_params::adjust_pVectLength(&pmVect, pmLength);

    //put all the values in array pointer
    for (int i = 0; i < pmLength; i++) {
        if (pmVect[i] > 0)
            pmatch[i] = pmVect[i] + randUnif(); //add white noise for continuity.
        else
            pmatch[i] = pmVect[i];

    }
    
    return ;
};

//generates random numbers between -0.5 to 0.5 
float hidrus_params::randUnif(){
    float r = (rand()%100)-50;
    r=r/100.0f;
    return(r);
};



 void hidrus_params::open_pMatchMB(string mbFileName){
     
     hidrus_params::inMap = new MapBaseControl(mbFileName.c_str(), MB_OPEN); // Opening the input file
     hyMapParameters mParm; //map parameters
     hyProjectionParameters pParm; //projection parameters
     mbSamples = hidrus_params::inMap->GetNoSamples(); //the number of samples in the image
     mbLines= hidrus_params::inMap->GetNoLines(); //the number of lines in the image
     pdat = hidrus_params::inMap->GetDataArray(); //getting data array pointer
     return ;
 };


bool hidrus_params::read_pMatchCentroidASCII(string fileName, vector<float> *pmatch){
    FILE* pmFile;
    pmFile = fopen(fileName.c_str(), "r");
    char fline[32] = {0}; //first line
    int npoints=0;      //no. of data points in the file
    
    if (fgets(fline, 32, pmFile) == NULL) {
        return false;
    }
    sscanf(fline, "%6d", &npoints);
    
    if(npoints != nsamples*nlines) 
    {
        cout<<"You may be trying to make simulations for the different size of domain than your parameters domain."<<endl;
        cout<<"Change 'npoints' for sampling in pMatch-cluster-plot.R to get correct size pMatch centroid."<<endl;
        cerr<<"ERROR: Supplied pMatch array is not of the same size as required pMatch array."<<endl;
        cout<< "required pMatch size = " << nsamples*nlines << "\t provided pMatch size = " << npoints  <<endl;
        exit(-1);
    }
    
    bool status=true;
    int nvalue=0;
    float pmValue=0;
    pmatch->clear();
    while (fgets(fline, 32, pmFile) != NULL && nvalue < npoints) {
        status = isalnum(fline[0]);
        sscanf(fline, "%5f", &pmValue);
        pmatch->push_back(pmValue);
        nvalue++;
    }
    return true;
};


int hidrus_params::get_zeroValueIndex(std::vector<float> *pvect){
    
    int iter=10000;
    int index=0;
    
    vector<float>& pvectRef = *pvect; // just referencing vector pointer here.
    
    for(int i=0; i<iter; i++){
        index = rand() % (pvectRef.size()-1); //generate random number in the range
        if(pvectRef[index]==0.0) 
            return(index);
    }
    return -999;
};

int hidrus_params::get_PositiveValueIndex(std::vector<float> *pvect){
    
    int iter=10000;
    int index=0;
    
    vector<float>& pvectRef = *pvect; // just referencing vector pointer here.
    
    for(int i=0; i<iter; i++){
        index = rand() % (pvectRef.size()-1); //generate random number in the range
        if(pvectRef[index]>0.0) 
            return(index);
    }
    return -999;
};

float hidrus_params::get_VectorSum(std::vector<float> *pvect) {

    vector<float>& pvectRef = *pvect; // just referencing vector pointer here.
    float vect_sum = 0.0;
    for (int i = 0; i < pvectRef.size(); i++) {
        vect_sum += pvectRef[i];
    }
    return(vect_sum);
};



bool hidrus_params::adjust_pVectMean(std::vector<float> *pmVect, float reqMean){
    
     float vect_sum = get_VectorSum(pmVect);
     float req_sum = reqMean * pmVect->size(); //requested accumulation of dbz
        
    /* **We will sample the pmVector and add or remove the values to get correct mean.** */
    
    //This is the amount we will add or remove.
    float diff=req_sum-vect_sum;
    
    float sample_sum=0.0;   //Total Accumulation that is being added or removed.
    int zero_idx = 0;       // index of vector where value is zero
    int pstv_idx = 0;       //and where the value is positive

    std::vector<float>& pmvRef = *pmVect; // just referencing vector pointer here.
    
    if (diff > 0) {         //if positive we will add
        while (sample_sum - diff < 0) {
            zero_idx = get_zeroValueIndex(pmVect);
            pstv_idx = get_PositiveValueIndex(pmVect);
            
            pmvRef[zero_idx] = pmvRef[pstv_idx];
            sample_sum += pmvRef[pstv_idx];
        }

    }
    else if (diff < 0) {  //else remove
        sample_sum=diff;       
        while(sample_sum<0){
        pstv_idx = get_PositiveValueIndex(pmVect); 
        
        sample_sum += pmvRef[pstv_idx];
        pmvRef[pstv_idx] = 0.0;
        }
    }
};


void hidrus_params::adjust_pVectLength(std::vector<float> *pmVect, int reqLength){
     std::vector<float>& pmvRef = *pmVect; 
     int diffLength=reqLength - pmVect->size();
     int index=0;
    while(reqLength-pmVect->size() > 0){
         index = rand() % (pmvRef.size()-1); //generate a random number in the range
         pmvRef.push_back(pmvRef[index]);
     }
     return ;
};