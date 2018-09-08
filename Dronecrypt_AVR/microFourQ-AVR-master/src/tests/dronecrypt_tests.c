/***********************************************************************************
* FourQlib: a high-performance crypto library based on the elliptic curve FourQ
*
*    Copyright (c) Microsoft Corporation. All rights reserved.
*
* Abstract: testing code for cryptographic functions
*
* Modified by Geovandro C. C. F. Pereira
************************************************************************************/ 

#include "../FourQ_api.h"
#include "../Dronecrypt.h"
#include "../FourQ_params.h"
#include "test_extras.h"
#include <stdio.h>

#define TEST_LOOPS        10        // Number of iterations per test
#define BENCH_LOOPS        10        // Number of iterations per test




int dronecrypt_tests()
{
    ECCRYPTO_STATUS Status = ECCRYPTO_SUCCESS;

    unsigned int n, len, valid = false;
    unsigned char secretKey[32], publicKey[64], publicKey_precomp[64], secret[64], ephemeralsecret[64], ephemeral_sk[32], ephemeral_PK[64], Signature[64]; 

    //CA assigns the Arazi key to the drone
    Status = AQHang_Offline(secretKey, publicKey);
    if (Status != ECCRYPTO_SUCCESS) {
        return Status;
    }

    precompute(publicKey, publicKey_precomp);
    if (Status != ECCRYPTO_SUCCESS) {
      return Status;
    }

    for (n = 0; n < BENCH_LOOPS; n++)
    {
        Status = BPV_AQHang_Online(secretKey, secret);
        if (Status != ECCRYPTO_SUCCESS) {
          return Status;
        }
    }

    //For ephemeral key exchange, we assume other party sends their ephemeral key, ephemeral_PK is their Public Key!
    Status = KeyGeneration(ephemeral_sk, ephemeral_PK);
    if (Status != ECCRYPTO_SUCCESS) {
        return Status;
    }

    for (n = 0; n < BENCH_LOOPS; n++)
    {
        Status = BPV_Ephemeral_AQHang_Online(secretKey, secret, ephemeral_PK, ephemeralsecret);
        if (Status != ECCRYPTO_SUCCESS) {
            return Status;
        }
    }

    void *msg = NULL; 
    msg = "a";  
    len = 1;
    Status = BPV_Schnorr(secretKey, msg, len, Signature);
    if (Status != ECCRYPTO_SUCCESS) {
        return Status;
    }    
    //Receiver precomputes publicKey_precomp and uses it to verify the message, to avoid scalar multiplications online.
    Status = BPV_Schnorr_Verify(publicKey_precomp, msg, len, Signature, &valid);
    if (Status != ECCRYPTO_SUCCESS) {
        return Status;
    }    
    if (valid == false) {
        return Status;
    }

    msg = "b";  
    Status = BPV_Schnorr_Verify(publicKey_precomp, msg, len, Signature, &valid);
    if (Status != ECCRYPTO_SUCCESS) {
        return Status;
    }   
    if (valid == true) {
        return Status;
    }

    msg = "a";  
    len = 1;
    for (n = 0; n < BENCH_LOOPS; n++)
    {
        Status = BPV_Schnorr(secretKey, msg, len, Signature);
        if (Status != ECCRYPTO_SUCCESS) {
            return Status;
        }    
    }

    for (n = 0; n < BENCH_LOOPS; n++)
    {
        Status = BPV_Schnorr_Verify(publicKey_precomp, msg, len, Signature, &valid);
        if (Status != ECCRYPTO_SUCCESS) {
            return Status;
        }    
    }

    for (n = 0; n < BENCH_LOOPS; n++)
    {
        Status = DBPV_ECIES_Encrypt(secretKey, ephemeral_PK);
        if (Status != ECCRYPTO_SUCCESS) {
            return Status; 
        }    
    }

    for (n = 0; n < BENCH_LOOPS; n++)
    {
        Status = DBPV_ECIES_Decrypt(receiver_sk, ephemeral_PK);
        if (Status != ECCRYPTO_SUCCESS) {
            return Status;
        }    
    }

    return true;
}
