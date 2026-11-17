#include <iostream>
#include <vector>
#include <chrono>

const size_t NUM_PARTICLES = 5'000'0000;

struct ParticleAoS {
    float x, y, z;
    int id;
    char tag[32]; 
};

struct ParticleSoA {
    std::vector<float> x;
    std::vector<float> y;
    std::vector<float> z;
    std::vector<int> id;
};

int main() {
    // 1. Benchmark AoS
    std::vector<ParticleAoS> aos(NUM_PARTICLES);
    for (size_t i = 0; i < NUM_PARTICLES; ++i) aos[i].x = 1.0f;

    auto start_aos = std::chrono::high_resolution_clock::now();
    float sum_aos = 0;
    for (size_t i = 0; i < NUM_PARTICLES; ++i) {
        sum_aos += aos[i].x;
    }
    auto end_aos = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> aos_time = end_aos - start_aos;

    // 2. Benchmark SoA
    ParticleSoA soa;
    soa.x.resize(NUM_PARTICLES, 1.0f);

    auto start_soa = std::chrono::high_resolution_clock::now();
    float sum_soa = 0;
    for (size_t i = 0; i < NUM_PARTICLES; ++i) {
        sum_soa += soa.x[i];
    }
    auto end_soa = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> soa_time = end_soa - start_soa;

    std::cout << "AoS Time: " << aos_time.count() << " ms (Sum: " << sum_aos << ")" << std::endl;
    std::cout << "SoA Time: " << soa_time.count() << " ms (Sum: " << sum_soa << ")" << std::endl;
    std::cout << "Throughput Tăng: " << (aos_time.count() / soa_time.count()) << "x" << std::endl;
    return 0;
}

/*

SoA nhanh hon tam 1.1 - 1.4 lan voi N ~ 5m = 50m

*/