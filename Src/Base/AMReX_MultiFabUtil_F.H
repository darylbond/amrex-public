#ifndef AMREX_MULTIFABUTIL_F_H
#define AMREX_MULTIFABUTIL_F_H

#include <AMReX_BLFort.H>

#ifdef __cplusplus
extern "C" {
#endif

void save_FAB(amrex_real* u, const int* ulo, const int* uhi, const int* ncomp,
              const int* comp, const char* name, const int* nchar);

#ifdef __cplusplus
}
#endif

#endif // MFP_UTILITY_F_H
