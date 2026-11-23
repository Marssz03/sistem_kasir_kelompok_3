#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "models.h"
#include "auth.h"
#include "inventori.h"
#include "transaksi.h"
#include "utils.h"

int main(void) {
    srand((unsigned int)time(NULL));

    User daftarUser[MAX_USER] = {
        {"atha", "ATH1"},
        {"chesya", "CHE2"},
        {"omar", "OMR3"},
        {"kaysan", "KAY4"}
    };

    Barang katalog[MAX_BARANG] = {
        {1, 899001100001ULL, "Beras Premium 5kg", 68000, 25, "10-11-2026", "10-05-2027"},
        {2, 899001100002ULL, "Minyak Goreng 2L", 36000, 30, "12-11-2026", "12-11-2027"},
        {3, 899001100003ULL, "Gula Pasir 1kg", 17500, 40, "05-11-2026", "05-11-2028"},
        {4, 899001100004ULL, "Telur Ayam 1kg", 28000, 20, "20-11-2026", "05-12-2026"},
        {5, 899001100005ULL, "Susu UHT 1L", 19000, 35, "15-11-2026", "15-03-2027"},
        {6, 899001100006ULL, "Kopi Bubuk 200g", 22500, 18, "01-11-2026", "01-11-2027"},
        {7, 899001100007ULL, "Teh Celup Isi 25", 9500, 45, "08-11-2026", "08-08-2028"},
        {8, 899001100008ULL, "Sabun Mandi Cair", 24000, 22, "14-11-2026", "14-11-2028"}
    };

    SesiKasir sesi;
    memset(&sesi, 0, sizeof(sesi));

    if (login(daftarUser, MAX_USER, &sesi)) {
        while (1) {
            bersihkanLayar();
            printf("=== MENU UTAMA APLIKASI KASIR ===\n");
            printf("Kasir Bertugas : %s\n", sesi.namaKasir);
            printf("Hari & Tanggal : %s\n\n", sesi.hariTanggalLogin);
            printf("1. Transaksi Kasir\n");
            printf("2. Restock Barang (Stok, Tgl Masuk & Expired)\n");
            printf("3. Keluar\n");
            printf("Pilih menu (1-3): ");

            int menu = 0;
            if (scanf("%d", &menu) != 1) {
                bersihkanBufferInput();
                printf("Input harus berupa angka!");
                tekanEnter();
                continue;
            }
            bersihkanBufferInput();

            if (menu == 1) {
                halamanTransaksi(katalog, MAX_BARANG, &sesi);
            } else if (menu == 2) {
                restockBarang(katalog, MAX_BARANG, &sesi);
            } else if (menu == 3) {
                printf("Sesi kasir selesai. Sampai jumpa, %s!\n", sesi.namaKasir);
                break;
            } else {
                printf("Pilihan menu tidak tersedia!");
                tekanEnter();
            }
        }
    }

    return 0;
}
