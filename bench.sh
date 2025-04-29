#!/bin/bash

# Configuration
PROJECT_DIR="/Users/viro/Projects/adt_test_lib"
BUILD_DIR="${PROJECT_DIR}/build"
REPLICATIONS=${1:-5}
CMAKE_GENERATOR="Ninja"
COMPILER="clang++"

# Create arrays to store tmes
declare -a normal_times
declare -a constexpr_times

echo "Cleaning build directory..."
rm -rf "${BUILD_DIR}"
mkdir -p "${BUILD_DIR}"

configure_and_build() {
  local build_type=$1
  local use_constexpr_flag=$2
  local start_time
  local end_time
  local duration

  echo "Configuring and building ${build_type} version..."
  cd "${BUILD_DIR}" || exit 1

  CXX=${COMPILER} cmake -G "${CMAKE_GENERATOR}" "${use_constexpr_flag}" ..

  start_time=$(date +%s.%N)
  cmake --build .
  end_time=$(date +%s.%N)

  # Calculate duration in milliseconds
  duration=$(echo "($end_time - $start_time) * 1000" | bc)
  echo "${build_type} build took ${duration} ms"

  echo "${duration}"
}

echo "Running ${REPLICATIONS} replications of each build..."

for ((i=1; i<=${REPLICATIONS}; i++)); do
  echo "Replication $i/${REPLICATIONS}"

  # Configure and build normal version
  normal_time=$(configure_and_build "normal" "-DUSE_CONSTEXPR=OFF")
  normal_times[i]=$(echo "$normal_time" | tail -n 1)

  # Configure and build constexpr version
  constexpr_time=$(configure_and_build "constexpr" "-DUSE_CONSTEXPR=ON")
  constexpr_times[i]=$(echo "$constexpr_time" | tail -n 1)
done

# Calculate statistics
normal_sum=0
constexpr_sum=0

for ((i=1; i<=${REPLICATIONS}; i++)); do
  # Handle potential empty values if bc fails
  [[ -z "${normal_times[$i]}" ]] && normal_times[$i]=0
  [[ -z "${constexpr_times[$i]}" ]] && constexpr_times[$i]=0
  normal_sum=$(echo "$normal_sum + ${normal_times[$i]}" | bc)
  constexpr_sum=$(echo "$constexpr_sum + ${constexpr_times[$i]}" | bc)
done

# Avoid division by zero if REPLICATIONS is 0
if [[ ${REPLICATIONS} -gt 0 ]]; then
  normal_avg=$(echo "scale=2; $normal_sum / ${REPLICATIONS}" | bc)
  constexpr_avg=$(echo "scale=2; $constexpr_sum / ${REPLICATIONS}" | bc)
  if (( $(echo "$normal_avg > 0" | bc -l) )); then
    percent_diff=$(echo "scale=2; ($normal_avg - $constexpr_avg) * 100 / $normal_avg" | bc)
  else
    percent_diff="N/A (Normal avg is 0)"
  fi
else
  normal_avg=0
  constexpr_avg=0
  percent_diff="N/A (0 replications)"
fi


echo "===== RESULTS ====="
echo "Normal build average time: ${normal_avg} ms"
echo "Constexpr build average time: ${constexpr_avg} ms"
echo "Improvement with constexpr: ${percent_diff}%"

# Save results to CSV
echo "Replication,Normal (ms),Constexpr (ms)" > build_times.csv
for ((i=1; i<=${REPLICATIONS}; i++)); do
  echo "$i,${normal_times[$i]:-0},${constexpr_times[$i]:-0}" >> build_times.csv
done

echo "Results saved to build_times.csv"