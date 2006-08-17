# Append to effective python path so that can find plplot modules
# depending on how python_location[12] are set.
# Note, by careful choice of python_location[12] for the build-tree
# case and the install tree case you can achieve
# a clear separation between build and install tree with the current CBS 
# that is not possible with the current ABS system.
import sys
sys.path.insert (0, "@python_location1@")
sys.path.insert (0, "@python_location2@")
