
# Include the sources

INCLUDE_DIRECTORIES(.)


add_executable(test_mixQUBO test_mixQUBO.cpp)
add_executable(etalon etalon.cpp)
add_executable(update_best update_best.cpp)
add_executable(main_student main_student.cpp)
add_executable(hill_climbing hill_climbing.cpp)
add_executable(simulated_annealing simulated_annealing.cpp)
add_executable(continuous_random continuous_random.cpp)
add_executable(continuous_local_search continuous_local_search.cpp)
add_executable(mixed_alternating mixed_alternating.cpp)
add_executable(mixed_simulated_annealing mixed_simulated_annealing.cpp)
add_executable(mixed_random mixed_random.cpp)
add_executable(continuous_simulated_annealing continuous_simulated_annealing.cpp)