from struct import pack


class PatchGenerator:
    def __init__(self):
        self.output = []  # Reset our output buffer
        self.writes = []  # Reset our gathered write lines

    def parse_gecko(self, lines):
        """
        Parse a Kamek generated Gecko file to gather writes.

        :param lines: The lines of the gecko file.
        """
        primary_payload_size = int(lines[0].split()[1], 16) / 8 + 1
        self.writes = [line.split() for line in lines[int(primary_payload_size):]]

    def process(self):
        """
        Produce the internal structure based on write lists.
        """
        self.output = []  # Reset output buffer
        for write in self.writes:
            w0 = write[0]
            if w0[:2] != "04":
                print("[WARN] Only 32 bit writes are currently supported!")
                continue
            self.output += [int("80" + w0[2:], 16), int(write[1], 16)]
        self.output += [0, 0]  # Append termination sequence

    def write_to_file(self, path):
        """
        Write the result to a file.

        :param path: Path to the file on disc.
        """
        with open(path, "wb") as file:
            for u in self.output:
                file.write(pack('>L', u))
