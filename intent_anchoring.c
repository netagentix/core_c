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

// KERNEL AGENTIX OS — intent_anchoring.c
// Module 10: Intent Anchoring (Branchless, C)
#include "corekernel.c"

typedef struct {
    double target[2];
    double weight;
    double decay_rate;
    int    born;
    int    ttl;
    int    has_target;
} IntentAnchor;

VectorND intent_force(IntentAnchor* anchors, int n, VectorND agent_pos, int iteration) {
    VectorND net = vec_zero(agent_pos.dim);
    for (int i = 0; i < n; i++) {
        IntentAnchor* a = &anchors[i];
        int age = iteration - a->born;
        double alive = (double)(age < a->ttl);
        double valid = alive * (double)(a->weight > 1e-4) * (double)(a->has_target);
        if (valid > 0.5) {
            double tgt_data[2] = {a->target[0], a->target[1]};
            VectorND tvec; tvec.data = tgt_data; tvec.dim = 2;
            VectorND diff = vec_sub(tvec, agent_pos);
            double dist = vec_norm(diff) + 1e-12;
            VectorND dir = vec_mul(diff, 1.0 / dist);
            VectorND contrib = vec_mul(dir, a->weight);
            net = vec_add(net, contrib);
        }
        a->weight *= a->decay_rate;
    }
    return net;
}

VectorND composite_force(IntentAnchor* anchors, int n, double global_blend, double attention, double temperature, double l_spatial, double l_temporal, VectorND agent_pos, int iteration) {
    VectorND local_f = intent_force(anchors, n, agent_pos, iteration);
    VectorND h_base = local_f; // Simplified for C branchless structural demo
    
    double u1 = ((double)rand() / RAND_MAX) + 1e-12;
    double u2 = ((double)rand() / RAND_MAX);
    double mag = temperature * sqrt(-2.0 * log(u1));
    double angle = 2.0 * M_PI * u2;
    double s_data[2] = {mag * cos(angle), mag * sin(angle)};
    VectorND v_stochastic; v_stochastic.data = s_data; v_stochastic.dim = 2;
    
    VectorND term1 = vec_mul(h_base, attention);
    VectorND term2 = vec_mul(v_stochastic, l_spatial);
    return vec_mul(vec_add(term1, term2), l_temporal);
}
