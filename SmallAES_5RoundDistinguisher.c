
/**The Random Generator used in this code is the "Mersenne Twister" one, developed by 1997 by Makoto Matsumoto
and Takuji Nishimura - MT19937.
The complete source code of the random generator can be found in http://www.math.sci.hiroshima-u.ac.jp/~m-mat/MT/MT2002/CODES/mt19937ar.c
We also attach the following:
"A C-program for MT19937, with initialization improved 2002/1/26.
   Coded by Takuji Nishimura and Makoto Matsumoto.
   Before using, initialize the state by using init_genrand(seed)
   or init_by_array(init_key, key_length).
   Copyright (C) 1997 - 2002, Makoto Matsumoto and Takuji Nishimura,
   All rights reserved.
   Redistribution and use in source and binary forms, with or without
   modification, are permitted provided that the following conditions
   are met:
     1. Redistributions of source code must retain the above copyright
        notice, this list of conditions and the following disclaimer.
     2. Redistributions in binary form must reproduce the above copyright
        notice, this list of conditions and the following disclaimer in the
        documentation and/or other materials provided with the distribution.
     3. The names of its contributors may not be used to endorse or promote
        products derived from this software without specific prior written
        permission.
   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
   "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
   LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
   A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER OR
   CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
   EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
   PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
   PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
   LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
   NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
   SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
   Any feedback is very welcome.
   http://www.math.sci.hiroshima-u.ac.jp/~m-mat/MT/emt.html
   email: m-mat @ math.sci.hiroshima-u.ac.jp (remove space)"
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "AES_common.h"
#include "AES_smallScale_sbox.h"
#include "multiplication.h"
#include "subspace_checks.h"

//#define N_Round 5
#define NUMBER_CP 65536
#define NUMBER_TEST 90

unsigned long init[5], length=5;

word8 cipher2copy[NUMBER_CP][16], cipher[NUMBER_CP][16], play2copy[NUMBER_CP][16], play[NUMBER_CP][16];

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int logarithm2(long int n)
{
    long int i = 1;
    int j = 1;

    if (n<=0)
        return -1;

    if (n == 1)
        return 0;

    while(i<n)
    {
        i = i * 2;
        j++;
    }

    return j;

}

long int pow2(int n)
{
    long int i = 1;
    int j;

    if(n == 0)
        return 1;

    for(j=0; j<n;j++)
    {
        i = i * 2;
    }

    return i;
}

//compute the exponantion only for positive integer exponent!
int pow(int n, int e)
{
    int i = 1, j, flag = 0;

    if(n==0)
        return 0;

    if((e == 0)||(n==1))
        return 1;

    if((e<0)||(n<0))
    {
        return -1;
    }

    for(j=0; j<e;j++)
    {
        i = i * n;
    }

    return i;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/**Merge-Sort Algorithm*/

