#include "FourQ_internal.h"
#include "FourQ_params.h"
#include "FourQ_api.h"
#include "../random/random.h"
#include "../sha512/sha512.h"
#include <malloc.h>
#include <string.h>
#include "Dronecrypt.h"


//Performed by resourceful Certification Authority in practice, therefore not benchmarked.
ECCRYPTO_STATUS AQHang_Offline(unsigned char* SecretKey, unsigned char* PublicKey){

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

	return Status;
}

// //Returns (H(PublicKey) x PublicKey + CA_PublicKey)
ECCRYPTO_STATUS precompute(const unsigned char* PublicKey, unsigned char* PrecomputedPublicKey){

	ECCRYPTO_STATUS Status = ECCRYPTO_ERROR_UNKNOWN;
	bool clear_cofactor = false;
	unsigned char hashed[64], receiver[64];
	eccset((point_affine*)receiver);
	digit_t* Hashed = (digit_t*)hashed;

	if (CryptoHashFunction(PublicKey, 64, hashed) != 0) {   
        Status = ECCRYPTO_ERROR;
        goto cleanup;
    }

    modulo_order(Hashed, Hashed);

    ecc_mul((point_affine*)PublicKey, Hashed, (point_affine*)receiver, clear_cofactor);

    point_extproj_t TempExtproj;
    point_extproj_t AddedExtproj;
    point_extproj_precomp_t TempExtprojPre;

    point_setup((point_affine*)CA_PublicKey, AddedExtproj);
    point_setup((point_affine*)receiver, TempExtproj);


	R1_to_R2(TempExtproj, TempExtprojPre);

	eccadd(TempExtprojPre, AddedExtproj);
	eccnorm(AddedExtproj, (point_affine*)PrecomputedPublicKey);

	return ECCRYPTO_SUCCESS;
	
cleanup:
	clear_words((unsigned int*)PrecomputedPublicKey, 512/(sizeof(unsigned int)*8));

	return Status;
}

ECCRYPTO_STATUS BPV_KeyGeneration(unsigned char* secretAll, unsigned char* publicAll_BPV, unsigned char* publicAll_DBPV)
{ 
	ECCRYPTO_STATUS Status = ECCRYPTO_ERROR_UNKNOWN;

	bool clear_cofactor = false;
	point_t R;
	unsigned char hashed[64], receiver[64];
	eccset((point_affine*)receiver);

	digit_t* Hashed = (digit_t*)hashed;

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


	R1_to_R2(TempExtproj, TempExtprojPre);

	eccadd(TempExtprojPre, AddedExtproj);
	eccnorm(AddedExtproj, (point_affine*)receiver);

	unsigned int k;
    unsigned char secretTemp[32], publicTemp[64], publicTemp2[64];
    
    for(k = 0; k < BPV_n; k++){

    	memmove(secretTemp, secretAll + k*32, 32);

		Status = PublicKeyGeneration(secretTemp, publicTemp);
		if (Status != ECCRYPTO_SUCCESS) {
			goto cleanup;
		}

		memmove(publicAll_BPV+k*64, publicTemp, 64);
	}


	for(k = 0; k < BPV_n; k++){

    	memmove(secretTemp, secretAll + k*32, 32);

		ecc_mul((point_affine*)receiver, (digit_t*)secretTemp, (point_affine*)publicTemp2, clear_cofactor);

		memmove(publicAll_DBPV+k*64, publicTemp2, 64);
		memset(publicTemp2, 0, 64);

	}

	return ECCRYPTO_SUCCESS;

cleanup:


	return Status;
}


