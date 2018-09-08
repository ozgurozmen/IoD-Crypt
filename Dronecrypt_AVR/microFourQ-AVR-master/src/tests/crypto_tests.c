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
#include "../FourQ_params.h"
#include "test_extras.h"
#include <stdio.h>

#define TEST_LOOPS        10        // Number of iterations per test


ECCRYPTO_STATUS SchnorrQ_test()
{ // Test the SchnorrQ digital signature scheme
    int n, passed;       
    void *msg = NULL; 
    unsigned int len, valid = false;
    unsigned char SecretKey[32], PublicKey[32], Signature[64];
    ECCRYPTO_STATUS Status = ECCRYPTO_SUCCESS;

    passed = 1;
    for (n = 0; n < TEST_LOOPS; n++)
    {
        // Signature key generation
        Status = SchnorrQ_FullKeyGeneration(SecretKey, PublicKey);
        if (Status != ECCRYPTO_SUCCESS) {
            return Status;
        }  

        // Signature computation
        msg = "a";  
        len = 1;
     
        Status = SchnorrQ_Sign(SecretKey, PublicKey, msg, len, Signature);
        if (Status != ECCRYPTO_SUCCESS) {
            return Status;
        }
        
        // Valid signature test
        Status = SchnorrQ_Verify(PublicKey, msg, len, Signature, &valid);
        if (Status != ECCRYPTO_SUCCESS) {
            return Status;
        }
        if (valid == false) {
            passed = 0;
            break;
        }

        // Invalid signature test (flipping one bit of the message)
        msg = "b";  
        Status = SchnorrQ_Verify(PublicKey, msg, len, Signature, &valid);
        if (Status != ECCRYPTO_SUCCESS) {
            return Status;
        }    
        if (valid == true) {
            passed = 0;
            break;
        }
    }
    if (passed==0) { 
      Status = ECCRYPTO_ERROR_SIGNATURE_VERIFICATION; 
    }
    
    return Status;
}


ECCRYPTO_STATUS compressedkex_test()
{ // Test ECDH key exchange based on FourQ
	int n, passed;
	unsigned int i;
	unsigned char SecretKeyA[32], PublicKeyA[32], SecretAgreementA[32];
	unsigned char SecretKeyB[32], PublicKeyB[32], SecretAgreementB[32];
	ECCRYPTO_STATUS Status = ECCRYPTO_SUCCESS;

	passed = 1;
	for (n = 0; n < TEST_LOOPS; n++)
	{
		// Alice's keypair generation
		Status = CompressedKeyGeneration(SecretKeyA, PublicKeyA);
		if (Status != ECCRYPTO_SUCCESS) {
				return Status;
		}
		// Bob's keypair generation
		Status = CompressedKeyGeneration(SecretKeyB, PublicKeyB);
		if (Status != ECCRYPTO_SUCCESS) {
				return Status;
		}

		// Alice's shared secret computation
		Status = CompressedSecretAgreement(SecretKeyA, PublicKeyB, SecretAgreementA);
		if (Status != ECCRYPTO_SUCCESS) {
				return Status;
		}
		// Bob's shared secret computation
		Status = CompressedSecretAgreement(SecretKeyB, PublicKeyA, SecretAgreementB);
		if (Status != ECCRYPTO_SUCCESS) {
				return Status;
		}

		for (i = 0; i < 32; i++) {
				if (SecretAgreementA[i] != SecretAgreementB[i]) {
						passed = 0;
						break;
				}
		}          
	}
	if (passed==0) {
		Status = ECCRYPTO_ERROR_SHARED_KEY; 
    }

	return Status;
}

ECCRYPTO_STATUS kex_test()
{ // Test ECDH key exchange based on FourQ
	int n, passed;
	unsigned int i;
	unsigned char SecretKeyA[32], PublicKeyA[64], SecretAgreementA[32];
	unsigned char SecretKeyB[32], PublicKeyB[64], SecretAgreementB[32];
	ECCRYPTO_STATUS Status = ECCRYPTO_SUCCESS;

	passed = 1;
	for (n = 0; n < TEST_LOOPS; n++)
	{
          // Alice's keypair generation
            Status = KeyGeneration(SecretKeyA, PublicKeyA);
            if (Status != ECCRYPTO_SUCCESS) {
                    return Status;
            }
            // Bob's keypair generation
            Status = KeyGeneration(SecretKeyB, PublicKeyB);
            if (Status != ECCRYPTO_SUCCESS) {
                    return Status;
            }
            // Alice's shared secret computation
            Status = SecretAgreement(SecretKeyA, PublicKeyB, SecretAgreementA);
            if (Status != ECCRYPTO_SUCCESS) {
                    return Status;
            }
            // Bob's shared secret computation
            Status = SecretAgreement(SecretKeyB, PublicKeyA, SecretAgreementB);
            if (Status != ECCRYPTO_SUCCESS) {
                    return Status;
            }

            for (i = 0; i < 32; i++) {
                    if (SecretAgreementA[i] != SecretAgreementB[i]) {
                            passed = 0;
                            break;
                    }
            }         
	}
	if (passed==0) {
          Status = ECCRYPTO_ERROR_SHARED_KEY; 
    }

	return Status;
}


int crypto_tests()
{
    ECCRYPTO_STATUS Status = ECCRYPTO_SUCCESS;

    Status = SchnorrQ_test();         // Test SchnorrQ signature scheme
    if (Status != ECCRYPTO_SUCCESS) {
        return false;
    }

    Status = compressedkex_test();    // Test Diffie-Hellman key exchange using compressed public keys
    if (Status != ECCRYPTO_SUCCESS) {
        return false;
    }

    Status = kex_test();              // Test Diffie-Hellman key exchange using uncompressed public keys
    if (Status != ECCRYPTO_SUCCESS) {
        return false;
    }

    return true;
}
