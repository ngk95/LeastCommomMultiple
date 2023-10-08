#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <climits>

// Calculates the smallest LCM for any combination
// of `reqNumbers` values from the `numbers` list.
//
// Overall Time Complexity: O(n^k * sqrt(m))
//      Where
//      n = size of the `numbers` array
//      k = the count of the numbers in each combination (`reqNumbers`),
//      m = maximum number in `numbers`.

// Function to check if a given number is prime
bool isPrime(long long int n) {
    if (n <= 1) return false;
    for (long long int i = 2; i <= sqrt(n); i++) {
        if (n % i == 0) return false;
    }
    return true;
}

// Function to return the prime factors of a given number
std::vector<int> primeFactors(long long int n) {
    std::vector<int> factors;
    for (long long int i = 2; i <= sqrt(n); i++) {
        while (n % i == 0) {
            factors.push_back(i);
            n /= i;
        }
    }
    if (n > 1) {
        factors.push_back(n);
    }
    return factors;
}

// Function to generate a list of prime numbers up to a given number n
std::vector<long long int> getPrimesUpToN(long long int n) {
    std::vector<long long int> primes;
    for (long long int i = 2; i <= n; i++) {
        if (isPrime(i)) {
            primes.push_back(i);
        }
    }
    return primes;
}

// Recursive function to generate combinations
void generateCombinations(std::vector<long long int>& nums, int depth, int start, int end, int reqDepth, std::vector<int>& currentComb, std::vector<std::vector<int>>& results) {
    if (depth == reqDepth) {
        results.push_back(currentComb);
        return;
    }

    for (int i = start; i <= end; i++) {
        currentComb.push_back(i);
        generateCombinations(nums, depth + 1, i + 1, end, reqDepth, currentComb, results);
        currentComb.pop_back();
    }
}

int main() {
    // Inputs - change these 2 values for different outcome
    std::vector<long long int> numbers = {1247,1455,1423,7845,9676};
    int reqNumbers = 3;  // change this value based on how many numbers you want in the combination

    long long int max_num = *std::max_element(numbers.begin(), numbers.end());
    std::vector<long long int> primes = getPrimesUpToN(sqrt(max_num));
    int num_primes = primes.size();

    // Create a 2D vector to store counts of prime factors for each number
    std::vector<std::vector<int>> primeCounts(numbers.size(), std::vector<int>(num_primes, 0));

    // Count prime factors for each number
    for (int i = 0; i < numbers.size(); i++) {
        std::vector<int> factors = primeFactors(numbers[i]);
        for (int prime : factors) {
            int index = std::find(primes.begin(), primes.end(), prime) - primes.begin();
            if (index >= 0 && index < primes.size()) {
                primeCounts[i][index]++;
            }
        }
    }

    // Generate combinations
    std::vector<std::vector<int>> combinations;
    std::vector<int> tempComb;
    generateCombinations(numbers, 0, 0, numbers.size() - 1, reqNumbers, tempComb, combinations);

    // Calculate the smallest LCM
    long long int smallestLCM = LLONG_MAX;
    std::vector<int> bestCombination;
    std::vector<int> bestPowers;

    for (const auto& combination : combinations) {
        long long int candidate = 1;
        std::vector<int> powers(num_primes, 0);
        for (int p = 0; p < num_primes; p++) {
            int maxPower = 0;
            for (int idx : combination) {
                maxPower = std::max(maxPower, primeCounts[idx][p]);
            }
            powers[p] = maxPower;
            for (int powCount = 0; powCount < powers[p]; powCount++) {
                candidate *= primes[p];
            }
        }

        int divisibleCount = 0;
        for (auto num : numbers) {
            if (candidate % num == 0) divisibleCount++;
        }

        if (divisibleCount >= reqNumbers && candidate < smallestLCM) {
            smallestLCM = candidate;
            bestCombination = combination;
            bestPowers = powers;
        }
    }

    // Output the results
    std::cout << "Smallest LCM: " << smallestLCM << std::endl;
    std::cout << "Combination: ";
    for (int idx : bestCombination) {
        std::cout << numbers[idx] << " ";
    }
    std::cout << std::endl;
    std::cout << "The powers of the prime factors needed to calculate the smallest LCM (starting 2, 3, 5, 7, 11, etc.):\n";
    for (int power : bestPowers) {
        std::cout << power << " ";
    }
    std::cout << std::endl;

    return 0;
}