ECCRYPTO_STATUS BPVOnline(unsigned char* lastSecret, unsigned char* lastPublic)
{
	ECCRYPTO_STATUS Status = ECCRYPTO_ERROR_UNKNOWN;
	digit_t* S = (digit_t*)(lastSecret);
	unsigned char index[BPV_k];
	unsigned char secretTemp[32], publicTemp[64];
	unsigned char secretTemp2[32];
	int k;
	//point_t Temp;
	point_t Added;
	point_extproj_t AddedExtproj;
	point_extproj_t TempExtproj;
	point_extproj_precomp_t TempExtprojPre;
	
	Status = RandomBytesFunction(index, BPV_k);
	if (Status != ECCRYPTO_SUCCESS) {
		goto cleanup;
	}
	memmove(secretTemp,scalar_table +32*index[0],32);
	memmove(publicTemp,BPV_table +64*index[0],64);
	
	point_setup((point_affine*)publicTemp, AddedExtproj);
	
	memmove(secretTemp2,scalar_table +32*index[1],32);
	memmove(publicTemp,BPV_table +64*index[1],64);
	
	point_setup((point_affine*)publicTemp, TempExtproj);
	R1_to_R2(TempExtproj, TempExtprojPre);
	
	eccadd(TempExtprojPre, AddedExtproj);
	add_mod_order((digit_t*)secretTemp, (digit_t*)secretTemp2, S);
	
	for(k = 0; k < BPV_k - 2; k++){
		memmove(secretTemp,scalar_table +32*index[k+2],32);
		memmove(publicTemp,BPV_table +64*index[k+2],64);

		point_setup((point_affine*)publicTemp, TempExtproj);
		R1_to_R2(TempExtproj, TempExtprojPre);
		
		eccadd(TempExtprojPre, AddedExtproj);

		add_mod_order((digit_t*)secretTemp, S, S);
	}
	
	eccnorm(AddedExtproj, (point_affine*)lastPublic);
	
	return ECCRYPTO_SUCCESS;
	
cleanup:
	return Status;
}

ECCRYPTO_STATUS DBPVOnline(unsigned char* lastSecret, unsigned char* lastPublic, unsigned char* lastDesignated)
{
	ECCRYPTO_STATUS Status = ECCRYPTO_ERROR_UNKNOWN;
	digit_t* S = (digit_t*)(lastSecret);
	unsigned char index[BPV_k];
	unsigned char secretTemp[32], publicTemp[64], publicTemp2[64];
	unsigned char secretTemp2[32];
	int k;
	//point_t Temp;
	point_t Added, Added2;
	point_extproj_t AddedExtproj, AddedExtproj2;
	point_extproj_t TempExtproj, TempExtproj2;
	point_extproj_precomp_t TempExtprojPre, TempExtprojPre2;
	
	Status = RandomBytesFunction(index, BPV_k);
	if (Status != ECCRYPTO_SUCCESS) {
		goto cleanup;
	}
	memmove(secretTemp,scalar_table +32*index[0],32);
	memmove(publicTemp,BPV_table +64*index[0],64);
	memmove(publicTemp2,DBPV_table +64*index[0],64);
	
	point_setup((point_affine*)publicTemp, AddedExtproj);
	point_setup((point_affine*)publicTemp2, AddedExtproj2);
	
	memmove(secretTemp2,scalar_table +32*index[1],32);
	memmove(publicTemp,BPV_table +64*index[1],64);
	memmove(publicTemp2,DBPV_table +64*index[1],64);
	
	point_setup((point_affine*)publicTemp, TempExtproj);
	point_setup((point_affine*)publicTemp2, TempExtproj2);
	R1_to_R2(TempExtproj, TempExtprojPre);
	R1_to_R2(TempExtproj2, TempExtprojPre2);
	
	eccadd(TempExtprojPre, AddedExtproj);
	eccadd(TempExtprojPre2, AddedExtproj2);
	add_mod_order((digit_t*)secretTemp, (digit_t*)secretTemp2, S);
	
	for(k = 0; k < BPV_k - 2; k++){
		memmove(secretTemp,scalar_table +32*index[k+2],32);
		memmove(publicTemp,BPV_table +64*index[k+2],64);
		memmove(publicTemp2,DBPV_table +64*index[k+2],64);

		point_setup((point_affine*)publicTemp, TempExtproj);
		point_setup((point_affine*)publicTemp2, TempExtproj2);
		R1_to_R2(TempExtproj, TempExtprojPre);
		R1_to_R2(TempExtproj2, TempExtprojPre2);
		
		eccadd(TempExtprojPre, AddedExtproj);
		eccadd(TempExtprojPre2, AddedExtproj2);
		add_mod_order((digit_t*)secretTemp, S, S);
	}
	
	eccnorm(AddedExtproj, (point_affine*)lastPublic);
	eccnorm(AddedExtproj2, (point_affine*)lastDesignated);

	// eccnorm(AddedExtproj, Added);
	// eccnorm(AddedExtproj2, Added2);

	// encode(Added, lastPublic);
	// encode(Added2, lastDesignated);
	
	return ECCRYPTO_SUCCESS;
	
cleanup:
	clear_words((unsigned int*)lastSecret, 256/(sizeof(unsigned int)*8));
	clear_words((unsigned int*)lastPublic, 512/(sizeof(unsigned int)*8));
	clear_words((unsigned int*)lastDesignated, 512/(sizeof(unsigned int)*8));



	return Status;



}