/*given two ciphertexts a and b, it return 0 if a<=b, 1 otherwise */
/*int lessOrEqual(word8 a[], word8 b[], int coset)
{
    int i, temp, aaa, bbb;

    for(i=0;i<4;i++)
    {
        temp = 4*coset - 3 * i;

        if(temp < 0)
            temp = temp + 16;

        aaa = (int) a[temp];
        bbb = (int) b[temp];

        if(aaa>bbb)
            return 1;

        if(aaa<bbb)
            return 0;
    }

    return 0;
}

double merging2(int low, int mid, int high, int coset, double numberTableLook) {

    int l1, l2, i, j;
    word8 text1[16], text2[16], ttt1[16], ttt2[16];

    l1 = low;
    l2 = mid;

    for(j = 0; j<16; j++)
    {
        text1[j] = cipher[l1][j];
        text2[j] = cipher[l2][j];

        ttt1[j] = play[l1][j];
        ttt2[j] = play[l2][j];
    }

    numberTableLook = numberTableLook + 2.0;

    for(i = low; ((l1 < mid) && (l2 < high)); i++)
    {
        numberTableLook = numberTableLook + 1.0;

        if(lessOrEqual(text1, text2, coset) == 0)
        {
            for(j = 0; j<16; j++)
            {
                cipher2copy[i][j] = text1[j];
                play2copy[i][j] = ttt1[j];
            }
            l1++;
            for(j = 0; j<16; j++)
            {
                text1[j] = cipher[l1][j];
                ttt1[j] = play[l1][j];
            }
        }
        else
        {
            for(j = 0; j<16; j++)
            {
                cipher2copy[i][j] = text2[j];
                play2copy[i][j] = ttt2[j];
            }
            l2++;
            for(j = 0; j<16; j++)
            {
                text2[j] = cipher[l2][j];
                ttt2[j] = play[l2][j];
            }
        }
    }

    while(l1 < mid)
    {
       numberTableLook = numberTableLook + 1.0;

       for(j = 0; j<16; j++)
        {
            cipher2copy[i][j] = cipher[l1][j];
            play2copy[i][j] = play[l1][j];
        }
        i++;
        l1++;
    }

    while(l2 < high)
    {
        numberTableLook = numberTableLook + 1.0;

       for(j = 0; j<16; j++)
        {
            cipher2copy[i][j] = cipher[l2][j];
            play2copy[i][j] = play[l2][j];
        }
        i++;
        l2++;
    }

    for(i = low; i < high; i++)
    {
        numberTableLook = numberTableLook + 1.0;

        for(j = 0; j<16; j++)
        {
            cipher[i][j] = cipher2copy[i][j];
            play[i][j] = play2copy[i][j];
        }
    }

    return numberTableLook;
}

double sort2(int coset, double numberTableLook)
{
    int log, i, j, division, high, low, middle, a, b, c;
    word8 t1[16], t2[16], ttt1[16], ttt2[16];

    log = logarithm2(NUMBER_CP);

    for(i=0; i<NUMBER_CP; i = i+2)
    {
        for(j=0;j<16;j++)
        {
            t1[j] = cipher[i][j];
            t2[j] = cipher[i+1][j];
            ttt1[j] = play[i][j];
            ttt2[j] = play[i+1][j];
        }

        numberTableLook = numberTableLook + 2.0;

        if(lessOrEqual(t1, t2, coset) == 1)
        {
            for(j=0;j<16;j++)
            {
                cipher[i][j] = t2[j];
                cipher[i+1][j] = t1[j];
                play[i][j] = ttt2[j];
                play[i+1][j] = ttt1[j];

            }

            numberTableLook = numberTableLook + 2.0;
        }
    }

    for(i = 2; i < log; i++)
    {
        a = pow2(i);
        b = a/2;
        division = NUMBER_CP / a;

        for(j = 0; j < division; j++)
        {
            high = a * (j+1);
            low = a * j;
            middle = low + b;

            numberTableLook = merging2(low, middle, high, coset, numberTableLook);

        }
    }

    return numberTableLook;

}*/


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int superTest(int indice1, int indice2, int coset)
{
    int k, l;

    word8 temp[4][4];

    for(k = 0; k<4; k++)
    {
        for(l=0;l<4;l++)
            temp[k][l] = cipher[indice1][k + l*4] ^ cipher[indice2][k + l*4];
    }

    for(k = (coset + 1); k < 4; k++)
    {
        if(belongToW_2(temp, k) == 1)
            return 1;

    }

    return 0;
}


/**Count number of collision in the SUBSPACE case!*/

double subspaceTest(word8 key[][4])
{
    unsigned long int numero = 0;
    int l, k, indice1, indice2, indice3, indice4, r, esito, coset, flag, i, j, arrray[65536], rTemp, boh;


    double numberTableLook, tot = 0.;//, numero = 0.;

    word8 c1[4][4], temp[4][4];

    for(k=0;k<4;k++)
    {
        for(l=0;l<4;l++)
            temp[l][k]=randomNibble();
    }

    i = 0;

    for(indice1 =0; indice1<16; indice1++)
    {
        for(indice2=0; indice2 <16; indice2 ++)
        {
            for(indice3=0; indice3<16; indice3++)
            {
                for(indice4=0; indice4<16; indice4++)
                {
                    temp[0][0] = (word8) indice1;
                    temp[1][1] = (word8) indice2;
                    temp[2][2] = (word8) indice3;
                    temp[3][3] = (word8) indice4;

                    encryption(temp, key, &(c1[0][0]));

                    //i = (long int) indice4 + (long int) (indice3 * 16) + (long int) (indice2 * 256) + (long int) (indice1 * 4096);

                    for(k = 0; k<4; k++)
                    {
                        for(l=0;l<4;l++)
                        {
                            play[i][k + l*4] = temp[k][l];
                            cipher[i][k + l*4] = c1[k][l];
                        }
                    }

                    i++;

                }
            }
        }
    }

    for(coset = 0; coset < 4; coset++)
    {
        for(i = 0; i<65536; i++)
        {
            arrray[i] = 0;
        }

        for(i = 0; i<NUMBER_CP; i++)
        {
            numero = 0;

            //numberTableLook++;

            for(j = 0; j<4; j++)
            {
                boh = coset - j;
                if(boh < 0)
                    boh = boh + 4;

                numero = numero + pow(16, j) * cipher[i][j + 4 * boh];
            }

            arrray[numero]++;
            //numberTableLook++;
        }

        numero = 0;

        for(i = 0; i<65536; i++)
        {
            rTemp = arrray[i];
            //numberTableLook++;
            rTemp = rTemp * (rTemp - 1);
            numero = numero + (rTemp/ 2);
        }

        tot = tot + (double) numero;

    }

    return tot;

}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void randEncryption(word8 temp[][4], word8 key[][4], word8* c1)
{
    word8 temp2[4][4];
    int i, j;

    encryption(temp, key, &(temp2[0][0]));
    encryption(temp2, key, &(temp2[0][0]));
    encryption(temp2, key, &(temp2[0][0]));
    encryption(temp2, key, &(temp2[0][0]));
    encryption(temp2, key, &(temp2[0][0]));


    for(i=0;i<4;i++)
    {
        for(j=0;j<4;j++)
            *(c1+j+4*i)=temp2[i][j];
    }

}


