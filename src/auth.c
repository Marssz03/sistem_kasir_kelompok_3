#include <stdio.h>
#include <string.h>
#include "auth.h"
#include "utils.h"

int login(User daftarUser[], int jumlahUser, SesiKasir *sesi) {
    char userInput[30];
    char passInput[30];

    while (1) {
        bersihkanLayar();
        printf("=== LOGIN APLIKASI KASIR TOKO ===\n");
        printf("Username: ");
        if (scanf("%29s", userInput) != 1) {
            bersihkanBufferInput();
            continue;
        }
        printf("Password: ");
        if (scanf("%29s", passInput) != 1) {
            bersihkanBufferInput();
            continue;
        }
        bersihkanBufferInput();

        int cocok = 0;
        for (int i = 0; i < jumlahUser; i++) {
            if (strcmp(userInput, daftarUser[i].username) == 0 &&
                strcmp(passInput, daftarUser[i].password) == 0) {
                strcpy(sesi->namaKasir, daftarUser[i].username);
                cocok = 1;
                break;
            }
        }

        if (!cocok) {
            printf("\nUsername atau password salah!");
            tekanEnter();
            continue;
        }

        char waktuOtomatis[50];
        ambilHariTanggalOtomatis(waktuOtomatis, sizeof(waktuOtomatis));

        printf("Hari & Tanggal Login [Enter untuk otomatis: %s]: ", waktuOtomatis);
        char inputTanggal[50];
        bacaBaris(inputTanggal, sizeof(inputTanggal));

        if (strlen(inputTanggal) == 0) {
            strcpy(sesi->hariTanggalLogin, waktuOtomatis);
        } else {
            strcpy(sesi->hariTanggalLogin, inputTanggal);
        }

        printf("\nLogin berhasil! Kasir bertugas: %s (%s).",
               sesi->namaKasir, sesi->hariTanggalLogin);
        tekanEnter();
        return 1;
    }
}
