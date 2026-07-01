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

// KERNEL SPATIOZ OS — memory_condensation.c
// Module 9: Memory Condensation (Branchless, C)
#include "corekernel.c"

typedef struct {
    double* vector;
    double  weight;
    int     dims;
    int     born;
} Engram;

typedef struct {
    Engram* engrams;
    int count;
    int capacity;
} EngramStore;

double cosine_sim(double* a, double* b, int dims) {
    double dot = 0, na = 0, nb = 0;
    for (int i = 0; i < dims; i++) { dot += a[i]*b[i]; na += a[i]*a[i]; nb += b[i]*b[i]; }
    return dot / (sqrt(na) * sqrt(nb) + 1e-12);
}

void engram_decay(EngramStore* es, double decay_rate) {
    for (int i = 0; i < es->count; i++) es->engrams[i].weight *= decay_rate;
}

int engram_best_match(EngramStore* es, double* query, int dims) {
    double best = -1e12; int winner = 0;
    for (int i = 0; i < es->count; i++) {
        double sim = cosine_sim(query, es->engrams[i].vector, dims) * es->engrams[i].weight;
        double beat = (double)(sim > best);
        best = best * (1.0 - beat) + sim * beat;
        winner = (int)((double)winner * (1.0 - beat) + (double)i * beat);
    }
    return winner;
}
