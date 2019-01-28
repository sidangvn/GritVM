/**************************************************************************************************/
// Test File for Programming Project 3
// Requires the Catch2 header file
// How to compile: g++ -std=c++17 -Wall -I$(CATCH_SINGLE_INCLUDE) (All cpp files)
// Example: Assuming Catch2 is in the same directory as the project and all of your source file are 
// in the directory: 
//    g++ -std=c++17 -Wall *.cpp 
//    or
//    g++ -std=gnu++17 -Wall *.cpp 
/**************************************************************************************************/

#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include <cmath>

#include "GritVM.hpp"

TEST_CASE("Project 3 Test Cases for GritVM") {
  GritVM vm;
  std::vector<long> initialMemory;
  std::vector<long> outputMemory;
  std::vector<long> checkMemory;

  SECTION("GritVM should be in WAITING state when no program is loaded") {
    REQUIRE(vm.run() == WAITING);
  }

  SECTION("GritVM should throw if the file cannot be loaded and be state READY if it is") {
    CHECK_THROWS(vm.load("", initialMemory));
    REQUIRE(vm.load("test.gvm", initialMemory) == READY);
  }

  SECTION("GritVM should run a program if READY and be HALTED when program completes") {
    std::vector<long> initialMemory = { 10 };
    vm.load("test.gvm", initialMemory);
    REQUIRE(vm.run() == HALTED);
  }

  SECTION("GritVM produces proper output for test.gvm") {
    long n1 = (rand() + 1) % 50, n2 = (rand() + 1) % 50;

    initialMemory = { n1 };
    checkMemory = {624, n1, 0};
    vm.load("test.gvm", initialMemory);
    vm.run();
    REQUIRE(vm.getDataMem() == checkMemory);

    vm.reset();
    
    initialMemory = { n2 };
    checkMemory = {624, n2, 0};
    vm.load("test.gvm", initialMemory);
    vm.run();
    REQUIRE(vm.getDataMem() == checkMemory);
  }

  SECTION("GritVM produces proper output for sumn.gvm") {
    long n1 = (rand() + 1) % 50, n2 = (rand() + 1) % 50;
    auto func = [](long n) -> long { 
      long sum = 0; 
      for (int i = 1; i <= n; i++) sum += i; 
      return sum; 
    };

    initialMemory = { n1 };
    checkMemory = {n1, func(n1), n1 + 1};
    vm.load("sumn.gvm", initialMemory);
    vm.run();
    REQUIRE(vm.getDataMem() == checkMemory);

    vm.reset();

    initialMemory = { n2 };
    checkMemory = {n2, func(n2), n2 + 1};
    vm.load("sumn.gvm", initialMemory);
    vm.run();
    REQUIRE(vm.getDataMem() == checkMemory);
  }

  SECTION("GritVM produces proper output for surfarea.gvm") {
    long l = (rand() + 1) % 50, w = (rand() + 1) % 50, h = (rand() + 1) % 50;

    auto func = [](long l, long w, long h) -> long {
      return 2*((l*w) + (h*w) + (l*h)); 
    };

    initialMemory = { l, w, h };
    checkMemory = { func(l,w,h) };
    vm.load("surfarea.gvm", initialMemory);
    vm.run();
    REQUIRE(vm.getDataMem() == checkMemory);

    vm.reset();
    l = (rand() + 1) % 50, w = (rand() + 1) % 50, h = (rand() + 1) % 50;

    initialMemory = { l, w, h };
    checkMemory = { func(l,w,h) };
    vm.load("surfarea.gvm", initialMemory);
    vm.run();
    REQUIRE(vm.getDataMem() == checkMemory);
  }

  SECTION("GritVM produces proper output for altseq.gvm") {
    long n = (rand() + 1) % 10, n1 = (rand() + 1) % 10;

    auto func = [](long n) -> long {
      return std::pow(-2, n) * 3 + 4; 
    };

    initialMemory = { n };
    checkMemory = { n, func(n), n+1 };
    vm.load("altseq.gvm", initialMemory);
    vm.run();
    REQUIRE(vm.getDataMem() == checkMemory);

    vm.reset();

    initialMemory = { n1 };
    checkMemory = { n1, func(n1), n1+1 };
    vm.load("altseq.gvm", initialMemory);
    vm.run();
    REQUIRE(vm.getDataMem() == checkMemory);
  }
}

TEST_CASE("Project 3 Test Cases for GritVM (EXTRA CREDIT!)") {
  GritVM vm;
  std::vector<long> initialMemory;
  std::vector<long> outputMemory;
  std::vector<long> checkMemory;

  SECTION("GritVM produces proper output for toh.gvm (The minimum steps for TOH)") {
    long n = (rand() + 1) % 10, n1 = (rand() + 1) % 10, n2 = (rand() + 1) % 10;

    auto func = [](long n) -> long {
      return std::pow(2, n) - 1; 
    };

    initialMemory = { n };
    checkMemory = { n, func(n) };
    vm.load("toh.gvm", initialMemory);
    vm.run();
    REQUIRE(vm.getDataMem() == checkMemory);
    
    vm.reset();

    initialMemory = { n1 };
    checkMemory = { n1, func(n1) };
    vm.load("toh.gvm", initialMemory);
    vm.run();
    REQUIRE(vm.getDataMem() == checkMemory);

    vm.reset();

    initialMemory = { n2 };
    checkMemory = { n2, func(n2) };
    vm.load("toh.gvm", initialMemory);
    vm.run();
    REQUIRE(vm.getDataMem() == checkMemory);
  }
}