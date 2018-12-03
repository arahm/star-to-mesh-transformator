# star-to-mesh-transformator
________________________________________________________________________________________________

Star-to-Mesh transformator for computing the total conductivity of a network of Ohmian resistors
________________________________________________________________________________________________

Let us consider an electrical network which exclusively consists of finitely many Ohmian resistors (it may be a subnetwork in a larger electrical network which also contains components of other types). In A. Rosen's PhD thesis (1924), formulas have been worked out which allow to replace the Ohmian resistors connected to one common node (hence forming a "star") by equivalent resistors between the nodes at the extremities of the "star", such that the common node is eliminated. This equivalent connection of the other nodes by Ohmian resistors is called the "mesh" in this situation (although the word "mesh" is somewhat confusing since it already has a different meaning in an electrical network; in German it is less confusingly called the "Polygon", even though it is not just a polygon, but in fact the corners of that polygon are getting mutually directly connected by further Ohmian resistors). See also:

https://en.wikipedia.org/wiki/Star-mesh_transform

The Star-to-Mesh transformator source code released here allows to reduce the network down to one single Ohmian resistor between two nodes which can be chosen beforehand (let us call them "the electrodes"). This reduction is achieved by eliminating each node which is not an "electrode" by a star-to-mesh transformation, one after another. 
