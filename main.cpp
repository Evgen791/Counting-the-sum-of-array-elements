 Реализация подсчёта суммы элементов массива несколькими потоками :


#include <iostream>
#include <thread>
#include <vector>
#include <numeric>
#include <chrono>

// Класс для подсчёта суммы элементов массива
class ArraySumCalculator {
public:
    ArraySumCalculator(const std::vector<int>& array, int numThreads)
        : array(array), numThreads(numThreads) {}

    // Метод для запуска подсчёта суммы в нескольких потоках
    int calculateSum() {
        int arraySize = array.size();
        int chunkSize = arraySize / numThreads;

        std::vector<std::thread> threads(numThreads);
        std::vector<int> results(numThreads);

        // Запускаем потоки для подсчёта суммы в каждой части массива
        for (int i = 0; i < numThreads; ++i) {
            threads[i] = std::thread(&ArraySumCalculator::partialSum, this, i * chunkSize, (i + 1) * chunkSize, std::ref(results[i]));
        }

        // Дожидаемся завершения всех потоков
        for (auto& thread : threads) {
            thread.join();
        }

        // Вычисляем общую сумму
        int sum = std::accumulate(results.begin(), results.end(), 0);

        return sum;
    }

private:
    const std::vector<int>& array;
    int numThreads;

    // Метод для подсчёта суммы в части массива
    void partialSum(int start, int end, int& result) {
        result = std::accumulate(array.begin() + start, array.begin() + end, 0);
    }
};

int main() {
    int N = 1000000; // Размер массива
    int M = 4; // Количество частей

    std::vector<int> array(N);
    std::iota(array.begin(), array.end(), 1); // Заполняем массив числами от 1 до N

    ArraySumCalculator calculator(array, M);

    // Засекаем время для подсчёта суммы с использованием нескольких потоков
    auto startTime = std::chrono::high_resolution_clock::now();
    int sumWithThreads = calculator.calculateSum();
    auto endTime = std::chrono::high_resolution_clock::now();
    auto durationWithThreads = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime);

    std::cout << "Sum with threads: " << sumWithThreads << std::endl;
    std::cout << "Time with threads: " << durationWithThreads.count() << " milliseconds" << std::endl;
    
    // Засекаем время для подсчёта суммы в основном потоке
    startTime = std::chrono::high_resolution_clock::now();
    int sumWithoutThreads = std::accumulate(array.begin(), array.end(), 0);
    endTime = std::chrono::high_resolution_clock::now();
    auto durationWithoutThreads = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime);

    std::cout << "Sum without threads: " << sumWithoutThreads << std::endl;
    std::cout << "Time without threads: " << durationWithoutThreads.count() << " milliseconds" << std::endl;

    return 0;
}
