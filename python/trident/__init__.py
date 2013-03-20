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