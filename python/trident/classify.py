"""
Abstraction for score classification.

Requires Scipy.
"""

from trident import TridentException

class InvalidInterpolator(TridentException):
    """
    Exception to be raised when an invalid interpolation function is used or provided.
    """
    pass

class InterpolatorNotDefined(TridentException):
    """
    Exception to be raised when an interpolation function is not defined.
    """
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
    
    def __call__(self,score):
        """
        Calls interpolate and allows the object to be callable.
        
        @param score: Trident score dict
        @ptype score: dict
        @return: float
        """
        return self.interpolate(score)
    
    def save(self,filename):
        """
        Saves an Interpolator object using the pickle module.
        
        @param filename: Output filename
        @ptype filename: str
        """
        import pickle,time
        self.ctime = time.strftime("%Y%m%d%H%M")
        pickle.dump(self,open(filename,"wb"))
        

class LinearInterpolator(Interpolator):
    """
    Template interpolation class only uses the energy and score data.
    """
    def __init__(self):
        self.funct = None
        pass
    
    def interpolate(self,score):
        return self.funct(score['energy'],score['score'])
    
class MultiSpeciesInterpolator(Interpolator):
    """
    Template interpolation class that maps the species abbreviation from the 'query_id' section of the score to its Interpolator class.
    """
    def __init__(self):
        self.species_map = {}
        
    def interpolate(self,score):
        species = score['query_id'].split('-')[0]
        if not species in self.species_map:
            raise InterpolatorNotDefined("Interpolation function is not defined for species '{0}'".format(species))
        return self.species_map[species].interpolate(score)
    
    def species(self):
        """
        Returns a list of species included in the interpolator.
        
        @return: Species List
        @rtype: list
        """
        return self.species_map.keys()
    

def create_linear_interpolator(infilename):
    """
    Reads a file and returns a LinearInterpolator object.
    
    Requires scipy
    
    @param infilename: Input file name
    @ptype infile: str
    @return: LinearInterpolator
    """
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
    """
    Loads an interpolator object.
    
    @param filename: Input file name
    @ptype filename: str
    @return: Interpolator instance
    """
    import pickle
    return pickle.load(open(filename,"rb"))

def get_grade(log_value):
    """
    Used to generate a Trident 'Grade' for scores. Argument expect to be the value of an Interpolator function, e.g. Interpolator.interpolate or Interplator.__call__
    
    @param log_value: Data returned from an interpolator
    @type log_value: float
    @return: Trident score grade
    @rtype: int
    """
    if log_value <= -5:
        return 1
    elif log_value <= -4:
        return 2
    elif log_value <= -3:
        return 3
    elif log_value <= -2:
        return 4
    return 5
