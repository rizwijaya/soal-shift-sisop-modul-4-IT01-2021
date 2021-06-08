# soal-shift-sisop-modul-4-IT01-2021
### Nama Anggota Kelompok:
1. Widya Inayatul Luthfiyah (05311940000010)
2. Muhammad Rizqi Wijaya (05311940000014)
3. Gavin Bagus Kanzie N (05311940000028)
---
## Soal Nomor 1
Di suatu jurusan, terdapat admin lab baru yang super duper gabut, ia bernama Sin. Sin baru menjadi admin di lab tersebut selama 1 bulan. Selama sebulan tersebut ia bertemu orang-orang hebat di lab tersebut, salah satunya yaitu Sei. Sei dan Sin akhirnya berteman baik. Karena belakangan ini sedang ramai tentang kasus keamanan data, mereka berniat membuat filesystem dengan metode encode yang mutakhir. Berikut adalah filesystem rancangan Sin dan Sei :
- Jika sebuah direktori dibuat dengan awalan “AtoZ_”, maka direktori tersebut akan menjadi direktori ter-encode.
- Jika sebuah direktori di-rename dengan awalan “AtoZ_”, maka direktori tersebut akan menjadi direktori ter-encode.
- Apabila direktori yang terenkripsi di-rename menjadi tidak ter-encode, maka isi direktori tersebut akan terdecode.
- Setiap pembuatan direktori ter-encode (mkdir atau rename) akan tercatat ke sebuah log. Format : /home/[USER]/Downloads/[Nama Direktori] → /home/[USER]/Downloads/AtoZ_[Nama Direktori]
- Metode encode pada suatu direktori juga berlaku terhadap direktori yang ada di dalamnya.(rekursif)

## Soal Nomor 2
Selain itu Sei mengusulkan untuk membuat metode enkripsi tambahan agar data pada komputer mereka semakin aman. Berikut rancangan metode enkripsi tambahan yang dirancang oleh Sei
- Jika sebuah direktori dibuat dengan awalan “RX_[Nama]”, maka direktori tersebut akan menjadi direktori terencode beserta isinya dengan perubahan nama isi sesuai kasus nomor 1 dengan algoritma tambahan ROT13 (Atbash + ROT13).
- Jika sebuah direktori di-rename dengan awalan “RX_[Nama]”, maka direktori tersebut akan menjadi direktori terencode beserta isinya dengan perubahan nama isi sesuai dengan kasus nomor 1 dengan algoritma tambahan Vigenere Cipher dengan key “SISOP” (Case-sensitive, Atbash + Vigenere).
- Apabila direktori yang terencode di-rename (Dihilangkan “RX_” nya), maka folder menjadi tidak terencode dan isi direktori tersebut akan terdecode berdasar nama aslinya.
- Setiap pembuatan direktori terencode (mkdir atau rename) akan tercatat ke sebuah log file beserta methodnya (apakah itu mkdir atau rename).
- Pada metode enkripsi ini, file-file pada direktori asli akan menjadi terpecah menjadi file-file kecil sebesar 1024 bytes, sementara jika diakses melalui filesystem rancangan Sin dan Sei akan menjadi normal. Sebagai contoh, Suatu_File.txt berukuran 3 kiloBytes pada directory asli akan menjadi 3 file kecil yakni:
  
  Suatu_File.txt.0000
  
  Suatu_File.txt.0001 
  
  Suatu_File.txt.0002

Ketika diakses melalui filesystem hanya akan muncul Suatu_File.txt

## Soal Nomor 3
Karena Sin masih super duper gabut akhirnya dia menambahkan sebuah fitur lagi pada filesystem mereka. 
- Jika sebuah direktori dibuat dengan awalan “A_is_a_”, maka direktori tersebut akan menjadi sebuah direktori spesial.
- Jika sebuah direktori di-rename dengan memberi awalan “A_is_a_”, maka direktori tersebut akan menjadi sebuah direktori spesial.
- Apabila direktori yang terenkripsi di-rename dengan menghapus “A_is_a_” pada bagian awal nama folder maka direktori tersebut menjadi direktori normal.
- Direktori spesial adalah direktori yang mengembalikan enkripsi/encoding pada direktori “AtoZ_” maupun “RX_” namun masing-masing aturan mereka tetap berjalan pada direktori di dalamnya (sifat recursive  “AtoZ_” dan “RX_” tetap berjalan pada subdirektori).
- Pada direktori spesial semua nama file (tidak termasuk ekstensi) pada fuse akan berubah menjadi lowercase insensitive dan diberi ekstensi baru berupa nilai desimal dari binner perbedaan namanya.


