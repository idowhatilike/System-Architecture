#include <array>
#include <chrono>
#include <cstdint>
#include <iostream>
#include <vector>

const int MAXN = 1e5;
const int TIME = 5;

// AoS
struct inf {
    float x, y, z;
    float vx, vy, vz;
    int id;
    bool active;
    char extra_metadata[48];
} a[MAXN];

// SoA
struct Inf {
    std::vector<float> x, y, z;
    std::vector<float> vx, vy, vz;
    std::vector<int> id;
    std::vector<std::uint8_t> active;
    std::vector<std::array<char, 48>> extra_metadata;

    explicit Inf(std::size_t count)
        : x(count), y(count), z(count),
          vx(count), vy(count), vz(count),
          id(count), active(count), extra_metadata(count) {}
};

int main() {
    Inf soa(MAXN);

    for (int i = 0; i < MAXN; ++i) {
        a[i] = {0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, i, true, {}};
        soa.vx[i] = a[i].vx;
        soa.vy[i] = a[i].vy;
        soa.vz[i] = a[i].vz;
        soa.id[i] = a[i].id;
        soa.active[i] = a[i].active;
    }

    const auto start_aos = std::chrono::steady_clock::now();
    for (int iteration = 0; iteration < TIME; ++iteration) {
        for (int i = 0; i < MAXN; ++i) {
            if (!a[i].active) continue;
            a[i].x += a[i].vx;
            a[i].y += a[i].vy;
            a[i].z += a[i].vz;
        }
    }
    const auto end_aos = std::chrono::steady_clock::now();

    const auto start_soa = std::chrono::steady_clock::now();
    for (int iteration = 0; iteration < TIME; ++iteration) {
        for (int i = 0; i < MAXN; ++i) {
            if (!soa.active[i]) continue;
            soa.x[i] += soa.vx[i];
            soa.y[i] += soa.vy[i];
            soa.z[i] += soa.vz[i];
        }
    }
    const auto end_soa = std::chrono::steady_clock::now();

    const double aos_ms =
        std::chrono::duration<double, std::milli>(end_aos - start_aos).count();
    const double soa_ms =
        std::chrono::duration<double, std::milli>(end_soa - start_soa).count();

    double checksum_aos = 0.0;
    double checksum_soa = 0.0;
    for (int i = 0; i < MAXN; ++i) {
        checksum_aos += a[i].x + a[i].y + a[i].z;
        checksum_soa += soa.x[i] + soa.y[i] + soa.z[i];
    }
    std::cout << "AoS time: " << aos_ms << " ms\n"
              << "SoA time: " << soa_ms << " ms\n";
}
