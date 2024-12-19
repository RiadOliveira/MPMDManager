from sys import argv
from ntpath import basename

def get_filename(): return basename(argv[0])[:-3]