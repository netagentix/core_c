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

// KERNEL SPATIOZ OS — temporal_dynamics.c
// Module 3: Temporal Dynamics (Branchless, C)
#include "corekernel.c"

typedef struct { double* buf; int size; int head; } RollingBuffer;

void rolling_push(RollingBuffer* rb, double value) {
    rb->buf[rb->head] = value;
    rb->head = (rb->head + 1) % rb->size;
}

double rolling_avg(RollingBuffer* rb, int n) {
    int len = n < rb->size ? n : rb->size;
    double sum = 0;
    for (int i = 0; i < len; i++) {
        int idx = ((rb->head - 1 - i) % rb->size + rb->size) % rb->size;
        sum += rb->buf[idx];
    }
    return sum / (len + 1e-12);
}

double integrate_signal(double prev, double input, double decay, double max_val) {
    double next = prev * decay + input;
    return next < 0.0 ? 0.0 : (next > max_val ? max_val : next);
}

double sample_oscillator(double freq, double phase, double amplitude, int iteration) {
    return amplitude * (0.5 + 0.5 * sin(2.0 * M_PI * freq * iteration + phase));
}

double pulse_oscillator(double freq, double amplitude, int iteration) {
    int period = (int)(1.0 / (freq + 1e-12));
    double fire = (period > 0 && iteration % period == 0) ? 1.0 : 0.0;
    return fire * amplitude;
}

double compute_temporal_lock(double target_phase, double current_phase, double temperature) {
    double diff = fabs(target_phase - current_phase);
    double violation = fmin(1.0, diff / M_PI);
    return 1.0 - (temperature * violation);
}
