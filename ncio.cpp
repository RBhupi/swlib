#include"ncio.hh"
#include <sstream>

using namespace std;
using namespace netCDF;
using namespace netCDF::exceptions;

ncread_llt::ncread_llt (string fpath, string lat_name, string lon_name, string t_name, string data_name)
{
  //open ncfile for reading
  NcFile *ifile = new NcFile (fpath, NcFile::read);
  //getVar only gets pointer to data object
  dataVar = ifile->getVar (data_name);
  if (dataVar.isNull ())
    {
      cerr << "variable not found " << data_name << endl;
      exit (NC_ERR);
    }
  latVar = ifile->getVar (lat_name);
  if (latVar.isNull ())
    {
      cerr << "variable not found " << lat_name << endl;
      exit (NC_ERR);
    }
  lonVar = ifile->getVar (lon_name);
  if (lonVar.isNull ())
    {
      cerr << "variable not found " << lon_name << endl;
      exit (NC_ERR);
    }
  tVar = ifile->getVar (t_name);
  if (tVar.isNull ())
    {
      cerr << "variable not found " << t_name << endl;
      exit (NC_ERR);
    }
};

ncread_llt::~ncread_llt ()
{
  delete &ifile;
};

float
ncread_llt::get_data_FillValue ()
{
  float Fill_Value;
  dataAtt = dataVar.getAtt ("_FillValue");
  dataAtt.getValues (&Fill_Value);
  return (Fill_Value);
};

string
ncread_llt::get_time_units ()
{
  string tunits;
  tAtt = tVar.getAtt ("units");
  tAtt.getValues (tunits);
  return (tunits);
};

string ncread_llt::get_lat_units ()
{
  string lat_units;
  latAtt = latVar.getAtt ("units");
  latAtt.getValues (lat_units);
  return (lat_units);
};

string ncread_llt::get_lon_units ()
{
  string lon_units;
  lonAtt = lonVar.getAtt ("units");
  lonAtt.getValues (lon_units);
  return (lon_units);
};

string ncread_llt::get_data_units ()
{
  string units;
  dataAtt = dataVar.getAtt ("units");
  dataAtt.getValues (units);
  return (units);
};


void
ncread_llt::set_LatLonTime_size (int num_lat, int num_lon, int num_time)
{
  //This is vector indicating start point of the reading
  //I am setting this vector here for the future use i.e. in get_data_frame function
  nlat = num_lat;
  nlon = num_lon;
  ntime = num_time;
  startp.push_back (0);
  startp.push_back (0);
  startp.push_back (0);
  //This vector indicate how many points to be read
  countp.push_back (1); //1 time steps
  countp.push_back (nlat); //no. of vlaues to read in lat direction
  countp.push_back (nlon); //no of values to read in lon direction
};

void
ncread_llt::set_llt_start (int lat_start, int lon_start, int t_start)
{
  //This is vector indicating start point of the reading
  startp.push_back (t_start);
  startp.push_back (lat_start);
  startp.push_back (lon_start);
};

void ncread_llt::set_llt_count(int lat_count, int lon_count, int t_count)
{
  nlat = lat_count;
  nlon = lon_count;
  ntime = t_count;
  //This vector indicate how many points to be read
  countp.push_back (t_count); 
  countp.push_back (lat_count); 
  countp.push_back (lon_count); 
};


bool
ncread_llt::get_llt (vector<float> *vlat, vector<float> *vlon, vector<int> *vtime)
{
  float lat[nlat];
  float lon[nlon];
  int time[ntime];
  //Now get all the data into array except main data array
  latVar.getVar (lat);
  lonVar.getVar (lon);
  tVar.getVar (time);
  //store data in to vectors
  for (int i = 0; i < nlat; i++) vlat->push_back (lat[i]);
  for (int j = 0; j < nlon; j++) vlon->push_back (lon[j]);
  for (int k = 0; k < ntime; k++) vtime->push_back (time[k]);
  return (true);
};

