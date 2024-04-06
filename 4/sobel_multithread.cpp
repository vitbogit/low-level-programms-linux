#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <pthread.h>
#include <stdio.h>
#include <time.h>
#include <chrono>


#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION

#include "stb/stb_image.h"
#include "stb/stb_image_write.h"

using namespace std;

struct ThreadData {
    unsigned char* image;
    unsigned char* output;
    int width;
    int height;
    int startRow;
    int endRow;
};

unsigned char* loadPNGImage(const char* filename, int& width, int& height) {
    int channels;
    unsigned char* image = stbi_load(filename, &width, &height, &channels, STBI_rgb);

    if (image == nullptr) {
        std::cerr << "Error loading image file: " << stbi_failure_reason() << std::endl;
        exit(1);
    }

    // cout << "length of stbi_load() result:" << strlen((const char*)image) << endl;
    //  cout << "bogdan test:" << image[100] << endl;

    // cout << "length of stbi_load() result:" << sizeof(image) << endl;

    return image;
}

void* sobelFilterThread(void* arg) {
    ThreadData* data = (ThreadData*)arg;

    int sobelX[] = {-1, 0, 1, -2, 0, 2, -1, 0, 1};
    int sobelY[] = {-1, -2, -1, 0, 0, 0, 1, 2, 1};

    for (int y = data->startRow + 1; y < data->endRow - 1; y++) {
        for (int x = 1; x < data->width - 2; x++) {
            int sumX = 0;
            int sumY = 0;

            for (int j = -1; j <= 1; j++) {
                for (int i = -1; i <= 1; i++) {
                    int index = ((y + j) * data->width + (x + i)) * 3;
                    sumX += data->image[index] * sobelX[(j + 1) * 3 + (i + 1)];
                    sumY += data->image[index] * sobelY[(j + 1) * 3 + (i + 1)];
                }
            }

            data->output[(y * data->width + x)] = sqrt(sumX * sumX + sumY * sumY);
        }
    }

    pthread_exit(NULL);
}


void sobelFilter(unsigned char* image, int width, int height, int numThreads) {
    auto begin = std::chrono::steady_clock::now();
    unsigned char* output = new unsigned char[width * height];

    pthread_t threads[numThreads];
    ThreadData threadData[numThreads];

    for (int i = 0; i < numThreads; i++) {
        threadData[i].image = image;
        threadData[i].output = output;
        threadData[i].width = width;
        threadData[i].height = height;
        threadData[i].startRow = i * (height / numThreads);
        threadData[i].endRow = (i + 1) * (height / numThreads);

        pthread_create(&threads[i], NULL, sobelFilterThread, &threadData[i]);
    }

    for (int i = 0; i < numThreads; i++) {
        pthread_join(threads[i], NULL);
    }

    stbi_write_png("output.png", width, height, 1, output, width);

    delete[] output;
     auto end = std::chrono::steady_clock::now();

    auto elapsed_ms = std::chrono::duration_cast<std::chrono::milliseconds> (end - begin);

    cout << "time:" << elapsed_ms.count() << endl;
}

int main(int argc, char *argv[]) {
    
    
    if (argc < 3) {
        cout << "Provide image path and threads count!" << endl;
        return -1;
    }
    auto path = argv[1];
    auto threads_count = atoi(argv[2]);

    int width, height;
    unsigned char* imageMatrix = loadPNGImage(path, width, height);

    cout << "Image size: " << width << "x" << height << endl;
    // stbi_write_png("output.png", width, height, 3, imageMatrix, width);
    //return -1;
    sobelFilter(imageMatrix, width, height, threads_count);

    stbi_image_free(imageMatrix);
    
   

    return 0;
}