Contohnya jika pada direktori asli nama filenya adalah “FiLe_CoNtoH.txt” maka pada fuse akan menjadi “file_contoh.txt.1321”. 1321 berasal dari biner 10100101001.


## Soal Nomor 4

Karena Sin masih super duper gabut akhirnya dia menambahkan sebuah fitur lagi pada filesystem mereka. 
- Log system yang akan terbentuk bernama “SinSeiFS.log” pada direktori home pengguna (/home/[user]/SinSeiFS.log). Log system ini akan menyimpan daftar perintah system call yang telah dijalankan pada filesystem.
- Karena Sin dan Sei suka kerapian maka log yang dibuat akan dibagi menjadi dua level, yaitu INFO dan WARNING.
- Untuk log level WARNING, digunakan untuk mencatat syscall rmdir dan unlink.
- Sisanya, akan dicatat pada level INFO.
Format untuk logging yaitu:

[Level]::[dd][mm][yyyy]-[HH]:[MM]:[SS]:[CMD]::[DESC :: DESC]

Level : Level logging, dd : 2 digit tanggal, mm : 2 digit bulan, yyyy : 4 digit tahun, HH : 2 digit jam (format 24 Jam),MM : 2 digit menit, SS : 2 digit detik, CMD : System Call yang terpanggil, DESC : informasi dan parameter tambahan

INFO::28052021-10:00:00:CREATE::/test.txt

INFO::28052021-10:01:00:RENAME::/test.txt::/rename.txt


## Penjelasan Code
```
static const char *dirpath = "/home/pepega/Downloads";
```
inisialisasi variabel statis untuk menyetel direktori default

```
void encodeAtbash(char *str)
```
fungsi ini dibuat agar direktori yang dibuat A-Z ter-encode. Dan akan dilakukan perulangan

```
void logging(char *call)
{
  FILE *log;
  log = fopen("/home/pepega/SinSeiFS.log", "a");
  char res[1000];

  fprintf(log, "%s\n", res);
  fclose(log);
}
```
dibuat fungsi logging yang dimana log system akan terbentuk dengan nama "SinSeiFS.log" pada direktori (/home/[user]/SinSeiFS.log). Log system ini akan menyimpan daftar perintah system call yang telah dijalankan pada filesystem.

```
static int xmp_mkdir(const char *path, mode_t mode)
{
  char fpath[1000];
  printf("path: %s\n", path);
  char *dirName = strstr(path, "AtoZ_");
  if (dirName != NULL)
  {
```
sebuah direktori akan dibuat dengan awalan AtoZ. Dan dilakukan perulangan

```
static int xmp_rename(const char *from, const char *to)
{
  printf("rename from:%s, to: %s\n", from, to);
  int res;

  res = rename(from, to);
  if (res == -1)
    return -errno;

  return 0;
}
```
Sebuah direktori akan direname dengan awalan AtoZ. Dan dilakukan perulangan
## Kendala: 
Pada soal no 1, kami mencoba untuk mengganti nama file di dalam folder ```AtoZ_``` dengan enkripsi dalam fungsi ```xmp_readdir```. Kami berhasil mengdebugnya dengan print setiap nama file yang terenkripsi: 
![image](https://user-images.githubusercontent.com/73151823/121157454-c7929a00-c873-11eb-8b97-56dde4cb9d38.png)

Namun, ketika kami coba lihat menggunakan perintah ```ls```, terjadi error sebagai berikut:
![image](https://user-images.githubusercontent.com/73151823/121157484-cf523e80-c873-11eb-9f7f-c8deb97798e5.png)


Untuk no 2, 3, dan 4 belum kami kerjakan, dikarenakan masih kurang pemahaman dan sumber referensi dalam mengerjakan FUSE

