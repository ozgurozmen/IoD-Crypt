/********************************************************************************
* FourQlib: a high-performance crypto library based on the elliptic curve FourQ
*
*    Copyright (c) Microsoft Corporation. All rights reserved.
*
* Abstract: Diffie-Hellman key exchange based on FourQ
*           option 1: co-factor ecdh using compressed 32-byte public keys,
*           (see https://datatracker.ietf.org/doc/draft-ladd-cfrg-4q/).         
*           option 2: co-factor ecdh using uncompressed, 64-byte public keys. 
*********************************************************************************/

#include "FourQ_internal.h"
#include "FourQ_params.h"
#include "FourQ_api.h"
#include "../random/random.h"
#include "../sha512/sha512.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>


static const unsigned char CA_secretKey[32] = {0x4e, 0x79, 0xa2, 0x3e, 0x29, 0x99, 0x15, 0x41, 0x76, 0x20, 0x5b, 0xac, 0x5e, 0x1a, 0xde, 0x3f, 0x91, 0xe, 0x88, 0x9b, 0x94, 0xf7, 0x1, 0x78, 0xde, 0xf3, 0xa3, 0xb0, 0x2e, 0xcd, 0xf4, 0xc8};
static const unsigned char CA_PublicKey[64] = {0x12, 0xe1, 0xb6, 0xf0, 0x7d, 0x5a, 0x48, 0x8f, 0x98, 0x9, 0x3c, 0xab, 0x7b, 0xc4, 0x54, 0x73, 0x53, 0x59, 0x55, 0xbc, 0xf8, 0x8f, 0x21, 0x32, 0xae, 0xb8, 0x39, 0xda, 0x3, 0xdb, 0xc, 0x5e, 0x23, 0xfa, 0x85, 0x84, 0xce, 0xb, 0xc0, 0xdd, 0x34, 0x8a, 0xd7, 0x58, 0x6, 0x5d, 0xe, 0x5, 0xe4, 0x4d, 0x41, 0x83, 0xe8, 0xed, 0xf8, 0xb0, 0xb5, 0x67, 0x62, 0xe0, 0xc8, 0xff, 0xfd, 0x14};
// static const unsigned char receiver_PK[64] = {0xf1, 0xb2, 0xef, 0xfb, 0xb0, 0x15, 0xf0, 0xad, 0xe2, 0x52, 0xf, 0xdf, 0xf8, 0x99, 0x9b, 0x5e, 0xa9, 0x82, 0x36, 0xd3, 0x79, 0xe4, 0x5f, 0x6a, 0xcc, 0x6a, 0x19, 0xfc, 0xd7, 0xd7, 0x19, 0x3e, 0x9b, 0xa5, 0xb, 0xcf, 0x4c, 0xf2, 0xea, 0x9a, 0x15, 0x1c, 0xb4, 0x3e, 0xf3, 0x71, 0x4b, 0x23, 0x85, 0x64, 0x7c, 0xc9, 0x93, 0x56, 0xc1, 0x77, 0xc0, 0xd4, 0x80, 0xb0, 0x85, 0xac, 0x13, 0xe};
static const unsigned char receiver_sk[32] = {0xb1, 0x15, 0xae, 0xcc, 0xee, 0xe6, 0x6d, 0x2f, 0x91, 0xfb, 0x4e, 0x46, 0x1a, 0xa2, 0x41, 0x3a, 0xa0, 0x79, 0xb1, 0xf1, 0xe8, 0xd7, 0xaa, 0xca, 0xf0, 0xa8, 0xbf, 0xd5, 0xa, 0xf5, 0xda, 0xc8};
static const unsigned char receiver_PK[64] = {0xdc, 0xf8, 0x1e, 0x67, 0x28, 0x4c, 0x3f, 0xd9, 0x5, 0xb3, 0xc3, 0x2e, 0x4, 0x0, 0x6, 0x6c, 0xdc, 0x3d, 0xcd, 0x59, 0x31, 0x4c, 0xee, 0x6a, 0xb7, 0xa6, 0x83, 0xa1, 0xfa, 0xe, 0x44, 0x7d, 0x99, 0xbb, 0x40, 0xaf, 0xd9, 0x56, 0x18, 0x7a, 0x79, 0xb3, 0xc6, 0xc, 0xb3, 0xab, 0xed, 0x71, 0xf0, 0x18, 0xb7, 0x64, 0x6, 0x82, 0x96, 0xf9, 0xef, 0xd, 0x5, 0xbf, 0x70, 0x86, 0x15, 0x67};


