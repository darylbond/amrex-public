#include <AMReX_HDF5.H>

#ifdef BL_HDF5

hid_t makeBox2D() {
  hid_t box_id = H5Tcreate(H5T_COMPOUND, sizeof(box2d_t));
  H5Tinsert(box_id, "lo_i", HOFFSET(box2d_t, lo_i), H5T_NATIVE_INT);
  H5Tinsert(box_id, "lo_j", HOFFSET(box2d_t, lo_j), H5T_NATIVE_INT);
  H5Tinsert(box_id, "hi_i", HOFFSET(box2d_t, hi_i), H5T_NATIVE_INT);
  H5Tinsert(box_id, "hi_j", HOFFSET(box2d_t, hi_j), H5T_NATIVE_INT);
  return box_id;
}

hid_t makeBox3D() {
  hid_t box_id = H5Tcreate(H5T_COMPOUND, sizeof(box3d_t));
  H5Tinsert(box_id, "lo_i", HOFFSET(box3d_t, lo_i), H5T_NATIVE_INT);
  H5Tinsert(box_id, "lo_j", HOFFSET(box3d_t, lo_j), H5T_NATIVE_INT);
  H5Tinsert(box_id, "lo_k", HOFFSET(box3d_t, lo_k), H5T_NATIVE_INT);
  H5Tinsert(box_id, "hi_i", HOFFSET(box3d_t, hi_i), H5T_NATIVE_INT);
  H5Tinsert(box_id, "hi_j", HOFFSET(box3d_t, hi_j), H5T_NATIVE_INT);
  H5Tinsert(box_id, "hi_k", HOFFSET(box3d_t, hi_k), H5T_NATIVE_INT);
  return box_id;
}

box2d_t fillBox2D(Box b) {
  box2d_t box;
  box.lo_i = b.smallEnd(0);
  box.lo_j = b.smallEnd(1);
  box.hi_i = b.bigEnd(0);
  box.hi_j = b.bigEnd(1);
  return box;
}

box3d_t fillBox3D(Box b) {
  box3d_t box;
  box.lo_i = b.smallEnd(0);
  box.lo_j = b.smallEnd(1);
  box.lo_k = b.smallEnd(2);
  box.hi_i = b.bigEnd(0);
  box.hi_j = b.bigEnd(1);
  box.hi_k = b.bigEnd(2);
  return box;
}

hid_t makeInt2D() {
  hid_t intvect_id = H5Tcreate(H5T_COMPOUND, sizeof(int2d_t));
  H5Tinsert(intvect_id, "intvecti", HOFFSET(int2d_t, i), H5T_NATIVE_INT);
  H5Tinsert(intvect_id, "intvectj", HOFFSET(int2d_t, j), H5T_NATIVE_INT);
  return intvect_id;
}

hid_t makeInt3D() {
  hid_t intvect_id = H5Tcreate(H5T_COMPOUND, sizeof(int3d_t));
  H5Tinsert(intvect_id, "intvecti", HOFFSET(int3d_t, i), H5T_NATIVE_INT);
  H5Tinsert(intvect_id, "intvectj", HOFFSET(int3d_t, j), H5T_NATIVE_INT);
  H5Tinsert(intvect_id, "intvectk", HOFFSET(int3d_t, k), H5T_NATIVE_INT);
  return intvect_id;
}

int2d_t fillInt2D(const int* in) {
  int2d_t i;
  i.i = in[0];
  i.j = in[1];
  return i;
}

int3d_t fillInt3D(const int* in) {
  int3d_t i;
  i.i = in[0];
  i.j = in[1];
  i.k = in[2];
  return i;
}

hid_t makeDouble2D() {
  hid_t realvect_id = H5Tcreate(H5T_COMPOUND, sizeof(double2d_t));
  H5Tinsert(realvect_id, "x", HOFFSET(double2d_t, x), H5T_NATIVE_DOUBLE);
  H5Tinsert(realvect_id, "y", HOFFSET(double2d_t, y), H5T_NATIVE_DOUBLE);
  return realvect_id;
}

hid_t makeDouble3D() {
  hid_t realvect_id = H5Tcreate(H5T_COMPOUND, sizeof(double3d_t));
  H5Tinsert(realvect_id, "x", HOFFSET(double3d_t, x), H5T_NATIVE_DOUBLE);
  H5Tinsert(realvect_id, "y", HOFFSET(double3d_t, y), H5T_NATIVE_DOUBLE);
  H5Tinsert(realvect_id, "z", HOFFSET(double3d_t, z), H5T_NATIVE_DOUBLE);
  return realvect_id;
}

double2d_t fillDouble2D(const double* in) {
  double2d_t vec;
  vec.x = in[0];
  vec.y = in[1];
  return vec;
}

double3d_t fillDouble3D(const double* in) {
  double3d_t vec;
  vec.x = in[0];
  vec.y = in[1];
  vec.z = in[2];
  return vec;
}

//==================================================================================
// H5
//==================================================================================

H5::H5() {}

H5::H5(std::string name) { createFile(name); }

H5::H5(std::string name, MPI_Comm comm) { createFile(name, comm); }

H5::H5(hid_t h5) { obj = h5; }

H5::~H5() {}

