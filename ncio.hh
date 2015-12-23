/**
 * File:   ncio.hh
 * Author: Bhupendra Raut
 *
 * @brief This is the collection of classes to read and write netcdf files
 * in simple ways. Classes are only capable of reading .nc files with certain
 * structure and it requires to know the variable names before hand.
 *
 * @todo Need to find a way to read variable names from the file and then read it using those name.
 *
 * Created on March 20, 2013, 10:43 PM
 */
#include <string>
#include<cstdlib>
#include <netcdf>
#include <vector>
#include <ctime>

// Return this code in case of failure.
static const int NC_ERR = 2;

#define LAT_UNITS "degrees_north"
#define LON_UNITS "degrees_east"
#define TIME_UNITS "seconds since 1970-01-01 00:00:00 UTC"

#ifndef NCIO_HH
#define	NCIO_HH

/*! @brief This class is used to read and write a 3d data of lat-lon-time kind. */
class ncread_llt {
private:
    netCDF::NcFile ifile;
    netCDF::NcVar latVar, lonVar, tVar, dataVar;
    netCDF::NcVarAtt latAtt, lonAtt, tAtt, dataAtt;
    std::vector<size_t> startp, countp; //for reading nc data
    int nlat, nlon, ntime;

public:
    //constructors
    ncread_llt();
    ncread_llt(std::string fpath, ///<file path with extension
            std::string lat_name, ///<lat variable name
            std::string lon_name, ///<lon variable name
            std::string t_name, ///< time variable name
            std::string data_name ///< data variable name
            );

    //destructor
    ~ncread_llt();
    //public functions
    /* Size of lat, lon and time dimension for reading. 
     * time=1 for frame-by-frame reading.*/
    void set_LatLonTime_size(int num_lat, int num_lon, int num_time);

    void set_llt_start(int lat_start, int lon_start, int t_start);
    void set_llt_count(int lat_count, int lon_count, int t_count);

    ///reads lat lon and time from the file into the vectors
    bool get_llt(std::vector<float> *vlat, std::vector<float> *vlon, std::vector<int> *vtime);

    ///reads only time from the file into the vector
    bool get_t(std::vector<long> *vtime);

    ///reads lat lon and time from the file into the vectors. Use this funct if time values are out of 32bit int range.4
    bool get_llt(std::vector<float> *vlat, std::vector<float> *vlon, std::vector<long> *vtime);

    ///reads data array one frame at a time
    bool get_data_frame(std::vector<float> *vdata, int t);

    bool get_data_frame(float **data, int t);

    ///reads whole data array at once (not good for the large files)
    bool get_dataArray_atOnce(double **data);
    bool get_dataArray_atOnce(float **data);

    ///This function reads all the data along the time axis for gievn lat-lon start and count values.
    bool get_dataTS(std::vector<float> *vdata);


    /// reads fill value or missing data flag
    float get_data_FillValue();
    ///reads time unit
    std::string get_time_units();
    ///reads latitude unit
    std::string get_lat_units();
    ///reads longitude unit
    std::string get_lon_units();
    ///reads data unit
    std::string get_data_units();

};


///@brief This class is used to read time-series (1d) data from netcdf.

class ncread_ts {
private:
    netCDF::NcFile ifile;
    netCDF::NcVar tVar, dataVar;
    netCDF::NcVarAtt tAtt, dataAtt;
    std::vector<size_t> startp, countp; //for reading nc data
    int ntime;
public:
    //constructors
    ncread_ts();
    ncread_ts(std::string fpath, ///<file path with extension
            std::string t_name, ///< time variable name
            std::string data_name ///< data variable name
            );
    //destructor
    ~ncread_ts();
    //public functions
    /*** Set length of time dimension. You can give a smaller value to read only few values from the begining.
       You can't choose a custom period.*/
    void set_TimeDim_size(int num_time);
    ///reads time values
    bool get_t(std::vector<float> *vtime);
    ///reads data values
    bool get_data(std::vector<float> *vdata);
    ///reads data values
    bool get_data(float *vdata);
    ///reads fill value
    float get_data_FillValue();
    /// reads time unit
    std::string get_time_units();
};

///@brief Classes for writing netcdf data.
///@todo Finish writing ncwrite class and test it.

class ncwrite_llt {
private:
    netCDF::NcFile *ofile;
    netCDF::NcVar latVar, lonVar, tVar, dataVar;
    netCDF::NcVarAtt latAtt, lonAtt, tAtt, dataAtt;
    netCDF::NcDim latDim, lonDim, tDim;

    std::vector<size_t> startp; //starting point for reading values for each dim; (0, 0, 0)
    std::vector<size_t> countp; //counter for the vaues to read in all the dim (1, 256, 256)
    std::vector<size_t> startt;
    std::vector<size_t> countt; //for reading nc data

    int nlat, nlon, ntime;

    std::string lat_name, lon_name, t_name, data_name;
    std::vector<netCDF::NcDim> dimVector;
    int deflat;
    float missing_data;
    int frame_number;
    std::string get_current_DateTime();

public:
    //constructors
    ncwrite_llt();
    ///@brief This function makes a netcdf variable to which we will write the data.
    ncwrite_llt(std::string fpath,
            std::string lat_name, std::string lon_name, std::string t_name, std::string data_name, ///<IN: variable names that you want to write
            int lat_size, int lon_size, ///size of lat and lon dimension
            std::string lat_units, std::string lon_units, std::string t_units, std::string data_units, ///Units for dimensions and the data
            float missing_data ///missing data flag
            );
    //destructor
    ~ncwrite_llt();

    void set_deflatLevel(int dfLevel);
    void set_chunkShape(std::vector<size_t> chunks);

    ///Set dimension units
    void set_dim_units(std::string lat_units, std::string lon_units, std::string t_units);
    ///< data variable name
    void make_data_var(std::string data_name,
            std::string data_units,
            float missing_data);

    void write_all_dims(std::vector<float> *lats, std::vector<float> *lons);

    //!Writes the netcdf frame from an array of pointers
    void write_data(float **grid, long frame_time);

    //!Writes the netcdf frame from a vector
    void write_data(std::vector<float> &grid, long frame_time);

    ///Writes a constant value at the given frame time for all lat-lon. This function is useful for writing zeros for no rain time-steps.
    void write_data(float value, long frame_time);
    void write_var_att(std::string attName, std::string attValue);
    void write_var_att(std::string attName, float attValue);
    ///Writes global attribute to the file.
    void write_global_att(std::string att_name, ///name of the global attribute.
            std::string att_txt ///string value of the attribute.
            );
    void write_global_att(std::string att_name, ///name of the global attribute.
            float attValue ///string value of the attribute.
            );
};

#endif	/* NCIO_HH */

