CFLAGS=-O3 -Wall

all: AES AES_smallScale SmallAES_5RoundDistinguisher SmallAES_5RoundDistinguisher_Variance SmallAES_5Round_numbCollision SmallAES_5Round_TruncAttack SmallAES_5Round_VarianceAttack

multiplication: multiplication.h multiplication.c multiplication_smallScale.c
	$(CC) $(CFLAGS) -c -o multiplication.o multiplication.c
	$(CC) $(CFLAGS) -c -o multiplication_smallScale.o multiplication_smallScale.c
	
subspace_checks: subspace_checks.h subspace_checks.c
	$(CC) $(CFLAGS) -c -o subspace_checks.o subspace_checks.c

aes_common: AES_common.h AES_common.c
	$(CC) $(CFLAGS) -c -o AES_common10.o AES_common.c -DN_Round=10
	$(CC) $(CFLAGS) -c -o AES_common5.o AES_common.c -DN_Round=5

AES: aes_common multiplication AES.c
	$(CC) $(CFLAGS) -o AES AES.c AES_common10.o multiplication.o

AES_smallScale: aes_common multiplication AES_smallScaleVersion.c
	$(CC) $(CFLAGS) -o AES_smallScale AES_smallScaleVersion.c AES_common10.o multiplication_smallScale.o

SmallAES_5RoundDistinguisher: aes_common multiplication subspace_checks SmallAES_5RoundDistinguisher.c
	$(CC) $(CFLAGS) -o SmallAES_5RoundDistinguisher SmallAES_5RoundDistinguisher.c AES_common5.o subspace_checks.o multiplication_smallScale.o

SmallAES_5RoundDistinguisher_Variance: aes_common multiplication subspace_checks SmallAES_5RoundDistinguisher_Variance.c
	$(CC) $(CFLAGS) -o SmallAES_5RoundDistinguisher_Variance SmallAES_5RoundDistinguisher_Variance.c AES_common5.o subspace_checks.o multiplication_smallScale.o

SmallAES_5Round_numbCollision: aes_common multiplication subspace_checks SmallAES_5Round_numbCollision.c
	$(CC) $(CFLAGS) -o SmallAES_5Round_numbCollision SmallAES_5Round_numbCollision.c AES_common5.o subspace_checks.o multiplication_smallScale.o

SmallAES_5Round_TruncAttack: aes_common multiplication subspace_checks SmallAES_5Round_TruncAttack.c
	$(CC) $(CFLAGS) -o SmallAES_5Round_TruncAttack SmallAES_5Round_TruncAttack.c AES_common5.o subspace_checks.o multiplication_smallScale.o

SmallAES_5Round_VarianceAttack: aes_common multiplication subspace_checks SmallAES_5Round_VarianceAttack.c
	$(CC) $(CFLAGS) -o SmallAES_5Round_VarianceAttack SmallAES_5Round_VarianceAttack.c AES_common5.o subspace_checks.o multiplication_smallScale.o

clean:
	$(RM) -f AES_common10.o AES_common4.o AES_common5.o 
	$(RM) -f multiplication.o multiplication_smallScale.o 
	$(RM) -f subspace_checks.o
	$(RM) -f AES AES_smallScale
	$(RM) -f AES_5RoundAttack_SmallScale
	$(RM) -f SmallAES_5RoundDistinguisher
	$(RM) -f SmallAES_5RoundDistinguisher_Variance
	$(RM) -f SmallAES_5Round_numbCollision
	$(RM) -f SmallAES_5Round_TruncAttack
	$(RM) -f SmallAES_5Round_VarianceAttack
	
.PHONY: clean
