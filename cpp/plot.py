import matplotlib
matplotlib.use('Agg')
import matplotlib.pyplot as plt
from matplotlib.path import Path
import matplotlib.patches as patches
import sys
from ast import literal_eval as make_tuple

plt.clf()

def read_piped_input():
	path = []
	for node in sys.stdin:
		path.append(make_tuple(node))
	return path


verts = read_piped_input()

largestx = -1
largesty = -1
codes = [Path.MOVETO]
for i in verts[0:-2]:
	if i[0] > largestx:
		largestx = i[0]
	if i[1] > largesty:
		largesty = i[1]
	codes.append(Path.LINETO)

codes.append(Path.CLOSEPOLY)

#print(codes)

path = Path(verts, codes)
fig = plt.figure()
ax = fig.add_subplot(111)
patch = patches.PathPatch(path, facecolor='white', lw=0.1)
ax.add_patch(patch)
ax.set_xlim(-5,largestx + 5)
ax.set_ylim(-5,largesty + 5)
plt.axis("off")
plt.savefig('./drawings/cpp_' + largestx.__str__() + ".png",format="PNG", dpi=1000)
