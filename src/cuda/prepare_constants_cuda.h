/*
 !=====================================================================
 !
 !               S p e c f e m 3 D  V e r s i o n  3 . 0
 !               ---------------------------------------
 !
 !     Main historical authors: Dimitri Komatitsch and Jeroen Tromp
 !                        Princeton University, USA
 !                and CNRS / University of Marseille, France
 !                 (there are currently many more authors!)
 ! (c) Princeton University and CNRS / University of Marseille, July 2012
 !
 ! This program is free software; you can redistribute it and/or modify
 ! it under the terms of the GNU General Public License as published by
 ! the Free Software Foundation; either version 3 of the License, or
 ! (at your option) any later version.
 !
 ! This program is distributed in the hope that it will be useful,
 ! but WITHOUT ANY WARRANTY; without even the implied warranty of
 ! MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 ! GNU General Public License for more details.
 !
 ! You should have received a copy of the GNU General Public License along
 ! with this program; if not, write to the Free Software Foundation, Inc.,
 ! 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 !
 !=====================================================================
 */

#ifndef PREPARE_CONSTANTS_CUDA_H
#define PREPARE_CONSTANTS_CUDA_H

typedef float realw;  // type of "working" variables

// macros for version output
#define VALUE_TO_STRING(x) #x
#define VALUE(x) VALUE_TO_STRING(x)
#define VAR_NAME_VALUE(var) #var " = "  VALUE(var)



// CONSTANT arrays setup

/* ----------------------------------------------------------------------------------------------- */

/* note:
 constant arrays when used in compute_forces_acoustic_cuda.cu routines stay zero,
 constant declaration and hipMemcpyToSymbol would have to be in the same file...

 extern keyword doesn't work for __constant__ declarations.

 also:
 hipMemcpyToSymbol(HIP_SYMBOL("deviceCaseParams"), caseParams, sizeof(CaseParams));
 ..
 and compile with -arch=sm_20

 see also: http://stackoverflow.com/questions/4008031/how-to-use-cuda-constant-memory-in-a-programmer-pleasant-way
 doesn't seem to work.

 we could keep arrays separated for acoustic and elastic routines...

 for now, we store pointers with hipGetSymbolAddress() function calls.

 */

// cuda constant arrays
//
// note: we use definition __device__ to use global memory rather than constant memory registers
//          to avoid over-loading registers; this should help increasing the occupancy on the GPU

__device__ __constant__ realw d_hprime_xx[NGLL2];
//__device__ __constant__ realw d_hprime_yy[NGLL2]; // only needed if NGLLX != NGLLY != NGLLZ
//__device__ __constant__ realw d_hprime_zz[NGLL2]; // only needed if NGLLX != NGLLY != NGLLZ

__device__ __constant__ realw d_hprimewgll_xx[NGLL2];
//__device__ __constant__ realw d_hprimewgll_yy[NGLL2]; // only needed if NGLLX != NGLLY != NGLLZ
//__device__ __constant__ realw d_hprimewgll_zz[NGLL2]; // only needed if NGLLX != NGLLY != NGLLZ

__device__ __constant__ realw d_wgllwgll_xy[NGLL2];
__device__ __constant__ realw d_wgllwgll_xz[NGLL2];
__device__ __constant__ realw d_wgllwgll_yz[NGLL2];

__device__ __constant__ realw d_wgll_cube[NGLL3]; // needed only for gravity case

// Adding dummy kernel is to fool compiler optimizer.
// If not added dummy kernel, optimizer will delete constant variables,
// because they have not used in any of the kernel.

__global__ void dummy_kernel(){
        d_hprime_xx[0]=1;
//      d_hprime_yy[0]=1;
//      d_hprime_zz[0]=1;
        d_hprimewgll_xx[0]=1;
//      d_hprimewgll_yy[0]=1;
//      d_hprimewgll_zz[0]=1;
        d_wgllwgll_xy[0]=1;
        d_wgllwgll_xz[0]=1;
        d_wgllwgll_yz[0]=1;
}

