# Bot Project
Entities in a 2D world compete against each other for resources. At the end of each round, the top scoring entities are bred from to create new entities for the next generation. Entities are able to pass on both their genetic knowledge and physical attributes to their offspring.

## Body
The body of each entity is made up of segments that are joined in a continuous chain. Any individual segment is capable of holding various parts, such as eyes and spikes.

## Brains
The "brain" for each entity is a standard [feedforward neural network](https://en.wikipedia.org/wiki/Feedforward_neural_network) that controls its actions based on its sensory inputs. Network weights are passed to child entities such that any knowledge can be retained. Each eye for an entity provides four inputs for the network - red, green, blue, and distance.
