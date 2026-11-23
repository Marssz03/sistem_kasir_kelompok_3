#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "utils.h"

void bersihkanLayar(void) {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void bersihkanBufferInput(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {}
}

void tekanEnter(void) {
    printf("\nTekan Enter untuk melanjutkan...");
    getchar();
}

void bacaBaris(char *buffer, int ukuran) {
    if (fgets(buffer, ukuran, stdin) != NULL) {
        size_t len = strlen(buffer);
        if (len > 0 && buffer[len - 1] == '\n') {
            buffer[len - 1] = '\0';
        }
    }
}

void ambilHariTanggalOtomatis(char *hasil, int ukuran) {
    const char *namaHari[] = {
        "Minggu", "Senin", "Selasa", "Rabu", "Kamis", "Jumat", "Sabtu"
    };
    const char *namaBulan[] = {
        "Januari", "Februari", "Maret", "April", "Mei", "Juni",
        "Juli", "Agustus", "September", "Oktober", "November", "Desember"
    };

    time_t sekarang = time(NULL);
    struct tm *waktuLokal = localtime(&sekarang);

    snprintf(hasil, ukuran, "%s, %02d %s %d (%02d:%02d)",
             namaHari[waktuLokal->tm_wday],
             waktuLokal->tm_mday,
             namaBulan[waktuLokal->tm_mon],
             waktuLokal->tm_year + 1900,
             waktuLokal->tm_hour,
             waktuLokal->tm_min);
}
