import numpy as np
import wavinfo as wv
import glob


class Shazam:

    def search(self, path):
        print("hello world!!!")

    def index(self, folder):
        for song in glob.glob(folder + "/*.wav"):
            try:
                self.sample(song)
            except Exception:
                print("Error in processing: " + song)

    def sample(self, song):
        print("???")


t = Shazam()
t.index("../data")
t.search("")
