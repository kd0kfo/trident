import pkg_resources
__version__ = pkg_resources.require("trident")[0].version

__all__ = ['parser','chromosome_chopper','mirna_chopper','validate_energies','core','classify']

class TridentException(Exception):
    """
    Base Class for Trident Exceptions
    """
class FastaError(TridentException):
    """
    Exception for errors in a fasta file.
    """
    pass


def load_energy_score_data(fname, dtype=float, comments="""#""", delimiter="\t", converters=None, skiprows=1, usecols=None, unpack=False, ndmin=0):
    """
    Reads a data file and returns a Numpy Array containing the data. 
    
    Two input file types are allowed, tab-delimited data and NetCDF files. 
    
    For tab-delimited data, the function parameters are passed along to numpy.loadtxt
    
    For NetCDF data, the function arguments, except for fname, are ignored. Data is assumed to be in the \"ranks\" variable. Loading NetCDF data requires the pupynere module.
    
    @param fname: Input file name
    @type fname: str
    @return: Numpy Array 
    """
    use_netcdf = False
    with open(fname,"r") as infile:
        magic_number = infile.read(3)
        if magic_number == "CDF":
            use_netcdf = True
    if use_netcdf:
        import pupynere as nc
        f = nc.netcdf_file(fname,"r")
        return f.variables['ranks'].data
    else:
        from numpy import loadtxt
        return loadtxt(fname, dtype, comments, delimiter, converters, skiprows, usecols, unpack, ndmin)
    
def get_content(seq,query):
    """
    Returns the fraction of occurances of "G" and "C" in a sequence. Only "A", "T"/"U", "G" and "C" are counted, i.e. "N" is ignored.
    
    @param seq: Sequence
    @type seq: str
    @return: float
    """
    total = 0.0
    qcount = 0.0
    q = query.lower()
    for l in seq.lower():
        if l in q:
            qcount += 1
        if l in "agctu":
            total += 1
         
    return qcount/total

def get_gc_content(seq):
    """
    Returns the fractional content of GC in a sequence.
    
    @param seq: Sequence to be counted
    @type seq: str
    @return: Fractional content of GC
    @rtype: float
    """
    return get_content(seq,"gc")

def get_ag_content(seq):
    """
    Returns the fractional content of AG in a sequence.
    
    @param seq: Sequence to be counted
    @type seq: str
    @return: Fractional content of AG
    @rtype: float
    """
    
    return get_content(seq,"ag")