void setConst_hprime_xx(realw* array, Mesh* mp){

  hipError_t err = hipMemcpyToSymbol(HIP_SYMBOL(d_hprime_xx), array, NGLL2*sizeof(realw));
  if (err != hipSuccess)
  {
    fprintf(stderr, "Error in setConst_hprime_xx: %s\n", hipGetErrorString(err));
    exit(1);
  }

  err = hipGetSymbolAddress((void**)&(mp->d_hprime_xx), HIP_SYMBOL(d_hprime_xx));
  if (err != hipSuccess) {
    fprintf(stderr, "Error with d_hprime_xx: %s\n", hipGetErrorString(err));
    exit(1);
  }
}

// void setConst_hprime_yy(realw* array,Mesh* mp)
// {

//   hipError_t err = hipMemcpyToSymbol(HIP_SYMBOL(d_hprime_yy), array, NGLL2*sizeof(realw));
//   if (err != hipSuccess)
//   {
//     fprintf(stderr, "Error in setConst_hprime_yy: %s\n", hipGetErrorString(err));
//     exit(1);
//   }

//   err = hipGetSymbolAddress((void**)&(mp->d_hprime_yy), HIP_SYMBOL("d_hprime_yy"));
//   if (err != hipSuccess) {
//     fprintf(stderr, "Error with d_hprime_yy: %s\n", hipGetErrorString(err));
//     exit(1);
//   }
// }

// void setConst_hprime_zz(realw* array,Mesh* mp)
// {

//   hipError_t err = hipMemcpyToSymbol(HIP_SYMBOL(d_hprime_zz), array, NGLL2*sizeof(realw));
//   if (err != hipSuccess)
//   {
//     fprintf(stderr, "Error in setConst_hprime_zz: %s\n", hipGetErrorString(err));
//     exit(1);
//   }

//   err = hipGetSymbolAddress((void**)&(mp->d_hprime_zz), HIP_SYMBOL("d_hprime_zz"));
//   if (err != hipSuccess) {
//     fprintf(stderr, "Error with d_hprime_zz: %s\n", hipGetErrorString(err));
//     exit(1);
//   }
// }


void setConst_hprimewgll_xx(realw* array,Mesh* mp)
{
  hipError_t err = hipMemcpyToSymbol(HIP_SYMBOL(d_hprimewgll_xx), array, NGLL2*sizeof(realw));
  if (err != hipSuccess)
  {
    fprintf(stderr, "Error in setConst_hprimewgll_xx: %s\n", hipGetErrorString(err));
    exit(1);
  }
  err = hipGetSymbolAddress((void**)&(mp->d_hprimewgll_xx), HIP_SYMBOL(d_hprimewgll_xx));
  if (err != hipSuccess) {
    fprintf(stderr, "Error with d_hprimewgll_xx: %s\n", hipGetErrorString(err));
    exit(1);
  }
}

/*
// only needed if NGLLX != NGLLY != NGLLZ
void setConst_hprimewgll_yy(realw* array,Mesh* mp)
{
  hipError_t err = hipMemcpyToSymbol(HIP_SYMBOL(d_hprimewgll_yy), array, NGLL2*sizeof(realw));
  if (err != hipSuccess)
  {
    fprintf(stderr, "Error in setConst_hprimewgll_yy: %s\n", hipGetErrorString(err));
    exit(1);
  }

  err = hipGetSymbolAddress((void**)&(mp->d_hprimewgll_yy), HIP_SYMBOL("d_hprimewgll_yy"));
  if (err != hipSuccess) {
    fprintf(stderr, "Error with d_hprimewgll_yy: %s\n", hipGetErrorString(err));
    exit(1);
  }
}
*/

