import json

class Config:
    def __init(self):
        # $DECOMP_ROOT
        self.variables = {}
        self.paths = {}

    def fromFile(self, path):
        with open(path, "r") as file:
            j = json.load(file)
            self.variables = j["variables"]
            self.paths = j["paths"]