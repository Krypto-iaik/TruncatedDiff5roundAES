# TruncatedDiff5roundAES

Verification of new Secret-Key Distinguishers and Key-Recovery Attacks on AES up to 5 rounds as proposed in 

"New Rigorous Analysis of Truncated Differentials for 5-round AES"

(see https://eprint.iacr.org/ for details)

Programs:

1) Verification_SmallScaleAES

1a) Secret-Key Distinguishers on AES:

- SmallAES_5RoundDistinguisher.c

- SmallAES_5RoundDistinguisher_Variance.c

1b) Key-Recovery Attacks on AES:

- SmallAES_5Round_numbCollision.c

- SmallAES_5Round_TruncAttack.c

- SmallAES_5Round_VarianceAttack.c

2) Verification_FullScaleAES

- main.cpp

The programs should run with almost C-compilers (we have used gcc version 4.8.1). None of these programs is speed-optimized, they are for verification purposes only.

In all the programs, the main function is always the last one. Note that "Verification_SmallScaleAES" folders contain a "makefile".

Our implementation of AES and of small-scale AES (encryption and decryption) have been verified against test-vectors.

A complete description of this small scale variant of AES can be found in "Small Scale Variants of the AES" - Authors: C. Cid, S. Murphy, and M.J.B. Robshaw The program "AES_smallScaleVersion.c" provides an implementation of this small scale AES (verified against test-vectors).

All the distinguishers/attack have been verified on small-scale AES (due to the high complexity of the attacks for real AES).
However, since the properties that they exploit are independent of the fact that the words are composed of 4 or 8 bits, our verification on small-scale AES is strong evidence for it to hold for real AES.
For a complete description of the distinguishers and of the attacks, we refer to the paper.

- Secret-Key Distinguishers:

1) "SmallAES_5RoundDistinguisher.c" verifies the - small-scale - 5-round AES secret-key distinguisher based on the average number of collisions presented in Sect. 7.
The idea is the following. Given n cosets of a diagonal space D_0, one counts the number of different pairs of ciphertexts that belong to the same coset of M_J after 5-round.
This number is a little higher for AES (approx. 32848.5) than for a random permutation (approx. 32767.5)
The program print the average number of collisions found for the two cases after n tests (the secret key is generated at random for each test).
It is possible to modify the number of initial cosets/test using "NUMBER_TEST" define in line 50 - NUMBER_TEST = 90 is sufficient to detect the difference between the two cases.

2) "SmallAES_5RoundDistinguisher_Variance.c" verifies the - small-scale - 5-round AES secret-key distinguisher based on the variance presented in Sect. 6.1.
The idea is the following. Given n cosets of a diagonal space D_0, one counts the number of different pairs of ciphertexts that belong to the same coset of M_J after 5-round, and then compute the variance.
The variance is higher for AES than for a random permutation (of a factor approx. 6)
The program print the variance found for the two cases after n tests (the secret key is generated at random for each test).
It is possible to modify the number of initial cosets/test using "NUMBER_TEST" define in line 50 - NUMBER_TEST = 256 is sufficient to have a good approximation of the variance.

3) "main.cpp" verifies the - full-size - 5-round AES secret-key distinguisher based on the variance presented in Sect. 6.1.
The program works in the same way as just describe for the small-scale AES.
The flags to compile the code on unix machines:

g++ -o <name> -g -O0 -Wall -msse2 -msse -march=native -maes -lpthread main.cpp 

- Key-Recovery Attacks (on small-scale AES): 

3) "SmallAES_5Round_numbCollision.c" verifies the 5-round key-recovery attack described in App. D.2.
Due to the choice of the texts, the idea is to guarantee that the number of collisions among the ciphertext pairs is a multiple of 4 with prob.1 only for the right key.
This allows to filter wrong keys.
It is possible to modify the secret key, defined in the main part - line 488.
Only for simplicity, in this implementation we limit to find part of the key, that is two bytes of the first diagonal. However, exactly the same attack can be used to find the full key.

4) "SmallAES_5Round_TruncAttack.c" verifies the 5-round key-recovery attack described in App. D.3. Due to the choice of the texts, the idea is to guarantee that the number of collisions among the ciphertext pairs is a little higher for the right key than for a wrong one (the behavior in this last case is similar to the one of a random permutation).
It is possible to modify the number of initial cosets/test using "NUMBER_TEST" define in line 51 used for the attack.
The choice of 280 initial of cosets used for the attack guarantees this fact with probability higher than 95%.
It is possible to modify the secret key, defined in the main part - line 488.
Only for simplicity, in this implementation we limit to find part of the key, that is one byte of the first diagonal. However, exactly the same attack can be used to find the full key.

5) "SmallAES_5Round_VarianceAttack.c" verifies the 5-round key-recovery attack described in App. D.4. Due to the choice of the texts, the idea is to guarantee that the variance of the number of collisions among the ciphertext pairs is higher for the right key than for a wrong one (the behavior in this last case is similar to the one of a random permutation).
It is possible to modify the secret key, defined in the main part - line 488.
Only for simplicity, in this implementation we limit to find part of the key, that is one byte of the first diagonal. However, exactly the same attack can be used to find the full key.

Finally, the pseudo-random generator used in these programs is the "Mersenne Twister" one, developed by 1997 by Makoto Matsumoto and Takuji Nishimura - MT19937. The complete source code and explanation of this random generator can be found in: http://www.math.sci.hiroshima-u.ac.jp/~m-mat/MT/emt.html
