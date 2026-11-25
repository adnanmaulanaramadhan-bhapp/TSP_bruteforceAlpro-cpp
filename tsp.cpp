// alpro11.cpp: File ini berisi fungsi 'main' tempat eksekusi program dimulai dan berakhir.
#include <bits/stdc++.h> // mengimport semua library standar C++ 
#include "Point.h"       // mengimport file header Point.h yang berisi kelas Point dan fungsi jarak

using namespace std;

// Konstanta batasan soal Operasi Water-Bombing Hutan Konoha
const int MAX_N = 11;             // batas maksimal jumlah lokasi (10! = 3.628.800 permutasi, aman dari TLE)
const double MAX_COORD = 1e6;     // batas maksimal nilai mutlak koordinat X dan Y (-10^6 <= X, Y <= 10^6)
const double EPS = 1e-12;         // toleransi presisi floating-point saat membandingkan total jarak

class Tsp_map {
private:
    vector<Point> cities; // deklarasi variable cities bertipe vector<Point> untuk menampung daftar lokasi

public:
    // fungsi untuk menambahkan lokasi baru menggunakan objek Point p ke dalam vector cities
    void add(const Point& p) {
        cities.push_back(p); // memasukkan objek p ke bagian belakang vector cities
    }

    // fungsi overload untuk menambahkan lokasi berdasarkan kode 1 karakter serta koordinat x dan y
    void add(string name, double x, double y) {
        Point p(name, x, y); // membuat objek Point baru dengan parameter name, x, dan y
        add(p);              // memasukkannya ke dalam vector cities menggunakan fungsi add di atas
    }

    // fungsi untuk mengembalikan total jumlah lokasi yang sudah terdaftar di dalam vector cities
    int size() const {
        return static_cast<int>(cities.size()); // mengembalikan ukuran vector cities
    }

    // fungsi untuk mengambil kode lokasi berdasarkan nomor index-nya di dalam vector cities
    string get_city_name(int index) const {
        return cities[index].getName(); // mengembalikan kode lokasi pada index tertentu (misal: "M", "A", "B")
    }

    // fungsi untuk menghitung jarak antara lokasi ke-i dan lokasi ke-j
    double dist_between(int i, int j) const {
        return dist(cities[i], cities[j]); // memanggil fungsi dist dari Point.h
    }

    // fungsi untuk menghitung total jarak keseluruhan dari suatu urutan rute penerbangan (tour)
    double score(const vector<int>& tour) const {
        int n = static_cast<int>(tour.size()); // menyimpan jumlah lokasi dalam rute
        if (n <= 1) return 0.0;                // jika lokasi 0 atau 1, total jarak penerbangan adalah 0

        // menghitung jarak dari titik api terakhir kembali ke Markas Utama (menutup siklus rute)
        double result = dist_between(tour[0], tour[n - 1]);

        // perulangan untuk menjumlahkan jarak antar lokasi secara berurutan sepanjang rute (i dimulai dari 1)
        for (int i = 1; i < n; ++i) {
            result += dist_between(tour[i - 1], tour[i]); // menambahkan jarak dari lokasi sebelumnya ke lokasi saat ini
        }

        return result; // mengembalikan total keseluruhan jarak rute penerbangan
    }

    // fungsi untuk membuat rute awal urutan lokasi secara default (0, 1, 2, dst)
    vector<int> get_default_tour() const {
        vector<int> tour(size()); // membuat vector integer berukuran sejumlah lokasi yang terdaftar
        for (int i = 0; i < size(); ++i) {
            tour[i] = i; // mengisi nomor urut index lokasi ke dalam vector tour
        }
        return tour; // mengembalikan vector rute default
    }

    // fungsi TSP Brute Force untuk mencari rute penerbangan terpendek dari seluruh kemungkinan permutasi
    pair<double, vector<int>> solve_brute_force() const {
        int n = size(); // mengambil jumlah total lokasi
        if (n == 0) return {0.0, {}};  // jika tidak ada lokasi, kembalikan jarak 0 dan rute kosong
        if (n == 1) return {0.0, {0}}; // jika hanya ada Markas Utama, kembalikan jarak 0 dan rute Markas itu sendiri

        // Prakomputasi matriks jarak agar tidak memanggil fungsi akar kuadrat berulang kali di dalam loop permutasi
        vector<vector<double>> dmat(n, vector<double>(n, 0.0));
        for (int i = 0; i < n; ++i) {
            for (int j = i + 1; j < n; ++j) {
                double d = dist_between(i, j);
                dmat[i][j] = d;
                dmat[j][i] = d;
            }
        }

        vector<int> current_tour = get_default_tour(); // mengambil rute awal (0, 1, 2, ..., n-1)
        vector<int> best_tour = current_tour;          // variable untuk menyimpan urutan index rute terbaik
        double min_distance = numeric_limits<double>::infinity(); // inisialisasi jarak terpendek dengan tak hingga

        // perulangan do-while untuk menguji semua kemungkinan urutan permutasi titik api (Brute Force)
        // Markas Utama (index 0) dikunci sebagai titik keberangkatan dan kepulangan wajib
        do {
            double current_distance = dmat[current_tour[0]][current_tour[n - 1]]; // jarak kembali ke Markas Utama
            for (int i = 1; i < n; ++i) {
                current_distance += dmat[current_tour[i - 1]][current_tour[i]];   // menjumlahkan jarak antar lokasi
            }

            if (current_distance < min_distance - EPS) { // mengecek apakah jarak saat ini lebih pendek
                min_distance = current_distance;         // memperbarui nilai jarak rute terpendek
                best_tour = current_tour;                // menyimpan urutan rute terbaik yang baru ditemukan
            }
        } while (next_permutation(current_tour.begin() + 1, current_tour.end())); // menghasilkan urutan berikutnya

        return {min_distance, best_tour}; // mengembalikan pasangan (jarak terbaik, urutan index rute terbaik)
    }
}; // akhir class Tsp_map

