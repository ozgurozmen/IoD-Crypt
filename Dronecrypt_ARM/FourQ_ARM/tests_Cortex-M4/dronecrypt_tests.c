#include "../FourQ_internal.h"
#include "../FourQ_params.h"
#include "test_extras.h"
#include "../stm32f4_wrapper.h"
#include <stdio.h>
#include "../Dronecrypt.h"

static unsigned int *DWT_CYCCNT = (unsigned int*)0xE0001004;
static unsigned int *DWT_CTRL = (unsigned int*)0xE0001000;
static unsigned int *SCB_DEMCR = (unsigned int*)0xE000EDFC;

// Benchmark and test parameters  
#define BENCH_LOOPS       10       // Number of iterations per bench
#define TEST_LOOPS        10       // Number of iterations per test

#define cpucycles() (*DWT_CYCCNT);


static void print_test(const char *text)
{
    unsigned char output[100];

    sprintf((char*)output, "%s", text);
    send_USART_str(output);
}


static void print_bench(const char *s, unsigned int cycles)
{
  unsigned char output[100];

  sprintf((char*)output, "%s %8u cycles", s, cycles);
  send_USART_str(output);
}




int main()
{
    clock_setup();
    gpio_setup();
    usart_setup(115200);
    rng_setup();

    *SCB_DEMCR = *SCB_DEMCR | 0x01000000;
    *DWT_CYCCNT = 0;             // reset the counter
    *DWT_CTRL = *DWT_CTRL | 1 ;  // enable the counter
    ECCRYPTO_STATUS Status = ECCRYPTO_SUCCESS;

    unsigned int cycles, cycles1, cycles2, n, len, valid = false;
    unsigned char secretKey[32], publicKey[64], publicKey_precomp[64], secret[64], ephemeralsecret[64], ephemeral_sk[32], ephemeral_PK[64], Signature[64];

    print_test("\n--------------------------------------------------------------------------------------------------------\n"); 
    print_test("Testing Dronecrypt!!: \n"); 

    //CA assigns the Arazi key to the drone
    Status = AQHang_Offline(secretKey, publicKey);
    if (Status != ECCRYPTO_SUCCESS) {
        print_test("Error in AQHang_Offline\n");
        signal_host();
        return 0;
    }

    print_test("\n--------------------------------------------------------------------------------------------------------\n");
    precompute(publicKey, publicKey_precomp);
    if (Status != ECCRYPTO_SUCCESS) {
        print_test("Error in precompute\n");
        signal_host();
        return 0;
    }

    cycles = 0;
    for (n = 0; n < BENCH_LOOPS; n++)
    {
        cycles1 = cpucycles();

        // Status = KeyGeneration(SecretKeyA, PublicKeyA);
        Status = BPV_AQHang_Online(secretKey, secret);
        if (Status != ECCRYPTO_SUCCESS) {
            print_test("Error in BPV_AQHang_Online\n");
            signal_host();
            return 0;
        }
        cycles2 = cpucycles();
        cycles = cycles + (cycles2 - cycles1);
    }
    print_bench("BPV_AQHang_Online runs in .............................................. ", cycles/BENCH_LOOPS);

    //For ephemeral key exchange, we assume other party sends their ephemeral key, ephemeral_PK is their Public Key!
    Status = KeyGeneration(ephemeral_sk, ephemeral_PK);
    if (Status != ECCRYPTO_SUCCESS) {
        print_test("Error in Ephemeral KeyGeneration\n");
        signal_host();
        return 0;
    }

    cycles = 0;
    for (n = 0; n < BENCH_LOOPS; n++)
    {
        cycles1 = cpucycles();

        // Status = KeyGeneration(SecretKeyA, PublicKeyA);
        Status = BPV_Ephemeral_AQHang_Online(secretKey, secret, ephemeral_PK, ephemeralsecret);
        if (Status != ECCRYPTO_SUCCESS) {
            print_test("Error in BPV_Ephemeral_AQHang_Online\n");
            signal_host();
            return 0;
        }
        cycles2 = cpucycles();
        cycles = cycles + (cycles2 - cycles1);
    }
    print_bench("BPV_Ephemeral_AQHang_Online runs in .......................................... ", cycles/BENCH_LOOPS);

    void *msg = NULL; 
    msg = "a";  
    len = 1;
    Status = BPV_Schnorr(secretKey, msg, len, Signature);
    if (Status != ECCRYPTO_SUCCESS) {
        print_test("Error in BPV_Schnorr\n");
        signal_host();
        return 0;   
    }    
    print_test("BPV_Schnorr successful\n");
    //Receiver precomputes publicKey_precomp and uses it to verify the message, to avoid scalar multiplications online.
    Status = BPV_Schnorr_Verify(publicKey_precomp, msg, len, Signature, &valid);
    if (Status != ECCRYPTO_SUCCESS) {
        print_test("Error in BPV_Schnorr_Verify\n");
        signal_host();
        return 0;   
    }    
    print_test("BPV_Schnorr_Verify successful\n");
    if (valid == false) {
        print_test("Error in BPV_Schnorr_Verify -- Signature not verified\n"); //Gave a problem here!!
        signal_host();
        return 0;   
    }


    msg = "b";  
    Status = BPV_Schnorr_Verify(publicKey_precomp, msg, len, Signature, &valid);
    if (Status != ECCRYPTO_SUCCESS) {
        print_test("Error in BPV_Schnorr_Verify\n");
        signal_host();
        return 0;   
    }   
    print_test("BPV_Schnorr_Verify successful 2\n");
    if (valid == true) {
        print_test("Error in BPV_Schnorr_Verify -- Signature verified, wasn't supposed to\n");
        signal_host();
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
            print_test("Error in BPV_Schnorr\n");
            signal_host();
            return 0;   
        }    
        cycles2 = cpucycles();
        cycles = cycles + (cycles2 - cycles1);
    }
    print_bench("BPV_Schnorr runs in ...................................................... ", cycles/BENCH_LOOPS);

    cycles = 0;
    for (n = 0; n < BENCH_LOOPS; n++)
    {
        cycles1 = cpucycles();

        // Status = KeyGeneration(SecretKeyA, PublicKeyA);
        Status = BPV_Schnorr_Verify(publicKey_precomp, msg, len, Signature, &valid);
        if (Status != ECCRYPTO_SUCCESS) {
            print_test("Error in BPV_Schnorr_Verify\n");
            signal_host();
            return 0;   
        }    
        cycles2 = cpucycles();
        cycles = cycles + (cycles2 - cycles1);
    }
    print_bench("BPV_Schnorr_Verify runs in ................................................ ", cycles/BENCH_LOOPS);


    cycles = 0;
    for (n = 0; n < BENCH_LOOPS; n++)
    {
        cycles1 = cpucycles();

        // Status = KeyGeneration(SecretKeyA, PublicKeyA);
        Status = DBPV_ECIES_Encrypt(secretKey, ephemeral_PK);
        if (Status != ECCRYPTO_SUCCESS) {
            print_test("Error in DBPV_ECIES_Encrypt\n");
            signal_host();
            return 0;   
        }    
        cycles2 = cpucycles();
        cycles = cycles + (cycles2 - cycles1);
    }
    print_bench("DBPV_ECIES_Encrypt runs in ................................................ ", cycles/BENCH_LOOPS);


    cycles = 0;
    for (n = 0; n < BENCH_LOOPS; n++)
    {
        cycles1 = cpucycles();

        // Status = KeyGeneration(SecretKeyA, PublicKeyA);
        Status = DBPV_ECIES_Decrypt(receiver_sk, ephemeral_PK);
        if (Status != ECCRYPTO_SUCCESS) {
            print_test("Error in DBPV_ECIES_Decrypt\n");
            signal_host();
            return 0;   
        }    
        cycles2 = cpucycles();
        cycles = cycles + (cycles2 - cycles1);
    }
    print_bench("DBPV_ECIES_Decrypt runs in ............................................. ", cycles/BENCH_LOOPS);



    signal_host();

    return 0;
}