#include "GeneralUtils.hh"
#include "btime_mo.h"
#include "steps_defines.h"
#include <iostream>
using namespace std;
/*!
 * @section This file contains several miscellaneous functions.
 */
//--------------Functions-------------------//

bool is_radar(int rn)
{
    if(rn==2||rn==64||rn==54||rn==66)
	return(true);
    else
	return(false);
};

float get_zra(int rn)
{
    switch(rn)
    {
    case 2:
        cout<<"Radar Location: MELBOURNE"<<endl;
        return(240);
        break;
    case 64:
        cout<<"Radar Location: ADELAIDE (BP)"<<endl;
        return(240);
        break;
    case 54:
        cout<<"Radar Location: SYDNEY (Kurnell)"<<endl;
        return(116);
        break;
    case 66:
        cout<<"Radar Location: BRISBANE (MtSt)"<<endl;
        return(116);
        break;
    default:
        cerr<<"Radar not identified :-/"<<endl;
        return(-999.0);
    };
};


float get_zrb(int rn)
{
    switch(rn)
    {
    case 2:
        cout<<"Radar Location: MELBOURNE"<<endl;
        return(1.5);
        break;
    case 64:
        cout<<"Radar Location: ADELAIDE (BP)"<<endl;
        return(1.5);
        break;
    case 54:
        cout<<"Radar Location: SYDNEY (Kurnell)"<<endl;
        return(1.5);
        break;
    case 66:
        cout<<"Radar Location: BRISBANE (MtSt)"<<endl;
        return(1.5);
        break;
    default:
        cerr<<"Radar not identified :-/"<<endl;
        return(-999.0);
    };
};

void get_dBZtoRR(double *rr,    ///<IN: an array for the rainrate
                 int rn        ///<IN: Radar id no (0 for default)
                 )
{
    float A=300.0;  //Valid for Australia
    float B=1.5;
    switch(rn)
    {
    case 2:
        A=240;
        B=1.5;
        cout<<"Radar Location: MELBOURNE\t A = "<<A<<"\tb = "<<B<<endl;
        break;
    case 64:
        A=240;
        B=1.5;
        cout<<"Radar Location: ADELAIDE (BP)\t A = "<<A<<"\tb = "<<B<<endl;
        break;
    case 54:
        A=116;
        B=1.5;
        cout<<"Radar Location: SYDNEY (Kurnell)\t A = "<<A<<"\tb = "<<B<<endl;
        break;
    case 66:
        A=116;
        B=1.5;
        cout<<"Radar Location: BRISBANE (MtSt)\t A = "<<A<<"\tb = "<<B<<endl;
        break;
    default:
        cout<<"Radar not identified :-/"<<endl;
        cout<<"Using standard radar constants for Australia.\t A = "<<A<<"\tb = "<<B<<endl;

    };
  for (int dBz = 0; dBz < 100; dBz++)
    {
      if (dBz < 5) rr[dBz] = 0.0f;
      else
        {
          float zed = pow (10.0, dBz / 10.0);
          rr[dBz] = pow (zed / A, 1.0 / B);
        }

    }


};

void get_dBZtoRR(float *rr, float A, float b)
{
  for (int dBz = 0; dBz < 100; dBz++)
    {
      if (dBz < 5) rr[dBz] = 0.0f;
      else
        {
          float zed = pow (10.0, dBz / 10.0);
          rr[dBz] = pow (zed / A, 1.0 / b);
        }
    }
};





int get_file_names(char *dirName, list<string> &fileNames, string ext)
{
    string fname(ext);  //filter for the filename
    string datFileName;
    DIR *dir;
    struct dirent *ent;
    dir=opendir(dirName);

    if (dir==NULL) return (0);  //empty directory
    for (int ia=0; ia<2; ia++) //to skip the first two files, "." and ".." in the directory listing.
    {
        ent=readdir(dir);
    }
    fileNames.clear();

    while ( (ent=readdir(dir)) != NULL )
    {
        string thisFileName=string(ent->d_name);
        size_t found =thisFileName.find(fname, 0);
        if (found != string::npos )
        {
            datFileName=thisFileName.substr(0, found);
            fileNames.push_back(datFileName);
        }
    }
    fileNames.sort();
    return fileNames.size();
};

