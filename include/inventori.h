#ifndef INVENTORI_H
#define INVENTORI_H

#include "models.h"

int cariBarangByInput(Barang katalog[], int jumlah, unsigned long long input);
int cariIndexBarangByBarcode(Barang katalog[], int jumlahBarang, unsigned long long barcode);
void tampilkanKatalog(Barang katalog[], int jumlahBarang);
void restockBarang(Barang katalog[], int jumlahBarang, const SesiKasir *sesi);

#endif
