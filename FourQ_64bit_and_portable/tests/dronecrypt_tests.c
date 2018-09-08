#include "../FourQ_internal.h"
#include "../FourQ_params.h"
#include "test_extras.h"
#include <stdio.h>
#include "../Dronecrypt.h"


// Benchmark and test parameters  
#define BENCH_LOOPS       1000       // Number of iterations per bench
#define TEST_LOOPS        1000       // Number of iterations per test


int main()
{
    
    ECCRYPTO_STATUS Status = ECCRYPTO_SUCCESS;

    unsigned int n, len, valid = false;
    unsigned long long cycles, cycles1, cycles2;
    unsigned char secretKey[32], publicKey[64], publicKey_precomp[64], secret[64], ephemeralsecret[64], ephemeral_sk[32], ephemeral_PK[64], Signature[64];

    printf("\n--------------------------------------------------------------------------------------------------------\n"); 
    printf("Testing Dronecrypt!!: \n"); 

    //CA assigns the Arazi key to the drone
    Status = AQHang_Offline(secretKey, publicKey);
    if (Status != ECCRYPTO_SUCCESS) {
        printf("Error in AQHang_Offline\n");
        return 0;
    }

    printf("\n--------------------------------------------------------------------------------------------------------\n");
    precompute(publicKey, publicKey_precomp);
    if (Status != ECCRYPTO_SUCCESS) {
        printf("Error in precompute\n");
        return 0;
    }

    cycles = 0;
    for (n = 0; n < BENCH_LOOPS; n++)
    {
        cycles1 = cpucycles();

        // Status = KeyGeneration(SecretKeyA, PublicKeyA);
        Status = BPV_AQHang_Online(secretKey, secret);
        if (Status != ECCRYPTO_SUCCESS) {
            printf("Error in BPV_AQHang_Online\n");
            return 0;
        }
        cycles2 = cpucycles();
        cycles = cycles + (cycles2 - cycles1);
    }
    printf(" BPV_AQHang_Online runs in ............................................... %8lld ", cycles/BENCH_LOOPS); print_unit;
    printf("\n");


    //For ephemeral key exchange, we assume other party sends their ephemeral key, ephemeral_PK is their Public Key!
    Status = KeyGeneration(ephemeral_sk, ephemeral_PK);
    if (Status != ECCRYPTO_SUCCESS) {
        printf("Error in Ephemeral KeyGeneration\n");
        return 0;
    }

    cycles = 0;
    for (n = 0; n < BENCH_LOOPS; n++)
    {
        cycles1 = cpucycles();

        // Status = KeyGeneration(SecretKeyA, PublicKeyA);
        Status = BPV_Ephemeral_AQHang_Online(secretKey, secret, ephemeral_PK, ephemeralsecret);
        if (Status != ECCRYPTO_SUCCESS) {
            printf("Error in BPV_AQHang_Online\n");
            return 0;
        }
        cycles2 = cpucycles();
        cycles = cycles + (cycles2 - cycles1);
    }
    printf(" BPV_Ephemeral_AQHang_Online runs in ............................................... %8lld ", cycles/BENCH_LOOPS); print_unit;
    printf("\n");
    
    void *msg = NULL; 
    msg = "a";  
    len = 1;
    Status = BPV_Schnorr(secretKey, msg, len, Signature);
    if (Status != ECCRYPTO_SUCCESS) {
        printf("Error in BPV_Schnorr\n");
        return 0;   
    }    
    printf("BPV_Schnorr successful\n");
    //Receiver precomputes publicKey_precomp and uses it to verify the message, to avoid scalar multiplications online.
    Status = BPV_Schnorr_Verify(publicKey_precomp, msg, len, Signature, &valid);
    if (Status != ECCRYPTO_SUCCESS) {
        printf("Error in BPV_Schnorr_Verify\n");
        return 0;   
    }    
    printf("BPV_Schnorr_Verify successful\n");
    if (valid == false) {
        printf("Error in BPV_Schnorr_Verify -- Signature not verified\n"); //Gave a problem here!!
        return 0;   
    }


    msg = "b";  
    Status = BPV_Schnorr_Verify(publicKey_precomp, msg, len, Signature, &valid);
    if (Status != ECCRYPTO_SUCCESS) {
        printf("Error in BPV_Schnorr_Verify\n");
        return 0;   
    }   
    printf("BPV_Schnorr_Verify successful 2\n");
    if (valid == true) {
        printf("Error in BPV_Schnorr_Verify -- Signature verified, wasn't supposed to\n");
        return 0;   
    }

    msg = "a";  
    len = 1;
    cycles = 0;
    for (n = 0; n < BENCH_LOOPS; n++)
    {
        cycles1 = cpucycles();

        // Status = KeyGeneration(SecretKeyA, PublicKeyA);
        Status = BPV_Schnorr(secretKey, msg, len, Signature);
        if (Status != ECCRYPTO_SUCCESS) {
            printf("Error in BPV_Schnorr\n");
            return 0;   
        }    
        cycles2 = cpucycles();
        cycles = cycles + (cycles2 - cycles1);
    }
    printf(" BPV_Schnorr runs in ............................................... %8lld ", cycles/BENCH_LOOPS); print_unit;
    printf("\n");

    cycles = 0;
    for (n = 0; n < BENCH_LOOPS; n++)
    {
        cycles1 = cpucycles();

        // Status = KeyGeneration(SecretKeyA, PublicKeyA);
        Status = BPV_Schnorr_Verify(publicKey_precomp, msg, len, Signature, &valid);
        if (Status != ECCRYPTO_SUCCESS) {
            printf("Error in BPV_Schnorr_Verify\n");
            return 0;   
        }    
        cycles2 = cpucycles();
        cycles = cycles + (cycles2 - cycles1);
    }
    printf(" BPV_Schnorr_Verify runs in ............................................... %8lld ", cycles/BENCH_LOOPS); print_unit;  
    printf("\n");

    cycles = 0;
    for (n = 0; n < BENCH_LOOPS; n++)
    {
        cycles1 = cpucycles();

        // Status = KeyGeneration(SecretKeyA, PublicKeyA);
        Status = DBPV_ECIES_Encrypt(secretKey, ephemeral_PK);
        if (Status != ECCRYPTO_SUCCESS) {
            printf("Error in DBPV_ECIES_Encrypt\n");
            return 0;   
        }    
        cycles2 = cpucycles();
        cycles = cycles + (cycles2 - cycles1);
    }
    printf(" DBPV_ECIES_Encrypt runs in ............................................... %8lld ", cycles/BENCH_LOOPS); print_unit;  
    printf("\n");

    cycles = 0;
    for (n = 0; n < BENCH_LOOPS; n++)
    {
        cycles1 = cpucycles();

        // Status = KeyGeneration(SecretKeyA, PublicKeyA);
        Status = DBPV_ECIES_Decrypt(receiver_sk, ephemeral_PK);
        if (Status != ECCRYPTO_SUCCESS) {
            printf("Error in DBPV_ECIES_Decrypt\n");
            return 0;   
        }    
        cycles2 = cpucycles();
        cycles = cycles + (cycles2 - cycles1);
    }
    printf(" DBPV_ECIES_Decrypt runs in ............................................... %8lld ", cycles/BENCH_LOOPS); print_unit;  
    printf("\n");

    return 0;
}