#include <stdio.h>
#include <string.h>
#include "inventori.h"
#include "utils.h"

int cariIndexBarangByBarcode(Barang katalog[], int jumlahBarang, unsigned long long barcode) {
    for (int i = 0; i < jumlahBarang; i++) {
        if (katalog[i].barcode == barcode) {
            return i;
        }
    }
    return -1;
}

void tampilkanKatalog(Barang katalog[], int jumlahBarang) {
    printf("====================================================================================================\n");
    printf("| %-13s | %-24s | %-10s | %-4s | %-10s | %-10s |\n",
           "Barcode", "Nama Barang", "Harga", "Stok", "Tgl Masuk", "Expired");
    printf("====================================================================================================\n");
    for (int i = 0; i < jumlahBarang; i++) {
        printf("| %-13llu | %-24s | Rp%-8ld | %-4d | %-10s | %-10s |\n",
               katalog[i].barcode,
               katalog[i].nama,
               katalog[i].harga,
               katalog[i].stok,
               katalog[i].tanggalMasuk,
               katalog[i].tanggalExpired);
    }
    printf("====================================================================================================\n");
}

void restockBarang(Barang katalog[], int jumlahBarang, const SesiKasir *sesi) {
    while (1) {
        bersihkanLayar();
        printf("=== RESTOCK BARANG ===\n");
        printf("Kasir Bertugas : %s\n", sesi->namaKasir);
        printf("Hari & Tanggal : %s\n\n", sesi->hariTanggalLogin);
        tampilkanKatalog(katalog, jumlahBarang);

        printf("\nMasukkan Barcode Barang untuk restock (ketik 0 untuk kembali): ");
        unsigned long long inputBarcode;
        if (scanf("%llu", &inputBarcode) != 1) {
            bersihkanBufferInput();
            printf("Input tidak valid! Masukkan angka barcode.");
            tekanEnter();
            continue;
        }
        bersihkanBufferInput();

        if (inputBarcode == 0) {
            break;
        }

        int idx = cariIndexBarangByBarcode(katalog, jumlahBarang, inputBarcode);
        if (idx == -1) {
            printf("Barcode %llu tidak ditemukan!", inputBarcode);
            tekanEnter();
            continue;
        }

        int tambahStok = 0;
        while (1) {
            printf("Jumlah stok masuk [%s - stok saat ini %d]: ",
                   katalog[idx].nama, katalog[idx].stok);
            if (scanf("%d", &tambahStok) != 1 || tambahStok <= 0) {
                bersihkanBufferInput();
                printf("Jumlah stok harus berupa angka lebih dari 0!\n");
                continue;
            }
            bersihkanBufferInput();
            break;
        }

        char tglMasukBaru[15];
        char tglExpiredBaru[15];

        printf("Tanggal Masuk baru (DD-MM-YYYY, sebelumnya %s): ", katalog[idx].tanggalMasuk);
        scanf("%14s", tglMasukBaru);
        bersihkanBufferInput();

        printf("Tanggal Expired baru (DD-MM-YYYY, sebelumnya %s): ", katalog[idx].tanggalExpired);
        scanf("%14s", tglExpiredBaru);
        bersihkanBufferInput();

        katalog[idx].stok += tambahStok;
        strcpy(katalog[idx].tanggalMasuk, tglMasukBaru);
        strcpy(katalog[idx].tanggalExpired, tglExpiredBaru);

        printf("\nRestock berhasil! Stok %s kini menjadi %d (Masuk: %s | Exp: %s).",
               katalog[idx].nama, katalog[idx].stok,
               katalog[idx].tanggalMasuk, katalog[idx].tanggalExpired);
        tekanEnter();
    }
}

// Implementasi di inventori.c
int cariBarangByInput(Barang katalog[], int jumlah, unsigned long long input) {
    for (int i = 0; i < jumlah; i++) {
        // Cek apakah input cocok dengan nomor urut (1, 2, 3...) ATAU barcode lengkap
        if (katalog[i].id == (int)input || katalog[i].barcode == input) {
            return i; // Mengembalikan index array barang yang ditemukan
        }
    }
    return -1; // Tidak ditemukan
}