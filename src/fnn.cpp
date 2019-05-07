#include "fnn.hpp"
#include <stdio.h>
#include <stdlib.h>
#include <cassert>
#include <cmath>
#include <iostream>

#define RAND_BETWEEN(a, b) ((b - a) * ((float)rand() / RAND_MAX) + a)
#define SIGMOID(x) (1.0 / (1 + pow(M_E, -x)))

int fnn_print(FNN *fnn) {
    assert(fnn);

    std::cout << "Num layers: " << fnn->num_layers << std::endl;
    std::cout << "Address: " << fnn << std::endl;

    std::cout << "Layer sizes:" << std::endl;
    for (int i = 0; i < fnn->num_layers; ++i) {
        std::cout << " " << fnn->layer_sizes[i] << std::endl;
    }
    std::cout << std::endl;
    std::cout << std::endl;
    for (int x = 0; x < fnn->num_layers; ++x) {
        std::cout << "Layer " << x << std::endl;
        for (int y = 0; y < fnn->layer_sizes[x]; ++y) {
            std::cout << fnn->neurons[x][y].in << " " << fnn->neurons[x][y].out
                      << std::endl;
            if (x > 0) {
                for (int w = 0; w < fnn->layer_sizes[x - 1]; ++w) {
                    std::cout << fnn->neurons[x][y].weights[w] << " ";
                }
                std::cout << std::endl;
            }
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;

    return 0;
}

int fnn_init(FNN *fnn, int num_layers, int *layer_sizes) {
    assert(fnn);
    assert(num_layers >= 0);
    assert(layer_sizes);

    fnn->num_layers = num_layers;
    fnn->layer_sizes = (int *)malloc(num_layers * sizeof(*fnn->layer_sizes));
    for (int i = 0; i < num_layers; ++i) {
        fnn->layer_sizes[i] = layer_sizes[i];
    }

    fnn->neurons = (Neuron **)malloc(num_layers * sizeof(*fnn->neurons));

    for (int x = 0; x < num_layers; ++x) {
        fnn->neurons[x] =
            (Neuron *)malloc(layer_sizes[x] * sizeof(*fnn->neurons[x]));

        for (int y = 0; y < layer_sizes[x]; ++y) {
            fnn->neurons[x][y].in = 0.0;
            fnn->neurons[x][y].out = 0.0;
            if (x > 0) {
                fnn->neurons[x][y].weights = (float *)malloc(
                    layer_sizes[x - 1] * sizeof(*fnn->neurons[x][y].weights));
                for (int w = 0; w < layer_sizes[x - 1]; ++w) {
                    fnn->neurons[x][y].weights[w] = 0.0;
                }
            }
        }
    }

    return 0;
}

int fnn_neurons_copy(FNN *dest, FNN *src) {
    assert(dest);
    assert(src);

    if (dest->num_layers != src->num_layers) {
        return -1;
    }

    for (int i = 0; i < dest->num_layers; ++i) {
        if (dest->layer_sizes[i] != src->layer_sizes[i]) {
            return -2;
        }
    }

    for (int x = 1; x < dest->num_layers; ++x) {
        for (int y = 0; y < dest->layer_sizes[x]; ++y) {
            for (int w = 0; w < dest->layer_sizes[x - 1]; ++w) {
                dest->neurons[x][y].weights[w] = src->neurons[x][y].weights[w];
            }
        }
    }

    return 0;
}

int fnn_weights_randomise(FNN *fnn) {
    assert(fnn);

    for (int x = 1; x < fnn->num_layers; ++x) {
        for (int y = 0; y < fnn->layer_sizes[x]; ++y) {
            for (int w = 0; w < fnn->layer_sizes[x - 1]; ++w) {
                fnn->neurons[x][y].weights[w] = RAND_BETWEEN(-1.0, 1.0);
            }
        }
    }

    return 0;
}

int fnn_weights_jiggle(FNN *fnn) {
    assert(fnn);

    for (int x = 1; x < fnn->num_layers; ++x) {
        for (int y = 0; y < fnn->layer_sizes[x]; ++y) {
            for (int w = 0; w < fnn->layer_sizes[x - 1]; ++w) {
                fnn->neurons[x][y].weights[w] += RAND_BETWEEN(-0.1, 0.1);
                //     if(fnn->neurons[x][y].weights[w] < -1.0)
                //     {fnn->neurons[x][y].weights[w] = -1.0;}
                // else if(fnn->neurons[x][y].weights[w] >  1.0)
                // {fnn->neurons[x][y].weights[w] =  1.0;}
            }
        }
    }

    return 0;
}

int fnn_weights_reinforce(FNN *fnn) {
    assert(fnn);

    for (int x = 1; x < fnn->num_layers; ++x) {
        for (int y = 0; y < fnn->layer_sizes[x]; ++y) {
            for (int w = 0; w < fnn->layer_sizes[x - 1]; ++w) {
                fnn->neurons[x][y].weights[w] *= 1.01;
            }
        }
    }

    return 0;
}

int fnn_weights_decay(FNN *fnn) {
    assert(fnn);

    for (int x = 1; x < fnn->num_layers; ++x) {
        for (int y = 0; y < fnn->layer_sizes[x]; ++y) {
            for (int w = 0; w < fnn->layer_sizes[x - 1]; ++w) {
                fnn->neurons[x][y].weights[w] *= 0.99;
            }
        }
    }

    return 0;
}

int fnn_feedforwards(FNN *fnn) {
    assert(fnn);

    for (int y = 0; y < fnn->layer_sizes[0]; ++y) {
        fnn->neurons[0][y].out = fnn->neurons[0][y].in;
        // fnn->neurons[0][y].out = SIGMOID(fnn->neurons[0][y].in);
    }

    for (int x = 1; x < fnn->num_layers; ++x) {
        for (int y = 0; y < fnn->layer_sizes[x]; ++y) {
            fnn->neurons[x][y].in = 0.0;

            for (int i = 0; i < fnn->layer_sizes[x - 1]; ++i) {
                fnn->neurons[x][y].in +=
                    fnn->neurons[x - 1][i].out * fnn->neurons[x][y].weights[i];
            }

            fnn->neurons[x][y].out = SIGMOID(fnn->neurons[x][y].in);
        }
    }

    return 0;
}

int fnn_backpropagate(FNN *fnn) {
    assert(fnn);

    return 0;
}
