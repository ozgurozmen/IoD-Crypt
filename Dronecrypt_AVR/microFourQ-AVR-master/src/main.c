/***********************************************************************************
* FourQlib: a high-performance crypto library based on the elliptic curve FourQ
*
* Abstract: main function
************************************************************************************/

#include "FourQ.h"


int main( void )
{
  bool OK = true;
  
  //OK = OK && fp_tests();
  //OK = OK && ecc_tests();
  //OK = OK && crypto_tests();
  OK = OK && dronecrypt_tests();
  if (!OK) {
    return false;   // Tests were not successful 
  }
  
  return true;
}
