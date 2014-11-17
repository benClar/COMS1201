#   maze.c is main().
#   I have implemented the basic and SDL options fully, with added options on
#   viewing the route, centred around storing the correct exit route
#   coordinates in a linked list.
#
#   I have taken a modular approach, hiding structure from other modules which
#   are only accessible via accessor and setter functions.  I have abstracted
#   away my structure members.  This approach has provides flexibilty for
#   future change and ensures structures are used correctly.
#
#   I have attempted the random maze generator, but have not achieved full
#   functionality.
#   To run: 1) make all
#           2) .compiled/maze .text/maze[N].txt
#           3) .compiled/maze .text/maze[N].txt SDL
#           4) .compiled/maze RANDOM
#
#
