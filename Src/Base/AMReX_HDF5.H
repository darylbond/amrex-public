#ifndef AMREX_HDF5_H
#define AMREX_HDF5_H

#ifdef BL_HDF5

#include <AMReX_Box.H>
#include <hdf5.h>

using namespace amrex;

typedef struct {
  double x;
  double y;
  double z;
} double3d_t;

typedef struct {
  int i;
  int j;
  int k;
} int3d_t;

typedef struct {
  int lo_i;
  int lo_j;
  int lo_k;
  int hi_i;
  int hi_j;
  int hi_k;
} box3d_t;

typedef struct {
  double x;
  double y;
} double2d_t;

typedef struct {
  int i;
  int j;
} int2d_t;

typedef struct {
  int lo_i;
  int lo_j;
  int hi_i;
  int hi_j;
} box2d_t;

hid_t makeBox2D();
hid_t makeBox3D();
box2d_t fillBox2D(Box b);
box3d_t fillBox3D(Box b);
hid_t makeInt2D();
hid_t makeInt3D();
int2d_t fillInt2D(const int* in);
int3d_t fillInt3D(const int* in);
hid_t makeDouble2D();
hid_t makeDouble3D();
double2d_t fillDouble2D(const double* in);
double3d_t fillDouble3D(const double* in);

class H5 {
 public:
  hid_t obj;  // the data object on which we will operate, may be a file,
              // dataset, etc
  herr_t status;

  H5();
  H5(std::string name);
  H5(std::string name, MPI_Comm comm);
  H5(hid_t h5);
  ~H5();

  void createFile(const std::string name, MPI_Comm comm = MPI_COMM_WORLD);
  void openFile(const std::string name);
  void closeFile();

  H5 createGroup(const std::string name);
  H5 openGroup(const std::string name);
  void closeGroup();

  // compound types
  template <class T>
  void saveAttribute(const std::string& vName, T& vData, long H5Ttype) {
    hid_t aid = H5Screate(H5S_SCALAR);
    hid_t attr =
        H5Acreate2(obj, vName.c_str(), H5Ttype, aid, H5P_DEFAULT, H5P_DEFAULT);
    if (attr < 0) {
      std::cerr << " Problem writing attribute " << vName.c_str() << std::endl;
    }
    status = H5Awrite(attr, H5Ttype, &vData);
    H5Sclose(aid);
    H5Aclose(attr);
  }

  template <class T>
  herr_t readAttribute(const std::string& vName, T& vData, long H5Ttype) {
    herr_t ret;
    // check if the attribute exists
    ret = H5Aexists(obj, vName.c_str());
    if (ret < 0) {
      std::cerr << " Problem writing attribute " << vName.c_str() << std::endl;
      return ret;
    }

    // open the attribute
    hid_t attr = H5Aopen(obj, vName.c_str(), H5P_DEFAULT);

    // read it
    ret = H5Aread(attr, H5Ttype, &vData);

    // close it
    H5Aclose(attr);

    return ret;
  }

  template <class T>
  void saveType(const std::string name, const std::vector<hsize_t>& dims,
                const std::vector<T>& data, long H5Ttype) {
    hid_t space, dset;
    hid_t plist;

    space = H5Screate_simple(dims.size(), dims.data(), NULL);

    dset = H5Dcreate(obj, name.c_str(), H5Ttype, space, H5P_DEFAULT,
                     H5P_DEFAULT, H5P_DEFAULT);

    plist = H5Pcreate(H5P_DATASET_XFER);
    H5Pset_dxpl_mpio(plist, H5FD_MPIO_COLLECTIVE);
    //    H5Pset_dxpl_mpio(plist_id, H5FD_MPIO_INDEPENDENT);

    status = H5Dwrite(dset, H5Ttype, H5S_ALL, H5S_ALL, plist, data.data());

    H5Dclose(dset);
    H5Sclose(space);
    H5Pclose(plist);

    return;
  }

  template <class T>
  void saveType(const std::string name, const std::vector<hsize_t>& dims,
                T* data, long H5Ttype) {
    hid_t space, dset;
    hid_t plist;

    space = H5Screate_simple(dims.size(), dims.data(), NULL);

    dset = H5Dcreate(obj, name.c_str(), H5Ttype, space, H5P_DEFAULT,
                     H5P_DEFAULT, H5P_DEFAULT);

    plist = H5Pcreate(H5P_DATASET_XFER);
    H5Pset_dxpl_mpio(plist, H5FD_MPIO_COLLECTIVE);
    //    H5Pset_dxpl_mpio(plist_id, H5FD_MPIO_INDEPENDENT);

    status = H5Dwrite(dset, H5Ttype, H5S_ALL, H5S_ALL, plist, data);

    H5Dclose(dset);
    H5Sclose(space);
    H5Pclose(plist);

    return;
  }

  template <class T>
  void saveSlab(const std::string name, const std::vector<hsize_t>& full_dims,
                const std::vector<hsize_t>& local_dims,
                const std::vector<hsize_t>& offset, std::vector<T>& data,
                hid_t type) {
    // generate the dataset
    hid_t space, dset;
    space = H5Screate_simple(full_dims.size(), full_dims.data(), NULL);
    dset = H5Dcreate(obj, name.c_str(), type, space, H5P_DEFAULT, H5P_DEFAULT,
                     H5P_DEFAULT);
    H5Sclose(space);

    // now write the bit that we have passed in

    // create the space associated with this slab
    hid_t memspace =
        H5Screate_simple(local_dims.size(), local_dims.data(), NULL);

    // Select hyperslab in the file.
    hid_t filespace = H5Dget_space(dset);
    H5Sselect_hyperslab(filespace, H5S_SELECT_SET, offset.data(), NULL,
                        local_dims.data(), NULL);

    hid_t plist_id;
    plist_id = H5Pcreate(H5P_DATASET_XFER);
    H5Pset_dxpl_mpio(plist_id, H5FD_MPIO_COLLECTIVE);
    //    H5Pset_dxpl_mpio(plist_id, H5FD_MPIO_INDEPENDENT);

    //    if (data.size()) {
    status = H5Dwrite(dset, type, memspace, filespace, plist_id, data.data());
    //    }

    H5Sclose(filespace);
    H5Sclose(memspace);
    H5Pclose(plist_id);
    H5Dclose(dset);
  }

  herr_t saveAttribute(std::map<std::string, int>& m_int,
                       std::map<std::string, double>& m_real,
                       std::map<std::string, std::string>& m_string);

  void saveString(const std::string name, const std::string& data);
  void saveString(const std::string name, const std::vector<std::string>& data);
};

#endif

#endif // AMREX_HDF5_H
