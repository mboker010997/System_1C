#pragma once
#include <string>
#include <iostream>
#include <filesystem>

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

class TonyMachine {
public:
    TonyMachine();
    ~TonyMachine();

    TonyMachine(const std::string& dir1,
                const std::string& dir2) 
        : dir1_(dir1), dir2_(dir2) {}

    std::string GetFirstDirectory();

    std::string GetSecondDirectory();

    void PrintIdentical(double identity);

    void PrintSame();

    void FirstMinusSecond();

    void SecondMinusFirst();

private:
    static std::vector<std::string> GetFilenamesInDirectory(const std::string& dir);

    static bool CheckIdentityFixPercentage(const std::string& first_file,
                                        const std::string& second_file,
                                        double identity);

    static void DirectoryDifference(const std::string& first_dir,
                                    const std::string& second_dir);

    void ReadDirectoriesNames();

private:
    std::string dir1_;
    std::string dir2_;
};