static __inline bool is_neutral_point(point_t P)
{ // Is P the neutral point (0,1)?
  // SECURITY NOTE: this function does not run in constant time (input point P is assumed to be public).
  
    if (is_zero_ct((digit_t*)P->x, 2*NWORDS_FIELD) && is_zero_ct(&((digit_t*)P->y)[1], 2*NWORDS_FIELD-1) && is_digit_zero_ct(P->y[0][0] - 1)) {  
		return true;
    }
    return false;
}


/*************** ECDH USING COMPRESSED, 32-BYTE PUBLIC KEYS ***************/

ECCRYPTO_STATUS CompressedPublicKeyGeneration(const unsigned char* SecretKey, unsigned char* PublicKey)
{ // Compressed public key generation for key exchange
  // It produces a public key PublicKey, which is the encoding of P = SecretKey*G (G is the generator).
  // Input:  32-byte SecretKey
  // Output: 32-byte PublicKey
    point_t P;
    
    ecc_mul_fixed((digit_t*)SecretKey, P);  // Compute public key                                       
	encode(P, PublicKey);                   // Encode public key

    return ECCRYPTO_SUCCESS;
}


ECCRYPTO_STATUS CompressedKeyGeneration(unsigned char* SecretKey, unsigned char* PublicKey)
{ // Keypair generation for key exchange. Public key is compressed to 32 bytes
  // It produces a private key SecretKey and a public key PublicKey, which is the encoding of P = SecretKey*G (G is the generator).
  // Outputs: 32-byte SecretKey and 32-byte PublicKey 
    ECCRYPTO_STATUS Status = ECCRYPTO_ERROR_UNKNOWN;

	Status = RandomBytesFunction(SecretKey, 32);
	if (Status != ECCRYPTO_SUCCESS) {
		goto cleanup;
	}
  
    Status = CompressedPublicKeyGeneration(SecretKey, PublicKey);
    if (Status != ECCRYPTO_SUCCESS) {
        goto cleanup;
    }

    return ECCRYPTO_SUCCESS;

cleanup:
    clear_words((unsigned int*)SecretKey, 256/(sizeof(unsigned int)*8));
    clear_words((unsigned int*)PublicKey, 256/(sizeof(unsigned int)*8));

    return Status;
}


ECCRYPTO_STATUS CompressedSecretAgreement(const unsigned char* SecretKey, const unsigned char* PublicKey, unsigned char* SharedSecret)
{ // Secret agreement computation for key exchange using a compressed, 32-byte public key
  // The output is the y-coordinate of SecretKey*A, where A is the decoding of the public key PublicKey.   
  // Inputs: 32-byte SecretKey and 32-byte PublicKey
  // Output: 32-byte SharedSecret
    point_t A;
    ECCRYPTO_STATUS Status = ECCRYPTO_ERROR_UNKNOWN;

    if ((PublicKey[15] & 0x80) != 0) {  // Is bit128(PublicKey) = 0?
		Status = ECCRYPTO_ERROR_INVALID_PARAMETER;
		goto cleanup;
    }

	Status = decode(PublicKey, A);    // Also verifies that A is on the curve. If it is not, it fails
	if (Status != ECCRYPTO_SUCCESS) {
		goto cleanup;
	}
         
    Status = ecc_mul(A, (digit_t*)SecretKey, A, true);
	if (Status != ECCRYPTO_SUCCESS) {
		goto cleanup;
	}

    if (is_neutral_point(A)) {  // Is output = neutral point (0,1)?
		Status = ECCRYPTO_ERROR_SHARED_KEY;
		goto cleanup;
    }
  
	memmove(SharedSecret, (unsigned char*)A->y, 32);

	return ECCRYPTO_SUCCESS;
    
cleanup:
    clear_words((unsigned int*)SharedSecret, 256/(sizeof(unsigned int)*8));
    
    return Status;
}


