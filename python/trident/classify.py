"""
Abstraction for score classification.

Requires Scipy.
"""

from trident import TridentException

class InvalidInterpolator(TridentException):
    pass

class InterpolatorNotDefined(TridentException):
    pass

class Interpolator:
    """
    Base class for Interpolator function. Overload interpolate.
    """
    def __init__(self):
        pass

    def interpolate(self,score):
        """
        Function that interpolates a score dict.
        
        @return: float
        """
        raise InterpolatorNotDefined("Interpolation function is not defined.")
    
    def save(self,filename):
        import pickle
        pickle.dump(self,open(filename,"wb"))
        

class LinearInterpolator(Interpolator):
    def __init__(self):
        self.funct = None
        pass
    
    def interpolate(self,score):
        return self.funct(score['energy'],score['score'])
    
class MultiSpeciesInterpolator(Interpolator):
    def __init__(self):
        self.species_map = {}
        
    def interpolate(self,score):
        species = score['query_id'].split('-')[0]
        if not species in self.species_map:
            raise InterpolatorNotDefined("Interpolation function is not defined for species '{0}'".format(species))
        return self.species_map[species].interpolate(score)
    

def create_linear_interpolator(infilename):
    from trident import load_energy_score_data
    import scipy.interpolate as SI
    import numpy as np
    data = load_energy_score_data(infilename)
    zmask = data[:,5] != 0
    x = data[zmask][:,0]
    y = data[zmask][:,1]
    z = np.log10(data[zmask][:,5])
    retval = LinearInterpolator()
    retval.funct = SI.LinearNDInterpolator((x,y),z)
    return retval
        
def load(filename):
        import pickle
        return pickle.load(open(filename,"rb"))