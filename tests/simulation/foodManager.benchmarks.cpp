#include <catch2/catch_test_macros.hpp>
#include <catch2/benchmark/catch_benchmark.hpp>
#include <iostream>

#include "foodManagerFixture.hpp"

namespace AntColony::Test::Simulation
{
    TEST_CASE("FoodManager update performance", "[foodmanager][benchmark]")
    {
        FoodManagerFixture fixture;

        BENCHMARK_ADVANCED("FoodManager update")(Catch::Benchmark::Chronometer meter)
        {
            auto foodManager = fixture.createDefaultManager();

            meter.measure([&](int)
                          { foodManager->update(); });
        };

        BENCHMARK_ADVANCED("FoodManager repeated update [1000]")(Catch::Benchmark::Chronometer meter)
        {
            auto foodManager = fixture.createDefaultManager();

            meter.measure([&](int)
                          { 
                              for (int i = 0; i < 1000; ++i)
                                  foodManager->update();
                              return foodManager->getFoodParticles().size(); });
        };

        BENCHMARK_ADVANCED("FoodManager repeated update [10000]")(Catch::Benchmark::Chronometer meter)
        {
            auto foodManager = fixture.createDefaultManager();

            meter.measure([&](int)
                          { 
                              for (int i = 0; i < 10000; ++i)
                                  foodManager->update();
                              return foodManager->getFoodParticles().size(); });
        };

        BENCHMARK_ADVANCED("FoodManager repeated update [100000]")(Catch::Benchmark::Chronometer meter)
        {
            auto foodManager = fixture.createDefaultManager();

            meter.measure([&](int)
                          { 
                              for (int i = 0; i < 100000; ++i)
                                  foodManager->update();
                              return foodManager->getFoodParticles().size(); });
        };

        BENCHMARK_ADVANCED("FoodManager repeated update [1000000]")(Catch::Benchmark::Chronometer meter)
        {
            auto foodManager = fixture.createDefaultManager();

            meter.measure([&](int)
                          { 
                              for (int i = 0; i < 1000000; ++i)
                                  foodManager->update();
                              return foodManager->getFoodParticles().size(); });
        };

        BENCHMARK_ADVANCED("FoodManager repeated update and remove all [1000]")(Catch::Benchmark::Chronometer meter)
        {
            FoodManagerFixture benchFixture;

            int p;

            meter.measure([&](int)
                          { p = benchFixture.runUpdateForFramesAndRemove(1000); });

            REQUIRE(p == 0);
        };

        BENCHMARK_ADVANCED("FoodManager repeated update and remove all [10000]")(Catch::Benchmark::Chronometer meter)
        {
            FoodManagerFixture benchFixture;

            int p;

            meter.measure([&](int)
                          { p = benchFixture.runUpdateForFramesAndRemove(10000); });

            REQUIRE(p == 0);
        };

        BENCHMARK_ADVANCED("FoodManager repeated update and remove all [100000]")(Catch::Benchmark::Chronometer meter)
        {
            FoodManagerFixture benchFixture;

            int p;

            meter.measure([&](int)
                          { p = benchFixture.runUpdateForFramesAndRemove(100000); });

            REQUIRE(p == 0);
        };

        BENCHMARK_ADVANCED("FoodManager repeated update and remove all [1000000]")(Catch::Benchmark::Chronometer meter)
        {
            FoodManagerFixture benchFixture;

            int p;

            meter.measure([&](int)
                          { p = benchFixture.runUpdateForFramesAndRemove(1000000); });

            REQUIRE(p == 0);
        };
    }
}
