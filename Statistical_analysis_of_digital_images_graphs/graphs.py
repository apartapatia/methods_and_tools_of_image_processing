import matplotlib.pyplot as plt

# Data
components = ['r', 'g', 'b']
component_colors = {'r': 'red', 'g': 'green', 'b': 'blue'}
delta_y = [-10, -5, 0, 5, 10]
autocorrelation_values = {
    'r': [1002.83, 952.332, 896.887, 821.999, 769.155],
    'g': [651.396, 588.887, 541.209, 473.054, 422.049],
    'b': [552.822, 532.552, 528.562, 497.22, 484.359]
}

# Plotting
fig, ax = plt.subplots(figsize=(10, 6))

bar_width = 0.2
index = delta_y
opacity = 0.8

for i, component in enumerate(components):
    plt.bar([x + i * bar_width for x in index],
            autocorrelation_values[component],
            bar_width,
            alpha=opacity,
            label='Component {}'.format(component),
            color=component_colors[component])

plt.xlabel('Delta y')
plt.ylabel('Autocorrelation')
plt.title('Autocorrelation for Components by Delta y')
plt.xticks([x + bar_width for x in index], delta_y)
plt.legend()

plt.tight_layout()
plt.show()
