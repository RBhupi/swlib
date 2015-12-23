/*
 * File:   hydrus_params.cpp
 * Author: bhupendra
 *
 * Created on 22 November 2013, 12:09 PM
 */

#include "hydrus_params.h"
#include "btime_mo.h"
#include <errno.h>
#include <string>

using namespace std;

hydrus_params::hydrus_params () { }

hydrus_params::hydrus_params (const hydrus_params& orig) { }

hydrus_params::~hydrus_params () { }

void
hydrus_params::read_configlist (string inFileName)
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
   Ens_start = 0;
   Ens_end = 0;
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

         for (int ia = 0; offset < 1024; offset++, ia++)
            {
               if (line[offset] == ';') break;
               if (line[offset] == 0) break;
               value[ia] = line[offset];
            }

         if (strncmp (field, "biasFileName", 12) == 0)
            sscanf (value, "%s", biasFileName);

         if (strncmp (field, "prmFileName", 11) == 0)
            sscanf (value, "%s", prmFileName);

         if (strncmp (field, "outFilePathPrefix", 11) == 0)
            sscanf (value, "%s", outFileName);

         if (strncmp (field, "logFileName", 11) == 0)
            sscanf (value, "%s", logFileName);

         if (strncmp (field, "isHeader", 8) == 0)
            sscanf (value, "%s", &isHeaderStr);

         if (strncmp (field, "nLevels", 7) == 0)
            sscanf (value, "%d", &nLevels);

         if (strncmp (field, "nLags", 5) == 0)
            sscanf (value, "%d", &nLags);
         
         if (strncmp (field, "lOne", 4) == 0)
            sscanf (value, "%d", &lone);

         if (strncmp (field, "Ens_start", 9) == 0)
            sscanf (value, "%d", &Ens_start);

         if (strncmp (field, "Ens_end", 7) == 0)
            sscanf (value, "%d", &Ens_end);

         if (strncmp (field, "tstep", 5) == 0)
            sscanf (value, "%d", &tstep);

         if (strncmp (field, "zra", 3) == 0)
            sscanf (value, "%f", &zra);

         if (strncmp (field, "zrb", 3) == 0)
            sscanf (value, "%f", &zrb);
         
         if (strncmp (field, "Simulate_Advection", 18) == 0)
            sscanf (value, "%s", &sim_advection);
         
         if (strncmp (field, "Calculated_Beta", 15) == 0)
            sscanf (value, "%s", &calc_beta);
         
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
      }
   
    stringstream ss;
   ss << rec_units << " since " << rec_epoch << " 00:00:00 UTC";
   string str_ss = ss.str ();
   strcpy (rec_units, str_ss.c_str ());
   if (isHeaderStr == 'y') prmHeader = true;

   cout << "bias file name " << biasFileName << endl;
   cout << "parameters file name " << prmFileName << endl;
   cout << "log file name " << logFileName << endl;
   cout << "nLevels " << nLevels << endl;
   cout << "nLags " << nLags << endl;
   cout << "Time Step " << tstep << " minutes" << endl;
   cout << "ZR constant a " << zra << endl;
   cout << "ZR constant b " << zrb << endl;
   cout << "start and end ensemble numbers are " << Ens_start << " and " << Ens_end << endl;
   if(calc_beta == 'y') cout<<"Scaling parameters will be computed."<<endl;
   if(sim_advection=='y') cout<<"Advection will be simulated."<<endl;
   fclose (paramFile);
}

bool
hydrus_params::read_simparams (FILE *prmFile)
{
   char line[1024] = {0};
   int vtime[7] = {0};

   if (fgets (line, 1024, prmFile) == NULL) return false;
   bool status = isalnum (line[0]);
   if (status)
      {
         sscanf (line, "%04d%02d%02d", vtime, vtime + 1, vtime + 2); //read date
         vtime[3] = vtime[4] = vtime[5] = 0; //add HH:MM:SS
         startTime = to_time (vtime);
         endTime = startTime + 86340; //for 24-hours simulation 86400=24*59*60
         const int nparms = 22;
         float params[nparms];
         char* pch;
         //I should read this info from the config.list. file is tab delimited
         pch = strtok (line, " ");
         for (int p = 0; p < nparms; p++)
            {
               if (pch == NULL)
                  {
                     cerr << "File ended before all the parameters were read." << endl;
                     exit (0);
                  }
               pch = strtok (NULL, " ");
               params[p] = atof (pch);
            }

         dbz_mean = params[0];
         dbz_var = params[1];
         dbz_azero = params[2];
         dbz_h = params[3];
         
         s_mean = params[4];
         s_var = params[5];
         s_azero = params[6];
         s_h = params[7];
         
         e_mean = params[8];
         e_var = params[9];
         e_azero = params[10];
         e_h = params[11];
         
         std_a = params[12];
         std_b = params[13];
         std_c = params[14];
         std_d = params[15];
         
         b1_a = params[16];
         b1_b = params[17];
         b1_c = params[18];
         
         b2_a = params[19];
         b2_b = params[20];
         b2_c = params[21];
      }

   /*printf("values as read -->%f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f \n",
           s_mean, s_var, s_azero,  s_h, e_mean, e_var, e_azero, e_h, dbz_mean, dbz_var, dbz_azero, dbz_h,  std_a, std_b, std_c, tlen_a,
         tlen_b , tlen_c , b1_a ,   b1_b , b1_c ,  b2_a , b2_b ,  b2_c , lone);*/
   return status;
}

