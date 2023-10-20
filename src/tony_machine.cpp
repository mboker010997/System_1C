#include "tony_machine.h"
#include <cmath>
#include <iostream>
#include <stdexcept>
#include <string>
#include <fstream>
#include <algorithm>
#include <unordered_set>

#include <sys/types.h>
#include <fcntl.h>

TonyMachine::TonyMachine() {
    ReadDirectoriesNames();
}

TonyMachine::~TonyMachine() = default;

void TonyMachine::ReadDirectoriesNames() {
    std::cout << "Write name of the first directory: ";
    std::cin >> dir1_;
    std::cout << std::endl;

    std::cout << "Write name of the second directory: ";
    std::cin >> dir2_;
    std::cout << std::endl;
}

std::string TonyMachine::GetFirstDirectory() {
    return dir1_;    
}

std::string TonyMachine::GetSecondDirectory() {
    return dir2_;
}

bool TonyMachine::CheckIdentityFixPercentage(const std::string& first_file,
                                            const std::string& second_file,
                                            double identity) {

    std::ifstream first_in(first_file, std::ios::binary);
    std::ifstream second_in(second_file, std::ios::binary);

    std::vector<unsigned char> first(std::istreambuf_iterator<char>(first_in), {});
    std::vector<unsigned char> second(std::istreambuf_iterator<char>(second_in), {});

    size_t first_size = first.size();
    size_t second_size = second.size();
    size_t max_size = std::max(first_size, second_size);
    size_t common_substr_size = ceil(identity * max_size);
    if (common_substr_size == 0) {
        first_in.close();
        second_in.close();
        return true;
    }
    if (common_substr_size > first_size || common_substr_size > second_size) {
        first_in.close();
        second_in.close();
        return false;
    }

    const long long MOD = 1e9 + 7;
    const long long BASE = 307;

    long long cur_hash = 0;
    long long cur_pow = 1;
    for (size_t i = 0; i < common_substr_size; ++i) {
        cur_hash *= BASE;
        cur_hash += first[i] + 1;
        cur_hash %= MOD;
        if (i > 0) {
            cur_pow *= BASE;
            cur_pow %= MOD;
        }
    }

    std::unordered_set<long long> hashes;
    hashes.insert(cur_hash);
    for (size_t i = common_substr_size; i < first_size; ++i) {
        cur_hash -= (cur_pow * (first[i - common_substr_size] + 1)) % MOD;
        cur_hash = (cur_hash + MOD) % MOD;
        cur_hash = (cur_hash * BASE + first[i] + 1) % MOD;
        hashes.insert(cur_hash);
    }

    cur_hash = 0;
    cur_pow = 1;
    for (size_t i = 0; i < common_substr_size; ++i) {
        cur_hash *= BASE;
        cur_hash += second[i] + 1;
        cur_hash %= MOD;
        if (i > 0) {
            cur_pow *= BASE;
            cur_pow %= MOD;
        }
    }
    if (hashes.count(cur_hash)) {
        first_in.close();
        second_in.close();
        return true;
    }

    for (size_t i = common_substr_size; i < second_size; ++i) {
        cur_hash -= (cur_pow * (second[i - common_substr_size] + 1)) % MOD;
        cur_hash = (cur_hash + MOD) % MOD;
        cur_hash = (cur_hash * BASE + second[i] + 1) % MOD;
        if (hashes.count(cur_hash)) {
            first_in.close();
            second_in.close();
            return true;
        }
    }

    first_in.close();
    second_in.close();
    return false;
}

void TonyMachine::PrintIdentical(double identity) {
    std::vector<std::string> first_files = GetFilenamesInDirectory(dir1_);
    std::vector<std::string> second_files = GetFilenamesInDirectory(dir2_);
    for (auto first_file : first_files) {
        for (auto second_file : second_files) {
            if (CheckIdentityFixPercentage(first_file, second_file, identity)) {
                std::cout << first_file 
                        << " - " << second_file << std::endl;
            }
        }
    }
}

void TonyMachine::PrintSame() {
    std::vector<std::string> first_files = GetFilenamesInDirectory(dir1_);
    std::vector<std::string> second_files = GetFilenamesInDirectory(dir2_);

    for (auto first_file : first_files) {
        for (auto second_file : second_files) {
            const double EPS = 1e-5;
            double left = 0;
            double right = 100;
            while (right - left > EPS) {
                double mid = (left + right) / 2;
                if (CheckIdentityFixPercentage(first_file, second_file, mid)) {
                    left = mid;
                } else {
                    right = mid;
                }
            }
            int percentage = round(left * 100);
            std::cout << first_file 
                        << " - " << second_file
                        << " - " << percentage << std::endl;
        }
    }
}

void TonyMachine::DirectoryDifference(const std::string& first_dir,
                                    const std::string& second_dir) {
    std::vector<std::string> first_files = GetFilenamesInDirectory(first_dir);
    std::vector<std::string> second_files = GetFilenamesInDirectory(second_dir);
    for (auto first_file : first_files) {
        bool ok = true;
        for (auto second_file : second_files) {
            if (CheckIdentityFixPercentage(first_file, second_file, 1)) {
                ok = false;
                break;
            }
        }
        if (ok) {
            std::cout << first_file << std::endl;
        }
    }
}

void TonyMachine::FirstMinusSecond() {
    DirectoryDifference(dir1_, dir2_);
}

void TonyMachine::SecondMinusFirst() {
    DirectoryDifference(dir2_, dir1_);
}

std::vector<std::string> TonyMachine::GetFilenamesInDirectory(const std::string& dir) {
    std::vector<std::string> res;
    for (const auto& file : std::filesystem::directory_iterator(dir)) {
        res.push_back(file.path().c_str());
    }
    return std::move(res);
}