bool
ncread_llt::get_llt (vector<float> *vlat, vector<float> *vlon, vector<long> *vtime)
{
  float lat[nlat];
  float lon[nlon];
  long time[ntime];
  //Now get all the data into array except main data array
  latVar.getVar (lat);
  lonVar.getVar (lon);
  tVar.getVar (time);
  //store data in to vectors
  for (int i = 0; i < nlat; i++) vlat->push_back (lat[i]);
  for (int j = 0; j < nlon; j++) vlon->push_back (lon[j]);
  for (int k = 0; k < ntime; k++) vtime->push_back (time[k]);
  return (true);
};

bool
ncread_llt::get_t(vector<long> *vtime)
{
  long time[ntime];
  //Now get all the data into array except main data array
  tVar.getVar (time);
  //store data in to vectors
  for (int k = 0; k < ntime; k++) vtime->push_back (time[k]);
  return (true);
};

bool
ncread_llt::get_data_frame (vector<float> *vdata, int t)
{
  startp[0] = t;
  float data[nlat][nlon];
  dataVar.getVar (startp, countp, data);
  vdata->clear ();
  for (int i = 0; i < nlat; i++)
    {
      for (int j = 0; j < nlon; j++)
        {
          vdata->push_back (data[i][j]);
        }
    }
  return (true);
};

bool ncread_llt::get_dataTS(std::vector<float> *vdata) {
    float data[ntime];
    dataVar.getVar(startp, countp, data);
    vdata->clear();

    for (int i = 0; i < ntime; i++) {
        vdata->push_back(data[i]);
    }
    return (true);
};


//I am calling the above function which reads the data in vector and
//then the data is being stored in the data frame. This is due to NC_EBADID error with direct use of dataf.
///not tested!!!
bool
ncread_llt::get_data_frame (float **dataf, int t)
{
  vector <float> vdata;
  ncread_llt::get_data_frame (&vdata, t);
  int offset=0;
  for(int i=0; i<nlat; i++)
     {
         for(int j=0; j<nlon; j++)
            {
                offset=(i*nlon)+j;
                dataf[i][j]=vdata[offset];
            }
     }
  return (true);
};


bool
ncread_llt::get_dataArray_atOnce (double **data)
{
  vector<float> vdata;
  for (int t_frame = 0; t_frame < ntime; t_frame++)
    {
      ncread_llt::get_data_frame (&vdata, t_frame);
      for (int idx = 0; idx < nlat * nlon; idx++)
        {
          data[t_frame][idx] = vdata[idx];
        }
      vdata.clear ();
    }
  return (true);
}

//This is the copy of the function bool ncread_llt::get_dataArray_atOnce (double **data).
//It gives data as float. The size of the array is (t, lat*lon)
bool
ncread_llt::get_dataArray_atOnce (float **data)
{
    vector<float> vdata;
  for (int t_frame = 0; t_frame < ntime; t_frame++)
    {
      ncread_llt::get_data_frame (&vdata, t_frame);
      for (int idx = 0; idx < nlat * nlon; idx++)
        {
          data[t_frame][idx] = vdata[idx];
        }
      vdata.clear ();
    }
  return (true);
}



ncread_ts::ncread_ts (string fpath, string t_name, string data_name)
{
  NcFile *ifile = new NcFile (fpath, NcFile::read);
  tVar = ifile -> getVar (t_name);
  if (tVar.isNull ())
    {
      cerr << "variable not found " << t_name << endl;
      exit (NC_ERR);
    }
  dataVar = ifile->getVar (data_name);
  if (dataVar.isNull ())
    {
      cerr << "variable not found " << data_name << endl;
      exit (NC_ERR);
    }

};

ncread_ts::~ncread_ts ()
{
  delete &ifile;
};

//These functions are same in all the classes
//I want to merge them someday to make common unified class for reading and writing netcdf easily.

float
ncread_ts::get_data_FillValue ()
{
  float Fill_Value;
  dataAtt = dataVar.getAtt ("_FillValue");
  dataAtt.getValues (&Fill_Value);
  return (Fill_Value);
};

string
ncread_ts::get_time_units ()
{
  string tunits;
  tAtt = tVar.getAtt ("units");
  tAtt.getValues (tunits);
  return (tunits);
};

