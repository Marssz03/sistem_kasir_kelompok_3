#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pembayaran.h"

void importQRDariFile(const char *namaFile, long totalBelanja) {
    FILE *fp = fopen(namaFile, "r");
    printf("\n+-----------------------------------------+\n");
    printf("|         SCAN QRIS UNTUK MEMBAYAR        |\n");
    printf("+-----------------------------------------+\n");

    if (!fp) {
        printf("| [!] File '%s' tidak ditemukan!     |\n", namaFile);
        printf("| Pastikan file QR berada di folder yang  |\n");
        printf("| sama dengan program ini.                |\n");
    } else {
        char baris[256];
        while (fgets(baris, sizeof(baris), fp) != NULL) {
            printf("%s", baris);
        }
        fclose(fp);
    }

    printf("\n+-----------------------------------------+\n");
    printf("| Nominal QRIS : Rp%-22ld |\n", totalBelanja);
    printf("| NMID         : ID1026884920194          |\n");
    printf("+-----------------------------------------+\n");
}

void generateKodeVA(int pilihanBank, long totalBelanja, int jumlahItem, char *namaBank, char *kodeVA) {
    const char *prefixBank = "8808";
    if (pilihanBank == 1) {
        strcpy(namaBank, "BCA Virtual Account");
        prefixBank = "3901";
    } else if (pilihanBank == 2) {
        strcpy(namaBank, "Mandiri Virtual Account");
        prefixBank = "8881";
    } else if (pilihanBank == 3) {
        strcpy(namaBank, "BRI Virtual Account");
        prefixBank = "2620";
    } else {
        strcpy(namaBank, "BNI Virtual Account");
        prefixBank = "8810";
    }

    int acak4Digit = rand() % 10000;
    int jejakNominal = (int)((totalBelanja + jumlahItem * 137L) % 10000L);
    if (jejakNominal < 0) {
        jejakNominal = -jejakNominal;
    }

    snprintf(kodeVA, 30, "%s%04d%04d", prefixBank, jejakNominal, acak4Digit);
}