/*************** ECDH USING UNCOMPRESSED PUBLIC KEYS ***************/

ECCRYPTO_STATUS PublicKeyGeneration(const unsigned char* SecretKey, unsigned char* PublicKey)
{ // Public key generation for key exchange
  // It produces the public key PublicKey = SecretKey*G, where G is the generator.
  // Input:  32-byte SecretKey
  // Output: 64-byte PublicKey

	ecc_mul_fixed((digit_t*)SecretKey, (point_affine*)PublicKey);  // Compute public key

	return ECCRYPTO_SUCCESS;
}

ECCRYPTO_STATUS KeyGeneration(unsigned char* SecretKey, unsigned char* PublicKey)
{ // Keypair generation for key exchange
  // It produces a private key SecretKey and computes the public key PublicKey = SecretKey*G, where G is the generator.
  // Outputs: 32-byte SecretKey and 64-byte PublicKey 
	ECCRYPTO_STATUS Status = ECCRYPTO_ERROR_UNKNOWN;

	Status = RandomBytesFunction(SecretKey, 32);
	if (Status != ECCRYPTO_SUCCESS) {
		goto cleanup;
	}

	Status = PublicKeyGeneration(SecretKey, PublicKey);
	if (Status != ECCRYPTO_SUCCESS) {
		goto cleanup;
	}

	return ECCRYPTO_SUCCESS;

cleanup:
	clear_words((unsigned int*)SecretKey, 256/(sizeof(unsigned int)*8));
	clear_words((unsigned int*)PublicKey, 512/(sizeof(unsigned int)*8));

	return Status;
}


ECCRYPTO_STATUS AQHang_Offline_Temp(unsigned char* SecretKey, unsigned char* PublicKey){

	unsigned char secretTemp[32];
	unsigned char hashed[64];

	digit_t* Hashed = (digit_t*)hashed;
	digit_t* sk = (digit_t*)secretTemp;

	ECCRYPTO_STATUS Status = ECCRYPTO_ERROR_UNKNOWN;

	Status = RandomBytesFunction(secretTemp, 32);
	if (Status != ECCRYPTO_SUCCESS) {
		goto cleanup;
	}

	Status = PublicKeyGeneration(secretTemp, PublicKey); 
	if (Status != ECCRYPTO_SUCCESS) {
		goto cleanup;
	}
  
    if (CryptoHashFunction(PublicKey, 64, hashed) != 0) {   
        Status = ECCRYPTO_ERROR;
        goto cleanup;
    }

    modulo_order(Hashed, Hashed);
    to_Montgomery(Hashed, Hashed);
    to_Montgomery(sk, sk);
    Montgomery_multiply_mod_order(sk, Hashed, sk);
    from_Montgomery(sk, sk);
    add_mod_order((digit_t*)CA_secretKey, sk, (digit_t*)SecretKey);

	return ECCRYPTO_SUCCESS;

	

cleanup:
	clear_words((unsigned int*)SecretKey, 256/(sizeof(unsigned int)*8));
	clear_words((unsigned int*)PublicKey, 512/(sizeof(unsigned int)*8));
	clear_words((unsigned int*)hashed, 512/(sizeof(unsigned int)*8));
	clear_words((unsigned int*)secretTemp, 256/(sizeof(unsigned int)*8));

	return Status;
}








