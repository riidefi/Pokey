import json

class Config:
    def __init(self):
        # $DECOMP_ROOT
        self.variables = {}

    def fromFile(self, path):
        with open(path, "r") as file:
            self.variables = json.load(file)