cascade_config::cascade_config () { }

cascade_config::cascade_config (const cascade_config& orig) { }

cascade_config::~cascade_config () { }

void
cascade_config::read_configlist (char inFileName[])
{
   prmHeader = false; // no header assume
   FILE *paramFile = fopen (inFileName, "r");
   if (paramFile == NULL)
      {
         cout << "cannot open the file " << inFileName << endl;
         exit (1);
      }
   char line[1024] = {0};
   int offset = 0;
   char field[1024] = {0};
   char value[1024] = {0};
   Ens_start = 1;
   Ens_end = 1;
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

         for (int ia = 0; offset < 1024; offset++, ia++)
            {
               if (line[offset] == ';') break;
               if (line[offset] == 0) break;
               value[ia] = line[offset];
            }

         if (strncmp (field, "pjnFileName", 11) == 0)
            sscanf (value, "%s", pjnFileName);

         if (strncmp (field, "biasFileName", 12) == 0)
            sscanf (value, "%s", biasFileName);

         if (strncmp (field, "prmDirName", 10) == 0)
            sscanf (value, "%s", prmDirName);

         if (strncmp (field, "outFilePathPrefix", 13) == 0)
            sscanf (value, "%s", outFilePrefix);

         if (strncmp (field, "isHeader", 11) == 0)
            sscanf (value, "%c", isHeaderStr);

         if (strncmp (field, "nLevels", 7) == 0)
            sscanf (value, "%d", &nLevels);

         if (strncmp (field, "nLags", 5) == 0)
            sscanf (value, "%d", &nLags);

         if (strncmp (field, "Ens_start", 9) == 0)
            sscanf (value, "%d", &Ens_start);

         if (strncmp (field, "Ens_end", 7) == 0)
            sscanf (value, "%d", &Ens_end);

         if (strncmp (field, "tstep", 5) == 0)
            sscanf (value, "%d", &tstep);

         if (strncmp (field, "zra", 3) == 0)
            sscanf (value, "%f", &zra);

         if (strncmp (field, "zrb", 3) == 0)
            sscanf (value, "%f", &zrb);

         if (strncmp (field, "psize", 5) == 0)
            sscanf (value, "%f", &psize_l);
         
         //Radar and domain information
         if (strncmp (field, "Radar_Lat", 9) == 0)
            sscanf (value, "%f", &radar_lat);

         if (strncmp (field, "Radar_Lon", 9) == 0)
            sscanf (value, "%f", &radar_lon);

         if (strncmp (field, "CAPPI_Height", 12) == 0)
            sscanf (value, "%f", &cappi_ht);

         if (strncmp (field, "nlines", 6) == 0)
            sscanf (value, "%d", &nlines);

         if (strncmp (field, "nsamples", 8) == 0)
            sscanf (value, "%d", &nsamples);
         
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
      }
   stringstream ss;
   ss << rec_units << " since " << rec_epoch << " 00:00:00 UTC";
   string str_ss = ss.str ();
   strcpy (rec_units, str_ss.c_str ());
   if (isHeaderStr == 'y') prmHeader = true;

   cout << "projection file " << pjnFileName << endl;
   cout << "bias file name " << biasFileName << endl;
   cout << "input directory " << prmDirName << endl;
   cout << "nLevels " << nLevels << endl;
   cout << "nLags " << nLags << endl;
   cout << "Time Step " << tstep << " minutes" << endl;
   cout << "ZR constants: A = " << zra << "\t b = " << zrb << endl;
   cout << "First ensemble member:" << Ens_start << "\t Last ensemble member:" << Ens_end << endl;
   fclose (paramFile);
};