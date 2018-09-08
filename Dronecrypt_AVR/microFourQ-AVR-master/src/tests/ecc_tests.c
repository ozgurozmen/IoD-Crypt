/***********************************************************************************
* FourQlib: a high-performance crypto library based on the elliptic curve FourQ
*
*    Copyright (c) Microsoft Corporation. All rights reserved.
*
* Abstract: testing code for FourQ's curve arithmetic 
*
* Modified by Geovandro C. C. F. Pereira
************************************************************************************/ 

#include "../FourQ_internal.h"
#include "../FourQ_params.h"
#include "test_extras.h"
#include <stdio.h>

#define TEST_LOOPS        10      // Number of iterations per test


bool ecc_test()
{
    bool clear_cofactor, OK = true;
    unsigned int n;
    int passed;
    point_t A;
    point_extproj_t P;
    point_extproj_precomp_t Q;
    f2elm_t t1;
    uint64_t res_x[4], res_y[4], scalar[4];

    // Point doubling
    passed = 1;

    eccset(A); 
    point_setup(A, P);

    for (n=0; n<TEST_LOOPS; n++)
    {
        eccdouble(P);                      // 2*P
    }
    eccnorm(P, A);
    mod1271(A->x[0]); mod1271(A->x[1]);    // Fully reduced P
    mod1271(A->y[0]); mod1271(A->y[1]);  

    // Result
    res_x[0] = 0xFA4FAD9EC7732700; res_x[1] = 0x619F5D1FD93BC4F5; res_x[2] = 0x814B78DADF6A9024; res_x[3] = 0x72EC1D429F026578;
    res_y[0] = 0x7FF28C92C8CEF9DE; res_y[1] = 0x799208A76EAD2BA3; res_y[2] = 0x9B1AE60FFFCB520A; res_y[3] = 0x051698145D42F3E2;

    if (fp2compare64((uint64_t*)A->x, res_x)!=0 || fp2compare64((uint64_t*)A->y, res_y)!=0) passed=0;
    if (passed==0) return false; 

    // Point addition
    eccset(A); 
    point_setup(A, P);
    
    for (n=0; n<TEST_LOOPS; n++)
    {
        fp2copy1271((felm_t*)&PARAMETER_d, t1);
        fp2mul1271(t1, P->ta, t1);         // d*ta
        fp2add1271(t1, t1, t1);            // 2*d*ta
        fp2mul1271(t1, P->tb, Q->t2);      // 2*d*t
        fp2add1271(P->x, P->y, Q->xy);     // x+y    
        fp2sub1271(P->y, P->x, Q->yx);     // y-x
        fp2copy1271(P->z, Q->z2); 
        fp2add1271(Q->z2, Q->z2, Q->z2);   // 2*z
        eccadd(Q, P);                      // 2*P
    }
    eccnorm(P, A);
    mod1271(A->x[0]); mod1271(A->x[1]);    // Fully reduced P
    mod1271(A->y[0]); mod1271(A->y[1]);  

    // Result
    res_x[0] = 0xFA4FAD9EC7732700; res_x[1] = 0x619F5D1FD93BC4F5; res_x[2] = 0x814B78DADF6A9024; res_x[3] = 0x72EC1D429F026578;
    res_y[0] = 0x7FF28C92C8CEF9DE; res_y[1] = 0x799208A76EAD2BA3; res_y[2] = 0x9B1AE60FFFCB520A; res_y[3] = 0x051698145D42F3E2;

    if (fp2compare64((uint64_t*)A->x, res_x)!=0 || fp2compare64((uint64_t*)A->y, res_y)!=0) passed=0;

    eccset(A); 
    point_setup(A, P);
    fp2copy1271((felm_t*)&PARAMETER_d, t1);
    fp2mul1271(t1, P->x, t1);              // d*x
    fp2add1271(t1, t1, t1);                // 2*d*x
    fp2mul1271(t1, P->y, Q->t2);           // 2*d*t
    fp2add1271(P->x, P->y, Q->xy);         // x+y    
    fp2sub1271(P->y, P->x, Q->yx);         // y-x
    fp2zero1271(Q->z2); *Q->z2[0] = 2;     // 2*z
    eccdouble(P);                          // P = 2P 

    for (n=0; n<TEST_LOOPS; n++)
    {
        eccadd(Q, P);                      // P = P+Q
    }    
    eccnorm(P, A);
    mod1271(A->x[0]); mod1271(A->x[1]);    // Fully reduced P
    mod1271(A->y[0]); mod1271(A->y[1]);    

    // Result
    res_x[0] = 0xB92B573D2C4B06FF; res_x[1] = 0x6B62D585800A9F6A; res_x[2] = 0xECB6DFB3FA1ACB7C; res_x[3] = 0x0D9D9F54A8335E2B;
    res_y[0] = 0xDF3BD744D9BB783D; res_y[1] = 0x2B827EEDA23988A6; res_y[2] = 0x947C187247366CDD; res_y[3] = 0x3B7E00BA2F9525B3;

    if (fp2compare64((uint64_t*)A->x, res_x)!=0 || fp2compare64((uint64_t*)A->y, res_y)!=0) passed=0;
    if (passed==0) return false;

#if (USE_ENDO == true)

    // Psi endomorphism
    eccset(A); 
    point_setup(A, P);

    for (n=0; n<TEST_LOOPS; n++)
    {
        ecc_psi(P);                        // P = Psi(P)
    }    
    eccnorm(P, A);
    mod1271(A->x[0]); mod1271(A->x[1]);    // Fully reduced P
    mod1271(A->y[0]); mod1271(A->y[1]);   

    // Result
    res_x[0] = 0xABC340A7DDC08580; res_x[1] = 0x6B74D34E155D2119; res_x[2] = 0x1B6E0A6DC6A5BC70; res_x[3] = 0x5CAE354597C9106A;
    res_y[0] = 0xE276B58944E2D60B; res_y[1] = 0x1812145CDE0E8DCB; res_y[2] = 0xF4D6895A6375AA22; res_y[3] = 0x1A593C1711EEBCDE;

    if (fp2compare64((uint64_t*)A->x, res_x)!=0 || fp2compare64((uint64_t*)A->y, res_y)!=0) passed=0;
    if (passed==0) return false;
   
    // Phi endomorphism
    {        
    eccset(A); 
    point_setup(A, P);

    for (n=0; n<TEST_LOOPS; n++)
    {
        ecc_phi(P);                        // P = Phi(P)
        eccnorm(P, A);
        point_setup(A, P);
    }    
    mod1271(A->x[0]); mod1271(A->x[1]);    // Fully reduced P
    mod1271(A->y[0]); mod1271(A->y[1]); 

    // Result
    res_x[0] = 0x1365D931AFEBC83E; res_x[1] = 0x1873BB71FF4FFF87; res_x[2] = 0x7BF9ACB5C770F61F; res_x[3] = 0x773EA05D9B4B0D62;
    res_y[0] = 0xCFFDD1A374E18F42; res_y[1] = 0x369B19C1F39C1A97; res_y[2] = 0x38B8E623E4E0049A; res_y[3] = 0x12435E356960429A;

    if (fp2compare64((uint64_t*)A->x, res_x)!=0 || fp2compare64((uint64_t*)A->y, res_y)!=0) passed=0;
    if (passed==0) return false;

    
    // Scalar decomposition and recoding
    {        
    uint64_t acc1, acc2, acc3, acc4;
    digit_t scalars[8];
    
    unsigned int digits[65], sign_masks[65];
    int i;

    for (n=0; n<TEST_LOOPS*10; n++)
    {
        random_scalar_test(scalars);
        decompose(scalars, scalars);  
        fp2copy1271((felm_t*)scalars, (felm_t*)scalar);
        recode(scalars, digits, sign_masks); 

        acc1 = acc2 = acc3 = acc4 = 0; 

        for (i = 64; i >= 0; i--)
        {
            acc1 = 2*acc1; acc2 = 2*acc2; acc3 = 2*acc3; acc4 = 2*acc4; 
            if (sign_masks[i] == (unsigned int)-1) {
                acc1 += 1;
                acc2 += (digits[i] & 1);
                acc3 += ((digits[i] >> 1) & 1);
                acc4 += ((digits[i] >> 2) & 1);
            } else if (sign_masks[i] == 0) {
                acc1 -= 1;
                acc2 -= (digits[i] & 1);
                acc3 -= ((digits[i] >> 1) & 1);
                acc4 -= ((digits[i] >> 2) & 1);
            }
        }   
        if (scalar[0] != acc1 || scalar[1] != acc2  || scalar[2] != acc3 || scalar[3] != acc4) { passed=0; break; }
    }
    if (passed==0) return false;
    }
    
    }
#endif

    // Scalar multiplication
    eccset(A); 
    clear_cofactor = false;
    scalar[0] = 0x3AD457AB55456230; scalar[1] = 0x3A8B3C2C6FD86E0C; scalar[2] = 0x7E38F7C9CFBB9166; scalar[3] = 0x0028FD6CBDA458F0;
    
    for (n=0; n<TEST_LOOPS; n++)
    {
        scalar[1] = scalar[2];
        scalar[2] += scalar[0];

        ecc_mul(A, (digit_t*)scalar, A, clear_cofactor);
    }

    res_x[0] = 0x8F7033298B9CD5A4; res_x[1] = 0x6A60DF430E52E299; res_x[2] = 0x51D6EAFEEA829A8B; res_x[3] = 0x56F40C1CE3C3CD34;
    res_y[0] = 0x5B611ABE0387F840; res_y[1] = 0x59C6A5C83477F57C; res_y[2] = 0xF33C879AB74E2490; res_y[3] = 0x12C18E67FB2A3A9D;
    if (fp2compare64((uint64_t*)A->x, res_x)!=0 || fp2compare64((uint64_t*)A->y, res_y)!=0) passed=0;

    eccset(A); 
    clear_cofactor = true;
    scalar[0] = 0x3AD457AB55456230; scalar[1] = 0x3A8B3C2C6FD86E0C; scalar[2] = 0x7E38F7C9CFBB9166; scalar[3] = 0x0028FD6CBDA458F0;
   
    for (n=0; n<TEST_LOOPS; n++)
    {
        scalar[1] = scalar[2];
        scalar[2] += scalar[0];

        ecc_mul(A, (digit_t*)scalar, A, clear_cofactor);
    }

    res_x[0] = 0x10EA7CF4F502CF5E; res_x[1] = 0x4FC1A5862ACAF69B; res_x[2] = 0x886D85328FB1E1A9; res_x[3] = 0x6F134E7E5129772A;
    res_y[0] = 0x35FFAD6E8F0681DC; res_y[1] = 0x681067510F99389E; res_y[2] = 0xA4BE7A70A1820895; res_y[3] = 0x34C0A821F434D672;
    if (fp2compare64((uint64_t*)A->x, res_x)!=0 || fp2compare64((uint64_t*)A->y, res_y)!=0) passed=0;
    if (passed==0) return false;

    {    
    point_t AA, B, C; 
    unsigned int j, w, v, e, d;
    uint64_t k[4];
    unsigned int digits_fixed[NBITS_ORDER_PLUS_ONE+(W_FIXEDBASE*V_FIXEDBASE)-1] = {0};    

    // Scalar recoding using the mLSB-set representation    
    w = W_FIXEDBASE;
    v = V_FIXEDBASE;
    e = E_FIXEDBASE;     
    d = D_FIXEDBASE;      
          
    for (n=0; n<TEST_LOOPS; n++) 
    {
        random_scalar_test((digit_t*)scalar);
        modulo_order((digit_t*)scalar, (digit_t*)scalar);    // k = k mod (order) 
        conversion_to_odd((digit_t*)scalar, (digit_t*)k);                       
        for (j = 0; j < NWORDS64_ORDER; j++) scalar[j] = k[j];
        mLSB_set_recode(k, digits_fixed);
            
        if (verify_mLSB_recoding(scalar, (int*)digits_fixed)==false) { passed=0; break; }
    }
    if (passed==0) return false;

    // Fixed-base scalar multiplication
    eccset(AA); 
    
    for (n=0; n<TEST_LOOPS; n++)
    {
        random_scalar_test((digit_t*)scalar); 
        ecc_mul_fixed((digit_t*)scalar, B);
        ecc_mul(AA, (digit_t*)scalar, C, false);
        
        if (fp2compare64((uint64_t*)B->x,(uint64_t*)C->x)!=0 || fp2compare64((uint64_t*)B->y,(uint64_t*)C->y)!=0) { passed=0; break; }
    }
    if (passed==0) return false; 
    }
  
    {    
    point_t PP, QQ, RR, UU, TT; 
    point_extproj_precomp_t AA;
    point_extproj_t BB;
    digit_t k[8], l[8], kk[8];     

    // Double scalar multiplication
    eccset(QQ); 
    eccset(PP);
    
    for (n=0; n<TEST_LOOPS; n++)
    {
        random_scalar_test(kk); 
        ecc_mul(QQ, (digit_t*)kk, QQ, false);
        random_scalar_test(k); 
        random_scalar_test(l); 
        ecc_mul_double(k, QQ, l, RR);
        ecc_mul(PP, k, UU, false);
        ecc_mul(QQ, l, TT, false);
    
        fp2add1271(UU->x, UU->y, AA->xy); 
        fp2sub1271(UU->y, UU->x, AA->yx); 
        fp2mul1271(UU->x, UU->y, AA->t2);    
        fp2add1271(AA->t2, AA->t2, AA->t2); 
        fp2mul1271(AA->t2, (felm_t*)&PARAMETER_d, AA->t2); 
        fp2zero1271(AA->z2); AA->z2[0][0] = 2;
        point_setup(TT, BB);             

        eccadd(AA, BB);
        eccnorm(BB, UU);
        
        if (fp2compare64((uint64_t*)UU->x,(uint64_t*)RR->x)!=0 || fp2compare64((uint64_t*)UU->y,(uint64_t*)RR->y)!=0) { passed=0; break; }
    }

    if (passed==0) return false;

    }

    return OK;
}


int ecc_tests()
{
    bool OK = true;

    OK = OK && ecc_test();         // Test FourQ's curve functions

    return OK;
}
