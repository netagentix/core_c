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

// KERNEL SPATIOZ OS — communication.c
// Module 6: Communication Interface (Branchless, C)
#include "corekernel.c"

typedef struct {
    int    sender_id;
    double valence;
    double pos[2];
    int    born;
    int    has_pos;
} ValencePacket;

typedef struct {
    ValencePacket* packets;
    int count;
    int capacity;
    int max_age;
} ValenceBus;

void bus_broadcast(ValenceBus* bus, int sender_id, double valence, double* pos, int iteration) {
    double v = valence < -1.0 ? -1.0 : (valence > 1.0 ? 1.0 : valence);
    ValencePacket p;
    p.sender_id = sender_id; p.valence = v; p.born = iteration;
    p.has_pos = (pos != NULL);
    if (pos) { p.pos[0] = pos[0]; p.pos[1] = pos[1]; }
    bus->packets[bus->count++] = p;
}

double aggregate_channel(ValenceBus* bus, int iteration) {
    double total_w = 0.0, sum_val = 0.0;
    for (int i = 0; i < bus->count; i++) {
        ValencePacket* p = &bus->packets[i];
        int age = iteration - p->born;
        double age_mask = (age <= bus->max_age) ? 1.0 : 0.0;
        double freshness = age_mask * pow(0.97, age);
        total_w += freshness; sum_val += p->valence * freshness;
    }
    return sum_val / (total_w + 1e-12);
}
