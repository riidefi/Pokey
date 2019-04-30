
def process_string(config, x):
    """"Process a string.

    This is necessary especially for the heavily used $DECOMP_ROOT variable.
    """
    for key, value in config.variables.items():
        x.replace(key, value)

    return x
