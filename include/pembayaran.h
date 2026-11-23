#ifndef PEMBAYARAN_H
#define PEMBAYARAN_H

void importQRDariFile(const char *namaFile, long totalBelanja);
void generateKodeVA(int pilihanBank, long totalBelanja, int jumlahItem, char *namaBank, char *kodeVA);

#endif