void fit_ls(
            double* x, ///< IN: Array of x values
            double* y, ///< IN: Array of y values
            int n, ///< IN: Number of points
            double &intercept, ///< OUT: Intercept
            double &slope ///< OUT: Slope
            )
{

    double sum_x = 0;
    double sum_x2 = 0;
    double sum_y = 0;
    double sum_xy = 0;

    for (int ia = 0; ia < n; ia++)
    {
        sum_x += x[ia];
        sum_x2 += x[ia] * x[ia];
        sum_y += y[ia];
        sum_xy += x[ia] * y[ia];
    }

    double bottom = n * sum_x2 - sum_x*sum_x;

    if (bottom > 0)
    {
        intercept = (sum_y * sum_x2 - sum_x * sum_xy) / bottom;
        slope = (n * sum_xy - sum_x * sum_y) / bottom;
    }
    else
    {
        slope = 0;
        intercept = 0;
    }
};

bool is_same_day(time_t t1, time_t t2, string &date1, string &date2)
{
    int mm1, dd1, yyyy1, mm2, dd2, yyyy2;
    tm *tm1;
    tm1 = gmtime(&t1);
    yyyy1 = tm1->tm_year + 1900;
    dd1 = tm1->tm_mday;
    mm1 = tm1->tm_mon + 1;

    tm1=gmtime(&t2);
    yyyy2=tm1->tm_year+1900;
    dd2=tm1->tm_mday;
    mm2=tm1->tm_mon + 1;

    stringstream str1;
    stringstream str2;//string stream
    //making string for date value
    if(mm1<10) str1<<yyyy1<<"0"<<mm1;
    else str1<<yyyy1<<mm1;
    if(dd1<10) str1<<"0"<<dd1;
    else str1<<dd1;
    //same as above for str2
    if(mm2<10) str2<<yyyy2<<"0"<<mm2;
    else str2<<yyyy2<<mm2;
    if(dd2<10) str2<<"0"<<dd2;
    else str2<<dd2;
    //output string
    date1=str1.str();
    date2=str2.str();

    if(dd1==dd2 && mm1==mm2 && yyyy1==yyyy2) return (true);
    else return (false);
};

///Informs user about the arguments required to run this prog.
void usage_calcParam()
{
    cerr << "Usage: CalculateParams "  << endl;
    cerr << "        $./calc_param <fpath> <rn>"<<endl;
    cerr << "        fpath - relative or full path of the directory"<<endl;
    cerr << "        rn - radar number or enter '0'"<<endl;
    cerr << "        Available Radars are: "<<endl;
    cerr << "        Melbourne: 02 \t Sydney: 54 \t Brisbane: 66 \t Adelaide: 64"<<endl;

    exit(1);
};

void usage_mkCasc()
{
    cerr << "! Usage: mkCasc "  << endl;
    cerr << "        $./mkCasc <pPath> <nsize>"<<endl;
    cerr << "        pPath - relative or full path of the input directory"<<endl;
    cerr << "        nsize - size of ensemble (Default: '1')"<<endl;
};

void usage_mb2nc()
{
    cerr << "Usage: MB2NC "  << endl;
    cerr << "        $./mb2nc <fpath>"<<endl;
    cerr << "        fpath - relative or full path of the directory"<<endl;
    exit(1);
};
///returns human readable time format
string get_niceTime(unsigned int t)
{
    stringstream str1;
    int sec = t % 60;
    int min = t/60;
    int hours = min / 60;
    min=min%60;
    if(hours<10) str1<<"0"<<hours<<":";
    else str1<<hours<<":";
    if(min<10)str1<<"0"<<min<<":";
    else str1<<min<<":";
    if(sec<10)str1<<"0"<<sec;
    else str1<<sec;

    return (str1.str());
};

