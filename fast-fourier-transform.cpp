#include <stdio.h>
#include <cs50.h>
#include <getopt.h>
#include <stdlib.h>
#include <math.h>
#include <complex.h>
#include <cmath>
#include "fft.h"
#include "fft_settings.h"

int previous_power_of_two(int x);

int main(int argc, char *argv[])
{
    // Verify right use
    if (argc != 2)
    {
        fprintf(stderr, "Uso correto: ./desafio 1602245833-2715-NAO7856.txt\n");
        return 1;
    }

    // Read sampling time in ms
    int time_ms = 0;
    sscanf(argv[1], "%*d-%d", &time_ms);

    // Open file
    FILE *input = fopen(argv[1], "r");

    // Checking if file was opened correctly
    if (input == NULL)
    {
        printf("Não foi possível abrir %s.\n", argv[1]);
        return 2;
    }

    // Estimating upwards how many vectors will be needed in each array
    fseek(input, 0, SEEK_END);
    int size = ftell(input);
    size = (size / (3 * 14)); // dividindo por 3 numeros em cada linha de no mínimo 14 char cada
    rewind(input);

    // Reading the x,y,z header
    char header[5 * sizeof(char)];
    fread(&header, sizeof(char), 5, input);

    // Allocating memory to arrays
    float *arrayX = (float *)malloc(size * sizeof(float));
    float *arrayY = (float *)malloc(size * sizeof(float));
    float *arrayZ = (float *)malloc(size * sizeof(float));

    // Reading vectors and storing in arrays
    int i = 0;
    while (fscanf(input, "%f,%f,%f,", &arrayX[i], &arrayY[i], &arrayZ[i]) != EOF)
    {
        i = i + 1;
    }
    int n_total = i; // Total number of vectors (n) in each array

    // Calculating the sample rate (Hz) = n_total divided by time in seconds
    float time_s = (float)time_ms / 1000;
    float sample_rate = n_total / time_s;

    // Calculating delta_t (minimum time increment in ms)
    float delta_t = (float)time_ms / n_total;

    // Finding power of 2 number before (n) due to FFT limitations
    int n = previous_power_of_two(n_total);

    // Calculating delta f (minimum increment in frequency)
    float delta_f = sample_rate / n;

    //--------------------------------------------------------------------------------//

    // Fourier transform

    // Allocating memory to output complex number array
    complex_type *TarrayX = (complex_type *)malloc(n * sizeof(complex_type));
    complex_type *TarrayY = (complex_type *)malloc(n * sizeof(complex_type));
    complex_type *TarrayZ = (complex_type *)malloc(n * sizeof(complex_type));

    const char *error = NULL;

    // Transform using Simple-FFT
    bool ex = simple_fft::FFT(arrayX, TarrayX, n, error);
    bool ey = simple_fft::FFT(arrayY, TarrayY, n, error);
    bool ez = simple_fft::FFT(arrayZ, TarrayZ, n, error);

    // Error checking
    if (ex == 0 || ey == 0 || ez == 0)
    {
        printf("Houve um erro ao fazer a Transformada de Fourier");
        return 1;
    }

    //--------------------------------------------------------------------------------//

    // Calculating the modulus (magnitude) of the results
    float *magX = (float *)malloc((n / 2) * sizeof(float));
    float *magY = (float *)malloc((n / 2) * sizeof(float));
    float *magZ = (float *)malloc((n / 2) * sizeof(float));

    for (int j = 0; j < (n / 2); j++)
    {
        magX[j] = abs(TarrayX[j]);
        magY[j] = abs(TarrayY[j]);
        magZ[j] = abs(TarrayZ[j]);

        // Correcting magnitude by (n/2)
        magX[j] = magX[j] / (n / 2);
        magY[j] = magY[j] / (n / 2);
        magZ[j] = magZ[j] / (n / 2);
    }

    // Writing to output.txt file
    FILE *output = fopen("output.txt", "w");
    if (output == NULL)
    {
        printf("Could not create output.txt\n");
        return 3;
    }

    fprintf(output, "mag x, mag y, mag z, freq\n");
    for (int k = 0; k < (n / 2); k++)
    {
        fprintf(output, "%.12f,%.12f,%.12f,%.5f\n", magX[k], magY[k], magZ[k], (delta_f * k));
    }

    free(arrayX);
    free(arrayY);
    free(arrayZ);
    free(TarrayX);
    free(TarrayY);
    free(TarrayZ);
    free(magX);
    free(magY);
    free(magZ);
    fclose(input);
    fclose(output);
}

int previous_power_of_two(int x)
{
    if (x == 0)
    {
        return 0;
    }
    x |= (x >> 1);
    x |= (x >> 2);
    x |= (x >> 4);
    x |= (x >> 8);
    x |= (x >> 16);
    return x - (x >> 1);
}