// Fungsi bantu untuk membaca satu baris yang tidak kosong dari aliran input
bool read_non_empty_line(istream& in, string& line) {
    while (getline(in, line)) {
        for (char c : line) {
            if (!isspace(static_cast<unsigned char>(c))) {
                return true; // baris valid ditemukan
            }
        }
    }
    return false; // mencapai akhir input (EOF)
}

int main() {
    string line;

    // 1. Membaca baris pertama untuk jumlah lokasi (N)
    if (!read_non_empty_line(cin, line)) {
        cout << "INVALID INPUT\n";
        return 0;
    }

    long long jumlahLokasi;
    string extraToken;
    istringstream issN(line);

    // Validasi: N harus bilangan bulat murni tanpa karakter tambahan di baris yang sama
    if (!(issN >> jumlahLokasi) || (issN >> extraToken)) {
        cout << "INVALID INPUT\n";
        return 0;
    }

    // 2. Validasi Boundary Out of Scope untuk N (0 <= N <= 11)
    if (jumlahLokasi < 0 || jumlahLokasi > MAX_N) {
        cout << "OUT OF SCOPE\n";
        return 0;
    }

    Tsp_map petaKonoha;               // membuat objek petaKonoha dari class Tsp_map
    unordered_set<string> daftarKode; // set untuk mendeteksi kode lokasi duplikat

    // 3. Membaca tepat N baris berikutnya untuk data setiap lokasi (Kode X Y)
    for (int i = 0; i < jumlahLokasi; ++i) {
        if (!read_non_empty_line(cin, line)) {
            cout << "INVALID INPUT\n"; // gagal jika jumlah baris yang diberikan kurang dari N
            return 0;
        }

        string kodeLokasi;
        double x, y;
        istringstream issPoint(line);

        // Validasi format "Kode X Y" dan memastikan tidak ada teks tambahan di belakang koordinat Y
        if (!(issPoint >> kodeLokasi >> x >> y) || (issPoint >> extraToken)) {
            cout << "INVALID INPUT\n";
            return 0;
        }

        // Menolak nilai koordinat NaN atau Infinity
        if (!isfinite(x) || !isfinite(y)) {
            cout << "INVALID INPUT\n";
            return 0;
        }

        // Menolak jika kode lokasi tidak tepat 1 karakter atau sudah pernah digunakan (duplikat)
        if (kodeLokasi.size() != 1 || daftarKode.count(kodeLokasi)) {
            cout << "INVALID INPUT\n";
            return 0;
        }

        // Validasi Boundary Out of Scope untuk koordinat (-10^6 <= X, Y <= 10^6)
        if (fabs(x) > MAX_COORD || fabs(y) > MAX_COORD) {
            cout << "OUT OF SCOPE\n";
            return 0;
        }

        daftarKode.insert(kodeLokasi);
        petaKonoha.add(kodeLokasi, x, y);
    }

    // 4. Menjalankan solver TSP Brute Force untuk mencari rute pemadaman terpendek
    pair<double, vector<int>> hasil = petaKonoha.solve_brute_force();
    double jarakTerbaik = hasil.first;
    vector<int> ruteTerbaik = hasil.second;

    // 5. Mencetak output bersih standar Competitive Programming
    cout << fixed << setprecision(6) << jarakTerbaik << "\n";
    if (ruteTerbaik.empty()) {
        cout << "-\n"; // keluaran rute jika N = 0 (tidak ada lokasi)
    } else {
        for (int i = 0; i < static_cast<int>(ruteTerbaik.size()); ++i) {
            cout << petaKonoha.get_city_name(ruteTerbaik[i]) << " ";
        }
        // mencetak kembali kode Markas Utama di akhir baris untuk menutup siklus penerbangan
        cout << petaKonoha.get_city_name(ruteTerbaik[0]) << "\n";
    }

    return 0; // menandakan program selesai dieksekusi dengan sukses
}