string make_enszStr(int ensz)
{
  stringstream str1;
  if(ensz<10) str1<<"00"<<ensz;
  else if (ensz >=10 && ensz < 100) str1<<"0"<<ensz;
  else if (ensz >=100 && ensz < 1000) str1<<ensz;
  else
    {
      cerr<<"Ensemble size should not exceed 1000 members"<<endl;
      exit(1);
    }
  return str1.str ();
};

void delete_MBFiles(string fPath)
{

  char ima[256], pjn[256], ind[256];
  strcpy (ima, (fPath+".ima").c_str ());
  strcpy (pjn, (fPath+".pjn").c_str ());
  strcpy (ind, (fPath+".ind").c_str ());
  if(remove (ima)!=0 || remove (pjn)!=0 || remove (ind)!=0)
    cerr<<"Some parts of MapBase file can not be deleted: "<<fPath<<endl;
};

void mkCasc_frontmatter()
{
  cout << "-------------------------------------------------------------------" << endl;
  cout << "  Cascade Simulator, Version - 13.02" << endl;
  cout << "1. Needs pMatch reference file (created by calc_param) for each scan in separate daily directory."<<endl;
  cout << "2. Advection vectors are taken as m/s and not pixels/time-step"<<endl;
  cout << "-------------------------------------------------------------------" << endl;
};
void cCasc_frontmatter()
{
  cout << "\tCASCADE PARAMETERS ESTIMATOR , Version - 13.02" << endl;;
  cout << "\t1. If data is in dBZ, it will be converted to rain rate (provide valid radar no.). "<<endl;
  cout << "\t2. Saves pMatch reference file for each scan in separate daily directory."<<endl;
  cout << "\t3. Unit of advection vector (now) is m/s instead of pixels/time_step"<<endl<<endl;;

};

void mb2nc_frontmatter()
{
    cout << "-------------------------------------------------------------------" << endl;
    cout << "  MapBase (64bits) to NetCDF-4 converter, Version - 12.09" << endl;
    cout << "   1) Converts data from GNOMONIC Projection to LAT-LON Projection" << endl;
    cout << "   2) Can use classic OR NetCDF-4 formate with/without compression" << endl;
    cout << "   3) Warning: Check if reflectivity is directly converted to Rainfall?"<<endl;
    cout << "-------------------------------------------------------------------" << endl;
};

string get_StrDateTime(time_t mapTime)
{
  int mon, day, year, hh, min, sec;
  tm *tm1;
    tm1 = gmtime(&mapTime);
    year = tm1->tm_year + 1900;
    day = tm1->tm_mday;
    mon = tm1->tm_mon + 1;
    hh =tm1->tm_hour;
    min=tm1->tm_min;
    sec=tm1->tm_sec;
    stringstream strm;
    char date_time[32];
    sprintf(date_time, "%04d-%02d-%02d_%02d%02d", year, mon, day, hh, min); //do not use seconds unless it is necessary.
    strm<<date_time;
    string datetime_str=strm.str ();
    return(datetime_str);
};

string get_date(time_t mapTime)
{
  int mon, day, year;
  tm *tm1;
    tm1 = gmtime(&mapTime);
    year = tm1->tm_year + 1900;
    day = tm1->tm_mday;
    mon = tm1->tm_mon + 1;

    stringstream strm;
    char date[16];
    sprintf(date, "%04d%02d%02d", year, mon, day);
    strm<<date;
    string date_str=strm.str ();
    return(date_str);
};
int get_date_AsInt(time_t mapTime)
{
  int mon, day, year;
  tm *tm1;
    tm1 = gmtime(&mapTime);
    year = tm1->tm_year + 1900;
    day = tm1->tm_mday;
    mon = tm1->tm_mon + 1;

    int date_int=year*10000+mon*100+day;
    return(date_int);
};