ECCRYPTO_STATUS BPV_AQHang_Online(const unsigned char* SecretKey, unsigned char* SharedSecret){

	ECCRYPTO_STATUS Status = ECCRYPTO_ERROR_UNKNOWN;
	point_t A;

	Status = ecc_mul((point_affine*)receiver_precomp, (digit_t*)SecretKey, A, true);  // Also verifies that PublicKey is a point on the curve. If it is not, it fails
	if (Status != ECCRYPTO_SUCCESS) {
		goto cleanup;
	}
  
	// encode(A, SharedSecret);

	return ECCRYPTO_SUCCESS;

cleanup:
	clear_words((unsigned int*)SharedSecret, 512/(sizeof(unsigned int)*8));

	return Status;
}

ECCRYPTO_STATUS BPV_Ephemeral_AQHang_Online(const unsigned char* SecretKey, const unsigned char* SharedSecret, const unsigned char* Ephemeral_receiverPK, unsigned char* EphemeralSecret){

	ECCRYPTO_STATUS Status = ECCRYPTO_ERROR_UNKNOWN;

	unsigned char ephSecret[32], ephPublic[64];
	//Send this ephPublic to other node
	Status = BPVOnline(ephSecret, ephPublic);
	if (Status != ECCRYPTO_SUCCESS) {
		goto cleanup;
	}

	point_t A;

	Status = ecc_mul((point_affine*)Ephemeral_receiverPK, (digit_t*)ephSecret, A, true);
	if (Status != ECCRYPTO_SUCCESS) {
		goto cleanup;
	}

	point_extproj_t TempExtproj;
    point_extproj_t AddedExtproj;
    point_extproj_precomp_t TempExtprojPre;

    point_setup(A, AddedExtproj);
    point_setup((point_affine*)SharedSecret, TempExtproj);


	R1_to_R2(TempExtproj, TempExtprojPre);

	eccadd(TempExtprojPre, AddedExtproj);
	eccnorm(AddedExtproj, (point_affine*)EphemeralSecret);

	return ECCRYPTO_SUCCESS;

cleanup:
	clear_words((unsigned int*)EphemeralSecret, 512/(sizeof(unsigned int)*8));

	return Status;
}


ECCRYPTO_STATUS BPV_Schnorr(const unsigned char* SecretKey, const unsigned char* Message, const unsigned int SizeMessage, unsigned char* Signature){

	ECCRYPTO_STATUS Status = ECCRYPTO_ERROR_UNKNOWN;

	unsigned char random_secret[32], random_public[64], hashed[64], h2[32], *temp = NULL;
	digit_t* H = (digit_t*)hashed;
	digit_t* H2 = (digit_t*)h2;
    digit_t* S = (digit_t*)(Signature+32);

	temp = (unsigned char*)calloc(1, SizeMessage+64);
    if (temp == NULL) {
		Status = ECCRYPTO_ERROR_NO_MEMORY;
        goto cleanup;
    }

    BPVOnline(random_secret, random_public);

    // modulo_order((digit_t*)random_secret, (digit_t*)random_secret);
    // unsigned char random_public2[64];
    // ecc_mul_fixed((digit_t*)random_secret, (point_affine*)random_public2);

 //    PublicKeyGeneration(random_secret, random_public2);

	// printf("static const unsigned char random_public[64] = {");
	// for (unsigned int i = 0; i < 64; i++) {
	// printf("0x%x, ", random_public[i]);
	// }
	// printf("};\n");

	// printf("static const unsigned cha random_public2[64] = {");
	// for (unsigned int i = 0; i < 64; i++) {
	// printf("0x%x, ", random_public2[i]);
	// }
	// printf("};\n");

    memmove(temp, Message, SizeMessage);
    memmove(temp+SizeMessage, random_public, 64);

    if (CryptoHashFunction(temp, SizeMessage+64, hashed) != 0) {   
        Status = ECCRYPTO_ERROR;
        goto cleanup;
    }

    modulo_order(H,H2);
    modulo_order((digit_t*)random_secret, (digit_t*)random_secret);

    memmove(Signature, h2, 32);

    to_Montgomery((digit_t*)SecretKey, S);          // Converting to Montgomery representation
	to_Montgomery(H2, H2);                    // Converting to Montgomery representation
	Montgomery_multiply_mod_order(S, H2, S);
	from_Montgomery(S, S);                  // Converting back to standard representation
	subtract_mod_order((digit_t*)random_secret, S, S);

	return ECCRYPTO_SUCCESS;

cleanup:
	clear_words((unsigned int*)Signature, 512/(sizeof(unsigned int)*8));

	return Status;
}