/**Count number of collision in the random case!*/

unsigned long int randomTest(word8 key[][4])
{
    unsigned long int numero = 0;
    int l, k, i, j, indice1, indice2, indice3, indice4, coset, arrray[65536], rTemp, boh;

    double numberTableLook, tot = 0.;

    word8 c1[4][4], temp[4][4];

    for(k=0;k<4;k++)
    {
        for(l=0;l<4;l++)
            temp[l][k]=randomNibble();
    }

    i = 0;

    for(indice1 =0; indice1<16; indice1++)
    {
        temp[0][0] = (word8) indice1;
        for(indice2=0; indice2 <16; indice2 ++)
        {
            temp[1][1] = (word8) indice2;
            for(indice3=0; indice3<16; indice3++)
            {
                temp[2][2] = (word8) indice3;
                for(indice4=0; indice4<16; indice4++)
                {

                    temp[3][3] = (word8) indice4;

                    randEncryption(temp, key, &(c1[0][0]));

                    //i = (long int) indice4 + (long int) (indice3 * 16) + (long int) (indice2 * 256) + (long int) (indice1 * 4096);

                    for(k = 0; k<4; k++)
                    {
                        for(l=0;l<4;l++)
                            cipher[i][k + l*4] = c1[k][l];
                    }

                    i++;

                }
            }
        }
    }

    for(coset = 0; coset < 4; coset++)
    {
        for(i = 0; i<65536; i++)
        {
            arrray[i] = 0;
        }

        for(i = 0; i<NUMBER_CP; i++)
        {
            numero = 0;

            //numberTableLook++;

            for(j = 0; j<4; j++)
            {
                boh = coset - j;
                if(boh < 0)
                    boh = boh + 4;

                numero = numero + pow(16, j) * cipher[i][j + 4 * boh];
            }

            arrray[numero]++;
            //numberTableLook++;
        }

        numero = 0;

        for(i = 0; i<65536; i++)
        {
            rTemp = arrray[i];
            //numberTableLook++;
            rTemp = rTemp * (rTemp - 1);
            numero = numero + (rTemp/ 2);
        }

        tot = tot + (double) numero;

    }

    return tot;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int main()
{
    word8 key[4][4];
//     = {
//        0x0, 0x4, 0x8, 0x3,
//        0x1, 0x2, 0xc, 0xd,
//        0xb, 0x6, 0xa, 0x1,
//        0x3, 0x7, 0x3, 0xf
//    };

    int j, k;

    unsigned long int i;
    //double numero = 0., numero2 = 0., tot = 0., tot2 = 0.;
    double numero2 = 0., tot2 = 0., numero = 0., tot = 0.;

    printf("Truncated Differential Distinguisher 5 rounds.\n\nFor each of the %d tests, the program generates texts and counts the total number of collisions both in the random and in the AES case.\n\n", NUMBER_TEST);

    printf("It prints the following things:\n");
    printf("Number of tests -\n TOTAL/AVERAGE Collisions in the random\n - TOTAL/AVERAGE Collision in the AES case\n\n");

    //initialize seed
    srand (time(NULL));

    for(j=0;j<length;j++)
    {
        init[j] = rand();
    }
    init_by_array(init, length);

    for(i=0; i < NUMBER_TEST; i++)
    {
        //I want to work with 4 bits, not 8!
        for(k=0;k<4;k++)
        {
            for(j=0;j<4;j++)
                key[j][k] =  randomNibble();
        }

        numero += randomTest(key);
        tot = numero / ((double) (4*(i+1)));
        numero2 += subspaceTest(key);
        tot2 = numero2 / ((double) (4*(i+1)));

        if(((i+1)%5) == 0)
        {
            printf("After %d tests:\n", (i+1));
            printf("Random Collisions -  Tot: %.0f - Average: %.5f\n", numero, tot);
            printf("AES Collisions -  Tot: %.0f - Average: %.5f\n\n", numero2, tot2);
        }

    }

    printf("In conclusion:\n");
    printf("Random Collisions -  Tot: %.0f - Average: %.5f\n", numero, tot);
    printf("AES Collisions -  Tot: %.0f - Average: %.5f\n\n", numero2, tot2);

    if(numero>numero2)
        printf("FAILS!!!\n");
    else
        printf("PERFECT!!!\n");

    return (0);
}