ECCRYPTO_STATUS KeyGeneration_TEMP(unsigned char* SecretKey, unsigned char* PublicKey)
{ // Keypair generation for key exchange
  // It produces a private key SecretKey and computes the public key PublicKey = SecretKey*G, where G is the generator.
  // Outputs: 32-byte SecretKey and 64-byte PublicKey
	int BPV_n = 2;
	ECCRYPTO_STATUS Status = ECCRYPTO_ERROR_UNKNOWN;

	// unsigned char receiver_sk[32], receiver_PK[64];
	// Status =  AQHang_Offline(receiver_sk, receiver_PK);
	// if (Status != ECCRYPTO_SUCCESS) {
	// 	goto cleanup;
	// }

	printf("static const unsigned char receiver_sk[32] = {");
	for (unsigned int i = 0; i < 32; i++) {
	printf("0x%x, ", receiver_sk[i]);
	}
	printf("};\n");

	printf("static const unsigned char receiver_PK[64] = {");
	for (unsigned int i = 0; i < 64; i++) {
	printf("0x%x, ", receiver_PK[i]);
	}
	printf("};\n");

	bool clear_cofactor = false;
	point_t R;
	unsigned char hashed[64], receiver[64], receiver2[64];
	eccset((point_affine*)receiver);

	digit_t* Hashed = (digit_t*)hashed;

	
	unsigned char *secretAll, *publicAll_BPV, *publicAll_DBPV;

	secretAll = malloc(32*BPV_n);
	publicAll_BPV = malloc(64*BPV_n);
	publicAll_DBPV = malloc(64*BPV_n);


	Status = RandomBytesFunction(secretAll, 32*BPV_n);
	if (Status != ECCRYPTO_SUCCESS) {
		goto cleanup;
	}

	if (CryptoHashFunction(receiver_PK, 64, hashed) != 0) {   
        Status = ECCRYPTO_ERROR;
        goto cleanup;
    }

    modulo_order(Hashed, Hashed);

    ecc_mul((point_affine*)receiver_PK, Hashed, (point_affine*)receiver, clear_cofactor);

    point_extproj_t TempExtproj;
    point_extproj_t AddedExtproj;
    point_extproj_precomp_t TempExtprojPre;

    point_setup((point_affine*)CA_PublicKey, AddedExtproj);
    point_setup((point_affine*)receiver, TempExtproj);

    if (ecc_point_validate(AddedExtproj) == false) {                     // Check if point lies on the curve
        printf("AddedExtProj not on curve");
    }
    if (ecc_point_validate(TempExtproj) == false) {                     // Check if point lies on the curve
        printf("TempExtproj not on curve");
    }

	R1_to_R2(TempExtproj, TempExtprojPre);

	eccadd(TempExtprojPre, AddedExtproj);

	if (ecc_point_validate(AddedExtproj) == false) {                     // Check if point lies on the curve
        printf("Point is invalidated after addition (AddedExtproj)!!!!  \n");
    }

	eccnorm(AddedExtproj, (point_affine*)receiver);


	printf("receiver = {");
	for (unsigned int i = 0; i < 64; i++) {
	printf("0x%x, ", receiver[i]);
	}
	printf("};\n");


	Status = PublicKeyGeneration(receiver_sk, receiver2);
	if (Status != ECCRYPTO_SUCCESS) {
		goto cleanup;
	}


	printf("receiver2 = {");
	for (unsigned int i = 0; i < 64; i++) {
	printf("0x%x, ", receiver2[i]);
	}
	printf("};\n");

	unsigned int k;
    unsigned char secretTemp[32], publicTemp[64], publicTemp2[64];
    
    for(k = 0; k < BPV_n; k++){

    	memmove(secretTemp, secretAll + k*32, 32);

		Status = PublicKeyGeneration(secretTemp, publicTemp);
		if (Status != ECCRYPTO_SUCCESS) {
			goto cleanup;
		}

		memmove(publicAll_BPV+k*64, publicTemp, 64);

		// ecc_mul((point_affine*)receiver, (digit_t*)secretTemp, (point_affine*)publicTemp2, clear_cofactor);


		// memmove(publicAll_DBPV+k*64, publicTemp2, 64);
		// memset(publicTemp2, 0, 64);

	}


	for(k = 0; k < BPV_n; k++){

    	memmove(secretTemp, secretAll + k*32, 32);

		ecc_mul((point_affine*)receiver, (digit_t*)secretTemp, (point_affine*)publicTemp2, clear_cofactor);

		memmove(publicAll_DBPV+k*64, publicTemp2, 64);
		memset(publicTemp2, 0, 64);

	}

	printf("static const unsigned char scalar_table[BPV_n*32] = {");
	for (unsigned int i = 0; i < 32*BPV_n; i++) {
	printf("0x%x, ", secretAll[i]);
	}
	printf("};\n");

	printf("static const unsigned char BPV_table[BPV_n*64] = {");
	for (unsigned int i = 0; i < 64*BPV_n; i++) {
	printf("0x%x, ", publicAll_BPV[i]);
	}
	printf("};\n");

	printf("static const unsigned char DBPV_table[BPV_n*64] = {");
	for (unsigned int i = 0; i < 64*BPV_n; i++) {
	printf("0x%x, ", publicAll_DBPV[i]);
	}
	printf("};\n");

	free(secretAll);
	free(publicAll_BPV);
	free(publicAll_DBPV);

	return ECCRYPTO_SUCCESS;

cleanup:
	clear_words((unsigned int*)SecretKey, 256/(sizeof(unsigned int)*8));
	clear_words((unsigned int*)PublicKey, 512/(sizeof(unsigned int)*8));

	return Status;
}

