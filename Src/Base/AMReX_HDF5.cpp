#include <AMReX_HDF5.H>

#ifdef BL_HDF5

hid_t makeBox() {
  hid_t box_id = H5Tcreate(H5T_COMPOUND, sizeof(box_h5_t));
#if BL_SPACEDIM >= 1
  H5Tinsert(box_id, "lo_i", HOFFSET(box_h5_t, lo_i), H5T_NATIVE_INT);
#endif
#if BL_SPACEDIM >= 2
  H5Tinsert(box_id, "lo_j", HOFFSET(box_h5_t, lo_j), H5T_NATIVE_INT);
#endif
#if BL_SPACEDIM >= 3
  H5Tinsert(box_id, "lo_k", HOFFSET(box_h5_t, lo_k), H5T_NATIVE_INT);
#endif
#if BL_SPACEDIM >= 1
  H5Tinsert(box_id, "hi_i", HOFFSET(box_h5_t, hi_i), H5T_NATIVE_INT);
#endif
#if BL_SPACEDIM >= 2
  H5Tinsert(box_id, "hi_j", HOFFSET(box_h5_t, hi_j), H5T_NATIVE_INT);
#endif
#if BL_SPACEDIM >= 3
  H5Tinsert(box_id, "hi_k", HOFFSET(box_h5_t, hi_k), H5T_NATIVE_INT);
#endif

  return box_id;
}

box_h5_t writeBox(const Box &b) {
  box_h5_t box;
#if BL_SPACEDIM >= 1
  box.lo_i = b.smallEnd(0);
  box.hi_i = b.bigEnd(0);
#endif
#if BL_SPACEDIM >= 2
  box.lo_j = b.smallEnd(1);
  box.hi_j = b.bigEnd(1);
#endif
#if BL_SPACEDIM >= 3
  box.lo_k = b.smallEnd(2);
  box.hi_k = b.bigEnd(2);
#endif
  return box;
}

void writeBox(const Box &b, box_h5_t &box) {
#if BL_SPACEDIM >= 1
  box.lo_i = b.smallEnd(0);
  box.hi_i = b.bigEnd(0);
#endif
#if BL_SPACEDIM >= 2
  box.lo_j = b.smallEnd(1);
  box.hi_j = b.bigEnd(1);
#endif
#if BL_SPACEDIM >= 3
  box.lo_k = b.smallEnd(2);
  box.hi_k = b.bigEnd(2);
#endif
  return;
}

Box readBox(box_h5_t &box) {
  IntVect lo(AMREX_D_DECL(box.lo_i, box.lo_j, box.lo_k));
  IntVect hi(AMREX_D_DECL(box.hi_i, box.hi_j, box.hi_k));
  Box b(lo, hi);
  return b;
}

hid_t makeRealBox() {
  hid_t box_id = H5Tcreate(H5T_COMPOUND, sizeof(rbox_h5_t));
#if BL_SPACEDIM >= 1
  H5Tinsert(box_id, "lo_x", HOFFSET(rbox_h5_t, lo_x), H5T_NATIVE_DOUBLE);
#endif
#if BL_SPACEDIM >= 2
  H5Tinsert(box_id, "lo_y", HOFFSET(rbox_h5_t, lo_y), H5T_NATIVE_DOUBLE);
#endif
#if BL_SPACEDIM >= 3
  H5Tinsert(box_id, "lo_z", HOFFSET(rbox_h5_t, lo_z), H5T_NATIVE_DOUBLE);
#endif
#if BL_SPACEDIM >= 1
  H5Tinsert(box_id, "hi_x", HOFFSET(rbox_h5_t, hi_x), H5T_NATIVE_DOUBLE);
#endif
#if BL_SPACEDIM >= 2
  H5Tinsert(box_id, "hi_y", HOFFSET(rbox_h5_t, hi_y), H5T_NATIVE_DOUBLE);
#endif
#if BL_SPACEDIM >= 3
  H5Tinsert(box_id, "hi_z", HOFFSET(rbox_h5_t, hi_z), H5T_NATIVE_DOUBLE);
#endif

  return box_id;
}

