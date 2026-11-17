from pathlib import Path

import matplotlib
matplotlib.use("Agg")
import matplotlib.pyplot as plt


root = Path(__file__).resolve().parent
loops = 1836
report = (root / "Measurement_Report.md").read_text(encoding="utf-8")
heading = f"## Sau warm-up — MAXLOOPS = {loops}"
section = report.split(heading, 1)[1].split("\n## ", 1)[0]

data = []
for line in section.splitlines():
    cells = [cell.strip() for cell in line.strip().strip("|").split("|")]
    if len(cells) == 5 and cells[0].isdigit():
        # The report lists pinned time before unpinned time.
        data.append(tuple(map(int, cells[:3])))

counts = [n for n, _, _ in data]
pinned = [time / (loops * n) for n, time, _ in data]
unpinned = [time / (loops * n) for n, _, time in data]

plt.rcParams.update({"font.family": "DejaVu Sans", "font.size": 11})
fig, ax = plt.subplots(figsize=(10, 5.6), layout="constrained")
ax.plot(counts, unpinned, "o-", color="#2563eb", label="Không khóa CPU", linewidth=1.8)
ax.plot(counts, pinned, "s-", color="#ea580c", label="Khóa CPU", linewidth=1.8)
ax.set_xscale("log", base=2)
ax.set_xticks(counts, [str(n) for n in counts])
ax.minorticks_off()
ax.set_ylim(0, max(pinned + unpinned) * 1.16)
ax.set_xlabel("NUMPAGES (số page, thang log₂)")
ax.set_ylabel("Thời gian trung bình (ns/cập nhật)")
ax.set_title(f"Chi phí cập nhật theo số page\nSau warm-up · MAXLOOPS = {loops}", pad=14)
ax.grid(axis="y", color="#e2e8f0")
ax.set_axisbelow(True)
ax.spines[["top", "right"]].set_visible(False)
ax.legend(frameon=False)

output = root / "measurement_plot.png"
fig.savefig(output, dpi=180)
plt.close(fig)
print(output)
