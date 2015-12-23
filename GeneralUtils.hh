/*!
 * File:   suppl_utils.hh
 * @author bhupendra
 *
 * Created on October 1, 2012, 1:56 PM
 */

#ifndef SUPPL_UTILS_HH
#define	SUPPL_UTILS_HH
//following libraries are for directory listings
#include <list>
#include <iterator>
#include <dirent.h>
#include <iostream>
#include <sstream>
#include <string>
#include <math.h>
#include <time.h>
#include <fstream> 
#include<stdio.h> //for deleting files
#include<vector>
#include "MapBaseControl.hh"
//<----------->
using namespace std;

///Returns a rainrate dBZ lookup table for given radar.
void get_dBZtoRR(double *rr,///< OUT: an array for the rainrate
                 int rn ///< IN: Radar id no (0 for default)
                 );

///Retruns a rainrate dBZ look up table for teh given ZR consntants
void get_dBZtoRR(float *rr,///< OUT: an array for the rainrate
        float A, ///ZR Constatnt 'A'
        float b ///ZR Constatnt 'b'
                );
float get_zra(int rn);
float get_zrb(int rn);

///function to get file names listing from given directory.
int get_file_names(char *dirName, list<string> &fileNames, string ext);

void fit_ls(
            double* x, ///< IN: Array of x values
            double* y, ///< IN: Array of y values
            int n, ///< IN: Number of points
            double &intercept, ///< OUT: Intercept
            double &slope ///< OUT: Slope
            );

///returns True if two unix times fall in the same day. Also outputs date strings.
bool is_same_day(time_t t1,/// <IN : time1
		time_t t2,  /// <IN : time2
                string &date1, ///<OUT : date string1
                string &date2  /// <OUT : date string2
		);

///checks if the given radar is in the list.
bool is_radar(int rn);

///Prints usage hints for calc_Param
void usage_calcParam();

///Prints usage hints for mkCasc
void usage_mkCasc();
void usage_mb2nc();

///Returns nice printable time in hh:mm:ss format
string get_niceTime(unsigned int t); ///<IN : time in seconds

///returns nicely formatted 3 digit int as a 3 char string
string make_enszStr(int ensz);

///deletes all the 3 MapBase files; No extension required!
void delete_MBFiles(string fPath);

void mkCasc_frontmatter();
void cCasc_frontmatter();
void mb2nc_frontmatter();
string get_StrDateTime(time_t mapTime);
string get_date(time_t mapTime);
int get_date_AsInt(time_t mapTime);
float vectorMean(std::vector<float> &inVector);
float vectorStDev(std::vector<float> &inVector);
void check_MBdataflag(unsigned char flag);
int get_MBRainyDays_dbz(std::string fname, std::vector<int> &rDays);
int get_MBRainyDays_mmhr(std::string file, std::vector<int> &rDays);
bool isday_in_thelist(std::vector<int>&dates, time_t time); //checks if the day of given time in the dates vector.
void print_current_tStep(time_t mapTime); //given mapTime prints the nice date time.
void overwrite_current_tStep(time_t mapTime); //given mapTime prints the nice date time on the same line (shows progress).

#endif	/* SUPPL_UTILS_HH */

