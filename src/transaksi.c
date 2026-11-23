#include <stdio.h>
#include <string.h>
#include "transaksi.h"
#include "inventori.h"
#include "pembayaran.h"
#include "utils.h"

void halamanTransaksi(Barang katalog[], int jumlahBarang, const SesiKasir *sesi) {
    ItemKeranjang keranjang[MAX_KERANJANG];
    int jumlahItem = 0;
    long totalBelanja = 0;

    while (1) {
        bersihkanLayar();
        printf("Kasir Bertugas : %s\n", sesi->namaKasir);
        printf("Hari & Tanggal : %s\n", sesi->hariTanggalLogin);
        printf("Total Sementara: Rp%ld\n\n", totalBelanja);
        tampilkanKatalog(katalog, jumlahBarang);

        printf("\nMasukkan Barcode Barang (ketik 0 untuk lanjut pembayaran): ");
        unsigned long long inputBarcode;
        if (scanf("%llu", &inputBarcode) != 1) {
            bersihkanBufferInput();
            printf("Input tidak valid! Masukkan angka barcode.");
            tekanEnter();
            continue;
        }
        bersihkanBufferInput();

        if (inputBarcode == 0) {
            if (jumlahItem == 0) {
                printf("Keranjang masih kosong! Tambahkan minimal 1 barang.");
                tekanEnter();
                continue;
            }
            break;
        }

        int idx = cariIndexBarangByBarcode(katalog, jumlahBarang, inputBarcode);
        if (idx == -1) {
            printf("Barcode %llu tidak ditemukan!", inputBarcode);
            tekanEnter();
            continue;
        }

        if (katalog[idx].stok <= 0) {
            printf("Stok %s habis! Silakan lakukan restock terlebih dahulu.", katalog[idx].nama);
            tekanEnter();
            continue;
        }

        if (jumlahItem >= MAX_KERANJANG) {
            printf("Keranjang sudah penuh (maksimal %d item)!", MAX_KERANJANG);
            tekanEnter();
            continue;
        }

        int jumlahBeli = 0;
        while (1) {
            printf("Jumlah beli [%s | Stok tersedia: %d]: ", katalog[idx].nama, katalog[idx].stok);
            if (scanf("%d", &jumlahBeli) != 1 || jumlahBeli <= 0) {
                bersihkanBufferInput();
                printf("Jumlah beli harus berupa angka lebih dari 0!\n");
                continue;
            }
            bersihkanBufferInput();

            if (jumlahBeli > katalog[idx].stok) {
                printf("Stok tidak mencukupi! Sisa stok hanya %d.\n", katalog[idx].stok);
                continue;
            }
            break;
        }

        katalog[idx].stok -= jumlahBeli;

        long subtotal = katalog[idx].harga * jumlahBeli;
        keranjang[jumlahItem].barcode = katalog[idx].barcode;
        strcpy(keranjang[jumlahItem].nama, katalog[idx].nama);
        keranjang[jumlahItem].harga = katalog[idx].harga;
        keranjang[jumlahItem].jumlah = jumlahBeli;
        keranjang[jumlahItem].subtotal = subtotal;

        jumlahItem++;
        totalBelanja += subtotal;

        printf("Ditambahkan ke keranjang! Subtotal: Rp%ld.", subtotal);
        tekanEnter();
    }

    // Tampilan Struk Belanja
    bersihkanLayar();
    printf("==================================================================================\n");
    printf("                                STRUK BELANJA TOKO                                \n");
    printf("==================================================================================\n");
    printf("Kasir Bertugas : %s\n", sesi->namaKasir);
    printf("Hari & Tanggal : %s\n", sesi->hariTanggalLogin);
    printf("----------------------------------------------------------------------------------\n");
    printf("| %-13s | %-24s | %3s | %-10s | %-12s |\n",
           "Barcode", "Barang", "Qty", "Harga", "Subtotal");
    printf("----------------------------------------------------------------------------------\n");
    for (int i = 0; i < jumlahItem; i++) {
        printf("| %-13llu | %-24s | %3d | Rp%-8ld | Rp%-10ld |\n",
               keranjang[i].barcode,
               keranjang[i].nama,
               keranjang[i].jumlah,
               keranjang[i].harga,
               keranjang[i].subtotal);
    }
    printf("----------------------------------------------------------------------------------\n");
    printf("TOTAL BELANJA: Rp%ld\n", totalBelanja);
    printf("==================================================================================\n");

    // Pilih 1 dari 3 Metode Pembayaran
    int metode = 0;
    while (1) {
        printf("\nPilih Metode Pembayaran:\n");
        printf("1. QRIS\n");
        printf("2. Transfer Bank (Virtual Account)\n");
        printf("3. Cash\n");
        printf("Pilihan (1-3): ");
        if (scanf("%d", &metode) != 1 || metode < 1 || metode > 3) {
            bersihkanBufferInput();
            printf("Pilihan tidak valid! Masukkan angka 1, 2, atau 3.\n");
            continue;
        }
        bersihkanBufferInput();
        break;
    }

    char namaMetode[35];
    char kodeVA[30] = "-";
    long uangBayar = totalBelanja;
    long kembalian = 0;

    if (metode == 1) {
        strcpy(namaMetode, "QRIS");
        importQRDariFile("qris_qr.txt", totalBelanja);
        int konfirmasi = 0;
        while (1) {
            printf("Konfirmasi pembayaran QRIS sudah diterima? (1 = Ya / 0 = Belum): ");
            if (scanf("%d", &konfirmasi) != 1 || (konfirmasi != 0 && konfirmasi != 1)) {
                bersihkanBufferInput();
                printf("Masukkan 1 atau 0.\n");
                continue;
            }
            bersihkanBufferInput();
            if (konfirmasi == 1) {
                break;
            }
            printf("Menunggu pembayaran QRIS...\n");
        }
        uangBayar = totalBelanja;
        kembalian = 0;
    } else if (metode == 2) {
        int pilihanBank = 0;
        while (1) {
            printf("\nPilih Bank Tujuan Virtual Account:\n");
            printf("1. BCA\n");
            printf("2. Mandiri\n");
            printf("3. BRI\n");
            printf("4. BNI\n");
            printf("Pilihan bank (1-4): ");
            if (scanf("%d", &pilihanBank) != 1 || pilihanBank < 1 || pilihanBank > 4) {
                bersihkanBufferInput();
                printf("Pilihan bank tidak valid! Masukkan 1 sampai 4.\n");
                continue;
            }
            bersihkanBufferInput();
            break;
        }

        char namaBank[35];
        generateKodeVA(pilihanBank, totalBelanja, jumlahItem, namaBank, kodeVA);
        snprintf(namaMetode, sizeof(namaMetode), "Transfer Bank (%s)", namaBank);

        printf("\n+--------------------------------------------------+\n");
        printf("|           VIRTUAL ACCOUNT TRANSFER BANK          |\n");
        printf("+--------------------------------------------------+\n");
        printf("| Bank Tujuan : %-34s |\n", namaBank);
        printf("| Kode VA     : %-34s |\n", kodeVA);
        printf("| Atas Nama   : %-34s |\n", "TOKO KASIR CLI");
        printf("| Tagihan     : Rp%-32ld |\n", totalBelanja);
        printf("+--------------------------------------------------+\n");

        int konfirmasi = 0;
        while (1) {
            printf("Konfirmasi pembayaran VA sudah terbayar? (1 = Ya / 0 = Belum): ");
            if (scanf("%d", &konfirmasi) != 1 || (konfirmasi != 0 && konfirmasi != 1)) {
                bersihkanBufferInput();
                printf("Masukkan 1 atau 0.\n");
                continue;
            }
            bersihkanBufferInput();
            if (konfirmasi == 1) {
                break;
            }
            printf("Menunggu pembayaran Virtual Account...\n");
        }
        uangBayar = totalBelanja;
        kembalian = 0;
    } else {
        strcpy(namaMetode, "Cash");
        while (1) {
            printf("Masukkan Uang Cash: Rp");
            if (scanf("%ld", &uangBayar) != 1 || uangBayar < 0) {
                bersihkanBufferInput();
                printf("Nominal uang tidak valid!\n");
                continue;
            }
            bersihkanBufferInput();

            if (uangBayar < totalBelanja) {
                printf("Uang cash kurang sebesar Rp%ld!\n", totalBelanja - uangBayar);
            } else {
                kembalian = uangBayar - totalBelanja;
                break;
            }
        }
    }

    printf("\n==================================================\n");
    printf("Kasir Bertugas    : %s\n", sesi->namaKasir);
    printf("Hari & Tanggal    : %s\n", sesi->hariTanggalLogin);
    printf("Metode Pembayaran : %s\n", namaMetode);
    if (metode == 2) {
        printf("Kode VA           : %s\n", kodeVA);
    }
    printf("Total Belanja     : Rp%ld\n", totalBelanja);
    printf("Uang Dibayar      : Rp%ld\n", uangBayar);
    printf("Kembalian         : Rp%ld\n", kembalian);
    printf("Status            : LUNAS\n");
    printf("==================================================\n");
    printf("Terima kasih telah berbelanja!\n");
    tekanEnter();
}