ECCRYPTO_STATUS BPV_Schnorr_Verify(const unsigned char* PublicKey, const unsigned char* Message, const unsigned int SizeMessage, const unsigned char* Signature, unsigned int* valid){

	ECCRYPTO_STATUS Status = ECCRYPTO_ERROR_UNKNOWN;

    unsigned char *temp, h[64], r[32], random_pub_ver[64];
	digit_t* H = (digit_t*)h;
    unsigned int i;

    *valid = false;

	temp = (unsigned char*)calloc(1, SizeMessage+32);
	if (temp == NULL) {
		Status = ECCRYPTO_ERROR_NO_MEMORY;
		goto cleanup;
	}
	
	memmove(r, Signature, 32);

	Status = ecc_mul_double((digit_t*)(Signature+32), (point_affine*)PublicKey, (digit_t*)r, (point_affine*)random_pub_ver);      
    if (Status != ECCRYPTO_SUCCESS) {                                                
        goto cleanup;
    }

	memmove(temp, Message, SizeMessage);
    memmove(temp+SizeMessage, random_pub_ver, 64);

    if (CryptoHashFunction(temp, SizeMessage+64, h) != 0) {   
        Status = ECCRYPTO_ERROR;
        goto cleanup;
    }

    modulo_order(H, H);

    for (i = 0; i < NWORDS_ORDER; i++) {
        if (((digit_t*)h)[i] != ((digit_t*)Signature)[i]) {
            goto cleanup;   
        }
    }
    *valid = true;

	return ECCRYPTO_SUCCESS;

cleanup:

	return Status;
}


//Receiver PK is not needed since we have the DBPV table.
ECCRYPTO_STATUS DBPV_ECIES_Encrypt(const unsigned char* SecretKey, unsigned char* onetime_Public){

	ECCRYPTO_STATUS Status = ECCRYPTO_ERROR_UNKNOWN;

	unsigned char random[32], secret[64], hashedSecret[64];

	Status = DBPVOnline(random, onetime_Public, secret);
	if (CryptoHashFunction(secret, 64, hashedSecret) != 0) {   
        Status = ECCRYPTO_ERROR;
        goto cleanup;
    }

    //use hashedSecret (divide it into 2 32-byte keys) for symmetric auth and encrypt functions.

	return ECCRYPTO_SUCCESS;


cleanup:
	clear_words((unsigned int*)onetime_Public, 512/(sizeof(unsigned int)*8));

	return Status;
}



ECCRYPTO_STATUS DBPV_ECIES_Decrypt(const unsigned char* SecretKey, const unsigned char* onetime_Public){


	ECCRYPTO_STATUS Status = ECCRYPTO_ERROR_UNKNOWN;
	bool clear_cofactor = false;
	unsigned char secret[64], hashedSecret[64];

    ecc_mul((point_affine*)onetime_Public, (digit_t*)SecretKey, (point_affine*)secret, clear_cofactor);

    if (CryptoHashFunction(secret, 64, hashedSecret) != 0) {   
        Status = ECCRYPTO_ERROR;
        goto cleanup;
    }

	return ECCRYPTO_SUCCESS;

cleanup:

	return Status;
}


