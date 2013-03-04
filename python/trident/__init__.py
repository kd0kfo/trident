__all__ = ['parser','chromosome_chopper','mirna_chopper','validate_energies','core']

class FastaError(Exception):
    """
    Exception for errors in a fasta file.
    """
    pass
