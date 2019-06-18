#ifndef AMREX_HDF5_H
#define AMREX_HDF5_H

#ifdef BL_HDF5

#include <AMReX_Utility.H>
#include <AMReX_Box.H>
#include <AMReX_RealBox.H>
#include <hdf5.h>

namespace amrex {

typedef struct {
#if BL_SPACEDIM >= 1
  double x;
#endif
#if BL_SPACEDIM >= 2
  double y;
#endif
#if BL_SPACEDIM >= 3
  double z;
#endif
} real_h5_t;

typedef struct {
#if BL_SPACEDIM >= 1
  int i;
#endif
#if BL_SPACEDIM >= 2
  int j;
#endif
#if BL_SPACEDIM >= 3
  int k;
#endif
} int_h5_t;

typedef struct {
#if BL_SPACEDIM >= 1
  int lo_i;
#endif
#if BL_SPACEDIM >= 2
  int lo_j;
#endif
#if BL_SPACEDIM >= 3
  int lo_k;
#endif
#if BL_SPACEDIM >= 1
  int hi_i;
#endif
#if BL_SPACEDIM >= 2
  int hi_j;
#endif
#if BL_SPACEDIM >= 3
  int hi_k;
#endif
} box_h5_t;

typedef struct {
#if BL_SPACEDIM >= 1
  double lo_x;
#endif
#if BL_SPACEDIM >= 2
  double lo_y;
#endif
#if BL_SPACEDIM >= 3
  double lo_z;
#endif
#if BL_SPACEDIM >= 1
  double hi_x;
#endif
#if BL_SPACEDIM >= 2
  double hi_y;
#endif
#if BL_SPACEDIM >= 3
  double hi_z;
#endif
} rbox_h5_t;

class H5;

hid_t makeH5Box();
box_h5_t writeH5Box(const amrex::Box &b);
void writeH5Box(const amrex::Box &b, box_h5_t &box);
amrex::Box readH5Box(box_h5_t &b);
void writeBoxOnHDF5(const amrex::Box& box, H5& h5, const std::string name);
amrex::Box readBoxFromHDF5(H5& h5, const std::string name);

hid_t makeH5RealBox();
rbox_h5_t writeH5RealBox(const amrex::RealBox &b);
amrex::RealBox readH5RealBox(rbox_h5_t &b);
void writeRealBoxOnHDF5(const RealBox& box, H5& h5, const std::string name);
amrex::RealBox readRealBoxFromHDF5(H5& h5, const std::string name);

hid_t makeH5IntVec();
int_h5_t writeH5IntVec(const int* in);
void readH5IntVec(int_h5_t &in, int *out);

hid_t makeH5RealVec();
real_h5_t writeH5RealVec(const Real* in);
void readH5RealVec(real_h5_t &in, double* out);

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

  H5 openDataset(const std::string name);
  void closeDataset();

