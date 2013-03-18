__all__ = ['parser','chromosome_chopper','mirna_chopper','validate_energies','core']

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
    from numpy import loadtxt
    return loadtxt(fname, dtype, comments, delimiter, converters, skiprows, usecols, unpack, ndmin)