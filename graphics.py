from argparse import ArgumentParser
from os import path

import matplotlib.pyplot as plt
import pandas as pd
import seaborn as sns
from matplotlib.colors import LogNorm

parser = ArgumentParser(description="Create graphics from the experiments data.")
parser.add_argument(
    "input_file",
    help="CSV file to be read and processed. Should be output of launcher.py",
)
parser.add_argument("out_folder", help="Folder where the images will be saved")
args = parser.parse_args()

input_file = args.input_file
data = pd.read_csv(input_file)
data["Time (secs)"] = data["Time"] * 10**-6

algorithms = ["row-column", "row-row"]
protocols = ["OpenMP", "MPI"]
fig, ax = plt.subplots(2, 2)
fig.set_size_inches(10, 10)
ax_idx = 0
for protocol in protocols:
    for algorithm in algorithms:
        position = divmod(ax_idx, 2)
        heatmap = data.query(f"Algorithm == '{algorithm}' and Protocol == '{protocol}'")
        heatmap = heatmap.pivot_table(
            values="Time (secs)",
            index="Matrix_Size",
            columns="N_Threads",
            aggfunc="mean",
        )
        sns.heatmap(heatmap, norm=LogNorm(), ax=ax[*position])
        ax[*position].set_title(f"Protocol={protocol} | Algorithm={algorithm}")
        ax_idx += 1
fig.suptitle("Time (secs) for every threads-size combination", fontsize="x-large")
plt.savefig(path.join(args.out_folder, "size-threads-time.png"))
plt.clf()
plt.cla()

data_size2k_threads20 = data.query("Matrix_Size == 2000 and N_Threads == 20")
algorithms = ["row-column", "row-row"]
protocols = ["OpenMP", "MPI"]
fig, ax = plt.subplots(2, 2)
fig.set_size_inches(12, 12)
ax_idx = 0
for protocol in protocols:
    for algorithm in algorithms:
        position = divmod(ax_idx, 2)
        dist = data_size2k_threads20.query(
            f"Algorithm == '{algorithm}' and Protocol == '{protocol}'"
        )
        sns.violinplot(dist, y="Time (secs)", inner="quart", ax=ax[*position])
        ax[*position].set_title(f"Protocol={protocol} | Algorithm={algorithm}")
        ax_idx += 1
fig.suptitle(
    "Time (secs) distribution for Matrix_Size=2000, N_Threads=20", fontsize="x-large"
)
plt.savefig(path.join(args.out_folder, "distribution.png"))
plt.clf()
plt.cla()

ax = sns.relplot(
    data,
    x="N_Threads",
    y="Time (secs)",
    hue="Matrix_Size",
    row="Protocol",
    col="Algorithm",
    palette="plasma",
    kind="line",
)
plt.savefig(path.join(args.out_folder, "threads-time.png"))
plt.clf()
plt.cla()

data["N_Threads"] = data["N_Threads"].astype(str)
ax = sns.relplot(
    data,
    x="Matrix_Size",
    y="Time (secs)",
    hue="N_Threads",
    row="Protocol",
    col="Algorithm",
    palette="plasma",
    kind="line",
)
plt.savefig(path.join(args.out_folder, "size-time.png"))
