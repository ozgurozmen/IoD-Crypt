/***********************************************************************************
* FourQlib: a high-performance crypto library based on the elliptic curve FourQ
*
* Abstract: pseudorandom generation function
*
* Author: Geovandro C. C. F. Pereira
************************************************************************************/ 

#include "../FourQ_internal.h"
#include "random.h"
#include <stdlib.h>

/**
 * SECURITY NOTE: this function does not provide cryptographically secure pseudo-random numbers.
 *                Users are responsible for replacing it with a secure function for production.
 */
int random_bytes(unsigned char* random_array, unsigned int nbytes)
{ // Generation of "nbytes" of random values

    for (uint8_t i = 0; i < nbytes; i++) {
        random_array[i] = (unsigned char)rand();             
    }

    return true;
}
