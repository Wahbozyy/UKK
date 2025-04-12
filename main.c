#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef struct {
    char nama[50];
    int harga;
    int jumlah;
    int total_harga;
    float diskon;
} Produk;

void nama_file(char *filename) {
    time_t t = time(NULL);
    struct tm *tm = localtime(&t);
    strftime(filename, 100, "struk_%Y%m%d_%H%M%S.txt", tm);
}

int bandingkan_produk(const void *a, const void *b) {
    Produk *produkA = (Produk *)a;
    Produk *produkB = (Produk *)b;
    return produkB->jumlah - produkA->jumlah;
}

void reset(Produk *produk, int jumlah_produk) {
    int i;
    for (i = 0; i < jumlah_produk; i++) {
        produk[i].jumlah = 0;
        produk[i].total_harga = 0;
        produk[i].diskon = 0;
    }
    printf("Pesanan anda telah direset!\n");
}

void menu() {
    printf("========================================\n");
    printf("      Selamat datang di Toko SKENSA     \n");
    printf("Silakan pilih barang yang Anda inginkan:\n");
    printf("========================================\n");
    printf("|No | Barang        | Harga            |\n");
    printf("----------------------------------------\n");
    printf("| 1 | Buku Tulis    | Rp.5000          |\n");
    printf("| 2 | Pensil        | Rp.2000          |\n");
    printf("| 3 | Penghapus     | Rp.1000          |\n");
    printf("| 4 | Penggaris     | Rp.1000          |\n");
    printf("| 5 | Bujur Sangkar | Rp.500           |\n");
    printf("========================================\n");
    printf("99. Struk Pembayaran\n");
    printf("55. Reset pilihan\n");
    printf("00. Keluar\n");
    printf("========================================\n");
}

void tampilkan_rekap(Produk *produk, int jumlah_produk, int total_bayar, int total_diskon) {
    int i;
    int tagihan = total_bayar - total_diskon;
    int nomor = 1;
    Produk produk_urut[jumlah_produk];
    memcpy(produk_urut, produk, sizeof(Produk) * jumlah_produk);
    qsort(produk_urut, jumlah_produk, sizeof(Produk), bandingkan_produk);

    printf("=========================================================================\n");
    printf("                           Rekap Pesanan Barang                          \n");
    printf("=========================================================================\n");
    printf("| %-1s | %-3s | %-16s | %-10s | %-10s | %-9s |\n", "No", "Jumlah", "Nama Barang", "Harga", "Total Harga", "Diskon");
    printf("=========================================================================\n");

    for (i = 0; i < jumlah_produk; i++) {
        if (produk_urut[i].jumlah > 0) {
            printf("| %-2d | %-6d | %-16s | Rp.%-7d | Rp.%-8d | Rp.%-6d |\n",
                   nomor++,
                   produk_urut[i].jumlah,
                   produk_urut[i].nama,
                   produk_urut[i].harga,
                   produk_urut[i].total_harga,
                   (int)(produk_urut[i].diskon * produk_urut[i].total_harga));
        }
    }

    printf("=========================================================================\n");
    printf("| Total Harga  = Rp.%-51d |\n", total_bayar);
    printf("| Total Diskon = Rp.%-51d |\n", total_diskon);
    printf("| Tagihan      = Rp.%-51d |\n", tagihan);
    printf("=========================================================================\n");
}

void simpan_struk(Produk *produk, int jumlah_produk, int total_bayar, int total_diskon, int pembayaran) {
    char filename[100];
    nama_file(filename);
    FILE *file = fopen(filename, "w");
    time_t now = time(NULL);
    char waktu[100];
    struct tm *tm_info = localtime(&now);
    strftime(waktu, 100, "%a %b %d %H:%M:%S %Y", tm_info);

    if (file == NULL) {
        printf("Error: Tidak dapat membuat file struk!\n");
        return;
    }

    int i;
    int tagihan = total_bayar - total_diskon;
    int kembalian = pembayaran - tagihan;
    Produk produk_urut[jumlah_produk];
    memcpy(produk_urut, produk, sizeof(Produk) * jumlah_produk);
    qsort(produk_urut, jumlah_produk, sizeof(Produk), bandingkan_produk);

    fprintf(file, "==============================================================\n");
    fprintf(file, "|                         Toko SKENSA                        |\n");
    fprintf(file, "|              Jl. HOS Cokroaminoto No. 84, Denpasar         |\n");
    fprintf(file, "|                           Bali                             |\n");
    fprintf(file, "|                    Telp : 08162855791                      |\n");
    fprintf(file, "|ID Struk : %ld                                       |\n", time(NULL));
    fprintf(file, "|============================================================|\n");
    fprintf(file, "| %-20s | %-10s | %-10s | %-9s |\n", "Nama Barang", "Harga", "Total", "Diskon");
    fprintf(file, "|============================================================|\n");

    for (i = 0; i < jumlah_produk; i++) {
        if (produk_urut[i].jumlah > 0) {
            char item_display[100];
            sprintf(item_display, "%dx %s", produk_urut[i].jumlah, produk_urut[i].nama);

            fprintf(file, "| %-20s | Rp.%-7d | Rp.%-7d | Rp.%-6d |\n",
                   item_display,
                   produk_urut[i].harga,
                   produk_urut[i].total_harga,
                   (int)(produk_urut[i].diskon * produk_urut[i].total_harga));
        }
    }

    fprintf(file, "|============================================================|\n");
    fprintf(file, "| Total Harga  = Rp.%-40d |\n", total_bayar);
    fprintf(file, "| Total Diskon = Rp.%-40d |\n", total_diskon);
    fprintf(file, "| Tagihan      = Rp.%-40d |\n", tagihan);
    fprintf(file, "| Pembayaran   = Rp.%-40d |\n", pembayaran);
    fprintf(file, "| Kembalian    = Rp.%-40d |\n", kembalian);
    fprintf(file, "|                   %-40s |\n", "");
    fprintf(file, "|                   %-40s |\n", "");
    fprintf(file, "| Waktu/Hari   = %-40s    |\n", waktu);
    fprintf(file, "|                   %-40s |\n", "");
    fprintf(file, "==============================================================\n");

    fclose(file);
    printf("\nStruk telah disimpan dalam file: %s\n", filename);
}

