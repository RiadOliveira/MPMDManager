from ntpath import basename

def get_filename_from_path(path: str): return basename(path)[:-3]