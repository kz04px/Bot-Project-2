#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include <math.h>
#include "fnn.h"

#define RAND_BETWEEN(a, b) ((b-a)*((float)rand()/RAND_MAX)+a)
#define SIGMOID(x) (1.0/(1+pow(M_E, -x)))

int fnn_print(s_fnn *fnn)
{
  assert(fnn != NULL);
  
  int i;
  
  printf("Num layers: %i\n", fnn->num_layers);
  
  printf("Layer sizes:");
  for(i = 0; i < fnn->num_layers; ++i)
  {
    printf(" %i", fnn->layer_sizes[i]);
  }
  printf("\n");
  
  return 0;
}

int fnn_init(s_fnn *fnn, int num_layers, int *layer_sizes)
{
  assert(fnn != NULL);
  assert(num_layers >= 0);
  assert(layer_sizes != NULL);
  
  fnn->num_layers = num_layers;
  fnn->layer_sizes = malloc(num_layers*sizeof(*fnn->layer_sizes));
  int i;
  for(i = 0; i < num_layers; ++i)
  {
    fnn->layer_sizes[i] = layer_sizes[i];
  }
  
  fnn->neurons = malloc(num_layers*sizeof(*fnn->neurons));
  
  int x;
  for(x = 0; x < num_layers; ++x)
  {
    fnn->neurons[x] = malloc(layer_sizes[x]*sizeof(*fnn->neurons[x]));
    
    int y;
    for(y = 0; y < layer_sizes[x]; ++y)
    {
      fnn->neurons[x][y].in = 0.0;
      fnn->neurons[x][y].out = 0.0;
      if(x > 0)
      {
        fnn->neurons[x][y].weights = malloc(layer_sizes[x-1]*sizeof(float));
        int w;
        for(w = 0; w < layer_sizes[x-1]; ++w)
        {
          fnn->neurons[x][y].weights[w] = 0.0;
        }
      }
    }
  }
  return 0;
}

int fnn_weights_randomise(s_fnn *fnn)
{
  assert(fnn != NULL);
  
  int x;
  for(x = 1; x < fnn->num_layers; ++x)
  {
    int y;
    for(y = 0; y < fnn->layer_sizes[x]; ++y)
    {
      int w;
      for(w = 0; w < fnn->layer_sizes[x-1]; ++w)
      {
        fnn->neurons[x][y].weights[w] = RAND_BETWEEN(-1.0, 1.0);
      }
    }
  }
  
  return 0;
}

int fnn_weights_jiggle(s_fnn *fnn)
{
  assert(fnn != NULL);
  
  int x;
  for(x = 1; x < fnn->num_layers; ++x)
  {
    int y;
    for(y = 0; y < fnn->layer_sizes[x]; ++y)
    {
      int w;
      for(w = 0; w < fnn->layer_sizes[x-1]; ++w)
      {
        fnn->neurons[x][y].weights[w] += RAND_BETWEEN(-0.05, 0.05);
             if(fnn->neurons[x][y].weights[w] < -1.0) {fnn->neurons[x][y].weights[w] = -1.0;}
        else if(fnn->neurons[x][y].weights[w] >  1.0) {fnn->neurons[x][y].weights[w] =  1.0;}
      }
    }
  }
  
  return 0;
}

int fnn_feedforwards(s_fnn *fnn)
{
  assert(fnn != NULL);
  
  int x;
  int y;
  
  for(y = 0; y < fnn->layer_sizes[0]; ++y)
  {
    fnn->neurons[0][y].out = SIGMOID(fnn->neurons[0][y].in);
  }
  
  for(x = 1; x < fnn->num_layers; ++x)
  {
    for(y = 0; y < fnn->layer_sizes[x]; ++y)
    {
      fnn->neurons[x][y].in = 0.0;
      
      int i;
      for(i = 0; i < fnn->layer_sizes[x-1]; ++i)
      {
        fnn->neurons[x][y].in += fnn->neurons[x-1][i].out * fnn->neurons[x][y].weights[i];
      }
      
      fnn->neurons[x][y].out = SIGMOID(fnn->neurons[x][y].in);
    }
  }
  
  return 0;
}