void check_MBdataflag(unsigned char mbflag)
{
    cout << "data flag validation... ";
         bool IsdBZ;
         if (IsReflectivity (mbflag))
            {
               IsdBZ = true;
               cout << "Valid \t Type=Reflectivity" << endl;
               
            }
         else if (IsRainIntensity (mbflag))
            {
               IsdBZ = false;
               cout << "Valid \t Type=Rain Intensity" << endl;
               cerr<< "Sorry, this program only works with radar reflectivity data. :("<<endl;
               exit(1);
            }
         else
            {
               cerr << "Invalid :( \n ERROR: data type must be Reflectivity (dBZ) or Rain Intensity (mm/hr)" << endl;
               cerr << "Check the data flag in MapBase File." << endl;
               cerr << "Valid options are \n\t1) MB_RAININTENSITY | MB_COMPRESSED, \n\t2) MB_DBZ | MB_COMPRESSED" << endl;
               exit (1);
            }
};

///Finds rainy days in a MapBase file. A rainy day has more than 4 scans with rainFraction>= MIN_RAINFRAC.
int get_MBRainyDays_dbz(std::string file, std::vector<int> &rDays) {
    rDays.clear();
    //Data Reading controls
    MapBaseControl *inMap; //MBC constructor object
    inMap = new MapBaseControl(file, MB_OPEN); // Opening the input file
    int nsamples = inMap->GetNoSamples(); //the number of samples in the image
    int nlines = inMap->GetNoLines(); //the number of lines in the image
    time_t startTime = inMap->GetStartTime(); //the time of the 1st scan in the database
    time_t endTime = inMap->GetEndTime(); //the time of the last scan in the database
    int mapInterval = inMap->GetMapInterval();
    int minutes = (mapInterval + 5) / 60; // get the nearest minute
    mapInterval = minutes * 60; 
    unsigned char flag = inMap->GetDataType();
    check_MBdataflag(flag); //check if data is reflectivity 

    char date[256];
    to_date(startTime, date);
    cout << "\tStart Time of the MapBase: " << date << endl;

    to_date(endTime, date);
    cout << "\tEnd Time of the MapBase: " << date << endl;
    time_t timeIntvl = inMap->GetMapInterval();
    cout << "\tMap Time Interval in sec   " << timeIntvl << endl;
    float **pdat = inMap->GetDataArray(); //getting data array pointer
    float rain_frac=0.0;
    int pix_count = 0;
    int rain_pix_count=0;
    int rain_scan_count=0;
    int datval = 0; //data value from data array
    int int_date=0;
    time_t prev_mapTime = 0.0;
    string thisDate, prevDate; //for saving dates
    bool FirstScan=true;
    time_t fixed_mapTime=0;
    //start reading each time step
    for (time_t mapTime = startTime; mapTime <= endTime; mapTime++) {
        fixed_mapTime=mapTime; //This is because maptime changes after reading :(
        
        bool same_day = is_same_day(prev_mapTime, fixed_mapTime, prevDate, thisDate);
        if (!same_day && !FirstScan){ //if new day then check
           
                if(rain_scan_count>4){ //call it a rainy day
                    int_date=get_date_AsInt(prev_mapTime);
                    rDays.push_back(int_date);
                    cout<<int_date<<"\t rain_scan_count="<<rain_scan_count<<endl;
                }
                rain_scan_count=0;
            }


        int status = inMap->ReadFromDatabase(mapTime);
        if (status < 0) exit(1);
        char date[256];
        to_date(fixed_mapTime, date);
        //cout<<date<<endl;

        //copy the data into output map array
        for (int iLine = 0; iLine < nlines; iLine++) {
            for (int iSample = 0; iSample < nsamples; iSample++) {
                datval = pdat[iLine][iSample];
                if (datval > -100) { //To ignore MISSING_DATA
                    pix_count++;
                    if(datval>10) 
                        rain_pix_count++;
                }

            }
        }
        
        rain_frac=((float)rain_pix_count/(float)pix_count);
        if(rain_frac>=MIN_RAINFRAC) 
            rain_scan_count++;
        
        pix_count = 0;
        rain_pix_count = 0;
        prev_mapTime = fixed_mapTime; 
        if(FirstScan) FirstScan=false;
    }
    cout<<"Total Rainy days in this month = "<<rDays.size()<<endl;
    return(rDays.size());
};


