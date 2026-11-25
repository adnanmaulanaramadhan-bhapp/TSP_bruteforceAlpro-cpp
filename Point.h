#pragma once
#include <cmath>  // mengimport library cmath untuk fungsi matematika seperti hypot (jarak Euclidean aman overflow)
#include <string> // mengimport library string agar titik dapat menyimpan nama (seperti A, B, C, dst)

using namespace std;

class Point {
private:
    string name; // deklarasi variable name bertipe string untuk menyimpan nama titik/kota (A, B, C, dst)
    double x;    // deklarasi variable x bertipe data double untuk koordinat sumbu X
    double y;    // deklarasi variable y bertipe data double untuk koordinat sumbu Y

public:
    // Constructor default (jika titik dibuat tanpa parameter)
    Point() {
        name = ""; // menginisialisasi nama titik kosong secara default
        x = 0.0;   // menginisialisasi nilai awal x menjadi 0.0 secara default
        y = 0.0;   // menginisialisasi nilai awal y menjadi 0.0 secara default
    }

    // Constructor berparameter (untuk mengisi nama titik serta nilai x dan y saat dipanggil)
    Point(string pname, double px, double py) {
        name = pname; // memasukkan nilai parameter pname ke variable name
        x = px;       // memasukkan nilai parameter px ke variable x
        y = py;       // memasukkan nilai parameter py ke variable y
    }

    // Fungsi untuk mengambil nama titik
    string getName() const {
        return name; // mengembalikan nilai variable name
    }

    // Fungsi untuk mengambil nilai X
    double getX() const {
        return x; // mengembalikan nilai variable x
    }

    // Fungsi untuk mengambil nilai Y
    double getY() const {
        return y; // mengembalikan nilai variable y
    }
}; // akhir class Point

// Fungsi global 'dist' untuk menghitung jarak antara dua titik (Euclidean Distance)
inline double dist(const Point& p1, const Point& p2) {
    // menggunakan std::hypot agar tidak terjadi overflow saat selisih koordinat sangat besar
    return hypot(p1.getX() - p2.getX(), p1.getY() - p2.getY());
}