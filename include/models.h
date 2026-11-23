#ifndef MODELS_H
#define MODELS_H

#define MAX_BARANG 8
#define MAX_KERANJANG 50
#define MAX_USER 4

typedef struct {
    char username[30];
    char password[30];
} User;

typedef struct {
    char namaKasir[30];
    char hariTanggalLogin[50];
} SesiKasir;

typedef struct {
    int no;
    unsigned long long barcode;
    char nama[50];
    long harga;
    int stok;
    char tanggalMasuk[15];   // Format: DD-MM-YYYY
    char tanggalExpired[15]; // Format: DD-MM-YYYY
} Barang;

typedef struct {
    unsigned long long barcode;
    char nama[50];
    long harga;
    int jumlah;
    long subtotal;
} ItemKeranjang;

#endif