rbox_h5_t writeRealBox(const RealBox &b) {
  rbox_h5_t box;
#if BL_SPACEDIM >= 1
  box.lo_x = b.lo(0);
  box.hi_x = b.hi(0);
#endif
#if BL_SPACEDIM >= 2
  box.lo_y = b.lo(1);
  box.hi_y = b.hi(1);
#endif
#if BL_SPACEDIM >= 3
  box.lo_z = b.lo(2);
  box.hi_z = b.hi(2);
#endif
  return box;
}

RealBox readRealBox(rbox_h5_t &box) {
  std::array<Real,AMREX_SPACEDIM> lo = {AMREX_D_DECL(box.lo_x, box.lo_y, box.lo_z)};
  std::array<Real, AMREX_SPACEDIM> hi = {AMREX_D_DECL(box.hi_x, box.hi_y, box.hi_z)};
  RealBox b(lo, hi);
  return b;
}


hid_t makeIntVec() {
  hid_t intvect_id = H5Tcreate(H5T_COMPOUND, sizeof(int_h5_t));
#if BL_SPACEDIM >= 1
  H5Tinsert(intvect_id, "intvecti", HOFFSET(int_h5_t, i), H5T_NATIVE_INT);
#endif
#if BL_SPACEDIM >= 2
  H5Tinsert(intvect_id, "intvectj", HOFFSET(int_h5_t, j), H5T_NATIVE_INT);
#endif
#if BL_SPACEDIM >= 3
  H5Tinsert(intvect_id, "intvectk", HOFFSET(int_h5_t, k), H5T_NATIVE_INT);
#endif
  return intvect_id;
}

int_h5_t writeIntVec(const int* in) {
  int_h5_t i;
#if BL_SPACEDIM >= 1
  i.i = in[0];
#endif
#if BL_SPACEDIM >= 2
  i.j = in[1];
#endif
#if BL_SPACEDIM >= 3
  i.k = in[2];
#endif
  return i;
}

void readIntVec(int_h5_t &in, int *out) {
#if BL_SPACEDIM >= 1
  out[0] = in.i;
#endif
#if BL_SPACEDIM >= 2
  out[1] = in.j;
#endif
#if BL_SPACEDIM >= 3
  out[2] = in.k;
#endif
}

hid_t makeDoubleVec() {
hid_t realvect_id = H5Tcreate(H5T_COMPOUND, sizeof(double_h5_t));
#if BL_SPACEDIM >= 1
  H5Tinsert(realvect_id, "x", HOFFSET(double_h5_t, x), H5T_NATIVE_DOUBLE);
#endif
#if BL_SPACEDIM >= 2
  H5Tinsert(realvect_id, "y", HOFFSET(double_h5_t, y), H5T_NATIVE_DOUBLE);
#endif
#if BL_SPACEDIM >= 3
  H5Tinsert(realvect_id, "z", HOFFSET(double_h5_t, z), H5T_NATIVE_DOUBLE);
#endif
  return realvect_id;
}

double_h5_t writeDoubleVec(const double* in) {
  double_h5_t vec;
#if BL_SPACEDIM >= 1
  vec.x = in[0];
#endif
#if BL_SPACEDIM >= 2
  vec.y = in[1];
#endif
#if BL_SPACEDIM >= 3
  vec.z = in[2];
#endif
  return vec;
}

void readDoubleVec(double_h5_t &in, double *out) {
#if BL_SPACEDIM >= 1
  out[0] = in.x;
#endif
#if BL_SPACEDIM >= 2
  out[1] = in.y;
#endif
#if BL_SPACEDIM >= 3
  out[2] = in.z;
#endif
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

herr_t H5::writeAttribute(std::map<std::string, int>& m_int,
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
        amrex::Abort(" Problem writing attribute " + p->first);       \
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
        amrex::Abort(" Problem writing attribute " + p->first);
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

void H5::writeString(const std::string name, const std::string& data) {
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

void H5::writeString(const std::string name,
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
