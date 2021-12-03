# fast-fourier-transform
This is an implementation of the Simple-FFT to transform X, Y and Z aceleration values (in m/s, recorded on an accelerometer and stored in a CSV file) to the corresponding magnitudes in the frequency scale. 

The libraries used were: stdio.h, cs50.h, getopt.h, stdlib.h, math.h, complex.h, cmath, and the local libraries included in the files "fft.h", "fft_settings.h". The latter comes from the Simple-FFT library, which is header-only, with no need for build, available at: https://github.com/d1vanov/Simple-FFT.
The code (challenge.cpp) was thought to be useful not only for the proposed example, but for other files with other amounts of samples collected and in different amounts of time. In the code, written in C++, I inserted comments about each part of the reasoning to make it easier to understand. The code follows, in summary, the following order:
- Check the correct use of the command to execute: ./challenge filename.txt. It expects a filename in the following format: START-TIME_NUMBER-OF-MS_NAME-OF-ACCELEROMETER
- Read the sampled time in [ms] from the file name and check if it is opened correctly.
- Read header and save in the "header" string, then allocate memory for the vectors based on the total file size, knowing that it is an upward approximation due to the header and the commas, after reading the vectors a counter indicates the actual amount of vectors (n).
- Calculate the variables: sample_rate in [Hz], delta_t in [ms], delta_f in [Hz] and also find the power of 2 number before (n). The truncation of information is necessary for the use of FTT, it could have been chosen to truncate to a value after the last value on the list, and complete the missing values with zero (zero-padding).
- The Fourier Transform is performed using the functions included in Simple-FFT, and an error check is performed. Magnitude is calculated through the modulus of the complex number up to (n/2) by the Nyquist Sampling Theorem (BRIGHAM, 1988, p.84) any frequency above half the sampling frequency (n) is a mirrored repetition of the frequencies above and cannot be considered.
- The magnitude is corrected by dividing by the sampling frequency and multiplying by two because the energy is “divided” across the entire spectrum, including the half that is not useful. Thus, we get magnitudes that equal the real magnitudes for each frequency.
- Then the data is written in an “output.txt” file (in CSV format) and finally the memory is freed.
