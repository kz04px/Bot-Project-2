#ifndef FNN_H_INCLUDED
#define FNN_H_INCLUDED

typedef struct
{
  float in;
  float out;
  float *weights;
} s_neuron;

typedef struct
{
  int num_layers;
  int *layer_sizes;
  s_neuron **neurons;
} s_fnn;

int fnn_print(s_fnn *fnn);
int fnn_init(s_fnn *fnn, int num_layers, int *layer_sizes);
int fnn_neurons_copy(s_fnn *dest, s_fnn *src);
int fnn_weights_randomise(s_fnn *fnn);
int fnn_weights_jiggle(s_fnn *fnn);
int fnn_feedforwards(s_fnn *fnn);
int fnn_weights_reinforce(s_fnn *fnn);
int fnn_weights_decay(s_fnn *fnn);
int fnn_backpropagate(s_fnn *fnn);

#endif // FNN_H_INCLUDED