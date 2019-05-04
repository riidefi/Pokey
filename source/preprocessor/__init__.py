
def process_string(config, x):
    """"Process a string.

    This is necessary especially for the heavily used $DECOMP_ROOT variable.
    """

    for key, item in config.variables.items():
        x = x.replace(key, item)

    return x
