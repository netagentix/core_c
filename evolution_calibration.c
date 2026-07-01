/*
 * ----------------------------------------------------------------------------
 * Spatioz OS - Quantum Continuous Engine
 * Copyright (C) 2026 Spatioz Project
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published
 * by the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 * ----------------------------------------------------------------------------
 */

// KERNEL SPATIOZ OS — evolution_calibration.c
// Module 5: Evolution & Calibration (Branchless, C)
#include "corekernel.c"
#include <stdlib.h>

double gaussian_noise(double mean, double sigma) {
    double u1 = ((double)rand() / RAND_MAX) + 1e-12;
    double u2 = (double)rand() / RAND_MAX;
    return mean + sigma * sqrt(-2.0 * log(u1)) * cos(2.0 * M_PI * u2);
}

void mutate_genome(double* weights, int len, double mutation_rate, double sigma) {
    for (int i = 0; i < len; i++) {
        double do_mutate = ((double)rand() / RAND_MAX < mutation_rate) ? 1.0 : 0.0;
        weights[i] += do_mutate * gaussian_noise(0, sigma);
    }
}

void crossover_genome(double* ga, double* gb, double* child, int len) {
    for (int i = 0; i < len; i++) {
        double use_a = ((double)rand() / RAND_MAX > 0.5) ? 1.0 : 0.0;
        child[i] = use_a * ga[i] + (1.0 - use_a) * gb[i];
    }
}
