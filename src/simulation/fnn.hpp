#ifndef FNN_HPP_INCLUDED
#define FNN_HPP_INCLUDED

struct Neuron {
    float in;
    float out;
    float *weights;
};

struct FNN {
    int num_layers;
    int *layer_sizes;
    Neuron **neurons;
};

int fnn_print(FNN *fnn);
int fnn_init(FNN *fnn, int num_layers, int *layer_sizes);
int fnn_neurons_copy(FNN *dest, FNN *src);
int fnn_weights_randomise(FNN *fnn);
int fnn_weights_jiggle(FNN *fnn);
int fnn_feedforwards(FNN *fnn);
int fnn_weights_reinforce(FNN *fnn);
int fnn_weights_decay(FNN *fnn);
int fnn_backpropagate(FNN *fnn);

#endif  // FNN_H_INCLUDED