  // compound types
  template <class T>
  void writeAttribute(const std::string& vName, T& vData, long H5Ttype) {
    hid_t aid = H5Screate(H5S_SCALAR);
    hid_t attr;
    if (H5Aexists(obj, vName.c_str())) {
      attr = H5Aopen(obj, vName.c_str(),H5P_DEFAULT);
    } else {
      attr = H5Acreate2(obj, vName.c_str(), H5Ttype, aid, H5P_DEFAULT, H5P_DEFAULT);
    }
    if (attr < 0) {
      amrex::Abort(" Problem writing attribute " + vName);
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
    if (ret == 0) {
      amrex::Abort(" Attribute " + vName + "does not exist");
    }
    if (ret < 0) {
      amrex::Abort(" Problem reading attribute " + vName);
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
  herr_t readAttribute(const std::string& vName, std::vector<T>& vData, long H5Ttype) {
    herr_t ret;
    // check if the attribute exists
    ret = H5Aexists(obj, vName.c_str());
    if (ret = 0) {
      amrex::Abort(" Attribute " + vName + "does not exist");
    }
    if (ret < 0) {
      amrex::Abort(" Problem reading attribute " + vName);
    }

    // open the attribute
    hid_t attr = H5Aopen(obj, vName.c_str(), H5P_DEFAULT);

    // get the size of the attribute
    hsize_t size =  H5Aget_storage_size(attr);

    // allocate space
    vData.resize(size/sizeof(H5Ttype));

    // read it
    ret = H5Aread(attr, H5Ttype, vData.data());

    // close it
    H5Aclose(attr);

    return ret;
  }


  template <class T>
  void readType(const std::string name, std::vector<T>& data, long H5Ttype) {

    // open dataset
    hid_t dset = H5Dopen2(obj, name.c_str(), H5P_DEFAULT);

    hid_t plist = H5Pcreate(H5P_DATASET_XFER);
    H5Pset_dxpl_mpio(plist, H5FD_MPIO_COLLECTIVE);
    //    H5Pset_dxpl_mpio(plist_id, H5FD_MPIO_INDEPENDENT);

    // get how big the buffer needs to be
    hsize_t size;
    herr_t err = H5Dvlen_get_buf_size(dset, H5Ttype, H5S_ALL, &size );

    data.resize(size/sizeof(H5Ttype));

    if (err < 0) {
      amrex::Abort("Error: failed to read " + name);
    }

    // read dataset
    err = H5Dread(dset, H5Ttype, H5S_ALL, H5S_ALL, plist, data.data());

    H5Dclose(dset);
    H5Pclose(plist);

    if (err < 0) {
      amrex::Abort("Error: failed to read " + name);
    }

    return;
  }

  template <class T>
  void writeType(const std::string name, const std::vector<hsize_t>& dims,
                const std::vector<T>& data, long H5Ttype) {

    // if it alrready exists, silently return
    if (H5Lexists(obj, name.c_str(), H5P_DEFAULT) > 0) {
      return;
    }

    hid_t dset;
    hid_t plist;

    plist = H5Pcreate(H5P_DATASET_XFER);
    H5Pset_dxpl_mpio(plist, H5FD_MPIO_COLLECTIVE);
    //    H5Pset_dxpl_mpio(plist_id, H5FD_MPIO_INDEPENDENT);
    hid_t space = H5Screate_simple(dims.size(), dims.data(), NULL);
    dset = H5Dcreate(obj, name.c_str(), H5Ttype, space, H5P_DEFAULT,
                   H5P_DEFAULT, H5P_DEFAULT);

    status = H5Dwrite(dset, H5Ttype, H5S_ALL, H5S_ALL, plist, data.data());

    H5Dclose(dset);
    H5Sclose(space);
    H5Pclose(plist);


    return;
  }


  template <class T>
  void writeType(const std::string name, const std::vector<hsize_t>& dims,
                T* data, long H5Ttype) {

    // if it alrready exists, silently return
    if (H5Lexists(obj, name.c_str(), H5P_DEFAULT) > 0) {
      return;
    }

    hid_t dset;
    hid_t plist;

      plist = H5Pcreate(H5P_DATASET_XFER);
      H5Pset_dxpl_mpio(plist, H5FD_MPIO_COLLECTIVE);
      //    H5Pset_dxpl_mpio(plist_id, H5FD_MPIO_INDEPENDENT);
      hid_t space = H5Screate_simple(dims.size(), dims.data(), NULL);
      dset = H5Dcreate(obj, name.c_str(), H5Ttype, space, H5P_DEFAULT,
                     H5P_DEFAULT, H5P_DEFAULT);

    status = H5Dwrite(dset, H5Ttype, H5S_ALL, H5S_ALL, plist, data);

    H5Dclose(dset);
    H5Sclose(space);
    H5Pclose(plist);


    return;
  }

  template <class T>
  void writeSlab(const std::string name, const std::vector<hsize_t>& full_dims,
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

  herr_t writeAttribute(std::map<std::string, int>& m_int,
                       std::map<std::string, double>& m_real,
                       std::map<std::string, std::string>& m_string);

  void writeString(const std::string name, const std::string& data);
  void writeString(const std::string name, const std::vector<std::string>& data);
};

}

#endif

#endif // AMREX_HDF5_H
