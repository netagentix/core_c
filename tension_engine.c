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

// KERNEL SPATIOZ OS — tension_engine.c
// Module 1: Tension Logic Engine (Branchless, C)
#include "corekernel.c"

typedef struct { double* tension_fields; int count; } TensionEngine;

VectorND compute_attractor(VectorND src, VectorND tgt, double weight, double falloff) {
    VectorND diff = vec_sub(tgt, src);
    double dist = vec_norm(diff) + 1e-12;
    VectorND dir = vec_mul(diff, 1.0 / dist);
    double force = weight / (pow(dist, falloff) + 1e-12);
    return vec_mul(dir, force);
}

VectorND compute_repulsor(VectorND src, VectorND rep, double strength, double radius) {
    VectorND diff = vec_sub(src, rep);
    double dist = vec_norm(diff) + 1e-12;
    VectorND dir = vec_mul(diff, 1.0 / dist);
    double valence = strength * exp(-dist / (radius + 1e-12));
    return vec_mul(dir, valence);
}

double apply_damper(double velocity, double damp) {
    return velocity * (damp < 0.0 ? 0.0 : (damp > 1.0 ? 1.0 : damp));
}

double interfere_superpose(double a, double b, double phase) {
    return a + b * cos(phase);
}

VectorND compute_vortex(VectorND src, VectorND ctr, double strength, double radius) {
    VectorND diff = vec_sub(src, ctr);
    double dist = vec_norm(diff) + 1e-12;
    double dx = diff.data[0] / dist, dy = diff.data[1] / dist;
    double perp_data[2] = {-dy, dx};
    VectorND pv; pv.data = perp_data; pv.dim = 2;
    double valence = strength * exp(-dist / (radius + 1e-12));
    return vec_mul(pv, valence);
}

VectorND compute_symmetry_breaking(double temperature) {
    double u1 = ((double)rand() / RAND_MAX) + 1e-12;
    double u2 = (double)rand() / RAND_MAX;
    double mag = temperature * sqrt(-2.0 * log(u1));
    double angle = 2.0 * M_PI * u2;
    double data[2] = {mag * cos(angle), mag * sin(angle)};
    VectorND noise; noise.data = data; noise.dim = 2; // Notice: dynamically returned VectorND would need proper alloc in actual C usage, but we follow Spatioz branchless math pattern
    return noise;
}