int get_MBRainyDays_mmhr(std::string file, std::vector<int> &rDays) {

    //Data Reading controls
    MapBaseControl *inMap; //MBC constructor object
    inMap = new MapBaseControl(file, MB_OPEN); // Opening the input file
    int nsamples = inMap->GetNoSamples(); //the number of samples in the image
    int nlines = inMap->GetNoLines(); //the number of lines in the image
    time_t startTime = inMap->GetStartTime(); //the time of the 1st scan in the database
    time_t endTime = inMap->GetEndTime(); //the time of the last scan in the database
    int mapInterval = inMap->GetMapInterval();
    int minutes = (mapInterval + 5) / 60; // get the nearest minute
    mapInterval = minutes * 60; //cascade requires timeIntvl in minutes.
    unsigned char flag = inMap->GetDataType();
    if(IsReflectivity(flag))
    {
        cerr<<"ERROR:This data is in reflectivity.\n "
                "Call get_MBRainyDays_dbz(std::string file, std::vector<int> &rDays) in main().";
        exit(0);
    }

    char date[256];
    to_date(startTime, date);
    cout << "\tStart Time of the MapBase: " << date << endl;

    to_date(endTime, date);
    cout << "\tEnd Time of the MapBase: " << date << endl;
    time_t timeIntvl = inMap->GetMapInterval();
    cout << "\tMap Time Interval in sec   " << timeIntvl << endl;
    float **pdat = inMap->GetDataArray(); //getting data array pointer
    float mar = 0.0; //mean area rain
    float rain_frac=0.0;
    int pix_count = 0;
    int rain_pix_count=0;
    int rain_scan_count=0;
    int datval = 0; //data value from data array
    int int_date=0;
    time_t prev_mapTime = 0.0;
    string thisDate, prevDate; //for saving dates
    bool FirstScan=true;
    //start reading each time step
    for (time_t mapTime = startTime; mapTime <= endTime; mapTime+=mapInterval) {
        bool same_day = is_same_day(prev_mapTime, mapTime, prevDate, thisDate);
        if (!same_day && !FirstScan){ //if new day then check 
           
                if(rain_scan_count>4){ //call it a rainy day
                    int_date=get_date_AsInt(prev_mapTime);
                    rDays.push_back(int_date);
                    cout<<int_date<<"\t rain_scan_count="<<rain_scan_count<<endl;
                }
                rain_scan_count=0;
            }


        int status = inMap->ReadFromDatabase(mapTime);
        if (status < 0) exit(1);
        char date[256];
        to_date(mapTime, date);

        //copy the data into output map array
        for (int iLine = 0; iLine < nlines; iLine++) {
            for (int iSample = 0; iSample < nsamples; iSample++) {
                datval = pdat[iLine][iSample];
                if (datval >=0) { //To ignore MISSING_DATA
                    pix_count++;
                    if(datval>0.1) 
                        rain_pix_count++;
                }

            }
        }
        
        rain_frac=((float)rain_pix_count/(float)pix_count);
        if(rain_frac>=MIN_RAINFRAC) 
            rain_scan_count++;
        
        //cout<<rain_pix_count<<"\t and count"<<rain_frac<<endl;
        pix_count = 0;
        rain_pix_count = 0;
        prev_mapTime = mapTime; //set prev_maptime
        if(FirstScan) FirstScan=false;
    }
    cout<<"Total Rainy days in this month = "<<rDays.size()<<endl;
    return(rDays.size());
};


bool isday_in_thelist(vector<int>&dates, time_t time) {
    int thisDay = 0;
    thisDay = get_date_AsInt(time);
    for (int i = 0; i < dates.size(); i++) {
        if (thisDay == dates[i]) {
            return (true);
        }
    }
    return (false);
};

//given mapTime prints the nice date time and puts newline.
void print_current_tStep(time_t mapTime) {
    char char_time[256];
    to_date(mapTime, char_time);
    cout << "Processing ==> " << char_time <<endl;
    flush(cout);
};

//given mapTime prints the nice date time and flush it.
void overwrite_current_tStep(time_t mapTime) {
    char char_time[256];
    to_date(mapTime, char_time);
    cout << "\b\b\b" << "\tProcessing ==> " << char_time << "\r";
    flush(cout);
};