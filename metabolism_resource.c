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

// KERNEL AGENTIX OS — metabolism_resource.c
// Module 8: Metabolism & Resource (Branchless, C)
#include "corekernel.c"

typedef struct {
    double charge;
    double capacity;
    double regen;
} Battery;

double charge_valence(Battery* b) {
    return b->charge / (b->capacity + 1e-12);
}

double consume_energy(Battery* b, double amount) {
    double actual = amount < b->charge ? amount : b->charge;
    b->charge = b->charge - actual;
    b->charge = b->charge < 0.0 ? 0.0 : b->charge;
    return actual;
}

void regenerate_battery(Battery* b) {
    b->charge = b->charge + b->regen;
    b->charge = b->charge > b->capacity ? b->capacity : b->charge;
}

double energy_gate(Battery* b, double threshold, double sharpness) {
    double v = charge_valence(b);
    return 1.0 / (1.0 + exp(-sharpness * (v - threshold)));
}

double afford_action(Battery* b, double cost, double desired, double threshold, double sharpness) {
    double gate = energy_gate(b, threshold, sharpness);
    double actual = desired * gate;
    consume_energy(b, cost * actual);
    return actual;
}