void H5::createFile(const std::string name, MPI_Comm comm) {
  // assume MPI already initialized
  hid_t plist_id = H5Pcreate(H5P_FILE_ACCESS);
  H5Pset_fapl_mpio(plist_id, comm, MPI_INFO_NULL);

  obj = H5Fcreate(name.c_str(), H5F_ACC_TRUNC, H5P_DEFAULT, plist_id);
  H5Pclose(plist_id);

  return;
}

void H5::openFile(const std::string name) {
  obj = H5Fopen(name.c_str(), H5F_ACC_RDWR, H5P_DEFAULT);
  return;
}

void H5::closeFile() { H5Fclose(obj); }

H5 H5::createGroup(const std::string name) {
  H5 out;
  out.obj = H5Gcreate(obj, name.c_str(), H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);
  return out;
}

void H5::closeGroup() { H5Gclose(obj); }

herr_t H5::saveAttribute(std::map<std::string, int>& m_int,
                         std::map<std::string, double>& m_real,
                         std::map<std::string, std::string>& m_string) {
  H5E_auto_t efunc;
  void* edata;
  H5Eget_auto2(H5E_DEFAULT, &efunc, &edata);
  herr_t ret;

#define INSERT2(Ttype, mapName, H5Ttype)                                      \
  for (std::map<std::string, Ttype>::const_iterator p = mapName.begin();      \
       p != mapName.end(); ++p) {                                             \
    hid_t aid = H5Screate(H5S_SCALAR);                                        \
    H5Eset_auto2(H5E_DEFAULT, NULL, NULL);                                    \
    hid_t attr = H5Acreate2(obj, p->first.c_str(), H5Ttype, aid, H5P_DEFAULT, \
                            H5P_DEFAULT);                                     \
    if (attr < 0) {                                                           \
      H5Adelete(obj, p->first.c_str());                                       \
      attr = H5Acreate2(obj, p->first.c_str(), H5Ttype, aid, H5P_DEFAULT,     \
                        H5P_DEFAULT);                                         \
      if (attr < 0) {                                                         \
        std::cerr << " Problem writing attribute " << p->first.c_str()        \
                  << std::endl;                                               \
      }                                                                       \
    }                                                                         \
    H5Eset_auto2(H5E_DEFAULT, efunc, edata);                                  \
    Ttype tmp = p->second;                                                    \
    ret = H5Awrite(attr, H5Ttype, &tmp);                                      \
    if (ret < 0) return ret;                                                  \
    H5Sclose(aid);                                                            \
    H5Aclose(attr);                                                           \
  }
  INSERT2(double, m_real, H5T_NATIVE_DOUBLE);
  INSERT2(int, m_int, H5T_NATIVE_INT);

  // string is different, of course
  for (std::map<std::string, std::string>::const_iterator p = m_string.begin();
       p != m_string.end(); ++p) {
    hid_t s_type = H5Tcopy(H5T_C_S1);
    H5Tset_size(s_type, p->second.length());  // extra requirement for strings
    hid_t aid = H5Screate(H5S_SCALAR);
    H5Eset_auto2(H5E_DEFAULT, NULL, NULL);
    hid_t attr = H5Acreate2(obj, p->first.c_str(), s_type, aid, H5P_DEFAULT,
                            H5P_DEFAULT);
    if (attr < 0) {
      H5Adelete(obj, p->first.c_str());
      attr = H5Acreate2(obj, p->first.c_str(), s_type, aid, H5P_DEFAULT,
                        H5P_DEFAULT);
      if (attr < 0) {
        std::cerr << " Problem writing attribute " << p->first.c_str()
                  << std::endl;
      }
    }
    H5Eset_auto2(H5E_DEFAULT, efunc, edata);
    char* tmp = (char*)p->second.c_str();
    ret = H5Awrite(attr, s_type, tmp);
    if (ret < 0) return ret;
    H5Sclose(aid);
    H5Aclose(attr);
    H5Tclose(s_type);
  }

  return 0;
}

void H5::saveString(const std::string name, const std::string& data) {
  // write a single string to obj

  hid_t type, space, dset;

  type = H5Tcopy(H5T_C_S1);
  H5Tset_size(type, data.size());

  hsize_t dims[1] = {1};
  space = H5Screate_simple(1, dims, NULL);

  dset = H5Dcreate(obj, name.c_str(), type, space, H5P_DEFAULT, H5P_DEFAULT,
                   H5P_DEFAULT);
  H5Dwrite(dset, type, H5S_ALL, H5S_ALL, H5P_DEFAULT, data.data());

  H5Dclose(dset);
  H5Sclose(space);
  H5Tclose(type);

  return;
}

void H5::saveString(const std::string name,
                    const std::vector<std::string>& data) {
  // write a list of strings
  // all strings must be the same length

  // create a 1d buffer of all the characters

  std::vector<char> buffer;

  for (std::string S : data) {
    for (size_t i = 0; i < S.size(); ++i) {
      buffer.push_back(S[i]);
    }
  }

  hsize_t dims[1] = {data.size()};
  hid_t type, space, dset;

  type = H5Tcopy(H5T_C_S1);
  H5Tset_size(type, data[0].size());

  space = H5Screate_simple(1, dims, NULL);

  dset = H5Dcreate(obj, name.c_str(), type, space, H5P_DEFAULT, H5P_DEFAULT,
                   H5P_DEFAULT);
  H5Dwrite(dset, type, H5S_ALL, H5S_ALL, H5P_DEFAULT, buffer.data());

  H5Dclose(dset);
  H5Sclose(space);
  H5Tclose(type);

  return;
}

#endif