bool
ncread_ts::get_t (vector<float> *vtime)
{
  int time[ntime];
  //Now get all the data into array except main data array
  tVar.getVar (time);
  //store data in to vectors
  for (int k = 0; k < ntime; k++) vtime->push_back (time[k]);
  return (true);
};

void
ncread_ts::set_TimeDim_size (int num_time)
{
  //This is vector indicating start point of the reading
  //I am setting this vector here for the future use i.e. in get_data_frame function
  ntime = num_time;
  startp.push_back (0);
  //This vector indicate how many points to be read
  countp.push_back (ntime); // all the time steps at once
};

bool
ncread_ts::get_data (vector<float> *vdata)
{
  float data[ntime];
  dataVar.getVar (startp, countp, data);

  for (int j = 0; j < ntime - 1; j++)
    {
      vdata->push_back (data[j]);
    }

  return (true);
};

bool
ncread_ts::get_data (float *data)
{
  dataVar.getVar (startp, countp, data);
  return (true);
};

//ncwrite_ll
ncwrite_llt::ncwrite_llt (string fpath,
                          string lat_name, string lon_name, string t_name, string data_name,
                          int lat_size, int lon_size,
                          string lat_units, string lon_units, string t_units, string data_units,
                          float missing_data)
{
  nlon = lon_size;
  nlat = lat_size;
  //open ncfile for writing
  NcFile *ofile = new NcFile (fpath, NcFile::replace, NcFile::nc4);
  // Create netCDF dimensions
  latDim = ofile->addDim (lat_name, nlat);
  lonDim = ofile->addDim (lon_name, nlon);
  tDim = ofile->addDim (t_name); //adds an unlimited dimension
  // Define the coordinate variables.
  latVar = ofile->addVar (lat_name, ncFloat, latDim);
  lonVar = ofile->addVar (lon_name, ncFloat, lonDim);
  tVar = ofile->addVar (t_name, ncInt64, tDim);
  deflat = 3;
  frame_number=0;  //set frame number to zero for the first frame.
  

  /*rest of the constructor was taken from the functions
   * As the object ofile goes out of scope after the constructor ends
   * I can no longer access ofile to make dataVar. so I'm doing everything (that requires ofile) here in the constructor.
   */

  latVar.putAtt ("units", lat_units);
  lonVar.putAtt ("units", lon_units);
  tVar.putAtt ("units", t_units);

  // Define the netCDF variables for the rainfall data
  dimVector.push_back (tDim);
  dimVector.push_back (latDim);
  dimVector.push_back (lonDim);
  dataVar = ofile->addVar (data_name, ncFloat, dimVector);

  dataVar.setCompression (true, true, deflat); //compression
  dataVar.putAtt ("units", data_units);
  dataVar.putAtt ("_FillValue", ncFloat, missing_data);
  
  ofile->putAtt ("HiDRUS", "developed at Monash University and Bureau of Meteorology.");
  ofile->putAtt ("creator_name", "Bhupendra A. Raut and Alan W. Seed");
  ofile->putAtt ("creator_webpage", "www.baraut.info");
  ofile->putAtt ("_institution", "School of Earth Atmosphere and Environment, Monash University");
  ofile->putAtt ("_acknowledgment", "Bureau of Meteorology and CRC for Water Sensitive Cities");
  ofile->putAtt ("_project", "B1.1: Cities as Water Supply Catchments – Urban Rainfall in a changing climate");
  ofile->putAtt("_project_leaders", "Christian Jakob, Michael J. Reeder");
  ofile->putAtt ("date_created", ncwrite_llt::get_current_DateTime ());
};

//!Read this before calling destructor http://netcdf-group.1586084.n2.nabble.com/NcFile-close-member-function-td7574815.html
ncwrite_llt::~ncwrite_llt ()
{
    cout<<"NCIO: netCDF file closed."<<endl;
    
};

void ncwrite_llt::set_deflatLevel(int dfLevel){
    dataVar.setCompression (true, true, dfLevel); //compression
};

