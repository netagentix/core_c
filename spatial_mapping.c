/*
 * ----------------------------------------------------------------------------
 * Agentix OS - Quantum Continuous Engine
 * Copyright (C) 2026 Agentix Project
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

// KERNEL AGENTIX OS — spatial_mapping.c
// Module 2: Spatial Mapping (Branchless, C)
#include "corekernel.c"

double zone_valence_circle(double cx, double cy, double radius, double px, double py, double sharpness) {
    double dx = px - cx, dy = py - cy;
    double dist = sqrt(dx*dx + dy*dy);
    return 1.0 / (1.0 + exp(sharpness * (dist - radius)));
}

double zone_valence_rect(double x1, double y1, double x2, double y2, double px, double py, double sharpness) {
    double dL = px - x1, dR = x2 - px, dT = py - y1, dB = y2 - py;
    double margin = fmin(fmin(dL, dR), fmin(dT, dB));
    double span = ((x2-x1) + (y2-y1)) * 0.25 + 1e-12;
    return 1.0 / (1.0 + exp(-sharpness * margin / span));
}

double compute_density(double px, double py, double* nx, double* ny, int n, double radius) {
    double density = 0;
    for (int i = 0; i < n; i++) {
        double dx = px - nx[i], dy = py - ny[i];
        double dist = sqrt(dx*dx + dy*dy);
        density += exp(-dist / (radius + 1e-12));
    }
    return density;
}

double compute_spatial_lock(VectorND target_pos, VectorND agent_pos, double temperature, double lock_radius) {
    VectorND diff = vec_sub(target_pos, agent_pos);
    double dist = vec_norm(diff);
    double closeness = exp(-dist / (lock_radius + 1e-12));
    return 1.0 - (temperature * closeness);
}