/*
// only needed if NGLLX != NGLLY != NGLLZ
void setConst_hprimewgll_zz(realw* array,Mesh* mp)
{
  hipError_t err = hipMemcpyToSymbol(HIP_SYMBOL(d_hprimewgll_zz), array, NGLL2*sizeof(realw));
  if (err != hipSuccess)
  {
    fprintf(stderr, "Error in setConst_hprimewgll_zz: %s\n", hipGetErrorString(err));
    exit(1);
  }

  err = hipGetSymbolAddress((void**)&(mp->d_hprimewgll_zz), HIP_SYMBOL("d_hprimewgll_zz"));
  if (err != hipSuccess) {
    fprintf(stderr, "Error with d_hprimewgll_zz: %s\n", hipGetErrorString(err));
    exit(1);
  }
}
*/

void setConst_wgllwgll_xy(realw* array,Mesh* mp)
{
  hipError_t err = hipMemcpyToSymbol(HIP_SYMBOL(d_wgllwgll_xy), array, NGLL2*sizeof(realw));
  if (err != hipSuccess)
  {
    fprintf(stderr, "Error in setConst_wgllwgll_xy: %s\n", hipGetErrorString(err));
    exit(1);
  }
  //mp->d_wgllwgll_xy = d_wgllwgll_xy;
  err = hipGetSymbolAddress((void**)&(mp->d_wgllwgll_xy), HIP_SYMBOL(d_wgllwgll_xy));
  if (err != hipSuccess) {
    fprintf(stderr, "Error with d_wgllwgll_xy: %s\n", hipGetErrorString(err));
    exit(1);
  }

}

void setConst_wgllwgll_xz(realw* array,Mesh* mp)
{
  hipError_t err = hipMemcpyToSymbol(HIP_SYMBOL(d_wgllwgll_xz), array, NGLL2*sizeof(realw));
  if (err != hipSuccess)
  {
    fprintf(stderr, "Error in  setConst_wgllwgll_xz: %s\n", hipGetErrorString(err));
    exit(1);
  }
  //mp->d_wgllwgll_xz = d_wgllwgll_xz;
  err = hipGetSymbolAddress((void**)&(mp->d_wgllwgll_xz), HIP_SYMBOL(d_wgllwgll_xz));
  if (err != hipSuccess) {
    fprintf(stderr, "Error with d_wgllwgll_xz: %s\n", hipGetErrorString(err));
    exit(1);
  }

}

void setConst_wgllwgll_yz(realw* array,Mesh* mp)
{
  hipError_t err = hipMemcpyToSymbol(HIP_SYMBOL(d_wgllwgll_yz), array, NGLL2*sizeof(realw));
  if (err != hipSuccess)
  {
    fprintf(stderr, "Error in setConst_wgllwgll_yz: %s\n", hipGetErrorString(err));
    exit(1);
  }
  //mp->d_wgllwgll_yz = d_wgllwgll_yz;
 err = hipGetSymbolAddress((void**)&(mp->d_wgllwgll_yz), HIP_SYMBOL(d_wgllwgll_yz));
  if (err != hipSuccess) {
    fprintf(stderr, "Error with d_wgllwgll_yz: %s\n", hipGetErrorString(err));
    exit(1);
  }

}

void setConst_wgll_cube(realw* array,Mesh* mp)
{
  hipError_t err = hipMemcpyToSymbol(HIP_SYMBOL(d_wgll_cube), array, NGLL3*sizeof(realw));
  if (err != hipSuccess)
  {
    fprintf(stderr, "Error in setConst_wgll_cube: %s\n", hipGetErrorString(err));
    exit(1);
  }
  //mp->d_wgll_cube = d_wgll_cube;
  err = hipGetSymbolAddress((void**)&(mp->d_wgll_cube), HIP_SYMBOL(d_wgll_cube));
  if (err != hipSuccess) {
    fprintf(stderr, "Error with d_wgll_cube: %s\n", hipGetErrorString(err));
    exit(1);
  }

}

#endif
