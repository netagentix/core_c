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

// KERNEL SPATIOZ OS — compliance_healing.c
// Module 7: Compliance & Self-Healing (Branchless, C)
#include "corekernel.c"

typedef struct {
    double health;
    double* min_vals;
    double* max_vals;
    int field_count;
} AgentContract;

double compliance_score(AgentContract* ac, double* field_vals) {
    double total = 0.0;
    for (int i = 0; i < ac->field_count; i++) {
        double range = ac->max_vals[i] - ac->min_vals[i] + 1e-12;
        double v = field_vals[i];
        double excess = fmax(0.0, ac->min_vals[i] - v) + fmax(0.0, v - ac->max_vals[i]);
        total += fmax(0.0, 1.0 - excess / range);
    }
    return total / (ac->field_count + 1e-12);
}

double update_health(double current, double compliance, double decay_good, double decay_bad) {
    double is_good = (double)(compliance > 0.5);
    double decay = is_good * decay_good + (1.0 - is_good) * decay_bad;
    double next = current + decay * (compliance - current);
    return next < 0.0 ? 0.0 : (next > 1.0 ? 1.0 : next);
}

double authority_mask(double health, double threshold, double sharpness) {
    return 1.0 / (1.0 + exp(-sharpness * (health - threshold)));
}