void hitung_total(Produk *produk, int jumlah_produk, int *total_bayar, int *total_diskon) {
    *total_bayar = 0;
    *total_diskon = 0;
    int i;
    for (i = 0; i < jumlah_produk; i++) {
        if (produk[i].jumlah > 0) {
            *total_bayar += produk[i].total_harga;
            *total_diskon += (int)(produk[i].diskon * produk[i].total_harga);
        }
    }
}

void kembalian(int total_bayar, int total_diskon, int pembayaran) {
    int tagihan = total_bayar - total_diskon;
    int kembalian = pembayaran - tagihan;
    printf("\nKembalian : Rp.%d\n", kembalian);
}

float hitung_diskon(int jumlah) {
    if (jumlah > 5) return 0.15;
    if (jumlah > 3) return 0.10;
    return 0.0;
}

int main() {
    Produk daftar_produk[5] = {
        {"Buku Tulis", 5000, 0, 0, 0},
        {"Pensil", 2000, 0, 0, 0},
        {"Penghapus", 1000, 0, 0, 0},
        {"Penggaris", 1000, 0, 0, 0},
        {"Bujur Sangkar", 500, 0, 0, 0}
    };

    int pilihan;
    int total_bayar = 0, total_diskon = 0;
    int pembayaran = 0;
    const int jumlah_produk = 5;

    do {
        system("cls");
        menu();

        printf("\nMasukkan pilihan: ");
        scanf("%d", &pilihan);

        if (pilihan >= 1 && pilihan <= 5) {
            int index = pilihan - 1;
            printf("Masukkan jumlah %s: ", daftar_produk[index].nama);
            scanf("%d", &daftar_produk[index].jumlah);
            daftar_produk[index].diskon = hitung_diskon(daftar_produk[index].jumlah);
            daftar_produk[index].total_harga = daftar_produk[index].harga * daftar_produk[index].jumlah;
        } else if (pilihan == 99) {
            int i;
            int keranjang_kosong = 1;
            for (i = 0; i < jumlah_produk; i++) {
                if (daftar_produk[i].jumlah > 0) {
                    keranjang_kosong = 0;
                    break;
                }
            }

            if (keranjang_kosong) {
                printf("\nKeranjang belanja Anda kosong! Silakan pilih barang terlebih dahulu.\n");
                printf("Tekan Enter untuk kembali ke menu...");
                getchar(); getchar();
                continue;
            }

            system("cls");
            hitung_total(daftar_produk, jumlah_produk, &total_bayar, &total_diskon);
            int tagihan = total_bayar - total_diskon;
            tampilkan_rekap(daftar_produk, jumlah_produk, total_bayar, total_diskon);

            printf("\nTotal tagihan Anda: Rp.%d\n", tagihan);
            do {
                printf("Masukkan jumlah pembayaran: Rp.");
                scanf("%d", &pembayaran);
                if (pembayaran < tagihan) {
                    printf("Pembayaran kurang! Silakan masukkan jumlah yang cukup.\n");
                }
            } while (pembayaran < tagihan);

            kembalian(total_bayar, total_diskon, pembayaran);
            simpan_struk(daftar_produk, jumlah_produk, total_bayar, total_diskon, pembayaran);

            printf("\nTekan Enter untuk kembali ke menu...");
            getchar(); getchar();
        } else if (pilihan == 55) {
            reset(daftar_produk, jumlah_produk);
            printf("Tekan Enter untuk melanjutkan...");
            getchar(); getchar();
        } else if (pilihan == 0) {
            printf("Terima kasih telah berbelanja!\n");
            break;
        }

    } while (1);

    return 0;
}