// //Returns (H(PublicKey) x PublicKey + CA_PublicKey)
// ECCRYPTO_STATUS precompute(const unsigned char* PublicKey, unsigned char* PrecomputedPublicKey){

// 	ECCRYPTO_STATUS Status = ECCRYPTO_ERROR_UNKNOWN;
// 	bool clear_cofactor = false;
// 	unsigned char hashed[64], receiver[64];
// 	eccset((point_affine*)receiver);
// 	digit_t* Hashed = (digit_t*)hashed;

// 	if (CryptoHashFunction(PublicKey, 64, hashed) != 0) {   
//         Status = ECCRYPTO_ERROR;
//         goto cleanup;
//     }

//     modulo_order(Hashed, Hashed);

//     ecc_mul((point_affine*)PublicKey, Hashed, (point_affine*)receiver, clear_cofactor);

//     point_extproj_t TempExtproj;
//     point_extproj_t AddedExtproj;
//     point_extproj_precomp_t TempExtprojPre;

//     point_setup((point_affine*)CA_PublicKey, AddedExtproj);
//     point_setup((point_affine*)receiver, TempExtproj);


// 	R1_to_R2(TempExtproj, TempExtprojPre);

// 	eccadd(TempExtprojPre, AddedExtproj);
// 	eccnorm(AddedExtproj, (point_affine*)PrecomputedPublicKey);

// 	return ECCRYPTO_SUCCESS;
	
// cleanup:
// 	clear_words((unsigned int*)PrecomputedPublicKey, 512/(sizeof(unsigned int)*8));

// 	return Status;
// }


ECCRYPTO_STATUS SecretAgreement(const unsigned char* SecretKey, const unsigned char* PublicKey, unsigned char* SharedSecret)
{ // Secret agreement computation for key exchange
  // The output is the y-coordinate of SecretKey*PublicKey. 
  // Inputs: 32-byte SecretKey and 64-byte PublicKey
  // Output: 32-byte SharedSecret
	point_t A;
	ECCRYPTO_STATUS Status = ECCRYPTO_ERROR_UNKNOWN;

    if (((PublicKey[15] & 0x80) != 0) || ((PublicKey[31] & 0x80) != 0) || ((PublicKey[47] & 0x80) != 0) || ((PublicKey[63] & 0x80) != 0)) {  // Are PublicKey_x[i] and PublicKey_y[i] < 2^127?
		Status = ECCRYPTO_ERROR_INVALID_PARAMETER;
		goto cleanup;
    }

	Status = ecc_mul((point_affine*)PublicKey, (digit_t*)SecretKey, A, true);  // Also verifies that PublicKey is a point on the curve. If it is not, it fails
	if (Status != ECCRYPTO_SUCCESS) {
		goto cleanup;
	}

    if (is_neutral_point(A)) {  // Is output = neutral point (0,1)?
		Status = ECCRYPTO_ERROR_SHARED_KEY;
		goto cleanup;
    }
  
	memmove(SharedSecret, (unsigned char*)A->y, 32);

	return ECCRYPTO_SUCCESS;

cleanup:
	clear_words((unsigned int*)SharedSecret, 256/(sizeof(unsigned int)*8));

	return Status;
}