void ncwrite_llt::set_chunkShape(std::vector<size_t> chunks)
{
    dataVar.setChunking(NcVar::nc_CHUNKED, chunks);
};


void
ncwrite_llt::set_dim_units (string lat_units, string lon_units, string t_units)
{
  latVar.putAtt ("units", lat_units);
  lonVar.putAtt ("units", lon_units);
  tVar.putAtt ("units", t_units);
};

void
ncwrite_llt::make_data_var (string data_name, string data_units, float missing_data)
{
  // Define the netCDF variables for the rainfall data
  dimVector.push_back (tDim);
  dimVector.push_back (latDim);
  dimVector.push_back (lonDim);
  dataVar = ofile->addVar (data_name, ncFloat, dimVector);

  dataVar.setCompression (true, true, deflat); //compression
  dataVar.putAtt ("units", data_units);
  dataVar.putAtt ("_FillValue", ncFloat, missing_data);
};

void
ncwrite_llt::write_all_dims (vector<float> *vlats, vector<float> *vlons)
{
  // Write the coordinate variables data to the file.
  float lats[nlat], lons[nlon];
  copy(vlats->begin(), vlats->end(), lats);
  copy(vlons->begin(), vlons->end(), lons);


  latVar.putVar (lats);
  lonVar.putVar (lons);
  //Vector specifying the index where the first data values will be written.
  startp.push_back (0);
  startp.push_back (0);
  startp.push_back (0);
  //Vector specifying the number of indices selected along each dimension.
  countp.push_back (1); //1 time value at a time
  countp.push_back (nlat);
  countp.push_back (nlon);

  //Make these vectors for time
  startt.push_back (0);
  countt.push_back (1); //1 time value at a time
  return;
};

//!Writes data from pointer array.
void
ncwrite_llt::write_data (float **grid, long frame_time)
{
  //writing data to netcdf
  startp[0] = frame_number; //starting point for each dimentions (frame_number, 0, 0)
  startt[0] =frame_number ; //starting value for time =frame_number
  dataVar.putVar (startp, countp, grid);
  tVar.putVar (startt, frame_time);
  frame_number++;
};

//!Writes data from c++ vector.
void
ncwrite_llt::write_data(vector <float> &vgrid, long frame_time)
{
   //writing data to netcdf
   startp[0] = frame_number;
   startt[0] = frame_number;
   float grid[nlat][nlon];
   
   int offset = 0;
   for (int i = 0; i < nlat; i++)
      {
         for (int j = 0; j < nlon; j++)
            {
               offset = (i * nlon) + j;
               grid[i][j] = vgrid[offset];
            }
      }
   dataVar.putVar (startp, countp, grid);
   tVar.putVar (startt, frame_time);
   frame_number++;
};

void ncwrite_llt::write_data(float value, long frame_time)
{
   //make a vector and fill it with zeros.
   vector<float> grid;
   grid.resize (nlat*nlon, 0.0);
   //write this data using following function.
   ncwrite_llt::write_data (grid, frame_time);
};

//! Writes variable attributes to the file.
void ncwrite_llt::write_var_att(std::string attName, std::string attValue)
{
   dataVar.putAtt (attName, attValue);
};

void ncwrite_llt::write_var_att(std::string attName, float attValue)
{
   dataVar.putAtt(attName, ncFloat, attValue);
};


///Writes global attribute to the file. Not working. :(
void ncwrite_llt::write_global_att(std::string att_name, std::string att_txt)
{
   ofile->putAtt(att_name.c_str (), att_txt.c_str ());
};

void ncwrite_llt::write_global_att(std::string attName, float attValue)
{
   ofile->putAtt(attName, ncFloat, attValue);
};




std::string
ncwrite_llt::get_current_DateTime ()
{
   time_t now = time (NULL); //get current sys time 
   tm *tm1;
   tm1 = localtime (&now); //convert to calendar time
   char* dt;
   dt=asctime (tm1);  //get formated ascii time
   std::string dt_str(dt);
   if (!dt_str.empty() && dt_str[dt_str.length()-1] == '\n') {
    dt_str.erase(dt_str.length()-1);
}
   return (dt_str);
}; 